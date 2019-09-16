# BenchMarking

Time Resolution Tests
###########################################
Function Use: clock()
Return value: if not sucessful, return -1
Testing times: 10000000
Value type: min
No operation between: 0 microsecond
310 iterations between: 1 microsecond
###########################################
Function Use: rdtsc
Testing tims: 10000000
Value tyle: min
No operation between: 18 clock cycles
15 * (mov %eax; mov %edx): 18 clock cycles
16 * (mov %eax; mov %edx): 21 clock cycles
###########################################
Function Use: rdtscp
Testing tims: 10000000
Value tyle: min
No operation between: 21 clock cycles
2 * (mov %eax; mov %edx): 24 clock cycles


Trivial System Call
###########################################
Function Use: getpid()
Return value: "These functions are always sucessful." (from Manual)
clock(): 10000000, min, call getpid() once = 0 microsecond
                        call getpid() twice = 1 microsecond
         Need to check the return value of clock()
rdtsc 10000000, min, call getpid() once = 1161 clock cycles
rdtscp 10000000, min, call getpid() once = 1176 clock cycles
###########################################################
Function Use: getUid()
Return value: "These functions are always sucessful." (from Manual)
clock(): 10000000, min, 
rdtsc 10000000, min, call getuid() once = 1152 clock cycles
rdtscp 10000000, min, call getuid() once = 1167 clock cycles

TCP

UDP
