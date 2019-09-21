#!/bin/sh

# Define variables
TEST_TIMEONE="./gettime"
TEST_TIMETWO="./gettimeWithMove"
TEST_RDTSCPONE="./rdtscpTest"
TEST_RDTSCPTWO="./rdtscpTestWithMove"

# Compile the executables
make

# Run ./gettime to find the cost of clock_gettime
COST_OF_CLOCK_GETTIME=$($TEST_TIMEONE)
echo The cost of clock_gettime is $COST_OF_CLOCK_GETTIME ns

# Run ./gettime and ./gettimeWithMove to find the resolution of clock_gettime
CLOCK_GETTIME=$($TEST_TIMEONE)
CLOCK_GETTIME_MOVE=$($TEST_TIMETWO)
CLOCK_GETTIME_RESOLUTION=$(expr $CLOCK_GETTIME_MOVE - $CLOCK_GETTIME  )
echo The resolution of clock_gettime is $CLOCK_GETTIME_RESOLUTION ns

# Run ./gettime to find the cost of clock_gettime
COST_OF_RDTSCP=$($TEST_RDTSCPONE)
echo The cost of RDTSCP is $COST_OF_RDTSCP clock cycles

# Run ./gettime and ./gettimeWithMove to find the resolution of clock_gettime
RDTSCP=$($TEST_RDTSCPONE)
RDTSCP_MOVE=$($TEST_RDTSCPTWO)
RDTSCP_RESOLUTION=$(expr $RDTSCP_MOVE - $RDTSCP  )
echo The resolution of RDTSCP is  $RDTSCP_RESOLUTION clock cycles

# To clean up
make clean
