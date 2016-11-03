// pfunction.c - to demo the function pointer technique
// Compile option: gcc pfunction.c -lm -g
// Questions: (1) where are the function pointers initialized ?
//            (2) how would you validate that the sin and cos functions are executed as advertised?  
//                See line 41.
#include <math.h>
#include <stdio.h>
void tabulate(double (*f)(double), double first, double last, double incr);
double mycos(double x);
double mysine(double x);
main()
{
		double final, increment, initial;
                double  (*fp)(double);
		printf ("Enter initial value: ");
		scanf ("%lf", &initial);
		printf ("Enter final value: ");
		scanf ("%lf", &final);
		printf ("Enter increment : ");
		scanf ("%lf", &increment);
                fp = mycos;
                printf("\nPointer function address for cos: %p\n",fp);
		printf("\n      x          cos(x) \n"
		         "  ----------  -----------\n");
		tabulate(fp, initial,final,increment);
                fp = mysine;
                printf("\nPointer function address for sin: %p\n",fp);
		printf("\n      x          sin (x) \n"
		"  ----------  -----------\n");
		tabulate(fp, initial,final,increment);
		return 0;
}
// when passed a pointer f prints a table showing the value of f
 void tabulate(double (*f) (double), double first, double last, double 		incr)
{
 	   double x;
           int i, num_intervals;
           num_intervals = ceil ( (last -first) /incr );
           for (i=0; i<=num_intervals; i++){
               x= first +i * incr;
               printf("%10.5f %10.5f\n", x , f(x)); // line 41
               // Can do this too: printf("%10.5f %10.5f\n", x , (*f) (x));
           }
}
double mycos(double x)
{
   return(cos(x));
}
double mysine(double x)
{
   return(sin(x));
}
