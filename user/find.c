#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int main(int argc, char* argv[]) {
    if(argc != 3) {
        fprintf(2, "ERROR: only 2 arguments are needed\n");
    }
    char* target_path = argv[1];
    char* target_file = argv[2];
    find(target_path,target_file);
    exit(0);
}

void find(char* path, char* target_file) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;
    
    if((fd = open(path, 0)) < 0) {
        fprintf(2, "ERROR: find can not open %s\n", path);
    }
    
}