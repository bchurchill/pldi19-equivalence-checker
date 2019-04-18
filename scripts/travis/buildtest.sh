#!/bin/bash
export PATH=$PATH:$HOME/equivalence-checker/bin:$HOME/SageMath
cd /home/equivalence/equivalence-checker
./configure.sh
make
echo "make returned code $N"
if [ "$N" != "0" ]; then
  exit $N
fi
make test
N=$?
echo "make test returned code $N"
if [ "$N" != "0" ]; then
  exit $N
fi
cd pldi19
./test.sh
N=$?
echo "test.sh returned code $N"
exit $N
