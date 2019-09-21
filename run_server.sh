#!/bin/bash

N_ITER=""
if [[ -z $1 ]]; then
    echo "Please specify -l for local tests or -r for remote tests"
    exit 1
elif [[ $1 == "-l" ]]; then
    # Number of iterations used in each local test w.r.t. data size
    N_ITER=(1000000 1000000 1000000 1000000 1000000 1000000 500000 200000 100000)
elif [[ $1 == "-r" ]]; then
    # Number of iterations used in each remot test w.r.t. data size
    N_ITER=(100000 100000 100000 100000 100000 100000 50000 20000 10000)
else 
    echo "ERROR: Invalid arguments"
    exit 1
fi

# Define data sizes
SIZES_STREAM=(4 16 64 256 1k 4k 64k 256k 512k)
SIZES_DGRAM=(4 16 64 256 1k 4k 65507)
BASE_PORT=8000

TLABELS=(tcp_latency tcp_throughput)
ULABELS=(udp_latency)
TPROGS=(./tcpl ./tcpt)
UPROGS=( ./udpl)

t=$BASE_PORT
for i in ${!TPROGS[@]} ; do 
    for j in ${!SIZES_STREAM[@]} ; do
        echo -e "Benchmark: ${TLABELS[$i]}\tsize: ${SIZES_STREAM[$j]}\titerations: ${N_ITER[$j]}\tport: $t"
        ${TPROGS[$i]} -s ${SIZES_STREAM[$j]} ${N_ITER[$j]} $t
        (( t=$t+1 ))
    done
done

for i in ${!UPROGS[@]} ; do 
    for j in ${!SIZES_DGRAM[@]} ; do
        echo -e "Benchmark: ${ULABELS[$i]}\tsize: ${SIZES_DGRAM[$j]}\titerations: ${N_ITER[$j]}\tport: $t"
        ${UPROGS[$i]} -s ${SIZES_DGRAM[$j]} ${N_ITER[$j]} $t
        (( t=$t+1 ))
    done
done
