CC = gcc
PROJECTROOT = ../
KVARLIBDIR = $(PROJECTROOT)/libkvar/lib
KVARINCDIR = $(PROJECTROOT)/libkvar/include
SRCDIR = src
BUILDDIR = build

OBJS = $(BUILDDIR)/kalt.o $(BUILDDIR)/parser.o $(BUILDDIR)/syntax.o


LDFLAGS = -L$(KVARLIBDIR) -lkvar
CCFLAGS = -I$(KVARINCDIR)



$(BUILDDIR)/kalt : $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(BUILDDIR)/kalt

$(BUILDDIR)/kalt.o : $(SRCDIR)/kalt.c
	$(CC) -c $< $(CCFLAGS) -o $(BUILDDIR)/kalt.o

$(BUILDDIR)/parser.o : $(SRCDIR)/parser.c $(SRCDIR)/parser.h
	$(CC) -c $< $(CCFLAGS) -o $(BUILDDIR)/parser.o

$(BUILDDIR)/syntax.o : $(SRCDIR)/syntax.c $(SRCDIR)/syntax.h
	$(CC) -c $< $(CCFLAGS) -o $(BUILDDIR)/syntax.o

.PHONY : clean
clean:
	rm -rf $(BUILDDIR)/*
