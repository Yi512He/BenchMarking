# Benchmarking
Source code of CS736 benchmarking paper 

## Preface
The code related to the benchmarking performance has not been changed since the deadline of the paper. However, we believe that the semi-automatic and messy scripts made this repository premature to be released. Therefore, the following changes are made for better user experience after the paper deadline:
1. Scripts are reorganized in a more intuitive way for the users to understand
2. C language files are slightly modified, mostly on the printing format, for easier processing of the scripts
3. `Makefile`s are created for automatic building
4. This README modified to reflect the new experience

We are certain that these changes make reproduction of our results simpler.

One more thing. Since the experiment methodology has been discussed in the paper, we will not repeat them here. We normally leave comments in critical places.

## Timing
In this part, we benchmark the resolution (precision) of candidate timing methods, `clock_gettime` and `RDTSCP`. To run the experiment, assuming `pwd` is the root of this repository:

```shell
cd TestingTime
make
./runTimeTest.sh
```

The result will be printed in the console in a human-readable manner.

## Trivial system calls
To better understand the overhead of system calls, we choose two simple ones, `getpid` and `getuid`, then we use both timing methods mentioned above to probe for the shortest time needed to complete one of these operations. To run (still assuming `pwd` to be the repository root, the same below):

```shell
cd TestingTrivial
make
./runTrivialTest.sh
```
Again, the result will be printed.

## Building the rest of the source code
At the repository root:

```shell
make
```

## Pipe
We test the best latency and throughput of the pipe for each *data size* and the corresponding *number of iterations*. The parameters are clear in the script. Assuming the build is successful:

```shell
./run_pipe.sh
```

The result numbers will be stored in `pipe.txt` in the same order as the list of data sizes in the script. To check the result:

```shell
cat pipe.txt
```

## Socket
This part of the experiment benchmarks TCP latency, TCP throughput and UDP latency. We will talk about UDP throughput later. You should first start two sessions, either on one host (the *local* benchmark) or on two hosts (the *remote* benchmark). The logic for both cases is the same. Run the server side first:

```shell
## Local
./run_server.sh -l

## Remote
./run_server.sh -r
```

Then run the client side:

```shell
## Local
./run_client.sh -l

## Remote
./run_client.sh -r <host>
```

where `host` is the IP address of the server.

The result will be saved to `socket.txt`. The format is similar to `pipe.txt`.

If you wish to run stand-alone experiment, run the corresponding command for hints, such as `./tcpt` (for TCP throughput) and `./udpl` (for UDP latency).

### UDP Throughput
We came across a situation where the server would not quit at the end of the program, while the client has already finished. Due to the time limit, we were not able to fix thie problem by the paper deadline. So we just leave it alone and the results are not affected. Run the server first:

```shell
./udpt -s <data size> <n iterations> <port>
```

Then run the client:
```shell
./udpt -c <data size> <n iterations> <host> <port>
```

The parameters we used in this experiment are the same as the above two scripts.

