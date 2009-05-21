CC = gcc
LIBDIR = lib
INCDIR = include
SRCDIR = src

CCFLAGS = -I$(INCDIR)



$(LIBDIR)/libkvar.a : $(LIBDIR)/kvar.o
	[ -d ${LIBDIR} ] || mkdir -p ${LIBDIR}
	ar rcs $(LIBDIR)/libkvar.a $(LIBDIR)/kvar.o

$(LIBDIR)/kvar.o : $(SRCDIR)/kvar.c $(INCDIR)/kvar.h
	[ -d ${LIBDIR} ] || mkdir -p ${LIBDIR}
	$(CC) -c $(SRCDIR)/kvar.c -o $(LIBDIR)/kvar.o $(CCFLAGS)

.PHONY : clean
clean:
	rm -rf $(LIBDIR)/*
