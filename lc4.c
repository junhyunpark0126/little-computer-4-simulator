/************************************************************************/
/* File Name : lc4.c 													*/
/* Purpose   : This file contains the main() for this project			*/
/*             main() will call the loader and disassembler functions	*/
/*             															*/
/* Author(s) : tjf and you												*/
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lc4_memory.h"
#include "lc4_hash.h"
#include "lc4_loader.h"
#include "lc4_disassembler.h"

/* program to mimic pennsim loader and disassemble object files */

// user-created hash function
int hash_function(void* table, void* key) {

	unsigned short int* id = key;

	// User Program Memory
	if ((*id) >= 0x0000 && (*id) <= 0x1FFF) {
		return 0;
	}

	// User Data Memory
	if ((*id) >= 0x2000 && (*id) <= 0x7FFF) {
		return 1;
	}

	// OS Program Memory
	if ((*id) >= 0x8000 && (*id) <= 0x9FFF) {
		return 2;
	}

	// OS Data Memory
	if ((*id) >= 0xA000 && (*id) <= 0xFFFF) {
		return 3;
	}

	// should not be reachable
	fprintf(stderr, "Invalid key under hash_function\n");
	return -1;

}

int main (int argc, char** argv) {

	/**
	 * main() holds the hashtable &
	 * only calls functions in other files
	 */

	int i;
    int str_len = 0;
    int contains_obj = 0;
    char* output_filename;
    char* filename;
    FILE *my_file;
	int out;

	/* step 1: create a pointer to the hashtable: memory 	*/
	lc4_memory_segmented* memory = NULL ;

	/* step 2: call create_hash_table() and create 4 buckets 	*/
	memory = create_hash_table(4, &hash_function);


	/* step 3: determine filename, then open it		*/
	/*   TODO: extract filename from argv, pass it to open_file() */
	    if (argc >= 3) {
        // output filename
        output_filename = argv[1];

        // checks to see if at least one file ends in .obj
        for (i = 2; i < argc; i++) {
          filename = argv[i];		// holds pointer to string of first file
          str_len = strlen(filename);
          if (filename[str_len - 4] == '.' && filename[str_len - 3] == 'o' && filename[str_len - 2] == 'b' && filename[str_len - 1] == 'j') {
            contains_obj = 1;
          }
        }

        // if no files ended in .obj, print error
        if (contains_obj == 0) {
          fprintf(stderr, "Please Enter At Least One .obj file -> error1: usage: ./lc4 <object_file.obj>\n");
		  delete_table(memory);
          return 0;
        }

        for (i = 2; i < argc; i++) {        // for every input file, we call parse_file to parse the file
          filename = argv[i];		// holds pointer to string of first file    
		  my_file = open_file(filename);
		  if (my_file == NULL) {
			fprintf(stderr, "filename DNE\n");
			delete_table(memory);
			return -1;
		  }
          out = parse_file(my_file, memory);
		  if (out != 0) {
			fprintf(stderr, "parse_file error\n");
			delete_table(memory); // free all memory if an error occurs
			return -1;
		  }

        }

	  } else {
		  fprintf(stderr, "Enter at least 2 arguments.\n");
		  delete_table(memory);
		  return -1;							
	  }


	/* step 4: call function: parse_file() in lc4_loader.c 	*/
	/*   TODO: call function & check for errors		*/


	/* step 5: repeat steps 3 and 4 for each .OBJ file in argv[] 	*/


	/* step 6: call function: reverse_assemble() in lc4_disassembler.c */
	/*   TODO: call function & check for errors		*/
	out = reverse_assemble(memory);
	if (out != 0) {
		fprintf(stderr, "invalid return under reverse_assemble() under lc4.c main()\n");
		delete_table(memory);
		return -1;
	}

	/* step 7: call function: print out the hashtable to output file */
	/*   TODO: call function 				*/
	FILE* output_file = fopen(output_filename, "w");
	fprintf(output_file, "<label>	<address>	<contents>	<assembly>\n");		// Header for Part 2
	print_table(memory, output_file);
	fclose(output_file);

	/* step 8: call function: delete_table() in lc4_hash.c */
	/*   TODO: call function & check for errors		*/
	delete_table(memory);


	/* only return 0 if everything works properly */
	return 0 ;
}
