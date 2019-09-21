#!/bin/bash

N_ITER=""
HOST=""
if [[ -z $1 ]]; then
    echo "Please specify -l for local tests or -r <host> for remote tests"
    exit 1
elif [[ $1 == "-l" ]]; then
    # Number of iterations used in each local test w.r.t. data size
    N_ITER=(1000000 1000000 1000000 1000000 1000000 1000000 500000 200000 100000)
    HOST="127.0.0.1"
elif [[ $1 == "-r" ]]; then
    if [[ -z $2 ]]; then
        echo "ERROR: Invalid arguments"
        exit 1
    fi
    # Number of iterations used in each remote test w.r.t. data size
    N_ITER=(100000 100000 100000 100000 100000 100000 50000 20000 10000)
    HOST=$2
else 
    echo "ERROR: Invalid arguments"
    exit 1
fi

SIZES_STREAM=(4 16 64 256 1k 4k 64k 256k 512k)
SIZES_DGRAM=(4 16 64 256 1k 4k 65507)
BASE_PORT=8000

TLABELS=(tcp_latency tcp_throughput)
ULABELS=(udp_latency)
TPROGS=(./tcpl ./tcpt)
UPROGS=(./udpl)

DEST="socket.txt"

echo "Run on $(date):" >> "$DEST"

t=$BASE_PORT
for i in ${!TPROGS[@]} ; do 
    echo ${TLABELS[$i]} >> "$DEST"
    for j in ${!SIZES_STREAM[@]} ; do
        echo -e "Benchmark: ${TLABELS[$i]}\tsize: ${SIZES_STREAM[$j]}\titerations: ${N_ITER[$j]}\tport: $t"
        ${TPROGS[$i]} -c ${SIZES_STREAM[$j]} ${N_ITER[$j]} $HOST $t | grep -E "^[0-9]+\.[0-9]+$" >> "$DEST"
        (( t=$t+1 ))
        echo "Sleeping 5s for the server to reset"
        sleep 5s
    done
done

for i in ${!UPROGS[@]} ; do 
    echo ${ULABELS[$i]} >> "$DEST"
    for j in ${!SIZES_DGRAM[@]} ; do
        echo -e "Benchmark: ${ULABELS[$i]}\tsize: ${SIZES_DGRAM[$j]}\titerations: ${N_ITER[$j]}\tport: $t"
        ${UPROGS[$i]} -c ${SIZES_DGRAM[$j]} ${N_ITER[$j]} $HOST $t | grep -E "^[0-9]+\.[0-9]+$" >> "$DEST"
        (( t=$t+1 ))
        echo "Sleeping 5s for the server to reset"
        sleep 5s
    done
done