# Macros
CC = gcc217
# CC = gcc217m
# CFLAGS =
CFLAGS = -g
# CFLAGS = -D NDEBUG
# CFLAGS = -D NDEBUG -O
# Dependency rules for non-file targets
listall:	testsymtablelist
listclobber:	clean
	rm -f *~ \#*\#
listclean:
	rm -f testsymtablelist *.o
hashall:	testsymtablehash
hashclobber:	clean
	rm -f *~ \#*\#
hashclean:
	rm -f testsymtablehash *.o
# Dependency rules for file targets
testsymtablelist: testsymtable.o symtablelist.o
	$(CC) $(CFLAGS) symtablelist.o testsymtable.o -o testsymtablelist
symtablelist.o: symtablelist.c symtable.h
	$(CC) $(CFLAGS) -c symtablelist.c symtable.h
testsymtable.o: testsymtable.c symtable.h
	$(CC) $(CFLAGS) -c testsymtable.c symtable.h

testsymtablehash: testsymtable.o symtablehash.o
	$(CC) $(CFLAGS) symtablehash.o testsymtable.o -o testsymtablehash
symtablehash.o: symtablehash.c symtable.h
	$(CC) $(CFLAGS) -c symtablehash.c symtable.h
testsymtable.o: testsymtable.c symtable.h
	$(CC) $(CFLAGS) -c testsymtable.c symtable.h