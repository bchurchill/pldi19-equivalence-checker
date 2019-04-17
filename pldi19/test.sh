#!/bin/bash

FAILED=0
SUCCESS=0

## TEST EXAMPLE
cd paper_example
make
timeout 10m ./demo.sh | grep "Equivalent: yes"
if [ $? -eq "0" ]; then
	echo "EXAMPLE: passed"
	SUCCESS=$((SUCCESS+1))
else
	echo "EXAMPLE: failed"
	FAILED=$((FAILED+1))
fi
make clean
cd ..

## TEST STRLEN
cd strlen
make
timeout 10m ./demo.sh | grep "Equivalent: yes"
if [ $? -eq "0" ]; then
	echo "STRLEN: passed"
	SUCCESS=$((SUCCESS+1))
else
	echo "STRLEN: failed"
	FAILED=$((FAILED+1))
fi
make clean
cd ..

echo "$SUCCESS tests passed, $FAILED tests failed"

exit $FAILED
