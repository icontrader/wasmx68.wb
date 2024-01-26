#!/bin/sh

mkdir -p lib
cp ../xdev68k/makefile .
cp ../ziglang/wasi.c ./lib
cp ../ziglang/panic.h ./lib
cp ../src/math.c ./lib
cp ../src/rmemcpy.c ./lib
