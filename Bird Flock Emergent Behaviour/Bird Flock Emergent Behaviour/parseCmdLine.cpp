#include "parseCmdLine.h"
#include <stdio.h>

#define ERRCODE -1	//Valor regresado por parseCmdLine en caso de error durante la lectura de los argumentos
typedef enum {CADENA_VACIA,FALTA_CLAVE,FALTA_VALOR,PARAMETRO_INVALIDO} errType; //Lista de errores comunicables por l�nea de comandos

void showError(char **argv,int errorPlace,errType err); //Se encarga de mostrar por l�nea de comandos el error producido, si corresponde

int parseCmdLine(int argc, char *argv[], pCallback p, void *userData) {
	enum {INICIAL,PARAMETRO,CLAVE,VALOR,CALLBACK,FIN} parseState = INICIAL; //Este programa emplea una m�quina de estados
	int bloques = 0;	//Cantidad de opciones+par�metros le�dos
	char *key = NULL;	//Primer argumento del Callback (clave de la opci�n, o NULL si es un par�metro)
	char *value = NULL; //Segundo argumento del Callback (valor de la opci�n, o par�metro)
	int iArg = 1;		//�ndice de Argumentos; se emplea como �ndice de argv.

	while (parseState != FIN) { //Hasta finalizar la lectura...
		switch (parseState) {	//... se deriva, seg�n el estado actual.

		case INICIAL:	//Listo para leer una nueva opci�n/par�metro
			if (argv[iArg] == NULL) {	//Si el puntero es nulo, significa que termin� de recorrer argv...
				parseState = FIN;		//...finalizando la lectura
			}
			else if (argv[iArg][0] == '\0') {	//Si la cadena apuntada est� vac�a, informa el error mediante l�nea de comandos...
				showError(argv, iArg, CADENA_VACIA);
				bloques = ERRCODE;		//...y se lo informa a la funci�n usuaria...
				parseState = FIN;	//...finalizando la lectura
			}
			else if (argv[iArg][0] == '-') {	//Si la cadena empieza con "-"...
				parseState = CLAVE;				//... se trata de una clave
			}
			else {								//De lo contrario...
				parseState = PARAMETRO;			//... se trata de un par�metro
			}
			break;

		case PARAMETRO:		//Lectura de un par�metro
			value = argv[iArg];	//El segundo argumento del Callback ser� el par�metro le�do (donde el primero sigue siendo NULL)
			iArg++;				//Avanza el �ndice al pr�ximo argumento
			parseState = CALLBACK;	//Y pasa a llamar al Callback
			break;

		case CLAVE:			//Lectura de una clave
			if ( argv[iArg][1] == '\0') {	//En caso de faltar la clave (se ingres� solo el gui�n, "-"), informa el error por consola...
				showError(argv, iArg, FALTA_CLAVE);
				bloques = ERRCODE;			//...y al programa usuario...
				parseState = FIN;		//...finalizando la lectura
			}
			else {							//De lo contrario...
				key = argv[iArg] + 1;	//Asigna al primer argumento de Callback la clave le�da (sin el "-")
				iArg++;					//Avanza el �ndice al pr�ximo argumento
				parseState = VALOR;		//Y pasa a leer el valor de la opci�n
			}
			break;

		case VALOR:			//Lectura de un valor
			if (argv[iArg] == NULL) {			//Si no existe un valor para la opci�n, lo informa por l�nea de comandos...
				showError(argv, iArg, FALTA_VALOR);
				bloques = ERRCODE;			//...y lo informa a la funci�n usuario...
				parseState = FIN;		//...finalizando la lectura
			}
			else if (argv[iArg][0]== '\0') {	//Si el valor es una cadena vac�a, informa el error por l�nea de comandos...
				showError(argv, iArg, CADENA_VACIA);
				bloques = ERRCODE;			//...y a la funci�n usuario...
				parseState = FIN;		//...finalizando la lectura
			}
			else {								//De lo contrario
				value = argv[iArg];		//Asigna el segundo argumento del Callback al valor le�do
				iArg++;					//Avanza el �ndice al siguiente argumento
				parseState = CALLBACK;	//Y pasa a llamar al Callback
			}
			break;

		case CALLBACK:	//Llamado al Callback: se entregan los argumentos le�dos, y el puntero a los datos del usuario
			if ( (*p) (key,value,userData) == 1) { //si fueron interpretados correctamente...
				bloques++;				//...incrementa la cantidad de parametros+opciones analizadas...
				value = NULL;			//...resetea los argumentos pasados...
				key = NULL;
				parseState = INICIAL;	//...y se prepara para leer un nuevo argumento
			}
			else {	//Si la opci�n/parametro ingresado no fue v�lida, informa el error por l�nea de comandos...
				showError(argv, iArg-1,PARAMETRO_INVALIDO);
				bloques = ERRCODE;		//...y lo informa al progrma usuario...
				parseState = FIN;	//...finalizando la lectura
			}
			break;

		}

	}

	return bloques; //Devuelve el n�mero de opciones+par�metros encontrados (o -1, en caso de error)
}


//	SHOW ERROR
//	Funcion: informar mediante l�nea de comandos que se produjo un determinado error
//	Recibe:
//		argv: puntero al arreglo de punteros de los argumentos pasados al programa
//		errorPlace: n�mero de argumento responsable del error producido
//		err: tipo de error producido
void showError(char **argv, int errorPlace, errType err) {
	int iStr;	//Indice de Strings. Mediante argv[iStr], se accede al string del argumento actual
	int iChar;	//Indice de Caracteres. Se emplea para barrer las cadenas de caracteres de argv, y de este modo, contar sus longitudes
	unsigned int cChar = 0; //Contador de caracteres: indica el n�mero de caracteres impresos ANTES del argumento conflictivo

	printf("\n\nERROR en parseCmdLine\n");

	switch (err) {	//Seg�n el tipo de error, imprime un mensaje diferente
	case CADENA_VACIA:
		printf("Se ingreso una cadena de caracteres vacia\n\n");
		break;
	case FALTA_CLAVE:
		printf("Se ingreso una opcion sin clave\n\n");
		break;
	case FALTA_VALOR:
		printf("Se ingreso una opcion sin valor\n\n");
		break;
	case PARAMETRO_INVALIDO:
		printf("Se ingreso una Opcion/Parametro invalido (respuesta del Callback = 0)\n\n");
		break;
	}


	for (iStr = 0; argv[iStr] != NULL; iStr++) {
		printf("%s ", argv[iStr]); //Imprime uno a uno todos los argumetos pasados por l�nea de comandos al programa

		if (iStr < errorPlace) { //Para los argumentos anteriores al "conflictivo", cuenta una a una las letras que los componen
			for (iChar = 0; argv[iStr][iChar] != '\0'; iChar++) {
				cChar++;
			}
			cChar++; //Incluye en la cuenta los espacios " " entre argumentos
		}
	}

	printf("\n");	//En la siguiente l�nea...

	for (; cChar > 0; cChar--) { //...imprime tantos espacios como letras contadas ANTES del argumento conflictivo
		printf(" ");
	}
	printf("^\n\n"); //De este modo, coloca un "^" justo debajo del argumento conflicivo

	return;
}