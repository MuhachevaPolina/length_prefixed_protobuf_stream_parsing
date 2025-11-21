#! /bin/bash

if [[ -d build ]] 
then 
    rm -rf build
fi 

mkdir build
cd build
cmake /home/template/102-cpp-protobuf-pmuhacheva/
make
./parser_tests