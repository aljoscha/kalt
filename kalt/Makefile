CC = gcc
PROJECTROOT = ../
KVARLIBDIR = $(PROJECTROOT)/libkvar/lib
KVARINCDIR = $(PROJECTROOT)/libkvar/include
SRCDIR = src
BUILDDIR = build

OBJS = kalt.o

LDFLAGS = -L$(KVARLIBDIR) -lkvar
CCFLAGS = -I$(KVARINCDIR)



$(BUILDDIR)/kalt : $(BUILDDIR)/kalt.o
	$(CC) $(BUILDDIR)/kalt.o $(LDFLAGS) -o $(BUILDDIR)/kalt

$(BUILDDIR)/kalt.o : $(SRCDIR)/kalt.c
	$(CC) -c $< $(CCFLAGS) -o $(BUILDDIR)/kalt.o

.PHONY : clean
clean:
	rm -rf $(BUILDDIR)/*
