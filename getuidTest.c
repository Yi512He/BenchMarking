#include <stdio.h>
#include <time.h>
#include <unistd.h>

unsigned long getTime() {

	return 0;

}

int main() {

	unsigned long min = 999999999;

    struct timespec s, t;
	__uid_t p;
	for(int i = 0; i < 10000000; i++){
		
		clock_gettime(CLOCK_REALTIME, &s);
		p = getuid();
		

		clock_gettime(CLOCK_REALTIME, &t);

		unsigned long tmp = (t.tv_nsec - s.tv_nsec);
		if(min > tmp){
			min = tmp;
		}
		// printf("time is %ld\n", tmp);
	}

	printf("%ld\n", min);


}
