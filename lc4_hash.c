/************************************************************************/
/* File Name : lc4_hash.c		 										*/
/* Purpose   : This file contains the definitions for the hash table  	*/
/*																		*/
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "lc4_hash.h"

/*
 * creates a new hash table with num_of_buckets requested
 */
lc4_memory_segmented* create_hash_table (int num_of_buckets, 
             					         int (*hash_function)(void* table, void *key) ) 
{
	int i;
	// allocate a single hash table struct
	lc4_memory_segmented* hash_table_struct = malloc(sizeof(lc4_memory_segmented));
	if (hash_table_struct == NULL) {
		fprintf(stderr, "no space on heap under create_hash_table()\n");
		return NULL;
	}

	// allocate memory for the buckets (head pointers)
	hash_table_struct->num_of_buckets = num_of_buckets;
	hash_table_struct->buckets = malloc(sizeof(row_of_memory*) * num_of_buckets);
	if (hash_table_struct->buckets == NULL) {
		fprintf(stderr, "no space on heap under create_hash_table()\n");
		return NULL;
	}

	// nullify each bucket initially
	for (i = 0; i < num_of_buckets; i++) {
		hash_table_struct->buckets[i] = NULL;
	}

	// assign function pointer to call back hashing function
	hash_table_struct->hash_function = hash_function;

	// return 0 for success, -1 for failure
	return hash_table_struct;
}


/*
 * adds a new entry to the table
 */
int add_entry_to_tbl (lc4_memory_segmented* table, 
					  unsigned short int address,
			    	  unsigned short int contents) 
{
	// apply hashing function to determine proper bucket #
	// add to bucket's linked list using linked list add_to_list() helper function
	
	if (table == NULL) {
		return -1;
	}

	int hash_index = table->hash_function(table, &address);

	int output = add_to_list(&table->buckets[hash_index], address, contents);

	if (output != 0) {
		fprintf(stderr, "output not valid under add_entry_to_tbl()\n");
		return -1;
	}

	return 0 ;
}

/*
 * search for an address in the hash table
 */
row_of_memory* search_tbl_by_address 	(lc4_memory_segmented* table,
			                   			 unsigned short int address ) 
{
	// apply hashing function to determine bucket # item must be located in
	// invoked linked_lists helper function, search_by_address() to return return proper node

	if (table == NULL) {
		fprintf(stderr, "table was null under search_tbl_by_address()\n");
		return NULL;
	}

	int hash_index = table->hash_function(table, &address);

	row_of_memory* output = search_address(table->buckets[hash_index], address);

	if (output == NULL) {
		return NULL;
	}

	return output;
}

/*
 * prints the linked list in a particular bucket
 */

void print_bucket (lc4_memory_segmented* table, 
				   int bucket_number,
				   FILE* output_file ) 
{
	// call the linked list helper function to print linked list

	if (table == NULL) {
		fprintf(stderr, "Table is Null under print_bucket\n");
		return;
	}

	print_list(table->buckets[bucket_number], output_file);

	return ;
}

/*
 * print the entire table (all buckets)
 */
void print_table (lc4_memory_segmented* table, 
				  FILE* output_file ) 
{
	// call the linked list helper function to print linked list to output file for each bucket

	int i;

	if (table == NULL) {
		fprintf(stderr, "Table is Null under print_table()\n");
		return;
	}

	for (i = 0; i < table->num_of_buckets; i++) {
		print_bucket(table, i, output_file);
	}

	return ;
}

/*
 * delete the entire table and underlying linked lists
 */

void delete_table (lc4_memory_segmented* table ) 
{
	// call linked list delete_list() on each bucket in hash table
	// then delete the table itself

	int i;

	if (table == NULL) {
		fprintf(stderr, "Table is Null under print_bucket\n");
		return;
	}

	for (i = 0; i < table->num_of_buckets; i++) {
		delete_list(&table->buckets[i]);
	}

	free(table->buckets);
	free(table);

	table = NULL;

	return ;
}
