#include<stdio.h>
int main()
{
char str[15]= "AAAAAAAAAAAAAA";
char **p = &str;


printf("%p\n",&str);
printf("%p\n",str);

printf("%d\n",str[14]);

str[13] = "?" ;

printf("%s",str);
}
