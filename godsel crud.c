
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "utils.h"
#include <stdlib.h> 

#define MAX 40
#define MAX_ING 30

typedef struct {
    cadena nombre;
    int magnitud;
    cadena tipoMagnitud;  // GR, ML o MIN
} Ingrediente;

typedef struct {
    cadena usuarioNombre;
    int usuarioTipo;
} Usuario;


typedef struct {
    cadena nombre;
    Ingrediente ingredientes[MAX_ING];
    int cantidadIngredientes; //para imprimir todo con un for en mostraringredientes y para limitar la cantidad de scanf usados en agregarreceta
    Usuario Autor;
} Receta;


// PROTOTIPOS
void mostrarmenu(int menuAMostrar);
void agregarReceta(Receta recetas[MAX], int *cantidadRecetas, Usuario Usuario);
void mostrarIngredientes(Receta receta);
void mostrarRecetas(Receta recetas[MAX], int cantidadRecetas, Usuario Usuario);
void buscarReceta(Receta recetas[MAX], int cantidadRecetas);
void ordenarRecetas(Receta recetas[MAX], int cantidadRecetas);
void modificarReceta(Receta recetas[MAX], int *cantidadRecetas, Usuario Usuario);
void modificarIngrediente(Receta *recetas, Usuario Usuario); //se pasa un puntero para tener acceso completo
void testRecipeDummy(Receta recetas[MAX], int *cantidadRecetas);
int sistemaBusquedas(Receta recetas[MAX], cadena valor, int cantidadRecetas);
void ingresarUsuario(Usuario *Usuario);
void writeVariableFile(Receta recetas[MAX], int cantidadRecetas);
void writeBinaryFiles(Receta Recetas[MAX], int cantidadRecetas);
void readBinaryFiles(Receta Recetas[MAX], int *cantidadRecetas);

// -------------------- MAIN --------------------


int main()
{
    Receta recetas[MAX];
    int cantidadRecetas = 0;
    int opcion = 0;
    Usuario Usuario;
    ingresarUsuario(&Usuario);       
    do
    {
        
        ordenarRecetas(recetas, cantidadRecetas);
        printf("Bienvenidx, %s\n", Usuario.usuarioNombre);
        mostrarmenu(1);
        opcion = leerEnteroEntre(1, 10, "Ingrese una opcion: ");
        clear_screen();
        switch (opcion)
        {
            case 1:
                agregarReceta(recetas, &cantidadRecetas, Usuario); //se pasa la direccion de catnidadrecetaas
                break;

            case 2:
                mostrarRecetas(recetas, cantidadRecetas, Usuario);
                break;

            case 3:
                printf("Saliendo del programa...\n");
                break;

            case 4:
                buscarReceta(recetas, cantidadRecetas);
                break;
            case 5:
                modificarReceta(recetas, &cantidadRecetas, Usuario);
                break;
            case 6:
                testRecipeDummy(recetas, &cantidadRecetas);
                break;
            case 7:
            //cambiarUsuario
            ingresarUsuario(&Usuario);
            break;
            case 8:
            writeVariableFile(recetas, cantidadRecetas);
            break;
            case 9:
            writeBinaryFiles(recetas, cantidadRecetas);
            break;
            case 10:
            readBinaryFiles(recetas, &cantidadRecetas);
            break;
        }

    } while (opcion != 3);

    return 0;
}



// -------------------- MENU --------------------

void mostrarmenu(int menuAMostrar)
{
    switch (menuAMostrar)
    {
    case 1:
        printf("1. add recipe, 2 check recipes, 3 leave program, 4 look for recipe, 5 modify recipe, 6 test recipe, 7 change user, 8 create txt file, 9 create binary, 10 add recipes via binary");
        break;

    case 2:
        printf("1. enter as user. 2 enter as admin. 3 enter as guest");
        break;
    }
}


// -------------------- AGREGAR RECETA --------------------


