// Filename:  struct.c
// Questions: (1) what is the value of *std_ptr at line 27
//            (2) what are the hexadecimal values of &std_ptr->ssid and &std_ptr->age at line 27
//            (3) what is the hexadecimal value of std_ptr at line 27 ? why this value is the same as &std_ptr->ssid ? 
//            (4) what are the debug command to print out the members (ssid, age, and name) of struct csus_student after line 29 ?
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
  update_student(&student_one);
  printf ("id=%d age=%d name = %s\n", student_one.ssid, student_one.age, student_one.name);
  return 0; 
}
int update_student(struct csus_student *std_ptr) 
{
  std_ptr->ssid = 5678; // line 27
  std_ptr->age = 19;
  strcpy(std_ptr->name, "New student one"); // line 29
  return 0;
}
