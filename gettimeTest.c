#include <stdio.h>
#include <time.h>

unsigned long getTime() {
    struct timespec s, t;

	clock_gettime(CLOCK_REALTIME, &s);

	clock_gettime(CLOCK_REALTIME, &t);

	return (t.tv_nsec - s.tv_nsec);

}

int main() {

	unsigned long min = 999999999;

	for(int i = 0; i < 10000000; i++){
		
		unsigned long tmp = getTime();
		if(min > tmp){
			min = tmp;
		}
		// printf("time is %ld\n", tmp);
	}

	printf("the execution time is %ld\n", min);


}
