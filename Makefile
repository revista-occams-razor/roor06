NUMBER=06
EDITION=01
SRC=oficial

NAME=roor-${NUMBER}-${EDITION}-${SRC}

all: *tex
	latex portada.tex && latex portada.tex && dvips portada.dvi -o ${NAME}.ps && ps2pdf -dALLOWPSTRANSPARENCY -dNOSAFER ${NAME}.ps
	rm -f ${NAME}.ps.gz
	gzip ${NAME}.ps

.PHONY:
clean:
	rm -f *aux *dvi *log 

clean-all:
	rm -f *aux *dvi *log ${NAME}.ps.gz ${NAME}.pdf portada.out
