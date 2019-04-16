#!/bin/bash

# fix PATH
echo "PATH=\"\$HOME/equivalence-checker/bin:\$PATH\"" >> .bashrc
echo "PATH=\"\$HOME/SageMath:\$PATH\"" >> .bashrc

# download sagemath and unpack
FILENAME=sage-8.5-Ubuntu_14.04-x86_64.tar.bz2

wget --no-verbose http://mirrors.xmission.com/sage/linux/64bit/$FILENAME
tar -xf $FILENAME
rm $FILENAME

# prepare sage
echo "exit" | /home/equivalence/SageMath/sage

# download stoke
git clone https://github.com/StanfordPL/stoke.git equivalence-checker

# compile stoke
cd equivalence-checker
touch bin/envvars
git checkout artifact
git reset --hard efd6d6acb15ab77
./configure.sh
make

