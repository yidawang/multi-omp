# multi-omp
The outer level parallelization is borrowed from [CTPL](https://github.com/vit-vit/CTPL).

Requires C++11 or up, and Boost.

## Usage
```
mkdir build && cd build && cmake .. && make -j
OMP_NUM_THREADS=`intra-omp-threads` ./multi-omp `inter-omp-threads`
```
