#!/bin/bash

source variables

/usr/bin/time -o demo.time -v stoke_debug_verify --strategy ddec --target $TARGET --rewrite $REWRITE --testcases testcases --test_set "{ 0 .. 1659 }" --heap_out --stack_out --live_out "$LIVE_OUTS" --def_in "$DEF_INS" --target_bound $TARGET_BOUND --vector_invariants --training_set_size 450 --alignment_predicate "t_%rax=r_%rax" #--obligation_checker postgres --postgres ~/stoke/bin/postgres --vector_invariants

