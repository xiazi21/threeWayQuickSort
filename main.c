//author Zihan Xia
//collaborator: 
//collaborator:
//Because my recitation is in tomorrow morning so I DO NOT HAVE collaborator WHEN i upload these code.
#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>
int main(void) 
{
 double f;
 char *temp= readline("Enter temperature in celsius: ");
 double c = atof(temp);
 f=(1.8*c)+32;
 printf("%f° in Celsius is equivalent to %f° Fahrenheit.\n",c,f);
return 0;
}