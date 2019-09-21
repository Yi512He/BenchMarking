!#/bin/sh

# Define variables
TEST_PID="./getpidRdtscp"
TEST_UID="./getuidRdtscp"
TEST_PID_ONE="./pidGettime"
TEST_UID_ONE="./uidGettime"

# Compile the executables
make

# Run ./getpidRdtscp to find the cost of getpid
COST_OF_PID=$($TEST_PID)
echo The cost of getpid is $COST_OF_PID clock cycles

# Run ./pidGettime to find the cost of getpid
COST_OF_PID_ONE=$($TEST_PID_ONE)
echo The cost of getpid is $COST_OF_PID_ONE ns

# Run ./getpidRdtscp to find the cost of getuid
COST_OF_UID=$($TEST_UID)
echo The cost of getuid is $COST_OF_UID clock cycles

# Run ./uidGettime to find the cost of getuid
COST_OF_UID_ONE=$($TEST_UID_ONE)
echo The cost of getuid is $COST_OF_UID_ONE ns


# To clean up
make clean
