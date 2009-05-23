CC = gcc
LIBDIR = lib
INCDIR = include
SRCDIR = src

CCFLAGS = -I$(INCDIR)



$(LIBDIR)/libkvar.a : mkinstalldirs $(LIBDIR)/kvar.o
	ar rcs $(LIBDIR)/libkvar.a $(LIBDIR)/kvar.o

$(LIBDIR)/kvar.o : $(SRCDIR)/kvar.c $(INCDIR)/kvar.h
	$(CC) -c $(SRCDIR)/kvar.c -o $(LIBDIR)/kvar.o $(CCFLAGS)
	
.PHONY : mkinstalldirs
mkinstalldirs:
	[ -d ${LIBDIR} ] || mkdir -p ${LIBDIR}

.PHONY : clean
clean:
	rm -rf $(LIBDIR)/*