void agregarReceta(Receta recetas[MAX], int *cantidadRecetas, Usuario Usuario)
{
    int pos = *cantidadRecetas;
    int opcMagnitud, opcIngrediente;
    bool max_ing_alcanzados;
    max_ing_alcanzados = false;

    if (pos >= MAX)
    {
        printf("Se alcanzo el maximo de recetas.\n");
        return;
    }

    if (Usuario.usuarioTipo == 3)
    {
        clear_screen();
        printf("Por favor, registre su usuario antes de crear una receta.\n");
        return;
    }

    leerCadena("Nombre de la receta: ", recetas[pos].nombre);
    vectorMayus(recetas[pos].nombre);

    recetas[pos].cantidadIngredientes = 0;   // de la receta nueva, no de la 0

    do
    {
        int n = recetas[pos].cantidadIngredientes;   // indice del ingrediente actual

        printf("DATOS DEL INGREDIENTE NUMERO %d\n", n + 1);
        leerCadena("Nombre del ingrediente: ", recetas[pos].ingredientes[n].nombre);
        vectorMayus(recetas[pos].ingredientes[n].nombre);
        recetas[pos].ingredientes[n].magnitud = leerEntero("Cantidad del ingrediente: ");

        opcMagnitud = leerEnteroEntre(1, 4, "1. Gramos\n2. Mililitros\n3. Minutos\n4. Unidades\n");
        switch (opcMagnitud)
        {
            case 1: strcpy(recetas[pos].ingredientes[n].tipoMagnitud, "GR"); break;
            case 2: strcpy(recetas[pos].ingredientes[n].tipoMagnitud, "ML"); break;
            case 3: strcpy(recetas[pos].ingredientes[n].tipoMagnitud, "MIN"); break;
            case 4: strcpy(recetas[pos].ingredientes[n].tipoMagnitud, "UNIDADES"); break;
        }
        recetas[pos].cantidadIngredientes++;

        if (recetas[pos].cantidadIngredientes >= MAX_ING)
        {
            printf("Se alcanzo el maximo de ingredientes.\n");
            max_ing_alcanzados = true;
            opcIngrediente = 2;
        }
        if (max_ing_alcanzados == false)
        {
        opcIngrediente = leerEnteroEntre(1, 2, "Desea agregar otro ingrediente? [1: SI / 2: NO]");
        }
    } while (opcIngrediente == 1);

    strcpy(recetas[pos].Autor.usuarioNombre, Usuario.usuarioNombre);
    recetas[pos].Autor.usuarioTipo = Usuario.usuarioTipo;
    (*cantidadRecetas)++;
    printf("Receta agregada correctamente.\n");
}




// -------------------- MOSTRAR RECETAS --------------------


void mostrarIngredientes(Receta receta)
{
    printf("Ingredientes:\n");

    for (int i = 0; i < receta.cantidadIngredientes; i++) //recorre todos los datos del struct de ingredientes.
    //es obligatorio que cada variable tenga los datos completos, asi que con una sola I/for queda perfecto
    {
        printf("- %s | Cantidad: %d %s\n",
            receta.ingredientes[i].nombre,
            receta.ingredientes[i].magnitud,
            receta.ingredientes[i].tipoMagnitud);
    }
}

void mostrarRecetas(Receta recetas[MAX], int cantidadRecetas, Usuario Usuario)
{
    if (cantidadRecetas == 0)
    {
        printf("No hay recetas para mostrar.\n");
        return;
    }

    for (int i = 0; i < cantidadRecetas; i++)
    {
        printf("\nReceta %d: %s\n", i + 1, recetas[i].nombre); //muestra el nombre de la receta
        printf("Autor: %s\n", recetas[i].Autor.usuarioNombre); 
        
        switch (recetas[i].Autor.usuarioTipo)
        {
            case 1: 
            printf("ROL: USUARIO\n");
            break;

            case 2:
            printf("ROL: ADMIN\n");
        }

        mostrarIngredientes(recetas[i]); //llama a la funcion de arriba (tecnicamente funciona como un for anidado).
        //en este for la i representa cada receta, en la otra funcion representa cada ingrediente.
    }
}

// -------------------- BUSCAR RECETA --------------------


int sistemaBusquedas(Receta recetas[MAX], cadena valor, int cantidadRecetas)
{
    int pos = 0;

    while (strcmp(recetas[pos].nombre, valor) != 0) 
    {
        pos++;
    }

    if (pos < cantidadRecetas)
        return pos;

    return -1;
}

