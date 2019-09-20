/** 
 * Pipe latency measurement
 * Authors: Tao Ji and Emma He
 */

#include <netdb.h> 
#include <netinet/in.h> 
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/socket.h> 
#include <sys/types.h> 

#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MULTIPLE 1024
#define RET_SZ 4
#define BATCH_SZ 100

int server(int *pipe2c, int *pipe2m, int size, int n) {

    char *buffer;
    char mul;
	int port, quickack, servsockfd, connsockfd, addrlen, on, i, j, one_size, total_size; 
	struct sockaddr_in servaddr, cliaddr; 

    // Allocate buffer
    buffer = (char *)malloc(size*sizeof(*buffer));
    printf("[MAIN] INFO: Created buffer of size %d bytes\n", size);

    // Close pipes not using
    close(pipe2c[0]);
    close(pipe2m[1]);

    for (i=0; i<n; i++) {
        if (i*10/(n+1) < (i+1)*10/(n+1)) {
            printf("[MAIN] INFO: On iteration %d out of %d\n", i, n);
        }

        for (j=0; j<BATCH_SZ; j++) {
            // Receive data
            total_size = 0;
            do {
                one_size = read(pipe2m[0], buffer + total_size, size - total_size);
                total_size += one_size;
            } while (one_size > 0 && total_size < size);
            if (total_size != size) {
                perror("[MAIN] ERROR: Failed to receive all data\n"); 
                return 1;
            }
        }

        // Send return
        if (write(pipe2c[1], "END", RET_SZ) < 0) {
            perror("ERROR: Failed to send return\n"); 
            return 1;
        }

    }

	// Close sockets
    close(pipe2c[1]);
	close(pipe2m[0]); 

    return 0;
}

int client(int *pipe2c, int *pipe2m, int size, int n) {

    char *hostname, *buffer;
    char mul;
	int port, quickack, sockfd, on, i, j, one_size, total_size; 
	struct sockaddr_in servaddr; 
    struct timespec s, t;
    unsigned long long high, low, rs, rt, min_time, tmp_time;

    // Allocate buffer
    buffer = (char *)malloc(size*sizeof(*buffer));
    printf("[CHILD] INFO: Created buffer of size %d bytes\n", size);

    // Close pipes not using
    close(pipe2c[1]);
    close(pipe2m[0]);

    min_time = 999999999;
    for (i=0; i<n; i++) {
        if (i*10/(n+1) < (i+1)*10/(n+1)) {
            printf("[CHILD] INFO: On iteration %d out of %d\n", i, n);
        }

#ifndef RDTSC
        clock_gettime(CLOCK_REALTIME, &s);
#else
	    asm volatile ("RDTSC" : "=r" (high), "=r" (low));
        rs = high << 32 | low;
#endif
        for (j = 0; j < BATCH_SZ; j++) {
            // Send data
            total_size = 0;
            do {
                one_size = write(pipe2m[1], buffer + total_size, size - total_size);
                total_size += one_size;
            } while (one_size > 0 && total_size < size);
            if (total_size != size) {
                perror("[CHILD] ERROR: Failed to send all data\n"); 
                return 1;
            }
        }

        // Receive echo
        if (read(pipe2c[0], buffer, RET_SZ) < 0) {
            perror("ERROR: Failed to receive return\n"); 
            return 1;
        }

#ifndef RDTSC
        clock_gettime(CLOCK_REALTIME, &t);
        tmp_time = (t.tv_sec - s.tv_sec) * 1e9 + t.tv_nsec - s.tv_nsec;
#else
        asm volatile ("RDTSC" : "=r" (high), "=r" (low));
        rt = high << 32 | low;
        tmp_time = rt - rs;
#endif
        if (tmp_time < min_time) {
            min_time = tmp_time;
        }
    }
	// Close sockets
	close(pipe2c[0]);
    close(pipe2m[1]); 

    printf("%.3lf\n", (size * BATCH_SZ + RET_SZ) * 1e9 / min_time / 1024.0 / 1024.0 );
    return 0;
}

int main(int argc, char* argv[]) {
    // Argument parsing
    // Arguments: <command> <size> <n>

    // pipe2c: Pipe to child (child closes 1, main closes 0)
    // pipe2m: Pipe to main 
    int pipe2c[2], pipe2m[2], size, n;
    char mul;
    pid_t cpid;

    sscanf(argv[1], "%d%c", &size, &mul);
    if (mul == 'k') {
        size *= MULTIPLE;
    }
    n = atoi(argv[2]) / BATCH_SZ;

    if (argc != 3) {
        perror("ERROR: Invalid arguments\n");
        printf("Arguments: <command> <size> <n> \n");
        return 1;
    } 

    if (pipe(pipe2c) || pipe(pipe2m)) {
        perror("ERROR: failed to pipe");
        return 1;
    }

    cpid = fork();
    if (cpid < 0) {
        perror("ERROR: failed to fork");
        return 1;
    } else if (!cpid) {
        return client(pipe2c, pipe2m, size, n);
    } else {
        return server(pipe2c, pipe2m, size, n);
    }
    
};