#include<stdlib.h>
#include<stdio.h>
#include<sys/time.h>

void imprimeCuadrado();

struct timeval stop,start;
unsigned long tiempo;
unsigned long ingreso=0;
unsigned long clave = 0xF49DC57D;
unsigned long token;
unsigned long tp=30000;

int main()
{
	printf("Pulse enter para sincronizar... ");
	getchar(); //lee enter para sincronizar la fotoresistencia del stm32
	imprimeCuadrado();
	gettimeofday(&start,NULL);
	printf("Ingresa el Token:\n");
	while(1){
		gettimeofday(&stop,NULL);
		tiempo=((stop.tv_sec - start.tv_sec)*1000000+stop.tv_usec-start.tv_usec);
		tiempo/=1000.0;
		printf("TIEMPO %i\n",tiempo);
		if(tiempo>=tp){ //si el tiempo es mayor a 30000ms tp cambia su rango
            tp=tp+30000;
            printf("TP ES %i\n",tp);
		}else{
		    scanf("%lX",&ingreso);
		}
		token = clave^tp;
		if(ingreso==token){
			printf("Acceso Concedido\n");
			printf("Ingresa un nuevo Token:\n");
		}else{
			printf("Acceso Denegado ");
			printf("Ingresa el Token:\n");
		}
	}
}

void imprimeCuadrado(){
    unsigned char dato = 219; //imprime el caracter 219 de ASCII
	int i = 0;
	for(i=1;i<289;i++){ //Cuadrado donde imprime caracter 219
		printf("%c",dato);
		if(0==(i%24))printf("\n");
	}
}
