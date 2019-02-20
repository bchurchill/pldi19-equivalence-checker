#!/bin/bash

source variables

/usr/bin/time -o demo.time -v stoke_debug_verify --strategy ddec --target $TARGET --rewrite $REWRITE --testcases testcases --heap_out --live_out "$LIVE_OUTS" --def_in "$DEF_INS" --target_bound $TARGET_BOUND --rewrite_bound $REWRITE_BOUND --alignment_predicate "0=0" --alignment_predicate_heap #--obligation_checker postgres --postgres ~/stoke/bin/postgres

