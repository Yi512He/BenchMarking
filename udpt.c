/** 
 * UDP throughput measurement
 * Author: Tao Ji
 */

#include <netdb.h> 
#include <netinet/in.h> 
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/socket.h> 
#include <sys/types.h> 
#include <sys/time.h>

#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#define MULTIPLE 1024
#define TIMEOUT_USEC 999999
#define BATCH_SZ 100

const unsigned int END = 0xffffffff;

int server(int argc, char* argv[]) {

    char *buffer;
    char mul;
	int size, n, port, sockfd, addrlen, on, i, j, one_size, total_size; 
	struct sockaddr_in servaddr, cliaddr; 
    unsigned int *record;
    unsigned long long seq;
    struct timeval timeout;
    unsigned long long high, low, rs, rt, min_time, tmp_time;
    struct timespec s, t;
    double maxt, tempt;

    // Argument parsing (no error handling)
    // Server arguments: <size> <n> <port>
    sscanf(argv[0], "%d%c", &size, &mul);
    if (mul == 'k') {
        size *= MULTIPLE;
        if (size > 65507) {
            size = 65507;
        }
    }
    n = atoi(argv[1]) / BATCH_SZ;
    if (!n) {
        n = 1;
    }
    port = atoi(argv[2]);

    // Allocate buffer
    buffer = (char *)malloc(size);
    printf("INFO: Created buffer of size %d bytes\n", size);

    // // Allocate record
    // record = (unsigned int *)malloc(n*sizeof(*record));
    // bzero(record, n*sizeof(*record));

	// Create socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
	if (sockfd == -1) { 
		perror("ERROR: Failed to create socket\n"); 
		return 1;
	} else {
		printf("INFO: Socket created\n");
    }

    // Fill in address
	bzero(&servaddr, sizeof(servaddr)); 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(port); 
    addrlen = sizeof(cliaddr);

    // Reuse addr
    on = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    
    // Set recv timeout to prevent recv deadlock
    timeout.tv_sec = 0;
    timeout.tv_usec = TIMEOUT_USEC;
    if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout))){
        perror("ERROR: Failed to set timeout");
        return 1;
    }

	// Bind
	if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) {
		perror("ERROR: Failed to bind\n"); 
		return 1;
	} else {
		printf("INFO: Bind successful\n"); 
    }

    // maxt = 0;
    
    for (i=0; i<n; i++) {
        if (i*10/(n+1) < (i+1)*10/(n+1)) {
            printf("INFO: On batch %d out of %d\n", i, n);
        }

        // Server does nothing but echoing
        // Receive data
        // on = 0;
BATCH:  for (j=0; j<BATCH_SZ; j++) {
        //     if (on) {
        //         j = 0;
        //     }
        one_size = recvfrom(sockfd, buffer, size, 0, (struct sockaddr*)&cliaddr, &addrlen);
            if (one_size < 0) {
                if (j == 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
                    // Timeout because client not started, do again
                    goto BATCH;
                } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    // Send something to push the client
                    one_size = sendto(sockfd, buffer, size, 0, (struct sockaddr*)&cliaddr, addrlen);
                    goto BATCH;
                }
                perror("ERROR: Failed to recvfrom");
                return 1;
            }

            if (!memcmp(buffer, &END, sizeof(END))) {
                break;
            }
        
        }

        // Response only when a whole batch has arrived
        memcpy(buffer, &END, sizeof(END));
        one_size = sendto(sockfd, buffer, size, 0, (struct sockaddr*)&cliaddr, addrlen);
        if (one_size < 0) {
            perror("ERROR: Failed to sendto");
            return 1;
        }
        bzero(buffer, sizeof(END));
// #ifndef RDTSC
//         clock_gettime(CLOCK_REALTIME, &t);
//         tmp_time = t.tv_nsec - s.tv_nsec;
// #else
//         asm volatile ("RDTSC" : "=r" (high), "=r" (low));
//         rt = high << 32 | low;
//         tmp_time = rt - rs;
// #endif
//         tempt = (size * j + sizeof(END)) * 1e9 / tmp_time / 1024.0 / 1024.0;
//         if (tempt > maxt) {
//             maxt = tempt;
//         }        
//     }

    // printf("%.3lf\n", maxt);

    // memcpy(buffer, &maxt, sizeof(maxt));
    // one_size = sendto(sockfd, buffer, size, 0, (struct sockaddr*)&cliaddr, addrlen);
    // if (one_size < 0) {
    //     perror("ERROR: Failed to report throughput");
    //     return 1;
    // }
    }
	// Close sockets
	close(sockfd); 

    return 0;
}

