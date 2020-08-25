#include <stdio.h>
#include <readline/readline.h>

int main(void) 
{
  char *name;
  printf("enter your name:");
  scanf("%s",name);
  printf("Hello %s!\n", name);
  return 0;
}