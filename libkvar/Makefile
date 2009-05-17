CC = gcc
LIBDIR = lib
INCDIR = include
SRCDIR = src

CCFLAGS = -I$(INCDIR)



$(LIBDIR)/libkvar.a : $(LIBDIR)/kvar.o
	ar rcs $(LIBDIR)/libkvar.a $(LIBDIR)/kvar.o

$(LIBDIR)/kvar.o : $(SRCDIR)/kvar.c
	$(CC) -c $(SRCDIR)/kvar.c -o $(LIBDIR)/kvar.o $(CCFLAGS)

.PHONY : clean
clean:
	rm -rf $(LIBDIR)/*
