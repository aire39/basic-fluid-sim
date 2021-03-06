#!/bin/bash

git submodule init
git submodule update

mkdir build
cd build

cmake -DCMAKE_BUILD_TYPE=Release ..

cmake --build . --config release

cp ../ToThePointRegular.ttf .
cp ../ToThePointRegular.ttf Release
