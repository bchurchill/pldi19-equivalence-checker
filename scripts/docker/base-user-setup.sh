#!/bin/bash

# download sagemath and unpack
FILENAME=sage-8.5-Ubuntu_14.04-x86_64.tar.bz2

wget --no-verbose http://mirrors.xmission.com/sage/linux/64bit/$FILENAME
tar -xf $FILENAME
rm $FILENAME

# prepare sage
echo "exit" | /home/equivalence/SageMath/sage

# compile some dependencies
cd /home/equivalence/base

echo "STOKE_PLATFORM=\"nehalem\"" > .stoke_config
echo "BUILD_TYPE=release" >> .stoke_config
echo "MISC_OPTIONS=\"\"" >> .stoke_config

make z3
make cvc4

