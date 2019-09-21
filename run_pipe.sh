#!/bin/bash

DEST="pipe.txt"

N_ITER=(1000000 1000000 1000000 1000000 1000000 1000000 500000 200000 100000)

SIZES=(4 16 64 256 1k 4k 64k 256k 512k)

LABELS=(pipe_latency pipe_throughput)
PROGS=(./pipel ./pipet)

echo "Run on $(date):" >> "$DEST"

for i in ${!PROGS[@]} ; do
    echo ${LABELS[$i]} >> "$DEST"
    for j in ${!SIZES[@]} ; do
        echo -e "Benchmark: ${LABELS[$i]}\tsize: ${SIZES[$j]}\titerations: ${N_ITER[$j]}"
        ${PROGS[$i]} ${SIZES[$j]} ${N_ITER[$j]} | grep -E "^[0-9]+\.[0-9]+$" >> "$DEST"
    done
done
