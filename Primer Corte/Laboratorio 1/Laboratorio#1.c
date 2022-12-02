#include <stdio.h>
#include <math.h>

int kernel[9]= {0,1,0,1,15,1,0,1,0}; //Kernel que realza los pixeles
FILE *in,*grises,*conv; //In es la imagen de entrada grises y conv con las imagenes
                        //de salida de la escala de grises y la convolucion
char padd,op,a=1;           //Padd es el padding de la imagen y op , la opcion elegida por el usuario
                            //a es la ultima opcio del usuario
unsigned long size,anc,c,rc,i=0;  //Size y anc , tamaño y ancho de la imagen,
                                      //c,rc e i con variables que ayudaran a contar los datos de lai

int main (int argc, char** argv){

    while(a==1){

        printf("\n//////////////////////////////////////////////////////////");  //Preesnetacio del programa y las opciones
        printf("\nBienvenido al programa de conversion de imagenes\n");

        printf("En caso de convertir a escala de grises oprima 1\n");
        printf("En caso de realizar una convolucion oprima 2\n");

        scanf("%d",&op);                                                    //Se lee la opcion elegida por el usuario

        in = fopen("hola.bmp", "rb");                                       // se abre una la imagen de entrada
        fseek(in,2,SEEK_SET);                                               // Se dirige la posicion 2 de la imagen
        fread(&size,sizeof(unsigned char),4,in);                            //Lee los siguientes 4 bytes desde la posicion actual y los guarda en size,
                                                                            //que tiene un tamaño de unsigned char
        unsigned char datos[size];                                          // se crea una lista datos con tamaño size
        fseek(in,18,SEEK_SET);                                              // Se dirige la posicion 18 de la imagen
        fread(&anc,sizeof(unsigned char),4,in);                             //Lee los siguientes 4 bytes desde la posicion actual y los guarda en anc,
                                                                            //que tiene un tamaño de unsigned char
        padd =(anc)%4;                                                      //se calcula el padding de la imagen

        while(i<size){                                                      //se escriben o reescriben los datos que hay en la imagen en datos
            fseek(in,i,SEEK_SET);
            datos[i]=getc(in);
            i++;
        }

        fclose(in);                                                         // se cierra la imagen de entrada


        switch(op){                                                         //Se evalua la opcion que digito el usuario

        case 1:

            for(c=54;c<=i-1;c=c+padd){                                      // Se recorre la lista datos desde 54 , onde empiezan los pixeles
                                                                            //hasta el final de la lista
                rc=0;
                while(rc<anc){                                              //Se repite el procedimiento hasta que sea mayor al ancho de la imagen
                    datos[c]=round((0.114*datos[c])+(0.587*datos[c+1])+(0.299*datos[c+2])); //Se usa el algoritmo para escala de grises
                    datos[c+1]=datos[c];                                                    //y se escribe en lo siguientes 3 datos
                    datos[c+2]=datos[c];
                    rc++;
                    c=c+3;
                }
            }


            grises=fopen("ResultadoEscalaDeGrises.bmp","wb");               // Se abre la imagen de salida y la escribe
            for(c=0;c<=i-1;c++){                                            //o rescribe con los datos de la lista
                putc(datos[c],grises);
            }

            fclose(grises);                                                  //Se cierra la imagen de salida

            printf("La imagen ha sido covertida a escala de grises\n");      //Se muestra que el proceso ha acabdo

                break;
        case 2:

            for(c=(54+(3*anc)+padd+3);c<i-(3*anc)-2*padd-3;c=c+padd+6){       // Se recorre la lista y se va modificando cada canal de color
                rc=0;                                                         //por un valor del kernel correspondiente
                while(rc<(3*(anc-2))){
                    datos[c]=((kernel[0]*datos[c+(3*anc)+padd]                //EL kernel debe dejar un marco de un pixel por un pixel a cada lado de la imagen
                                   +kernel[1]*datos[c+(3*anc)+padd+3]         //para evitar errores
                                   +kernel[2]*datos[c+(3*anc)+padd+6]         //Cada canal de color de cada pixel sera modificado y
                                   +kernel[3]*datos[c-3]                      //esto se repetira hasta que se acaebn los pixeles del ancho -2, que es el marco
                                   +kernel[4]*datos[c]
                                   +kernel[5]*datos[c+3]
                                   +kernel[6]*datos[c-(3*anc)-padd-6]
                                   +kernel[7]*datos[c-(3*anc)-padd-3]
                                   +kernel[8]*datos[c-(3*anc)-padd])/9);
                    rc++;
                    c++;
                }
            }


            conv=fopen("ResultadoConvolucion.bmp","wb");                      // Se abre la imagen de salida y la escribe
            for(c=0;c<=i-1;c++){                                              //o rescribe con los datos de la lista
                putc(datos[c],conv);
            }
            fclose(conv);                                                     //Se cierra la imagen de salida

            printf("Se le ha aplicado el kernel a la imagen\n");              //Se muestra que el proceso ha acabdo


                break;

        }

        printf("\n¿Desea hacer otra operacion?\n");                            //Se le pregunta al usuarrio si desea continuar

        printf("Si, oprima 1\n");                                              //Opciones
        printf("No, oprima otro numero\n");

        scanf("%d",&a);                                                        //Se lee la respuesta del usuario
    }

    printf("\nPrograma cerrado\n");                                            //Si el usuario decide no continuar se cierra el programa

    return 0;
}


