#!/bin/sh

cd task_a && mkdir build && cd build && cmake .. && make && cd ../../
cd task_b && mkdir build && cd build && cmake .. && make && cd ../../
cd task_c && sh compile.sh                               && cd ../
cd task_d && mkdir build && cd build && cmake .. && make

