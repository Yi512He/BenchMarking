#!/bin/sh

DEST="timing_and_syscall.txt"

TIMINGS="./gettimeTest ./rdtscTest ./rdtscpTest" 
SYSCALLS="./getpidTest ./getuidTest"

echo "Run on $(date):" >> $DEST

for TIMING in $TIMINGS ; do
    echo "Running $TIMING"
    $TIMING >> $DEST
done

for SYSCALL in $SYSCALLS ; do
    echo "Running $SYSCALL"
    $SYSCALL >> $DEST
done
