/************************************************************************/
/* File Name : lc4_disassembler.c 										*/
/* Purpose   : This file implements the reverse assembler 				*/
/*             for LC4 assembly.  It will be called by main()			*/
/*             															*/
/* Author(s) : tjf and you												*/
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lc4_hash.h"

#define INSN_OP(I) ((I) >> 12)
#define INSN_11_9(I) (((I) >> 9) & 0x7)
#define INSN_5_3(I) (((I) >> 3) & 0x7)
#define INSN_8_6(I) (((I) >> 6) & 0x7)
#define INSN_2_0(I) ((I) & 0x7)
#define INSN_8_7(I) (((I) >> 7) & 0x3)
#define INSN_5_4(I) (((I) >> 4) & 0x3)

int reverse_assemble (lc4_memory_segmented* memory) 
{

	// create a character array to store strings in depending on opcode, and then strcpy this into heap for memory->assembly
	char temp_assembly_storage [30];

	if (memory == NULL) {
		fprintf(stderr, "memory is null under reverse_assemble()\n");
		return -1;
	}

	// Bucket 0

	row_of_memory* curr = search_opcode(memory->buckets[0], 0x0001);	// search for opcode 0001 in user program memory

	while (curr != NULL) {		// while there still exists row_of_memory with valid 0001 opcode, keep updating the assembly strings
		unsigned short int instruction = curr->contents;
		unsigned short int subopcode = INSN_5_3(instruction);

		unsigned short int rd = INSN_11_9(instruction);
		unsigned short int rs = INSN_8_6(instruction);
		unsigned short int rt = INSN_2_0(instruction);


		if (((instruction >> 5) & 1) == 1) { // 1IIIII - ADD IMM5

			signed short int imm5;

			// if IMM9 value is negative, sign extend to 16 bits
			if ((instruction & 0x001F) >> 4 == 1) {
				imm5 = 0xFFE0 | instruction;
			} else {
				imm5 = instruction & 0x001F;
			}

			sprintf(temp_assembly_storage, "ADD R%d, R%d, #%d", rd, rs, imm5);	// store this string in a temporary array
			

		}

		else if (subopcode == 0) { // 000 - ADD

			sprintf(temp_assembly_storage, "ADD R%d, R%d, R%d", rd, rs, rt);

		}

		else if (subopcode == 1) { // 001 - MUL 

			sprintf(temp_assembly_storage, "MUL R%d, R%d, R%d", rd, rs, rt);

		}

		else if (subopcode == 2) { // 010 - SUB

			sprintf(temp_assembly_storage, "SUB R%d, R%d, R%d", rd, rs, rt);

		}

		else if (subopcode == 3) { // 011 - DIV

			sprintf(temp_assembly_storage, "DIV R%d, R%d, R%d", rd, rs, rt);

		}

		else {
			fprintf(stderr, "Invalid subopcode under ArithmeticOp\n");
			return -1;
		}

		curr->assembly = malloc(sizeof(char) * strlen(temp_assembly_storage) + 1);	// make space on heap for string
		if (curr->assembly == NULL) {
			fprintf(stderr, "no space in heap under reverse_assemble()\n");
			return -1;
		}
		strcpy(curr->assembly, temp_assembly_storage);		// copy temporary string into curr->assembly to populate row_of_memory

		curr = search_opcode(memory->buckets[0], 0x0001);		// search for the next one

	}

	// same thing but for opcode 0101

	curr = search_opcode(memory->buckets[0], 0x0005);	// search for opcode 0001 in user program memory

	while (curr != NULL) {		// while there still exists row_of_memory with valid 0101 opcode, keep updating the assembly strings

		unsigned short int instruction = curr->contents;
		unsigned short int subopcode = INSN_5_3(instruction);

		unsigned short int rd = INSN_11_9(instruction);
		unsigned short int rs = INSN_8_6(instruction);
		unsigned short int rt = INSN_2_0(instruction);

		if (subopcode == 0) { // 000 - AND
			sprintf(temp_assembly_storage, "AND R%d, R%d, R%d", rd, rs, rt);
		}

		else if (subopcode == 1) { // 001 - NOT
			sprintf(temp_assembly_storage, "NOT R%d, R%d", rd, rs);
		}

		else if (subopcode == 2) { // 010 - OR
			sprintf(temp_assembly_storage, "OR R%d, R%d, R%d", rd, rs, rt);
		}

		else if (subopcode == 3) { // 011 - XOR
			sprintf(temp_assembly_storage, "XOR R%d, R%d, R%d", rd, rs, rt);
		}

		else if (((instruction >> 5) & 1) == 1) { // AND IMM5
			signed short int imm5;

			// if IMM5 value is negative, sign extend to 16 bits
			if ((instruction & 0x001F) >> 4 == 1) {
				imm5 = 0xFFE0 | instruction;
			} else {
				imm5 = instruction & 0x001F;
			}

			sprintf(temp_assembly_storage, "AND R%d, R%d, #%d", rd, rs, imm5);

		}

		else {
			fprintf(stderr, "Invalid subopcode under LogicalOp\n");
			return -1;
		}		

		curr->assembly = malloc(sizeof(char) * strlen(temp_assembly_storage) + 1);	// make space on heap for string
		if (curr->assembly == NULL) {
			fprintf(stderr, "no space in heap under reverse_assemble()\n");
			return -1;
		}
		strcpy(curr->assembly, temp_assembly_storage);

		curr = search_opcode(memory->buckets[0], 0x0005);

	}

	// Bucket 2 - do exact same thing except for OS Program Memory

	curr = search_opcode(memory->buckets[2], 0x0001);	// search for opcode 0001 in os program memory

	while (curr != NULL) {		// while there still exists row_of_memory with valid 0001 opcode, keep updating the assembly strings
		unsigned short int instruction = curr->contents;
		unsigned short int subopcode = INSN_5_3(instruction);

		unsigned short int rd = INSN_11_9(instruction);
		unsigned short int rs = INSN_8_6(instruction);
		unsigned short int rt = INSN_2_0(instruction);


		if (((instruction >> 5) & 1) == 1) { // 1IIIII - ADD IMM5

			signed short int imm5;

			// if IMM9 value is negative, sign extend to 16 bits
			if ((instruction & 0x001F) >> 4 == 1) {
				imm5 = 0xFFE0 | instruction;
			} else {
				imm5 = instruction & 0x001F;
			}

			sprintf(temp_assembly_storage, "ADD R%d, R%d, #%d", rd, rs, imm5);	// store this string in a temporary array
			

		}

		else if (subopcode == 0) { // 000 - ADD

			sprintf(temp_assembly_storage, "ADD R%d, R%d, R%d", rd, rs, rt);

		}

		else if (subopcode == 1) { // 001 - MUL 

			sprintf(temp_assembly_storage, "MUL R%d, R%d, R%d", rd, rs, rt);

		}

		else if (subopcode == 2) { // 010 - SUB

			sprintf(temp_assembly_storage, "SUB R%d, R%d, R%d", rd, rs, rt);

		}

		else if (subopcode == 3) { // 011 - DIV

			sprintf(temp_assembly_storage, "DIV R%d, R%d, R%d", rd, rs, rt);

		}

		else {
			fprintf(stderr, "Invalid subopcode under ArithmeticOp\n");
			return -1;
		}

		curr->assembly = malloc(sizeof(char) * strlen(temp_assembly_storage) + 1);	// make space on heap for string
		if (curr->assembly == NULL) {
			fprintf(stderr, "no space in heap under reverse_assemble()\n");
			return -1;
		}
		strcpy(curr->assembly, temp_assembly_storage);

		curr = search_opcode(memory->buckets[2], 0x0001);

	}

	// same thing but for opcode 0101

	curr = search_opcode(memory->buckets[2], 0x0005);	// search for opcode 0001 in user program memory

	while (curr != NULL) {		// while there still exists row_of_memory with valid 0101 opcode, keep updating the assembly strings

		unsigned short int instruction = curr->contents;
		unsigned short int subopcode = INSN_5_3(instruction);

		unsigned short int rd = INSN_11_9(instruction);
		unsigned short int rs = INSN_8_6(instruction);
		unsigned short int rt = INSN_2_0(instruction);

		if (subopcode == 0) { // 000 - AND
			sprintf(temp_assembly_storage, "AND R%d, R%d, R%d", rd, rs, rt);
		}

		else if (subopcode == 1) { // 001 - NOT
			sprintf(temp_assembly_storage, "NOT R%d, R%d", rd, rs);
		}

		else if (subopcode == 2) { // 010 - OR
			sprintf(temp_assembly_storage, "OR R%d, R%d, R%d", rd, rs, rt);
		}

		else if (subopcode == 3) { // 011 - XOR
			sprintf(temp_assembly_storage, "XOR R%d, R%d, R%d", rd, rs, rt);
		}

		else if (((instruction >> 5) & 1) == 1) { // AND IMM5
			signed short int imm5;

			// if IMM5 value is negative, sign extend to 16 bits
			if ((instruction & 0x001F) >> 4 == 1) {
				imm5 = 0xFFE0 | instruction;
			} else {
				imm5 = instruction & 0x001F;
			}

			sprintf(temp_assembly_storage, "AND R%d, R%d, #%d", rd, rs, imm5);

		}

		else {
			fprintf(stderr, "Invalid subopcode under LogicalOp\n");
			return -1;
		}		

		curr->assembly = malloc(sizeof(char) * strlen(temp_assembly_storage) + 1);	// make space on heap for string
		if (curr->assembly == NULL) {
			fprintf(stderr, "no space in heap under reverse_assemble()\n");
			return -1;
		}
		strcpy(curr->assembly, temp_assembly_storage);

		curr = search_opcode(memory->buckets[2], 0x0005);

	}


	return 0 ;
}
