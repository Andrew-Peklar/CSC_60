// Questions: (1) Provide the hexadecimal values for foo_ptr in all assignment's statements involved foo_ptr  
//            (2) What are the decimal values of foo and bar before the return 0 statment? 
#include <stdio.h>
int main ()
{
    int foo = 42;
    int bar = -1;
    int *foo_ptr;

    foo_ptr = & foo;

    printf ("Get the existing values of foo, bar, foo_ptr, and * foo_ptr:\n");

    printf ("foo = %d\n", foo);
    printf ("bar = %d\n", bar);
    printf ("foo_ptr = %p\n", foo_ptr);
    printf ("* foo_ptr = %d\n", * foo_ptr);

    printf ("Change the value of * foo_ptr:\n");

    *foo_ptr = 99;

    printf ("foo = %d\n", foo);
    printf ("bar = %d\n", bar);
    printf ("foo_ptr = %p\n", foo_ptr);
    printf ("* foo_ptr = %d\n", * foo_ptr);

    printf ("Change the value of foo_ptr to & bar:\n");

    foo_ptr = &bar;

    printf ("foo = %d\n", foo);
    printf ("bar = %d\n", bar);
    printf ("foo_ptr = %p\n", foo_ptr);
    printf ("* foo_ptr = %d\n", * foo_ptr);

    return 0;
}
