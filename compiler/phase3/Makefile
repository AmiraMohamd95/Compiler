# Makefile

OBJS	= bison.o lex.o main.o

CC	= g++
CFLAGS	= -g -Wall -ansi -pedantic -std=c++11

Semantic:		$(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o Semantic -lfl

lex.o:		lex.c
		$(CC) $(CFLAGS) -c lex.c -o lex.o

lex.c:		Semantic.lex 
		flex Semantic.lex
		cp lex.yy.c lex.c

bison.o:	bison.c
		$(CC) $(CFLAGS) -c bison.c -o bison.o

bison.c:	Semantic.y
		bison -d -v Semantic.y
		cp Semantic.tab.c bison.c
		cmp -s Semantic.tab.h token.h || cp Semantic.tab.h token.h

main.o:		main.cc
		$(CC) $(CFLAGS) -c main.cc -o main.o

lex.o yac.o main.o	: heading.h
lex.o main.o		: token.h

clean:
	rm -f *.o *~ lex.c lex.yy.c bison.c token.h Semantic.tab.c Semantic.tab.h Semantic.output Semantic

