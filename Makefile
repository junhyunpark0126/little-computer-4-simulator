all: lc4

lc4: lc4_disassembler.o lc4_hash.o lc4_loader.o lc4_memory.o lc4.c
	clang -g lc4_disassembler.o lc4_hash.o lc4_loader.o lc4_memory.o lc4.c -o lc4

lc4_disassembler.o: lc4_disassembler.c
	clang -c lc4_disassembler.c -o lc4_disassembler.o

lc4_hash.o: lc4_hash.c
	clang -c lc4_hash.c -o lc4_hash.o

lc4_loader.o: lc4_loader.c
	clang -c lc4_loader.c -o lc4_loader.o

lc4_memory.o: lc4_memory.c
	clang -c lc4_memory.c -o lc4_memory.o

clean:
	rm -rf *.o

clobber: clean
	rm -rf lc4