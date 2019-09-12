#include <linux/module.h>
#include <linux/kernel.h>
// #include <linux/init.h>
// #include <linux/hardirq.h>
// #include <linux/preempt.h>
#include <linux/sched.h>
// #include <asm/uaccess.h>
#include <stdio.h>


unsigned long long returnTime(){
	unsigned long long start, end;
	unsigned long long cycles_low, cycles_high, cycles_low1, cycles_high1;

	// inline assembly read timestamp twice
	__asm__ __volatile__ (
		"RDTSC" : "=r" (cycles_high), "=r" (cycles_low));
	__asm__ __volatile__ (	
		"RDTSC;" : "=r" (cycles_high1), "=r" (cycles_low1));

	start = ( (cycles_high << 32) | cycles_low );
       	end = ( (cycles_high1 << 32) | cycles_low1 );

	return (end-start);
}

int main(void){

	//unsigned long flags;
	//unsigned long long start, end;
	//unsigned long long cycles_low, cycles_high, cycles_low1, cycles_high1;
	printf("Loading test module...\n");

	// disable the preemption on the CPU
	// preempt_disable();
	
	// disable the hard interrups on the CPU
	// raw_local_irq_save(flags);

	// call a serializing instruction to make sure
	// the CPU has finished all preceding instructions
	

	// inline assembly to get the time stamp of cpu twice
	// declare the clobbered register to keep register untainted
	/*__asm__ __volatile__ (
		"RDTSC" : "=r" (cycles_high), "=r" (cycles_low));
	__asm__ __volatile__ (	
		"RDTSC;" : "=r" (cycles_high1), "=r" (cycles_low1));
	*/

	// enable the hart interrupts on the CPU
	// raw_local_irq_restore(flags);	

	// enable the preemption
	// preempt_enable();

	//start = ( (cycles_high << 32) | cycles_low );
       	//end = ( (cycles_high1 << 32) | cycles_low1 );

	unsigned long long min = 10000000;
	for(int i = 0; i < 1000000; i++){
		unsigned long long tmp = returnTime();
		if(tmp < min){
			min = tmp;
		}
	}

	printf("the minimum execution time is %llu clock cycles\n", min);

	return 0;
}

/*unsigned long long returnTime(){
	unsigned long long start, end;
	unsigned long long cycles_low, cycles_high, cycles_low1, cycles_high1;

	// inline assembly read timestamp twice
	__asm__ __volatile__ (
		"RDTSC" : "=r" (cycles_high), "=r" (cycles_low));
	__asm__ __volatile__ (	
		"RDTSC;" : "=r" (cycles_high1), "=r" (cycles_low1));

	start = ( (cycles_high << 32) | cycles_low );
       	end = ( (cycles_high1 << 32) | cycles_low1 );

	return (end-start);
}*/
