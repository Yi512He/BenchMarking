#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <limits.h>

const int scale = 1e9;
const int testCount = 1e6;

int main(){
	struct timespec start, end;
	long min = LONG_MAX;
	int tmp= 0;
		
	for(int i = 0; i < testCount; i++){
		clock_gettime(CLOCK_REALTIME, &start);

 		__asm__ __volatile__ (
                	"mov %%edx, %0\n\t":"=r"(tmp)
        	);

		clock_gettime(CLOCK_REALTIME, &end);

		long temp = (end.tv_sec - start.tv_sec) * scale + (end.tv_nsec - start.tv_nsec);

		if(temp < min){
			min = temp;
		}	
	}

	printf("%ld", min);

	return 0;
}
