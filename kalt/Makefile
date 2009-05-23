CC = gcc
PROJECTROOT = ..
KVARLIBDIR = $(PROJECTROOT)/libkvar/lib
KVARINCDIR = $(PROJECTROOT)/libkvar/include
SRCDIR = src
BUILDDIR = build

OBJS = $(BUILDDIR)/kalt.o $(BUILDDIR)/parser.o $(BUILDDIR)/syntax.o $(BUILDDIR)/compile.o $(BUILDDIR)/functions.o $(BUILDDIR)/vm.o $(BUILDDIR)/test_results.o



LDFLAGS = 
ALL_LDFLAGS = -L$(KVARLIBDIR) -lkvar $(LDFLAGS)
CFLAGS = -g
ALL_CFLAGS = -I$(KVARINCDIR) $(CFLAGS)


$(BUILDDIR)/kalt : $(BUILDDIR) $(OBJS)
	$(CC) $(OBJS) $(ALL_LDFLAGS) -o $(BUILDDIR)/kalt

$(BUILDDIR)/kalt.o : $(SRCDIR)/kalt.c
	$(CC) -c $< $(ALL_CFLAGS) -o $@

$(BUILDDIR)/parser.o : $(SRCDIR)/parser.c $(SRCDIR)/parser.h
	$(CC) -c $< $(ALL_CFLAGS) -o $@

$(BUILDDIR)/syntax.o : $(SRCDIR)/syntax.c $(SRCDIR)/syntax.h
	$(CC) -c $< $(ALL_CFLAGS) -o $@

$(BUILDDIR)/compile.o : $(SRCDIR)/compile.c $(SRCDIR)/compile.h
	$(CC) -c $< $(ALL_CFLAGS) -o $@

$(BUILDDIR)/functions.o : $(SRCDIR)/functions.c $(SRCDIR)/functions.h $(SRCDIR)/parser.h $(SRCDIR)/test_results.h
	$(CC) -c $< $(ALL_CFLAGS) -o $@

$(BUILDDIR)/vm.o : $(SRCDIR)/vm.c $(SRCDIR)/vm.h $(SRCDIR)/compile.h
	$(CC) -c $< $(ALL_CFLAGS) -o $@

$(BUILDDIR)/test_results.o : $(SRCDIR)/test_results.c $(SRCDIR)/test_results.h
	$(CC) -c $< $(ALL_CFLAGS) -o $@

$(BUILDDIR) :
	if test ! -d $(BUILDDIR); then mkdir $(BUILDDIR); fi;

.PHONY : clean
clean:
	rm -rf $(BUILDDIR)
