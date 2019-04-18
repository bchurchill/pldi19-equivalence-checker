#!/bin/bash

FAILED=0
SUCCESS=0

function run_test () {
  local FOLDER=$1
  local NAME=$2
  cd $FOLDER
  echo "Running $NAME"
  make > /dev/null
  time timeout 10m ./demo.sh > trace
  grep "Equivalent: yes" trace
  if [ $? -eq "0" ]; then
    echo "$NAME: passed"
    SUCCESS=$((SUCCESS+1))
  else
    echo "$NAME: failed"
    FAILED=$((FAILED+1))
  fi
  make clean >/dev/null
  cd ..

}

run_test "paper_example" "EXAMPLE"
echo "$SUCCESS tests passed, $FAILED tests failed"

exit $FAILED
