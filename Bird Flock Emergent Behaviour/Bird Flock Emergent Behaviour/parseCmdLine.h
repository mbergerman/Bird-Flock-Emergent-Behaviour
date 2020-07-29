#ifndef PARSECMDLINE_H
#define PARSECMDLINE_H

/*
	PARSE COMMAND LINE
	parseCmdLine es una funcion encargada de procesar los argumentos recibidos por línea de comandos.

	NOTA:
		Opciones: constan de 2 argumentos; una clave, precedida por "-", y su valor.
	Ejemplo: progrma.exe -clave argumento
	El valor puede comenzar con "-"
		Parámetros: constan de 1 argumento, el cual NO puede comenzar con "-"
	Ejemplo: programa.exe argumento

	Recibe:
		argc, contador de argumentos;
		argv, vector de argumentos; estos dos datos son recibidos por el main del programa usuario, y deben ser
	pasados a parseCmdLine tal como son recibidos al iniciar el programa
		p, puntero a función callback. Esta función debe ser definida por el usuario, y debe ser capaz de
	interpretar y evaluar la validez de las opciones y parámetros pasados por línea de comandos, para preparar
	al programa principal para su accionar
		userData, puntero genérico. parseCmdLine no modifica ni lee el contenido apuntado; unicamente pasa dicho
	puntero a la función Callback. Éste es el medio por el cual el Callback se comunicará con el programa principal

	Devuelve:
		Número de argumentos y opciones ingresadas.
		En caso de error (opción sin clave o valor, argumento inválido), devuelve -1.

	Modo de uso:
		En primer lugar, el usuario deberá programar un callback. Éste recibirá de parseCmdLine dos cadenas de
	caracteres por cada opción o parámetro leído desde la línea de comandos; el primero (key) contiene la clave de
	la opción ingresada (sin el "-"), o es un puntero nulo en caso de haberse ingresado un parámetro. El segundo
	(value), contiene el valor de la opción pasada, o el contenido del parámetro, según corresponda.
		En función del parametro u opción recibida, el Callback debe informar al programa principal de las operaciones
	que éste deberá realizar una vez finalizada la lectura de la línea de comandos. Para ello, dispone de un puntero
	genérico (userData), cuyo contenido e inicialización deben ser definidas por el usuario. Estos Datos del Usuario,
	idealmente, deben ser modificados ante cada llamado al Callback, según los datos pasados, y luego deben ser
	interpretados por el programa principal.
		El Callback deberá informarle a parseCmdLine si la opción o parámetro ingresados son válidos; en caso
	afirmativo, deberá devolver 1; de presentarse un error en la interpretación, deberá devolver 0.

		Una vez programado el Callback, y habiendo definido los Datos del Usuario, para llamar a parseCmdLine
	es necesario entregarle los valores de argc y argv (recibidos por el main), donde argc contiene el número de
	argumetos pasados al programa, y argv es un arreglo de punteros a cadenas de caracteres, los cuales contienen
	dichos argumentos.
		parseCmdLine devuelve el número de opciones y argumentos ingresados por el usuario. De ocurrir un error
	durante la lectura (parámetro inválido, opción sin clave o valor, cadena de caracteres vacía), devuelve -1,
	e imprime en línea de comandos el error producido, indicando el argumento conflictivo.

	CONSIDERACIONES:
	-Las cadenas de caracteres pasadas al Callback (mediante sus punteros) NO son una copia, sinó las originales,
	accesibles mediante argv. Al no crear cadenas auxiliares para pasar los argumentos, se evita la necesidad de
	definir una longitud máxima para los mismos. Se desalienta la modificación de estas cadenas por el Callback;
	el uso de ésta librería permite eliminar la necesidad de manipular de manera directa los argumentos pasados
	por línea de comandos.


*/

typedef int (*pCallback) (char *key, char *value, void *userData);

int parseCmdLine(int argc, char *argv[], pCallback p, void *userData);

#endif