#!/bin/bash

mkdir -p ../build
pushd ../build


#build lil game object file
c++ -c -fpic ../code/base.cpp  -g `sdl2-config --cflags --libs` -DBUILD_INTERNAL=1 -DBUILD_SLOW=1 -DBUILD_LINUX=1 -Wno-write-strings -ldl

#Creating shared library
c++ -shared -o ../build/base.so ../build/base.o 


c++  -o sdl_base ../code/sdl_base.cpp  -g `sdl2-config --cflags --libs` -DBUILD_INTERNAL=1 -DBUILD_SLOW=1 -DBUILD_LINUX=1 -Wno-write-strings  -ldl # -L../build/base.so


# 
popd
cd ../build 



