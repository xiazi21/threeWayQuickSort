#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>
int main(void) 
{
  double f;
 char *temp= readline("Enter temperature in celsius: ");
 double c = atof(temp);
 f=(1.8*c)+32;
 printf("%f° in Celsius is equivalent to %f° Fahrenheit.",c,f);
return 0;
}