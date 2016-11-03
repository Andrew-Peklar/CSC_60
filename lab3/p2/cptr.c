// Question: Explain the compiler's errors reported on line 8, 11, and 14
#include <stdio.h>
int main()
{
	int a, b;
	b = a = 7; /* a is an integer */
	const int *aPtr1 = &a; /* aPtr1 is a pointer to an integer of a */
        *aPtr1 = 9;
	//
	int *const aPtr2 = &a; /* aPtr2 is a pointer to an integer of a */
        aPtr2 = &b;
        //
        const int *const aPtr3 = &a;  /* aPtr2 is a pointer to an integer of a */
        *aPtr3 = 329;
        //
	return 0;
}
