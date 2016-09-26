HTMLS	= $(patsubst %.md,%.html,$(wildcard *.md))
PDFS	= $(patsubst %.html,%.pdf,$(HTMLS))

all: $(HTMLS) $(PDFS)

%.html: %.md
	markdown_py $< > $@

%.pdf: %.html
	xhtml2pdf $< $@

clean:
	$(RM) $(HTMLS)
	$(RM) $(PDFS)
	$(RM) *~

