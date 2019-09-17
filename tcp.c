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

int server(int argc, char* argv[]) {

    char *buffer;
    char mul;
	int size, n, port, quickack, servsockfd, connsockfd, addrlen, on, i, one_size, total_size; 
	struct sockaddr_in servaddr, cliaddr; 

    // Argument parsing (no error handling)
    // Server arguments: <size> <n> <port> [--quickack]
    sscanf(argv[0], "%d%c", &size, &mul);
    if (mul == 'k') {
        size *= MULTIPLE;
    }
    n = atoi(argv[1]);
    port = atoi(argv[2]);
    if (argc == 4 && !strcmp(argv[3], "--quickack")) {
        quickack = 1;
    } else {
        quickack = 0;
    }

    // Allocate buffer
    buffer = (char *)malloc(size);

	// Create socket
	servsockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (servsockfd == -1) { 
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

	// Bind
	if (bind(servsockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) {
		perror("ERROR: Failed to bind\n"); 
		return 1;
	} else {
		printf("INFO: Bind successful\n"); 
    }

	// Listen
	if (listen(servsockfd, 1)) {
		perror("ERROR: Failed to listen\n");
        return 1;
	} else {
		printf("INFO: Listening started\n"); 
    }

	// Accept
    addrlen = sizeof(cliaddr); 
	connsockfd = accept(servsockfd, (struct sockaddr*)&cliaddr, &addrlen); 
	if (connsockfd < 0) {
		perror("ERROR: Failed to accept\n"); 
		return 1;
	} else {
		printf("INFO: Connection accepted\n"); 
    }
    
    // Set flags
    on = 1;
    if(!setsockopt(connsockfd, IPPROTO_TCP, TCP_NODELAY, &on, sizeof(on))) {
		perror("ERROR: Failed to set TCP_NODELAY\n"); 
		return 1;
    }

    for (i=0; i<n; i++) {
        printf("INFO: On iteration %d\n", i);

        // Receive data
        total_size = 0;
        one_size = recv(connsockfd, buffer + total_size, size, 0);
        while (one_size > 0 && total_size < size) {
            total_size += one_size;
            one_size = recv(connsockfd, buffer + total_size, size, 0);
        }
        if (total_size != size) {
            perror("ERROR: Failed to send all data\n"); 
            return 1;
        }

        // Send data back
        total_size = 0;
        one_size = send(connsockfd, buffer + total_size, size, 0);
        while (one_size > 0 && total_size < size) {
            total_size += one_size;
            one_size = send(connsockfd, buffer + total_size, size, 0);
        }
        if (total_size != size) {
            perror("ERROR: Failed to send all data\n"); 
            return 1;
        }
    }

	// Close sockets
    close(connsockfd);
	close(servsockfd); 

    return 0;
}

int client(int argc, char* argv[]) {

    char *hostname, *buffer;
    char mul;
	int size, n, port, quickack, sockfd, on, i, one_size, total_size; 
	struct sockaddr_in servaddr; 
    struct timespec s, t;
    unsigned long min_time, tmp_time;

    // Argument parsing (no error handling)
    // Client arguments: <size> <n> <hostname> <port> [--quickack]
    sscanf(argv[0], "%d%c", &size, &mul);
    if (mul == 'k') {
        size *= MULTIPLE;
    }
    n = atoi(argv[1]);
    hostname = argv[2];
    port = atoi(argv[3]);
    if (argc == 5 && !strcmp(argv[4], "--quickack")) {
        quickack = 1;
    } else {
        quickack = 0;
    }

    // Allocate buffer
    buffer = (char *)malloc(size);

	// Create socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		perror("ERROR: Failed to create socket\n"); 
		return 1;
	} else {
		printf("INFO: Socket created\n");
    }
    
    // Fill in address
	bzero(&servaddr, sizeof(servaddr)); 
	servaddr.sin_family = AF_INET; 
    inet_pton(AF_INET, hostname, &(servaddr.sin_addr));
	servaddr.sin_port = htons(port); 

	// Connect
	if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) {
		perror("ERROR: Failed to connect\n"); 
		return 1;
	} else {
		printf("INFO: Connection successful\n"); 
    }

    // Set flags
    on = 1;
    if(!setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &on, sizeof(on))) {
		perror("ERROR: Failed to set TCP_NODELAY\n"); 
		return 1;
    }

    min_time = 999999999;
    for (i=0; i<n; i++) {
        printf("INFO: On iteration %d\n", i);

        clock_gettime(CLOCK_REALTIME, &s);

        // Send data
        total_size = 0;
        one_size = send(sockfd, buffer + total_size, size, 0);
        while (one_size > 0 && total_size < size) {
            total_size += one_size;
            one_size = send(sockfd, buffer + total_size, size, 0);
        }
        if (total_size != size) {
            perror("ERROR: Failed to send all data\n"); 
            return 1;
        }

        // Receive echo
        total_size = 0;
        one_size = recv(sockfd, buffer + total_size, size, 0);
        while (one_size > 0 && total_size < size) {
            total_size += one_size;
            one_size = recv(sockfd, buffer + total_size, size, 0);
        }
        if (total_size != size) {
            perror("ERROR: Failed to receive all data\n"); 
            return 1;
        }

        clock_gettime(CLOCK_REALTIME, &t);
        
        tmp_time = t.tv_nsec - s.tv_nsec;
        if (tmp_time < min_time) {
            min_time = tmp_time;
        }
    }
	// Close sockets
	close(sockfd); 

    printf("%ld\n", min_time);
    return 0;
}

int main(int argc, char* argv[]) {
    // Argument parsing
    // Arguments: <process> -(s <server_arguments>|c <client_arguments>)
    if (argc > 1) {
        if (!strcmp(argv[1], "-s")) {
            return server(argc-2, argv+2);
        } else if (!strcmp(argv[1], "-c")) {
            return client(argc-2, argv+2);
        }
    }
    perror("ERROR: Invalid arguments\n");
    return 1;
}