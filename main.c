#include <sys/types.h> 
#include <sys/stat.h>
#include <unistd.h> 
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mitr.h"

/**
 * @param argc contador de argumentos
 * @param argv argumentos
 */
int main(int argc, char* argv[]){
    // option argument
    extern char* optarg;
    // option index
    extern int optind;
    // variables de control
    int c, n = 0, dflag = 0, sflag = 0;
    // strings y archivos
    char *s1, *s2, *file1, *file2;

    // Control de las opciones, getopt() nos indica la existencia de opciones, si no hubieran retornaría -1
    while((c = getopt(argc, argv, "ds")) != -1){
        switch(c){
            // Si la opcion es -d, activamos la flag correspondiente
            case 'd':
                dflag = 1;
                break;
            // Si la opción es -s, activamos la flag correspondiente
            case 's':
                sflag = 1;
                break;
            // Se ha escrito una opción diferente a las disponibles
            default:
                check_error(7);
        }
    }

    // Asignación del resto de los argumentos a variables locales
    for (; optind < argc; optind++){
        // n variable de control
        switch(n){
            // Asignación primer parámetro
            case 0:
                s1 = argv[optind];
                n++;
                break;
            // Asignación del segundo parámetro
            case 1:
                s2 = argv[optind];
                n++;
                break;
            // Asignación del tercer parámetro
            case 2:
                file1 = argv[optind];
                n++;
                break;
            // Asignación del cuarto parámetro
            case 3:
                if (dflag == 1 || sflag == 1) check_error(7);
                file2 = argv[optind];
                n++;
                break;
            // No puede haber más
            default:
                check_error(7);
        }
    }

    // Checkea que no pueden estar ambas flags activadas
    if (dflag == 1 && sflag == 1) check_error(7);
    // Si alguna estuviera activada, cambiar órden de parámetros
    else if (dflag == 1 || sflag == 1){
        file2 = file1;
        file1 = s2;
    // Uso normal, checkeamos tamaño de las strings pasadas por parámetro para que tengan la misma
    } else {
        checkLength(s1, s2);
    }

    // Control de archivos
    int fd1, fd2;
    // Abrir primer archivo en modo de lectura
    fd1 = openFile(file1, O_RDONLY);
    // Una vez abierto el primer archivo, comprobamos si son los mismos
    int boolean = isTheSame(file1, file2);
    // Si son el mismo archivo, abrirlo en modo escritura
    if (boolean == 1) fd2 = openFile(file2, O_WRONLY);
    // Si no lo son, crearlo/sobreescribir con los permisos de lectura y escritura 
    // para users, de lectura para el dueño del archivo, y de lectura para otros usuarios
    else fd2 = createFile(file2, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

    // Control de ejecución, llamar a la función adecuada dependiendo de las flags
    if (dflag == 1) delete(s1, fd1, fd2);
    else if (sflag == 1) squeeze(s1, fd1, fd2);
    else trim(s1, s2, fd1, fd2);
}