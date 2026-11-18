#include <stdio.h>
#include <stdbool.h> // Para usar el tipo de dato bool
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
typedef char cadena[200]; // Para tratar a los arrays de chars como 'cadena'

///////////////////////////////////////////////////////////////////////////
////////////////////////////  PROTOTIPOS  /////////////////////////////////
///////////////////////////////////////////////////////////////////////////

int leerEntero(cadena mensaje);
float leerFloat(cadena mensaje);
char leerCaracter(cadena mensaje);
int leerEnteroEntre(int valorMin, int valorMax, cadena mensaje);
float leerFloatEntre(float valorMin, float valorMax, cadena mensaje);
bool confirmaUsuario(cadena mensaje);
void leerCadena(cadena mensaje, cadena texto);
void limpiarBuffer();
void vectorMayus(cadena texto);
void clear_screen();

///////////////////////////////////////////////////////////////////////////
/////////////////////////  IMPLEMENTACIONES  //////////////////////////////
///////////////////////////////////////////////////////////////////////////

int leerEntero(cadena mensaje) {
    int entero = 0;
    printf("%s: ", mensaje);
    fflush(stdout);
    scanf("%d", &entero);
    limpiarBuffer();          // saca el \n que dejó scanf
    return entero;
}
float leerFloat(cadena mensaje) {
    float numero = 0;
    printf("%s: ", mensaje);
    fflush(stdout);
    scanf("%f", &numero);
    limpiarBuffer();
    return numero;
}

char leerCaracter(cadena mensaje) {
    char caracter;
    printf("%s: ", mensaje);
    fflush(stdout);
    scanf(" %c", &caracter);
    limpiarBuffer();
    return caracter;
}

int leerEnteroEntre(int valorMin, int valorMax, cadena mensaje) {
    int numero = 0;
    do {
        printf("%s (de %d a %d): ", mensaje, valorMin, valorMax);
        fflush(stdout);
        scanf("%d", &numero);
        limpiarBuffer();      // también descarta basura si escribió letras
        if (numero < valorMin || numero > valorMax) {
            printf("ERROR. NUMERO INVALIDO, REINGRESAR UN VALOR ENTRE %d y %d \n", valorMin, valorMax);
        }
    } while (numero < valorMin || numero > valorMax);
    return numero;
}

float leerFloatEntre(float valorMin, float valorMax, cadena mensaje) {
    printf("%s: ", mensaje);
float numero;
    do {
        printf("%s (de %f a %f): ", mensaje, valorMin, valorMax);
        scanf("%f", &numero);
        if (numero < valorMin || numero > valorMax) {
            printf("ERROR. NUMERO INVALIDO, REINGRESAR UN VALOR ENTRE %d y %d \n", valorMin, valorMax);
        }
    } while (numero < valorMin || numero > valorMax);
    return numero;
}

bool confirmaUsuario(cadena mensaje) {
    /* Muestra el mensaje al usuario junto a la leyenda "[S/N]" que
       representa "Si" o "No". Luego, lee chars mientras éstos no sean los
       previstos ('S' o 'N', incluyendo minúsculas) informando al
       usuario de su error. Retorna si el usuario seleccionó que sí. */
        char opcion; 

       printf("%s: [S/N]\n", mensaje);
       scanf(" %c", &opcion);
        if ((opcion >= 65 && opcion <= 77) || (opcion >= 79 && opcion <= 82) || (opcion >= 84 && opcion <= 90) || (opcion >= 97 && opcion <= 109) || (opcion >= 111 && opcion <= 114) || (opcion >= 116 && opcion <= 122))
       {
        do 
        {
            printf("ERROR. CARACTER NO ES VALIDO.\n");
            scanf(" %c", &opcion);
        }
        while ((opcion >= 65 && opcion <= 77) || (opcion >= 79 && opcion <= 82) || (opcion >= 84 && opcion <= 90)  || (opcion >= 97 && opcion <= 109) || (opcion >= 111 && opcion <= 114) || (opcion >= 116 && opcion <= 122)); 
       }

       if (opcion == 83 || opcion == 115)
       {
        return true;
       } 
       else if ((opcion == 78) || (opcion == 110))
       {
        return false;
       }

       
}

void leerCadena(cadena mensaje, cadena texto)
{
    printf("%s", mensaje);
    fflush(stdout);
    fgets(texto, sizeof(cadena), stdin);      // sizeof(cadena), NO sizeof(texto)
    texto[strcspn(texto, "\n")] = '\0';       // saca el \n que deja fgets

    /* ACA EXPLICO EL PROBLEMA!!!
        anteriormente, la funcion leercadena (y gran parte de utils) funcionaba como
        
        print mensaje
        limpiarbuffer
        gets

        lo cual funcionaba bien. imprime el mensaje, limpia el buffer y guarda la cadena
        en el array de texto. el problema es que "limpiarbuffer" funcionaba con getchar()

        getchar agarra los caracteres restantes adentro del buffer
        y lo guarda adentro de una variable "c", si el caracter es distinto de \n (osea, el caracter
        que se guarda en el flujo al apretar enter), limpia TODO lo que se encuentra adentro del buffer MENOS el \n

        el problema es cuando empieza la ejecucion y no hay nada adentro del buffer
        getchar() se queda "bloqueado" y espera a que se introduzca un caracter por teclado para despues borrarlo

    */

    /*
    Con esta nueva redaccion, 
    
    FFLUSH hace que el printf salga del buffer a la fuerza
    FGETS es una mejora a GETS, el cual se limita a solo el tamaño de CADENA (200 bytes).
    delinea cada string leido adentro del array de cadena con un \0, y tambien guarda el \n del enter
    por ejemplo, " 'A' 'n' 'a' '\n' '\0'"

    texto[strcspn(texto, "\n")] = '\0' ---> devuelve la posicion del \n, y en cambio la reemplaza por un \0
    funciona igual que limpiarbuffer(), pero sin el getchar que bloqueaba al esperar un input.

    */
}

void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

void vectorMayus(cadena texto)
{
    for (int i = 0; texto[i] != '\0'; i++)
    {
        texto[i] = toupper((unsigned char)texto[i]);
    }
}

void clear_screen() {
    //printf("\033[H\033[2J"); //aun se pueden ver las salidas previas
    printf("\033[H\033[2J\033[3J"); // borra las salidas previas!!
    fflush(stdout);
}