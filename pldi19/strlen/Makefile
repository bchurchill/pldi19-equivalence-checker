
include variables

all:
	gcc -O1 -std=c99 source.c -c -o opt1.o
	gcc -O1 -std=c99 source.c -o opt1bin
	gcc -O3 -std=c99 source.c -c -o opt3.o
	gcc -O3 -std=c99 -DBENCHMARK_F1 source.c -o benchmark_glibc
	gcc -O3 -std=c99 -DBENCHMARK_F3 source.c -o benchmark_ours
	stoke extract -i opt1.o -o opt1
	stoke extract -i opt3.o -o opt3
	stoke debug cfg --target $(TARGET) -o $(NAME).pdf
	stoke debug cfg --target $(REWRITE) -o $(NAME)_vec.pdf

tcgen:
	mkdir -p tcs
	stoke_tcgen --target $(REWRITE) --bound 3 --def_in $(DEF_INS) --live_out $(LIVE_OUTS) --output tcs/tcgen.base --mutants 0 --randomize_order >/dev/null
	stoke_tcgen --target $(REWRITE) --bound 3 --def_in $(DEF_INS) --live_out $(LIVE_OUTS) --output tcs/tcgen2 --mutants 4 --randomize_order >/dev/null
	cat tcs/tcgen.base tcs/tcgen2 > tcs/testcases.combo

clean:
	rm -rf opt1* opt3* *.pdf sage* stoke_sage* state* *.tmp
