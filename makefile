all:	
	listall
	hashall
clobber:	clean
	rm -f *~ \#*\#
clean: 	
	rm -f testsymtablelist testsymtablehash *.o
listall:	testsymtablelist

hashall:	testsymtablehash


testsymtablelist: testsymtable.o symtablelist.o
	gcc217 symtablelist.o testsymtable.o -o testsymtablelist
symtablelist.o: symtablelist.c symtable.h
	gcc217 -c symtablelist.c symtable.h
testsymtablehash: testsymtable.o symtablehash.o
	gcc217 symtablehash.o testsymtable.o -o testsymtablehash
symtablehash.o: symtablehash.c symtable.h
	gcc217 -c symtablehash.c symtable.h

testsymtable.o: testsymtable.c symtable.h
	gcc217 -c testsymtable.c symtable.h
