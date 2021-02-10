/**
 * Desarrollo del comando tr; aquí llamado mitr que sirve para sustituir los caracteres de cadena1 en los de cadena2 leyendo el fichero1 y creando fichero2 con las sustituciones hechas.
 * El programa se invocará según la sintaxis ./mitr [-d][-s] cadena1 cadena2 fichero1 fichero2
 * La opción -d realiza un borrado de las ocurrencias de todos los caracteres que aparecen en cadena1. En este caso, no se indica cadena2. Se lee de fichero1 y se guarda en fichero2.
 * La opción -s elimina las repeticiones consecutidas de los caracteres que estén en cadena1. Lee de fichero1 y guarda en fichero2.
 * Solo se permiten las llamadas al sistema de Linux, por lo que no se permite el uso de funciones de más alto nivel, como las de stdio.h
 * 
 * v. 5; Tratamos caracter a caracter y con manejo de variables locales
 */

/**
 * Librerías utilizadas
 */
#include <sys/types.h> 
#include <sys/stat.h>
#include <unistd.h> 
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Función general que maneja el control de errores, muestra mensaje por pantalla indicando el error que ha sucedido
 * @param eCode, codigo de error
 */
void check_error(int eCode){
    switch(eCode){
        case 0:
            fprintf(stderr, "[!] Error al abrir el archivo: %s\n", strerror(errno));
            exit(-1);
        case 1:
            fprintf(stderr, "[!] Error al crear el archivo: %s\n", strerror(errno));
            exit(-1);
        case 2:
            fprintf(stderr, "[!] Error al cerrar el archivo: %s\n", strerror(errno));
            exit(-1);
        case 3:
            fprintf(stderr, "[!] Error al leer el archivo: %s\n", strerror(errno));
            exit(-1);
        case 4:
            fprintf(stderr, "[!] Error al escribir el archivo: %s\n", strerror(errno));
            exit(-1);
        case 5:
            fprintf(stderr, "[!] Error al comprobar STATS: %s\n", strerror(errno));
            exit(-1);
        case 6:
            fprintf(stderr, "[!] Error en el formato de los archivos: %s\n", strerror(errno));
            exit(-1);
        case 7:
            printf("[!] Invocación incorrecta: ./mitr [-d || -s] string1 [string2] file1 file2\n");
            exit(-1);
        case 8:
            printf("[!] Error en la longitud de parámetros\n");
            exit(-1);
    }
}

/**
 * Comprueba que ambas strings pasadas por parámetro tengan la misma longitud, si no, envía error
 * @param s1, primera cadena pasada por parámetro por línea de órdenes
 * @param s2, segunda cadena pasada por parámetro por línea de órdenes
 */
void checkLength(char* s1, char* s2){
    if (strlen(s1) != strlen(s2)) check_error(8);
}

/**
 * Abre un archivo y devuelve su identificador, además checkea si hay errores
 * @param ruta del archivo
 * @param mode que indica el modo en el que se abre (write only, read only, ...)
 * @return identificador
 */
int openFile(char* ruta, int mode){
    int fd;
    fd = open(ruta, mode);

    if (fd == -1) check_error(0);
    return fd;
}

/**
 * Crea un archivo y devuelve su identificador, además checkea si hay errores
 * @param ruta del archivo
 * @param mode que indica el modo en el que se abre
 * @return identificador
 */
int createFile(char* ruta, int mode){
    int fd;
    fd = creat(ruta, mode);
    
    if (fd == -1) check_error(1);
    return fd;
}

/**
 * Checkea si hay errores y devuelve 0 si no. Se comporta igual que la llamada al sistema close();
 * @param fd identificador del archivo
 * @return int indicando si no hay errores, mensaje de error si lo hubiera
 */
int closeFile(int fd){
    if (close(fd) == -1) check_error(2);
    return 0;
}

/**
 * Lee el archivo que corresponde al identificador pasado por parametro, ademaś checkea si hay errores o si ha llegado al final del archivo
 * @param fd identificador del archivo
 * @return res, carácter leído del fichero
 */
char readFile(int fd){
    char res;
    int end;
    end = read(fd, &res, sizeof(char));
    if (end == -1) check_error(3);
    if (end == 0) res = '\0';
    return res;
}

