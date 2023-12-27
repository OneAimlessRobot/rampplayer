#/bin/bash
make clean -C player && make clean -C editor && make clean
make  -j && make -C player -j