int client(int argc, char* argv[]) {

    char *hostname, *buffer;
    char mul;
	int size, n, port, sockfd, addrlen, on, i, j, one_size, total_size; 
	struct sockaddr_in servaddr; 
    struct timespec s, t;
    unsigned long long high, low, rs, rt, min_time, tmp_time;
    struct timeval timeout;
    double maxt, tempt;

    // Argument parsing (no error handling)
    // Client arguments: <size> <n> <hostname> <port>
    sscanf(argv[0], "%d%c", &size, &mul);
    if (mul == 'k') {
        size *= MULTIPLE;
        if (size > 65507) {
            size = 65507;
        }
    }
    n = atoi(argv[1]) / BATCH_SZ;
    if (!n) {
        n = 1;
    }
    hostname = argv[2];
    port = atoi(argv[3]);

    // Allocate buffer
    buffer = (char *)malloc(size);
    printf("INFO: Created buffer of size %d bytes\n", size);

	// Create socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
	if (sockfd == -1) { 
		perror("ERROR: Failed to create socket\n"); 
		return 1;
	} else {
		printf("INFO: Socket created\n");
    }
    
    // Fill in address
	bzero(&servaddr, sizeof(servaddr)); 
	servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr(hostname);
	servaddr.sin_port = htons(port); 
    addrlen = sizeof(servaddr);

    min_time = 999999999;
    for (i=0; i<n; i++) {
        if (i*10/(n+1) < (i+1)*10/(n+1)) {
            printf("INFO: On batch %d out of %d\n", i, n);
        }

#ifndef RDTSC
        clock_gettime(CLOCK_REALTIME, &s);
#else
	    asm volatile ("RDTSC" : "=r" (high), "=r" (low));
        rs = high << 32 | low;
#endif
        for (j=0; j<BATCH_SZ; j++) {
            // Send data
            one_size = sendto(sockfd, buffer, size, 0, (struct sockaddr*)&servaddr, addrlen);
            if (one_size < 0) {
                perror("ERROR: Failed to sendto");
                return 1;
            }
            
        }

        // // END
        // memcpy(buffer, &END, sizeof(END));
        // one_size = sendto(sockfd, buffer, size, 0, (struct sockaddr*)&servaddr, addrlen);
        // if (one_size < 0) {
        //     perror("ERROR: Failed to sendto");
        //     return 1;
        // }
        
        // Receive echo
        one_size = recvfrom(sockfd, buffer, size, 0, (struct sockaddr*)&servaddr, &addrlen);
        if (one_size <0) {
            perror("ERROR: Failed to recvfrom");
            return 1;
        }
        // Not END here -> abandon this batch
        if (memcmp(buffer, &END, sizeof(END))) {
            continue;
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

        bzero(buffer, sizeof(END));
    }
    maxt = (size * BATCH_SZ + 4) * 1e9 / min_time / 1024.0 / 1024.0;
    printf("%.3lf\n", maxt);

    // one_size = recvfrom(sockfd, buffer, size, 0, (struct sockaddr*)&servaddr, &addrlen);
    // if (one_size <0) {
    //     perror("ERROR: Failed to receive report");
    //     return 1;
    // }
    // // throughput report received
    // memcpy(&maxt, buffer, sizeof(maxt));

    // printf("%.3lf\n", maxt);

	// Close sockets
	close(sockfd); 

    
    return 0;
}

int main(int argc, char* argv[]) {
    // Argument parsing
    // Arguments: <command> -(s <server_arguments>|c <client_arguments>)
    if (argc > 1) {
        if (!strcmp(argv[1], "-s")) {
            return server(argc-2, argv+2);
        } else if (!strcmp(argv[1], "-c")) {
            return client(argc-2, argv+2);
        }
    }
    perror("ERROR: Invalid arguments\n");
    printf("Arguments: <command> -(s <server_arguments>|c <client_arguments>)\n" \
           "Server arguments: <size> <n> <port> [--quickack]\n" \
           "Client arguments: <size> <n> <hostname> <port> [--quickack]\n");
    return 1;
}