void buscarReceta(Receta recetas[MAX], int cantidadRecetas)
{
    cadena recetaABuscar;
    int posValor;

    if (cantidadRecetas == 0)
    {
        printf("No hay recetas para buscar.\n");
        return;
    }

    leerCadena("Que receta desea buscar? ", recetaABuscar);
    vectorMayus(recetaABuscar);

    posValor = sistemaBusquedas(
        recetas, recetaABuscar, cantidadRecetas
    );

    if (posValor != -1)
    {
        printf("\nReceta encontrada: %s\n", recetas[posValor].nombre);
        mostrarIngredientes(recetas[posValor]);
    }
    else
    {
        printf("No se encontro esa receta.\n");
    }
}

// -------------------- ORDENAR RECETAS --------------------


void ordenarRecetas(Receta recetas[MAX], int cantidadRecetas)
{
    Receta temporal;

    for (int i = 0; i < cantidadRecetas - 1; i++)
    {
        for (int j = 0; j < cantidadRecetas - 1 - i; j++)
        {
            if (strcmp(recetas[j].nombre,
                       recetas[j + 1].nombre) > 0)
            {
                temporal = recetas[j];
                recetas[j] = recetas[j + 1];
                recetas[j + 1] = temporal;
            }
        }
    }
}

// -------------------- BUSQUEDA LINEAL --------------------

// -------------------- MODIFICAR RECETA --------------------

void modificarReceta(Receta recetas[MAX], int *cantidadRecetas, Usuario Usuario)
{
    int IDRecetaAModificar, idIng, opcMenu;

    if (*cantidadRecetas == 0)
    {
        printf("No hay recetas para modificar.\n");
        return;
    }

    if (Usuario.usuarioTipo == 3)
    {
        clear_screen();
        printf("Por favor, registre su usuario para poder modificar recetas.\n");
        return;
    }
    else
    {
        mostrarRecetas(recetas, *cantidadRecetas, Usuario);
    IDRecetaAModificar = leerEnteroEntre(1, *cantidadRecetas, "RECIPE TO MODIFY: ");
    IDRecetaAModificar--;

    int test = strcmp(Usuario.usuarioNombre, recetas[IDRecetaAModificar].Autor.usuarioNombre);
    if (test != 0 && Usuario.usuarioTipo == 1)
    {
        printf("No puedes modificar recetas ajenas, bastardo\n");
        return;
    }

    opcMenu = leerEnteroEntre(1, 4, "1. NAME\n2. DELETE RECIPEa\n3. CHANGE INGREDIENTS\n4. Salir\n");

    switch (opcMenu)
    {
    case 1:
        leerCadena("Nuevo nombre de la receta: ", recetas[IDRecetaAModificar].nombre);
        vectorMayus(recetas[IDRecetaAModificar].nombre);
        break;
    case 2:
        recetas[IDRecetaAModificar] = recetas[*cantidadRecetas - 1]; 
        (*cantidadRecetas)--;
        printf("Receta borrada correctamente.\n");
        break;

    case 3:
        modificarIngrediente(&recetas[IDRecetaAModificar], Usuario);
        break;

    case 4:
        printf("Saliendo del menu de modificacion...\n");
        break;      
    }
   }
}

