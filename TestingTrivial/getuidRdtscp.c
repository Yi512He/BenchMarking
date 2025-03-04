#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

const int testCount = 1e6;

unsigned long long returnTime(){
	unsigned long long start, end;
	unsigned long long cycles_low, cycles_high, cycles_low1, cycles_high1;

	// inline assembly read timestamp twice
	__asm__ __volatile__ (
		"RDTSCP" : "=r" (cycles_high), "=r" (cycles_low));

	getuid();

	__asm__ __volatile__ (	
		"RDTSCP" : "=r" (cycles_high1), "=r" (cycles_low1));

	start = ( (cycles_high << 32) | cycles_low );
       	end = ( (cycles_high1 << 32) | cycles_low1 );

	return (end-start);
}

int main(void){
	unsigned long long min = 10000000;
	for(int i = 0; i < testCount; i++){
		unsigned long long tmp = returnTime();
		if(tmp < min){
			min = tmp;
		}
	}

	printf("%llu", min);

	return 0;
}

