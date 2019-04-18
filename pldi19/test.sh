#!/bin/bash

FAILED=0
SUCCESS=0

## TEST EXAMPLE
cd paper_example
make
echo "[debug] disk space"
df -h
echo "[debug] running example"
time timeout 10m ./demo.sh | tee trace
echo "[debug] trace size"
du -hs trace
echo "[debug] grepping"
grep "Equivalent: yes" trace
if [ $? -eq "0" ]; then
	echo "EXAMPLE: passed"
	SUCCESS=$((SUCCESS+1))
else
	echo "EXAMPLE: failed"
	FAILED=$((FAILED+1))
fi
echo "[debug] running make clean"
make clean
echo "[debug] running cd"
cd ..

## TEST STRLEN
echo "[debug] running cd 2"
cd strlen
echo "[debug] running make"
make
echo "[debug] running checker for strlen"
time timeout 20m ./demo.sh | tee trace
echo "[debug] grepping"
grep "Equivalent: yes" trace
if [ $? -eq "0" ]; then
	echo "STRLEN: passed"
	SUCCESS=$((SUCCESS+1))
else
	echo "STRLEN: failed"
	FAILED=$((FAILED+1))
fi
echo "[debug] make clean"
make clean
echo "[debug] cd"
cd ..

echo "$SUCCESS tests passed, $FAILED tests failed"

exit $FAILED
