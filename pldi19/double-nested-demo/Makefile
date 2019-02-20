
include variables

all:
	gcc -O1 -std=c99 source.c -o opt1bin
	gcc -O2 -std=c99 source.c -o opt2bin
	gcc -O3 -std=c99 source.c -o opt3bin
	stoke extract -i opt1bin -o opt1
	stoke extract -i opt2bin -o opt2
	stoke extract -i opt3bin -o opt3
	stoke debug cfg --target $(TARGET) -o $(NAME).pdf
	stoke debug cfg --target $(REWRITE) -o $(NAME)_opt.pdf

tcgen:
	stoke_tcgen --target $(REWRITE) --bound 4 --def_in $(DEF_INS) --live_out $(LIVE_OUTS) --output testcases --mutants 4

clean:
	rm -rf opt1* opt3* opt2* *.pdf sage* stoke_sage* state* *.tmp
