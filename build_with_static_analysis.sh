#!/bin/bash
set -xe
cd $(dirname $(realpath $0))
echo $(pwd)
mkdir -p build 
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DENABLE_CLANG_TIDY=ON 
make
