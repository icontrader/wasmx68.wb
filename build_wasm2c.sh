#!/bin/sh

mkdir -p ./BIN
clang ./ziglang/wasm2c.c -o ./BIN/wasm2c
