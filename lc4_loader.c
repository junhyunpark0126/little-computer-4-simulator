    /************************************************************************/
/* File Name : lc4_loader.c		 										*/
/* Purpose   : This file implements the loader (ld) from PennSim		*/
/*             It will be called by main()								*/
/*             															*/
/* Author(s) : tjf and you												*/
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lc4_memory.h"
#include "lc4_hash.h"

/* declarations of functions that must defined in lc4_loader.c */

FILE* open_file(char* file_name)
{

	// opens file and if NULL, returns NULL
	FILE* my_file = fopen(file_name, "rb");
	if (my_file == NULL) {
		return NULL;
	}

	return my_file;
}

int parse_file (FILE* my_obj_file, lc4_memory_segmented* memory)
{

	int i;
  int out;
	// temporary array to fread into, to detect if xCADE or xDADA
	unsigned char temp_arr [2];
	
	// temporary pointer to fread into; which line of memory we should write
	unsigned char memory_address_storage [2];
	int memory_address = 0;

	// temporary array to fread into, how many words are coming up that we must read
	unsigned char upcoming_words_storage [2];
	int upcoming_words = 0;

	// temporary array to fread into, switching endianess of the actual data values
	unsigned char actual_data_storage [2];
	unsigned short int actual_data = 0;
  row_of_memory* curr;

	FILE* my_file = my_obj_file;
	
	// read in first 2 bytes using fread (can be CADE, DADA, etc) and then this could possibly determine where in memory to access (Ed)
  // double precision float should be 1 byte
  while (fread(temp_arr, sizeof(char), 2, my_file) == 2) {

      // if its a code or data label, we must populate the CPU memory. else, we just offset to read in the next label
      if ((temp_arr[0] == 0xCA && temp_arr[1] == 0xDE) || (temp_arr[0] == 0xDA && temp_arr[1] == 0xDA)) {

        fread(memory_address_storage, sizeof(char), 2, my_file);
        // store the actual memory address
        memory_address = (memory_address_storage[0] << 8) + memory_address_storage[1];

        // Read in how many words are coming up
        fread(upcoming_words_storage, sizeof(char), 2, my_file);
        // store the actual number of words
        upcoming_words = (upcoming_words_storage[0] << 8) + upcoming_words_storage[1];

        // now actually populate CPU memory with data while switching Endianess
        for (i = 0; i < upcoming_words; i++) {
          fread(actual_data_storage, sizeof(char), 2, my_file);
          actual_data = (actual_data_storage[0] << 8) + actual_data_storage[1];

          curr = search_tbl_by_address(memory, memory_address + i);

          // if this data entry does not exist, then add it to table. else, we must update
          if (curr == NULL) { 
            add_entry_to_tbl(memory, memory_address + i, actual_data); // add this data entry to the table
          } else {
            curr->contents = actual_data;
          }
          
        }
      } else if (temp_arr[0] == 0xC3 && temp_arr[1] == 0xB7) {

        fread(memory_address_storage, sizeof(char), 2, my_file);
        // store the actual memory address
        memory_address = (memory_address_storage[0] << 8) + memory_address_storage[1];

        // Read in how many words are coming up
        fread(upcoming_words_storage, sizeof(char), 2, my_file);
        // store the actual number of words
        upcoming_words = (upcoming_words_storage[0] << 8) + upcoming_words_storage[1];

        char* label = malloc(sizeof(char) * upcoming_words + 1);    // allocate memory on the heap to store incoming label
        if (label == NULL) {
          fprintf(stderr, "no space on heap under parse_file 0xc3b7\n");
          return -1;
        }

        row_of_memory* find_node = search_tbl_by_address(memory, memory_address); // search the table to find row_of_memory associated with the address

        // if that node was never there, then create the node
        if (find_node == NULL) {
          out = add_entry_to_tbl(memory, memory_address, 0x0000);
          if (out != 0) {
            fprintf(stderr, "error under 0xC3B7 add_entry_to_tbl()\n");
            return -1;
          }
          find_node = search_tbl_by_address(memory, memory_address);
        }

		    // now actually populate CPU memory with data while switching Endianess
        for (i = 0; i < upcoming_words; i++) {
          fread(label + i, sizeof(char), 1, my_file);
        }
        label[upcoming_words] = '\0';
        find_node->label = label;

      } else if (temp_arr[0] == 0xF1 && temp_arr[1] == 0x7E) {

        // Read in how many words are coming up
        fread(upcoming_words_storage, sizeof(char), 2, my_file);
        // store the actual number of words
        upcoming_words = (upcoming_words_storage[0] << 8) + upcoming_words_storage[1];

        // we skip upcoming_words * 1 bytes because each character is 1 byte
        fseek(my_file, upcoming_words , SEEK_CUR);

      } else if (temp_arr[0] == 0x71 && temp_arr[1] == 0x5E) {
        // we skip 6 bytes because we have 3 labels after 0x715E
        fseek(my_file, 6 , SEEK_CUR);
      } else {
        fprintf(stderr, "File Contains Invalid Label\n"); // shouldn't be reachable 
        return -1;
      }
  }

  fclose(my_file);
	return 0 ;
}
