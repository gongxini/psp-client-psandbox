#!/bin/bash

rm -rf submodules/dpdk
git submodule update --init --recursive
make -C ${PSP_DIR}/submodules/fake_work libfake
make -j 4 -C ${PSP_DIR}/submodules/rocksdb static_lib
mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DDPDK_MELLANOX_SUPPORT=OFF 
