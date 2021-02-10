/**
 * Función general que maneja el control de errores, muestra mensaje por pantalla indicando el error que ha sucedido
 * @param eCode, codigo de error
 */
void check_error(int eCode);

/**
 * Comprueba que ambas strings pasadas por parámetro tengan la misma longitud, si no, envía error
 * @param s1, primera cadena pasada por parámetro por línea de órdenes
 * @param s2, segunda cadena pasada por parámetro por línea de órdenes
 */
void checkLength(char* s1, char* s2);

/**
 * Abre un archivo y devuelve su identificador, además checkea si hay errores
 * @param ruta del archivo
 * @param mode que indica el modo en el que se abre (write only, read only, ...)
 * @return identificador
 */
int openFile(char* ruta, int mode);

/**
 * Crea un archivo y devuelve su identificador, además checkea si hay errores
 * @param ruta del archivo
 * @param mode que indica el modo en el que se abre
 * @return identificador
 */
int createFile(char* ruta, int mode);

/**
 * Checkea si hay errores y devuelve 0 si no. Se comporta igual que la llamada al sistema close();
 * @param fd identificador del archivo
 * @return int indicando si no hay errores, mensaje de error si lo hubiera
 */
int closeFile(int fd);

/**
 * Lee el archivo que corresponde al identificador pasado por parametro, ademaś checkea si hay errores o si ha llegado al final del archivo
 * @param fd identificador del archivo
 * @return res, carácter leído del fichero
 */
char readFile(int fd);

/**
 * Escribe en el fichero correspondiente al identificador pasado por parámetro el carácter pasado por parámetro, además checkea si hay errores
 * @param fd identificador del archivo
 * @param caracter a escribir
 * @return int indicando que no han habido errores, mensaje de error si lo hubiera
 */
int writeFile(int fd, char caracter);

/**
 * [FUNCIÓN PRINCIPAL]
 * Lee el primer archivo, devolviendo el caracter a escribir. Donde se encuentre este caracter en el primer archivo se escribe en el archivo modificado.
 * @param s1, cadena a sustituir
 * @param s2, cadena que sustituye
 * @param fd1, identificador del primer archivo, donde leemos
 * @param fd2, identificador del segundo archivo, donde escribimos
 */
void trim(char* s1, char* s2, int fd1, int fd2);

/**
 * [FUNCIÓN OPCIONAL]
 * Lee el primer archivo, devolviendo el caracter a borrar. Donde se encuentre este caracter en el primer archivo no se escribe en el archivo modificado.
 * @param s1, cadena a borrar
 * @param fd1, identificador del primer archivo, donde leemos
 * @param fd2, identificador del segundo archivo, donde escribimos
 */
void delete(char* s1, int fd1, int fd2);

/**
 * [FUNCIÓN OPCIONAL]
 * Lee el primer archivo, devolviendo el caracter que debe ser evitado de repetir. Esto se lleva a cabo a través de una operación de control (char end). Cuando se encuentra por primera vez, se escribe
 * pero cuando se encuentre seguidamente ya no.
 * @param s1, cadena a no repetir
 * @param fd1, identificador del primer archivo, donde leemos
 * @param fd2, identificador del segundo archivo, donde escribimos
 */
void squeeze(char* s1, int fd1, int fd2);

/**
 * Comprueba si los archivos pasados por parámetro son iguales.
 * @param s1 archivo comparador
 * @param s2 archivo comparador
 * @return 0 si no son iguales, 1 si lo son
 */
int isTheSame(char* s1, char* s2);