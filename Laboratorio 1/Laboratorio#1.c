#include <stdio.h>
#include <math.h>

int main (int argc, char** argv){

    FILE *in,*grises;
    char padd;
    unsigned long size,anc,c,rc,i=0;
    in = fopen("DEKU.bmp", "rb");
    fseek(in,2,SEEK_SET);                     //Se posiciona en el byte 2 de la lista
    fread(&size,sizeof(unsigned char),4,in);  // Se guarda el tamaño de la imagen (numero de bytes)
                                              //en la variable anc, con un tamaño de unsigned char los siguientes 4 bytes
    unsigned char datos[size];                // Se declara una lista de datos unsined char del tamaño total leido
    fseek(in,18,SEEK_SET);                    //Se posiciona en el byte 18 de la lista
    fread(&anc,sizeof(unsigned char),4,in);   // Se guarda el ancho en la variable anc, con un tamaño de unsigned char los siguientes 4 bytes
    padd =(anc)%4;                            // Se calcula el padding de la imagen usando el modulo de 4 en el ancho


    while(i<size){
        fseek(in,i,SEEK_SET);
        datos[i]=getc(in);
        i++;
    }

    for(c=54;c<=i-1;c=c+padd){
        rc=0;
        while(rc<anc){
            datos[c]=round((0.114*datos[c])+(0.587*datos[c+1])+(0.299*datos[c+2]));
            datos[c+1]=datos[c];
            datos[c+2]=datos[c];
            rc++;
            c=c+3;
        }
    }

    fclose(in);

    grises=fopen("Resultado1.bmp","wb"); //Se crea un archivo y se abre en modod de lectura binaria

    for(c=0;c<=i-1;c++){                //Se empiezza a escribir en un nuevo archivo los datos guardados en la lista
        putc(datos[c],grises);
    }

    fclose(grises);

    printf("La imagen ha sido covertida a escala de grises",c);

    return 0;
}


