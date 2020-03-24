#!/bin/bash

tmp=$(cd $(dirname $0); pwd)
cd $tmp

if !(type "cmake3" > /dev/null 2>&1); then
    echo "cmake3 don't exist!"
    echo "Please install cmake3!"
    exit
fi

#YARR install
git clone https://github.com/syamagay/YARR_forThresholdOperation.git YARR
cd YARR
mkdir build
cd build
cmake3 ..
make install -j4
cd ..

cd configs
./create_defaultconfig.sh -a rd53a -m test -c 1 
