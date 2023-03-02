#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAX_ARG_LEN 1024

int main(int argc, char* argv[]) {
    int pid, n, buf_index = 0;
    char buf, arg[MAX_ARG_LEN], *args[MAXARG];

    if(argc < 2) {
        fprintf(2, "usage: xargs ...\n");
    }

      // read arguments of xargs
    for(int i = 1; i < argc; ++i) {
        args[i - 1] = argv[i];
    }

      // read arg from stdin
    while((n = read(0, &buf, 1)) > 0) {
        if(buf == '\n') {
            arg[buf_index] = 0;
            if((pid = fork()) < 0) {
                fprintf(2, "fork error\n");
                exit(1);
            }
            else if(pid == 0) {
                //child process
                // put arg (stdin) into argv (xargs, command + param)
                args[argc - 1] = arg;
                args[argc] = 0;
                exec(args[0], args);
            }
            else {
                //parent process
                wait(0);
                buf_index = 0;
            }
        }
        else{
            arg[buf_index++] = buf;
        }
    }
    exit(0);
}