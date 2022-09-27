#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>


void EscribirSerie(HANDLE idComDev, char *buf);
void LeerSerie(HANDLE Com, char *bufrec);


struct timeval stop,start;
unsigned long tiempo;
unsigned long ingreso=0;
unsigned long token=0;
unsigned long tp;
int i=0;
char key[20]="KE";
char ho[20]="HO";


int main(){

	HANDLE Com;
	COMMTIMEOUTS Timeouts;

    time_t hi;
    struct tm *info;
    char horaC[10];

	char claved[9];
	char recibidos[256];

	char a[2];
	unsigned long clave,hora;

	//////////////////////////ABRIMOS EL PUERTO COM Y REAFIRMAMOS SI ES CORRECTO//////////////////////////////////////////

	Com = CreateFileA("COM7", GENERIC_READ | GENERIC_WRITE,0, NULL, OPEN_EXISTING, 0, NULL);
    if (Com == INVALID_HANDLE_VALUE){
        printf("\nError en el puerto\n");
        printf("Verifique que este conectado el puerto o que COM sea el correcto\n");
        return 0;
    }else{
        printf("\nPuerto abierto\n");

    }

	//////////////////////////CONGIGURAMOS LA FORMA DE MANDAR DATOS//////////////////////////////////////////

	// Leer time-outs actuales

    GetCommTimeouts(Com,&Timeouts);

	// Nuevos valores de timeout:
    Timeouts.ReadIntervalTimeout = 50;
	Timeouts.ReadTotalTimeoutConstant = 50;
	Timeouts.ReadTotalTimeoutMultiplier = 10;
	Timeouts.WriteTotalTimeoutConstant = 50;
	Timeouts.WriteTotalTimeoutMultiplier = 10;

	// Establecer timeouts:
	SetCommTimeouts (Com, &Timeouts);


	//Mandamos la clave

	printf("Digite su clave para el token\n");
	scanf("%s",&claved);
	strcat(key,claved);
	printf("%s",key);
	EscribirSerie(Com,key);
	LeerSerie(Com,recibidos);
	clave=strtoul(claved, NULL, 16);

	printf("\nLa clave digitada fue %X\n",clave);

	/////////////////////SINCRONIZAMOS//////////////////////

	printf("Pulse la tecla  s para sincronizar... \n");
	scanf("%s",&a);

    while(i==0){
        if(a[0]=='s' || a[0]=='S'){
            i=2;
        }else{
            printf("Tecla incorrecta\n");
            printf("Pulse la tecla  S para sincronizar... \n");
            scanf("%s",&a);
        }
    }



    ///////OBTENEMOS LA HORA/////////////

    gettimeofday(&start, NULL);
    hi = start.tv_sec;
    info = localtime(&hi);
    strftime(horaC, sizeof(horaC), "%H%M%S", info);

    ///////////COLOCAMOS UNA PALABRA PARA IDENTIFICAR LA HORA EN EL STM////////

    strcat(ho,horaC);

    //////////MANDAMOS LAS HORA////////////////

    EscribirSerie(Com,ho);
    LeerSerie(Com,recibidos);

    /////////TRANFORMAMOS DE HORA C A HORA DIGITO//////////////////////////

    hora=strtoul(horaC, NULL, 10);

    printf("\nLA HORA ES: %s\n",horaC);

    hora=hora+30;
    //////////////////SE ENVIA EL ACTIVADOR///////////////

    EscribirSerie(Com,a);
    LeerSerie(Com,recibidos);

    printf("\nIngresa el Token:\n");
    tp=hora;

    while(1){
		gettimeofday(&stop,NULL);
		tiempo = (stop.tv_sec - start.tv_sec) + tp;
		if(tiempo>=(hora+30)){ //si el tiempo es mayor a 30000ms tp cambia su rango
            hora=hora+30;
		}else{
		    scanf("%08X",&ingreso);
		}
            token = clave^hora;
            if(ingreso==token){
                printf("Acceso Concedido\n");
                printf("Ingresa un nuevo Token:\n");
            }else{
                printf("Acceso Denegado ");
                printf("Ingresa el Token:\n");
            }
	}

	CloseHandle(Com);
	return 0;
}

void EscribirSerie(HANDLE Com, char *buf){
    char oBuffer[256];  /* Buffer de salida */
    DWORD iBytesWritten;

    iBytesWritten = 0;
    strcpy(oBuffer, buf);
    WriteFile(Com, oBuffer, strlen(oBuffer), &iBytesWritten, NULL);
}

void LeerSerie(HANDLE Com, char *bufrec){

    BOOL Status;
    DWORD bitsno;
    DWORD mask;
    char temp;
    i=0;

    SetCommMask(Com, EV_RXCHAR);
    Status=WaitCommEvent(Com, &mask, NULL);

    if (Status == FALSE){
					printf("\n    Error! in Setting WaitCommEvent()");
				}
			else
				{
					do
						{
							Status = ReadFile(Com, &temp, sizeof(temp), &bitsno, NULL);
							bufrec[i] = temp;
							i++;
					    }
					while (bitsno > 0);
                    for (int j = 0; j < i-1; j++){
                        printf("\n");
                        printf("%c", bufrec[j]);
                    }		// j < i-1 to remove the dupliated last character

    }
}
