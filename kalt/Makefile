CC = gcc
PROJECTROOT = ../
KVARLIBDIR = $(PROJECTROOT)/libkvar/lib
KVARINCDIR = $(PROJECTROOT)/libkvar/include
SRCDIR = src
BUILDDIR = build

OBJS = $(BUILDDIR)/kalt.o $(BUILDDIR)/parser.o $(BUILDDIR)/syntax.o $(BUILDDIR)/compile.o $(BUILDDIR)/functions.o $(BUILDDIR)/vm.o $(BUILDDIR)/test_results.o




LDFLAGS = -L$(KVARLIBDIR) -lkvar -lreadline
CCFLAGS = -I$(KVARINCDIR) -g



$(BUILDDIR)/kalt : $(OBJS)
	[ -d ${BUILDDIR} ] || mkdir -p ${BUILDDIR}
	$(CC) $(OBJS) $(LDFLAGS) -o $(BUILDDIR)/kalt

$(BUILDDIR)/kalt.o : $(SRCDIR)/kalt.c
	[ -d ${BUILDDIR} ] || mkdir -p ${BUILDDIR}
	$(CC) -c $< $(CCFLAGS) -o $(BUILDDIR)/kalt.o

$(BUILDDIR)/parser.o : $(SRCDIR)/parser.c $(SRCDIR)/parser.h
	[ -d ${BUILDDIR} ] || mkdir -p ${BUILDDIR}
	$(CC) -c $< $(CCFLAGS) -o $(BUILDDIR)/parser.o

$(BUILDDIR)/syntax.o : $(SRCDIR)/syntax.c $(SRCDIR)/syntax.h
	[ -d ${BUILDDIR} ] || mkdir -p ${BUILDDIR}
	$(CC) -c $< $(CCFLAGS) -o $(BUILDDIR)/syntax.o

$(BUILDDIR)/compile.o : $(SRCDIR)/compile.c $(SRCDIR)/compile.h
	[ -d ${BUILDDIR} ] || mkdir -p ${BUILDDIR}
	$(CC) -c $< $(CCFLAGS) -o $(BUILDDIR)/compile.o

$(BUILDDIR)/functions.o : $(SRCDIR)/functions.c $(SRCDIR)/functions.h $(SRCDIR)/parser.h $(SRCDIR)/test_results.h
	[ -d ${BUILDDIR} ] || mkdir -p ${BUILDDIR}
	$(CC) -c $< $(CCFLAGS) -o $(BUILDDIR)/functions.o

$(BUILDDIR)/vm.o : $(SRCDIR)/vm.c $(SRCDIR)/vm.h $(SRCDIR)/compile.h
	[ -d ${BUILDDIR} ] || mkdir -p ${BUILDDIR}
	$(CC) -c $< $(CCFLAGS) -o $(BUILDDIR)/vm.o

$(BUILDDIR)/test_results.o : $(SRCDIR)/test_results.c $(SRCDIR)/test_results.h
	[ -d ${BUILDDIR} ] || mkdir -p ${BUILDDIR}
	$(CC) -c $< $(CCFLAGS) -o $(BUILDDIR)/test_results.o

.PHONY : clean
clean:
	rm -rf $(BUILDDIR)/*
