// Questions: (1) what is the hexadecimal of aptr right after line 16 is executed ?
//            (2) what is the value of debug command "p/x array_ptr - aptr" right after line 16 is executed?
//                Explain why you have this result ?
//            (3) what is the decimal value of *aptr after line 17 is executed ?
//            (4) what is the decimal value of *(array+2) 
#include <stdio.h>
int main ()
{
    int array[] = { 45, 67, 89 };
    int *array_ptr;
    int *aptr;
    array_ptr = array;
    printf(" first element: %i\n", *(array_ptr++));
    printf(" second element: %i\n", *(array_ptr++));
    printf(" third element: %i\n", *array_ptr);
    aptr = &array[0]; // line 16
    aptr = array_ptr; // line 17
    return 0;
}

