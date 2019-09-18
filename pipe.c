#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <time.h>

const int BUFFER_SIZE = 64;

unsigned long sendAndReceive(int* pipefd, int* pipebk, int cpid, char* buffer, int receiveCount, int backCount, char buf){
        struct timespec s, t;

        if (cpid == 0) {    /* Child reads from pipe */
                close(pipefd[1]);          /* Close unused write end */
                close(pipebk[0]);

                while (read(pipefd[0], &buf, 1) > 0) {
                        receiveCount++;
                }

                if(receiveCount == BUFFER_SIZE){
                        write(pipebk[1], &buffer, BUFFER_SIZE);
                        close(pipefd[0]);
                        close(pipebk[1]);
                } else {
                        fprintf(stderr, "child receive has %d bytes\n", receiveCount);
                        exit(EXIT_FAILURE);
                }

                exit(EXIT_SUCCESS);

        } else {            /* Parent writes send to pipe */
                close(pipefd[0]);               /* Close unused read end */

                clock_gettime(CLOCK_REALTIME, &s);

                write(pipefd[1], &buffer, BUFFER_SIZE);

                close(pipefd[1]);

                wait(NULL);

                close(pipebk[1]);
                while (read(pipebk[0], &buf, 1) > 0){
                        backCount++;
                }

                if(backCount != BUFFER_SIZE){
                        fprintf(stderr, "parent receive has %d bytes\n", backCount);
                        exit(EXIT_FAILURE);
                }

                clock_gettime(CLOCK_REALTIME, &t);

                close(pipebk[0]);

                return (t.tv_nsec - s.tv_nsec);
        }

}

int main(){

        printf("Loading test module...\n");

        int pipefd[2];
        int pipebk[2];

        unsigned long min = 99999999;
        for(int i = 0; i < 1000000; i++){


                pid_t cpid;
                char buffer[BUFFER_SIZE];
                memset (buffer, 0, sizeof(buffer));
                char buf = '\0';
                int receiveCount = 0;
                int backCount = 0;

                if (pipe(pipefd) == -1) {
                        perror("pipe");
                        exit(EXIT_FAILURE);
                }

                if(pipe(pipebk) == -1) {
                        perror("pipe");
                        exit(EXIT_FAILURE);
                }

                cpid = fork();
                if (cpid == -1) {
                        perror("fork");
                        exit(EXIT_FAILURE);
                }

                unsigned long tmp = sendAndReceive(pipefd, pipebk, cpid, buffer, receiveCount, backCount, buf);

                if(tmp < min){
                        min = tmp;
                }
        }

        printf("the minimum execution time is %lu ns\n", min);

        return 0;
}
