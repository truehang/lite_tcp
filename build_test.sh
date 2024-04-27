#!/bin/bash
set -xe
cd $(dirname $(realpath $0))
echo $(pwd)
mkdir -p build 
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DENABLE_TESTS=ON
make
ctest -C Release --return-failed --output-on-failure
