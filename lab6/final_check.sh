#!/bin/bash
if [ $# -ne 2 ] 
   then
    echo "Incorrect inputs"
    echo "try i.e. final_check csus_student_id sem_id"
    exit
fi
echo "*******************" 
echo "Date:" 
date
echo "*******************" 
echo "Student:" 
whoami
echo "*******************" 
echo "Student Id:"
echo $1
echo "*******************" 
echo "ps u"
ps u 
echo "*******************" 
echo "ipcs:"
ipcs -s -i $2