void modificarIngrediente(Receta *receta, Usuario Usuario)
{
    int idIng, opcionMenu, opcMagnitud;

    mostrarIngredientes(*receta);
    printf("1. Nombre Ing\n2. Cantidad Ing\n3. Tipo Magnitud\n4.Borrar Ingrediente\n5. Agregar Ingrediente\n6. Salir\n");
    opcionMenu = leerEnteroEntre(1, 6, "Opcion: ");  
    if (opcionMenu != 5 && opcionMenu != 6)
    {
    idIng = leerEnteroEntre(1, receta->cantidadIngredientes, "Ingrese el numero del ingrediente a modificar: ") - 1;
    }
    

    switch (opcionMenu)
    {
        case 1:
            leerCadena("Nuevo nombre del ingrediente: ", receta->ingredientes[idIng].nombre);
            vectorMayus(receta->ingredientes[idIng].nombre);
            break;

        case 2:
            receta->ingredientes[idIng].magnitud = leerEntero("Nueva cantidad del ingrediente: ");
            break;

        case 3:
            opcMagnitud = leerEnteroEntre(1, 4, "1. Gramos\n2. Mililitros\n3. Minutos\n4. Unidades\n");
            if (opcMagnitud == 1)
            {
                strcpy(receta->ingredientes[idIng].tipoMagnitud, "GR");
            }
            if (opcMagnitud == 2)
            {
                strcpy(receta->ingredientes[idIng].tipoMagnitud, "ML");
            }
            if (opcMagnitud == 3)
            {
                strcpy(receta->ingredientes[idIng].tipoMagnitud, "MIN");
            }
            if (opcMagnitud == 4)
            {
                strcpy(receta->ingredientes[idIng].tipoMagnitud, "UNIDADES");
            }
            break; 

        case 4:
            for (int i = idIng; i < receta->cantidadIngredientes - 1; i++)
            {
                receta->ingredientes[i] = receta->ingredientes[i + 1];
            }

            receta->ingredientes[receta->cantidadIngredientes - 1].nombre[0] = '\0';
            receta->ingredientes[receta->cantidadIngredientes - 1].magnitud = 0;
            receta->ingredientes[receta->cantidadIngredientes - 1].tipoMagnitud[0] = '\0';

            receta->cantidadIngredientes--;
            printf("Ingrediente borrado correctamente.\n");
            break;

            case 5:
            int IDIngredienteNuevo;
            IDIngredienteNuevo = (receta->cantidadIngredientes);

        if (IDIngredienteNuevo >= MAX_ING)
        {
            printf("Se alcanzo el maximo de ingredientes.\n");
            return;
        }
        else 
        {
        leerCadena("Nombre del ingrediente: ", receta->ingredientes[IDIngredienteNuevo].nombre);
        vectorMayus(receta->ingredientes[IDIngredienteNuevo].nombre);

        receta->ingredientes[IDIngredienteNuevo].magnitud =
            leerEntero("Cantidad del ingrediente: ");

        opcMagnitud = leerEnteroEntre(1, 4, "1. Gramos\n2. Mililitros\n3. Minutos\n4. Unidades\n");

        switch (opcMagnitud)
        {
            case 1:
                strcpy(receta->ingredientes[IDIngredienteNuevo].tipoMagnitud, "GR");
                break;

            case 2:
                strcpy(receta->ingredientes[IDIngredienteNuevo].tipoMagnitud, "ML");
                break;

            case 3:
                strcpy(receta->ingredientes[IDIngredienteNuevo].tipoMagnitud, "MIN");
                break;
            case 4:
                strcpy(receta->ingredientes[IDIngredienteNuevo].tipoMagnitud, "UNIDADES");
                break;
        }
        }
        receta->cantidadIngredientes++;
            break;

            case 6:
            printf("Saliendo del menu...\n");
            break;
}
}


void testRecipeDummy(Receta recetas[MAX], int *cantidadRecetas)

{
    strcpy(recetas[0].nombre, "PASTA");
    strcpy(recetas[0].Autor.usuarioNombre, "USUARIODEPRUEBA");
    recetas[0].Autor.usuarioTipo = 1;
    recetas[0].cantidadIngredientes = 2;
    strcpy(recetas[0].ingredientes[0].nombre, "FIDEOS");
    recetas[0].ingredientes[0].magnitud = 200;
    strcpy(recetas[0].ingredientes[0].tipoMagnitud, "GR");
    strcpy(recetas[0].ingredientes[1].nombre, "SALSA");
    recetas[0].ingredientes[1].magnitud = 100;
    strcpy(recetas[0].ingredientes[1].tipoMagnitud, "ML");

    strcpy(recetas[1].nombre, "ENSALADA");
    strcpy(recetas[1].Autor.usuarioNombre, "ADMINDEPRUEBA");
    recetas[1].Autor.usuarioTipo = 2;
    recetas[1].cantidadIngredientes = 3;
    strcpy(recetas[1].ingredientes[0].nombre, "LECHUGA");
    recetas[1].ingredientes[0].magnitud = 100;
    strcpy(recetas[1].ingredientes[0].tipoMagnitud, "GR");
    strcpy(recetas[1].ingredientes[1].nombre, "TOMATE");
    recetas[1].ingredientes[1].magnitud = 50;
    strcpy(recetas[1].ingredientes[1].tipoMagnitud, "GR");
    strcpy(recetas[1].ingredientes[2].nombre, "ADEREZO");
    recetas[1].ingredientes[2].magnitud = 30;
    strcpy(recetas[1].ingredientes[2].tipoMagnitud, "ML");

    *cantidadRecetas = 2;
}

