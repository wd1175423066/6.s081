#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


/*
key point: 
1. when the pipe is not used, close it in time
2. chech if the program can read data from pipe, if not(return 0) exit
*/


void child(int* p1);

int main(int argc, char* argv[]) {
    int p[2];
    pipe(p);
    int pid = fork();
    if(pid < 0) {
        exit(1);
    }
    else if(pid != 0) {
        //parent process
        // close pipe which is not used
        close(p[0]);
        // write number to child process
        for(int i = 2; i <= 35; ++i) {
            write(p[1], &i, sizeof(int));
        }
        close(p[1]);
        wait(0);
    }
    else{
        child(p);
    }
    exit(0);
}

void child(int* p1) {
    //child process
    //close pipe which is not used
    close(p1[1]);
    int n;
    //read first number from p
    if(read(p1[0], &n, sizeof(int)) == 0) {
        exit(1);
    }
    //pass reminded number to its child
    int cp[2];
    pipe(cp);
    int cpid = fork();

    if(cpid < 0) {
        exit(1);
    }
    else if(cpid != 0) {
        close(cp[0]);
        int prime = n;
        printf("prime %d\n", prime);
        while(read(p1[0], &n, sizeof(int))) {
            if(n % prime != 0) {
                write(cp[1], &n, sizeof(int));
            }
        }
        close(cp[1]);
        wait(0);
    }
    else {
        child(cp);
    }
    exit(0);
}