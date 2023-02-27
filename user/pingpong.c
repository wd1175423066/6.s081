#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
    int p1[2];
    int p2[2];
    char buf[1];

    //create pipe
    pipe(p1); // parent write to child
    pipe(p2); // child write to parent 

    //create child process
    int pid = fork();

    if(pid < 0) {
        exit(1);
    }
    else if(pid == 0) {
        //child process
        close(p1[1]);
        close(p2[0]);
        read(p1[0], buf, 1);
        printf("%d:received ping\n", getpid());
        write(p2[1], " ", 1);
        close(p1[0]);
        close(p2[1]);
    }
    else {
        //parent process
        close(p1[0]);
        close(p2[1]);
        write(p1[1], " ", 1);
        read(p2[0], buf, 1);
        printf("%d:received pong\n", getpid());
        close(p1[1]);
        close(p2[0]);
        wait(0);
    }
    exit(0);
}