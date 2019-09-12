# BenchMarking

clockTest.c: run getTime() method for 10000000(10 million) times
             min time is 0
             getTime() calls clock() twice and return (end-start)
             
rdtscTest.c: run getTime() method for 1000000(1 million times)
             min time is 18 cycles
             getTime() calls rdtsc twice and return (end-start)