void ingresarUsuario(Usuario *Usuario)
{
    mostrarmenu(2);
    Usuario->usuarioTipo = leerEnteroEntre(1, 3, "");
    
    if (Usuario->usuarioTipo != 3)
    {
    leerCadena("Cual es su nombre de usuario? ", Usuario->usuarioNombre);
    vectorMayus(Usuario->usuarioNombre);
    }
    else
    {
        strcpy(Usuario->usuarioNombre, "INVITADO"); 
    }
    clear_screen();
}

void writeVariableFile(Receta Recetas[], int cantidadRecetas)
{
    FILE* fp;
    fp = fopen("recetasTexto.txt", "w");
    fprintf(fp, "---------------------------------------\n");
    for (int i = 0; i < cantidadRecetas; i++)
    {
    fprintf(fp, "RECETA N°%d | %s  \nAUTOR: %s\n\n", (i+1), Recetas[i].nombre, Recetas[i].Autor.usuarioNombre);
    for (int j = 0; j < Recetas[i].cantidadIngredientes; j++)
    {
    fprintf(fp, "-%s | %d %s\n", Recetas[i].ingredientes[j].nombre, Recetas[i].ingredientes[j].magnitud, Recetas[i].ingredientes[j].tipoMagnitud); 
    }
    fprintf(fp, "---------------------------------------\n");
    }
    fclose(fp);
}

void writeBinaryFiles(Receta Recetas[], int cantidadRecetas)
{
    FILE *fp = fopen("datosBinarios", "wb");

    if (fp == NULL)
    {
        printf("Error al abrir el archivo!\n");
        return;
    }

    fwrite(&cantidadRecetas, sizeof(int), 1, fp);

    for (int i = 0; i < cantidadRecetas; i++)
    {
        fwrite(Recetas[i].nombre, sizeof(cadena), 1, fp);
        fwrite(Recetas[i].Autor.usuarioNombre, sizeof(cadena), 1, fp);
        fwrite(&Recetas[i].Autor.usuarioTipo, sizeof(int), 1, fp);
        fwrite(&Recetas[i].cantidadIngredientes, sizeof(int), 1, fp);

        for (int j = 0; j < Recetas[i].cantidadIngredientes; j++)
        {
            fwrite(Recetas[i].ingredientes[j].nombre, sizeof(cadena), 1, fp);
            fwrite(&Recetas[i].ingredientes[j].magnitud, sizeof(int), 1, fp);
            fwrite(Recetas[i].ingredientes[j].tipoMagnitud, sizeof(cadena), 1, fp);
        }
    }

    fclose(fp);
    printf("Recetas guardadas correctamente.\n");
}

void readBinaryFiles(Receta Recetas[], int *cantidadRecetas)
{
    FILE *fp = fopen("datosBinarios", "rb");

    if (fp == NULL)
    {
        printf("No se pudo abrir el archivo.\n");
        return;
    }

    fread(cantidadRecetas, sizeof(int), 1, fp);   // sin &, ya es un puntero

    if (*cantidadRecetas < 0 || *cantidadRecetas > MAX)
    {
        printf("Archivo invalido.\n");
        *cantidadRecetas = 0;
        fclose(fp);
        return;
    }

    for (int i = 0; i < *cantidadRecetas; i++)
    {
        fread(Recetas[i].nombre, sizeof(cadena), 1, fp);
        fread(Recetas[i].Autor.usuarioNombre, sizeof(cadena), 1, fp);
        fread(&Recetas[i].Autor.usuarioTipo, sizeof(int), 1, fp);
        fread(&Recetas[i].cantidadIngredientes, sizeof(int), 1, fp);

        if (Recetas[i].cantidadIngredientes < 0 || Recetas[i].cantidadIngredientes > MAX_ING)
        {
            Recetas[i].cantidadIngredientes = 0;
        }

        for (int j = 0; j < Recetas[i].cantidadIngredientes; j++)
        {
            fread(Recetas[i].ingredientes[j].nombre, sizeof(cadena), 1, fp);
            fread(&Recetas[i].ingredientes[j].magnitud, sizeof(int), 1, fp);
            fread(Recetas[i].ingredientes[j].tipoMagnitud, sizeof(cadena), 1, fp);
        }
    }

    fclose(fp);
    printf("Se cargaron %d recetas.\n", *cantidadRecetas);
}