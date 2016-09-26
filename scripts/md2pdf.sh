#!/bin/bash

#CSS_FILE=~/path/to/style.css

MARKDOWN_FILE=$1.md
HTML_FILE=$1.html
PDF_FILE=$1.pdf

markdown_py $MARKDOWN_FILE > $HTML_FILE
#xhtml2pdf --quiet --css $CSS_FILE $HTML_FILE $PDF_FILE
xhtml2pdf --quiet $HTML_FILE $PDF_FILE
rm $HTML_FILE

