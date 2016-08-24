#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

	int a; int b; int *p;
	a= 10;
	b= 15;
	//p = malloc(sizeof(int));
	p = &a;

	printf("\nEl tamaño de un entero es de: %d", (int)sizeof(int));

	printf("\nla direccion de a es: ");
	printf("%lu", (long)&a);
	printf("\nla direccion de b es: ");
	printf("%lu", (long)&b);
	printf("\nla direccion de p es: ");
	printf("%lu", (long)&p);

	printf("\n\n");


	return EXIT_SUCCESS;
}