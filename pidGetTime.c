#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <limits.h>

const int testingTIme = 1000000;
const int scale = 1e9;

int main(){
	struct timespec start, end;

	long min = LONG_MAX;

	for(int i = 0; i < testingTIme; i++){
		clock_gettime(CLOCK_REALTIME, &start);

		getpid();

		clock_gettime(CLOCK_REALTIME, &end);
		
		long temp = (end.tv_sec - start.tv_sec) * scale + (end.tv_nsec - start.tv_nsec);

		if(temp < min){
			min = temp;
		}
	}

	printf("The min execution time of getpid is %ld\n", min);

	return 0;


}
