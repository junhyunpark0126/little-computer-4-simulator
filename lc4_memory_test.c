/************************************************************************/
/* File Name : lc4_memory_test.c		 								*/
/* Purpose   : This file is for part1 of the assignment only			*/
/* 			   It allows you to test functions in lc4_memory.c			*/
/*             															*/
/* Author(s) : tjf and you												*/
/************************************************************************/

#include <stdio.h>
#include "lc4_memory.h"


int main ()
{

    /* This main() is simply to test the basic functionalty of lc4_memory.c for part 1.
       It must not be used for part 2 of the assignment
    */

    /* Step 1: create a local linked list head pointer and allocate memory to it using 'add_to_list' */
	row_of_memory* test_list = NULL ;
	add_to_list(&test_list, 0x4000, 0x0001);

    /* Step 2: test your linked list by adding some 'dummy' data: 
               add_to_list(..., 0x0000, 0x1234), add_to_list(..., 0x0001, 0x5678), ... */
	add_to_list(&test_list, 0x3000, 0x0002);
	add_to_list(&test_list, 0x2000, 0x0003);
	add_to_list(&test_list, 0x5000, 0x0004);
	add_to_list(&test_list, 0x3500, 0x0005);
    
	/* Step 3: print your list, see it works! */
	FILE* my_file = fopen("test.txt", "w");
	print_list(test_list, my_file);

    /* Step 4: try adding data to the list out of order: 
             add_to_list(..., 0x0003, 0x9ABC), add_to_list(..., 0x0002, 0xDEF0) */
	add_to_list(&test_list, 0x1000, 0x0006);
	add_to_list(&test_list, 0x9999, 0x0007);
	add_to_list(&test_list, 0x1500, 0x0008);

    /* Step 5: print your list, ensure it is in ascedning order by address */
	print_list(test_list, my_file);
    
	/* Step 6: delete a single node, say the one with address: 0x0002 */
	delete_from_list(&test_list, 0x2000);

    /* Step 7: print your list (to FILE:stdout), ensure that the list is still intact */
	print_list(test_list, my_file);

    /* Step 8: try deleting the first node in the list and then 
               print your list, ensure that the list is still intact */
	delete_from_list(&test_list, 0x1000);
	delete_from_list(&test_list, 0x1500);
	delete_from_list(&test_list, 0x5000);
	delete_from_list(&test_list, 0x9999);
	print_list(test_list, my_file);

	delete_from_list(&test_list, 0xABCD); // shouldn't do anything

	delete_list(&test_list);


    /* Step 9: when finished, run valgrind on your program to see if there are any leaks
               points will be lost even if your program is working but leaking memory */
	fclose(my_file);

	return 0;
}