/**
 * Escribe en el fichero correspondiente al identificador pasado por parámetro el carácter pasado por parámetro, además checkea si hay errores
 * @param fd identificador del archivo
 * @param caracter a escribir
 * @return int indicando que no han habido errores, mensaje de error si lo hubiera
 */
int writeFile(int fd, char caracter){
    if (write(fd, &caracter, sizeof(char)) == -1) check_error(4);
    return 0;
}

/**
 * [FUNCIÓN PRINCIPAL]
 * Lee el primer archivo, devolviendo el caracter a escribir. Donde se encuentre este caracter en el primer archivo se escribe en el archivo modificado.
 * @param s1, cadena a sustituir
 * @param s2, cadena que sustituye
 * @param fd1, identificador del primer archivo, donde leemos
 * @param fd2, identificador del segundo archivo, donde escribimos
 */
void trim(char* s1, char* s2, int fd1, int fd2){
    int i;
    char res;
    
    trim1:
    while((res = readFile(fd1)) != '\0'){
        for (i = 0; i < strlen(s1); i++){
            if (res == s1[i]) {
                writeFile(fd2, s2[i]);
                goto trim1;
            }
        }
        writeFile(fd2, res);
    }
}

/**
 * [FUNCIÓN OPCIONAL]
 * Lee el primer archivo, devolviendo el caracter a borrar. Donde se encuentre este caracter en el primer archivo no se escribe en el archivo modificado.
 * @param s1, cadena a borrar
 * @param fd1, identificador del primer archivo, donde leemos
 * @param fd2, identificador del segundo archivo, donde escribimos
 */
void delete(char* s1, int fd1, int fd2){
    int i;
    char res;

    del1:
    while((res = readFile(fd1)) != '\0'){
        for (i = 0; i < strlen(s1); i++){
            if (res == s1[i]) goto del1;
        }
        writeFile(fd2, res);
    }
}

/**
 * [FUNCIÓN OPCIONAL]
 * Lee el primer archivo, devolviendo el caracter que debe ser evitado de repetir. Esto se lleva a cabo a través de una operación de control (char end). Cuando se encuentra por primera vez, se escribe
 * pero cuando se encuentre seguidamente ya no.
 * @param s1, cadena a no repetir
 * @param fd1, identificador del primer archivo, donde leemos
 * @param fd2, identificador del segundo archivo, donde escribimos
 */
void squeeze(char* s1, int fd1, int fd2){
    int i;
    char res;
    char end;

    squ1:
    while((res = readFile(fd1)) != '\0'){
        for (i = 0; i < strlen(s1); i++){
            if (res == s1[i]){
                if (res != end) writeFile(fd2, res);
                end = res;
                goto squ1;
            }
        }

        end = '\0';
        writeFile(fd2, res);
    }
}

/**
 * Comprueba si los archivos pasados por parámetro son iguales.
 * @param s1 archivo comparador
 * @param s2 archivo comparador
 * @return 0 si no son iguales, 1 si lo son
 */
int isTheSame(char* s1, char* s2){
    struct stat stat1, stat2;
    // Si no se puede acceder al primer archivo, mandar mensaje de error
    if (stat(s1, &stat1) == -1) check_error(5);
    // Si no se puede acceder al segundo archivo, puede ser porque no exista, luego se retorna un -1 en vez de mensaje de error
    if (stat(s2, &stat2) == -1) return -1; //check_error(5);
    // Comprobación si ambos archivos son del mismo formato (archivo regular, archivo especial de caracter, archivo especial de bloque)
    if ((S_ISREG(stat1.st_mode) == S_ISREG(stat2.st_mode) == 0) || (S_ISCHR(stat1.st_mode) == S_ISCHR(stat2.st_mode) == 0) || (S_ISBLK(stat1.st_mode) == S_ISBLK(stat2.st_mode) == 0)) check_error(6);
    // Comprobación si se trata de un único archivo, a través del dispositivo y de los inodos. Si fuera así, retornar 1
    if ((stat1.st_dev == stat2.st_dev) && (stat1.st_ino == stat2.st_ino)) return 1;
    // Todo OK
    return 0;
}