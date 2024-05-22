/************************************************************************/
/* File Name : lc4_memory.c		 										*/
/* Purpose   : This file implements the linked_list helper functions	*/
/* 			   to manage the LC4 memory									*/
/*             															*/
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "lc4_memory.h"


/*
 * adds a new node to linked list pointed to by head
 */
int add_to_list (row_of_memory** head,
		 		 unsigned short int address,
		 		 unsigned short int contents)
{

	/* allocate memory for a single node */
	row_of_memory* new_node = malloc(sizeof(row_of_memory));
	if (new_node == NULL) {
		return -1;
	}

	/* populate fields in newly allocated node with arguments: address/contents */
	new_node->address = address;
	new_node->contents = contents;

    /* make certain to set other fields in structure to NULL */
	new_node->label = NULL;
	new_node->assembly = NULL;
	new_node->next = NULL;

	/* if head==NULL, node created is the new head of the list! */
	if ((*head) == NULL) {
		(*head) = new_node;
	}

	/* otherwise, insert the node into the linked list keeping it in order of ascending addresses */
	else {
		
		row_of_memory* curr = *head;

		if (new_node->address < curr->address) {
			(*head) = new_node;
			(*head)->next = curr;
			return 0;
		}

		while (new_node->address > curr->address) {
			if (curr->next == NULL) {
				curr->next = new_node;
				return 0;
			}

			if (curr->next->address > new_node->address) {
				break;
			}

			curr = curr->next;
		}

		new_node->next = curr->next;
		curr->next = new_node;
	}

	/* return 0 for success, -1 if malloc fails */

	return 0 ;
}


/*
 * search linked list by address field, returns node if found
 */
row_of_memory* search_address (row_of_memory* head,
			        		   unsigned short int address )
{
	/* traverse linked list, searching each node for "address"  */

	/* return pointer to node in the list if item is found */

	/* return NULL if list is empty or if "address" isn't found */

	if (head == NULL) {
		return NULL;
	}

	row_of_memory* curr = head;

	while (curr != NULL) {
		if (curr->address == address) {
			return curr;
		}
		curr = curr->next;	
	}

	return NULL ;
}

/*
 * search linked list by opcode field, returns node if found
 */
row_of_memory* search_opcode (row_of_memory* head,
				      		  unsigned short int opcode  )
{
	/* traverse linked list until node is found with matching opcode
	   AND "assembly" field of node is empty */

	/* return pointer to node in the list if item is found */

	/* return NULL if list is empty or if no matching nodes */

	if (head == NULL) {
		return NULL;
	}

	row_of_memory* curr = head;

	while (curr != NULL) {
		if (((curr->contents >> 12) == opcode) && (curr->assembly == NULL)) {
			return curr;
		}
		curr = curr->next;	
	}

	return NULL ;
}

/*
 * delete the node from the linked list with matching address
 */
int delete_from_list (row_of_memory** head,
			          unsigned short int address ) 
{
	/* if head isn't NULL, traverse linked list until node is found with matching address */
	if ((*head) == NULL) {
		return -1;
	}

	row_of_memory* curr = (*head);

	if (curr->address == address) {
		(*head) = curr->next;
		if (curr->label != NULL) {
			free(curr->label);
		}
		if (curr->assembly != NULL) {
			free(curr->assembly);
		}
		free(curr);
		return 0;
	}

	while (curr->address != address) {
		if (curr->next == NULL) {
			return -1;
		}
		if (curr->next->address == address) {
			row_of_memory* temp = curr->next;
			curr->next = curr->next->next;
			if (temp->label != NULL) {
				free(temp->label);
			}
			if (temp->assembly != NULL) {
				free(temp->assembly);
			}
			free(temp);
			return 0;
		}
		curr = curr->next;
	}

	/* delete the matching node, re-link the list */

	/* make certain to update the head pointer - if original was deleted */

	/* return 0 if successfully deleted the node from list, -1 if node wasn't found */

	return 0 ;

}

void print_list (row_of_memory* head, 
				 FILE* output_file )
{
	int i = 0;
	
	char* label_print;
	char* assembly_print;

	/* make sure head isn't NULL */
	if (head == NULL) {
		return;
	}

	/* print out a header to output_file */
	// fprintf(output_file, "PART 1:\n");

	/* traverse linked list, print contents of each node to output_file */
	row_of_memory* curr = head;
	while (curr != NULL) {

		// Need to account for when label or assembly is NULL and need to print
		if (curr->label == NULL) {
			label_print = "";
		} else {
			label_print = curr->label;
		}

		if (curr->assembly == NULL) {
			assembly_print = "";
		} else {
			assembly_print = curr->assembly;
		}

		fprintf(output_file, "%s	%04X	%04X	%s\n", label_print, curr->address, curr->contents, assembly_print);
		i += 1;
		curr = curr->next;
	}

	return ;
}

/*
 * delete entire linked list
 */
void delete_list (row_of_memory** head )
{
	/* delete entire list node by node */
	/* set head = NULL upon deletion */

	if ((*head) == NULL) {
		return;
	}

	row_of_memory* curr = (*head);
	row_of_memory* temp;

	while (curr != NULL) {
		temp = curr->next;
		if (curr->label != NULL) {
			free(curr->label);
		}
		if (curr->assembly != NULL) {
			free(curr->assembly);
		}
		free(curr);
		curr = temp;
	}

	(*head) = NULL;

	return ;
}
