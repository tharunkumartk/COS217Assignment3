# Macros
CC = gcc217
# CC = gcc217m
CFLAGS =
# CFLAGS = -g
# CFLAGS = -D NDEBUG
# CFLAGS = -D NDEBUG -O
# Dependency rules for non-file targets
all:	testsymtablelist
clobber:	clean
	rm -f *~ \#*\#
clean:
	rm -f testsymtablelist *.o
# Dependency rules for file targets
testsymtablelist: testsymtable.o symtablelist.o
	$(CC) $(CFLAGS) symtablelist.o testsymtable.o -o testsymtablelist
symtablelist.o: symtablelist.c symtable.h
	$(CC) $(CFLAGS) -c symtablelist.c symtable.h
testsymtable.o: testsymtable.c symtable.h
	$(CC) $(CFLAGS) -c testsymtable.c symtable.h