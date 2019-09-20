all: timing syscall ipc

timing: gettimeTest rdtscTest rdtscpTest

syscall: getpidTest getuidTest

ipc: pipel pipet tcpl tcpt udpl udpt

gettimeTest rdtscTest rdtscpTest getpidTest getuidTest pipel pipet tcpl tcpt udpl udpt: % : %.c
	gcc -o $@ $^


