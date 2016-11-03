// Filename:  structval.c
// Questions: (1) what are the values for the CSUS struct's member before calling test_update_student 
//            (2) what are the values for the CSUS struct's member after calling test_update_student 
//            (3) explain the outcome ? 
// 
#include <stdio.h>
#include <string.h>
struct csus_student {
     signed int ssid;	/* social security id */
     signed int age; 	/* name */ 
     char name[50];     /* name */  
};
typedef struct csus_student CsusStudent;

int main()
{
  CsusStudent studentone;
  struct csus_student student_one = { 1234,  18, "Student One" };
  printf ("id=%d age=%d name = %s\n", student_one.ssid, student_one.age, student_one.name);
  test_update_student(student_one);
  printf ("id=%d age=%d name = %s\n", student_one.ssid, student_one.age, student_one.name);
  return 0; 
}
int test_update_student(struct csus_student temp_struct ) 
{
  temp_struct.ssid = 5678; // line 27
  temp_struct.age = 19;
  strcpy(temp_struct.name, "New student one"); // line 29
  return 0;
}
