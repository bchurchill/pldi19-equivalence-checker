#!/bin/bash

FAILED=0
SUCCESS=0

## TEST EXAMPLE
timeout 10m ./example/demo.sh | grep "Equivalent: yes"
if [ $? -eq "0" ]
	echo "EXAMPLE: passed"
	SUCCESS=$((SUCCESS+1))
else
	echo "EXAMPLE: failed"
	FAILED=$((FAILED+1))
fi

## TEST STRLEN
timeout 10m ./strlen/demo.sh | grep "Equivalent: yes"
if [ $? -eq "0" ]
	echo "STRLEN: passed"
	SUCCESS=$((SUCCESS+1))
else
	echo "STRLEN: failed"
	FAILED=$((FAILED+1))
fi

echo "$SUCCESS tests passed, $FAILED tests failed"

return $FAILED
