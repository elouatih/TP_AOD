CC=cc
LATEXC=pdflatex
DOCC=doxygen
CFLAGS=-g -Wall

REFDIR=.
SRCDIR=$(REFDIR)/src
BINDIR=$(REFDIR)/bin
DOCDIR=$(REFDIR)/doc
REPORTDIR=$(REFDIR)/rapport


LATEXSOURCE=$(wildcard $(REPORTDIR)/*.tex)
CSOURCE=$(wildcard $(SRCDIR)/compileBST.c)
COTHER = $(SRCDIR)/ABR.c
PDF=$(LATEXSOURCE:.tex=.pdf)
CCOTHER = $(REPORTDIR)/fi.png


all: binary report doc 


$(BINDIR)/compileBST: $(CSOURCE) $(COTHER)
	$(CC) $(CFLAGS) $^ -o $@

%.pdf: $(LATEXSOURCE) $(CCOTHER)
	$(LATEXC) -output-directory $(REPORTDIR) $^ 

$(DOCDIR)/index.html: $(SRCDIR)/Doxyfile $(CSOURCE)
	$(DOCC) $(SRCDIR)/Doxyfile

binary: $(BINDIR)/compileBST

report: $(PDF) $(CCOTHER)

doc: $(DOCDIR)/index.html

clean:
	rm -rf $(DOCDIR) $(BINDIR)/* $(REPORTDIR)/*.aux $(REPORTDIR)/*.log  $(REPORTDIR)/rapport.pdf 


.PHONY: all doc binary report 
