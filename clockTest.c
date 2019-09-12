#include <stdio.h>
#include <time.h>

unsigned long getTime() {

	clock_t start, end;

	start = clock();

	end = clock();

	return (end - start);

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
