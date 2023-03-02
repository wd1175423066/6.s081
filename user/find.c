#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* fmtname(char *path);
void find(char *path, const char *filename);

int main(int argc, char* argv[]) {
    if(argc != 3) {
        fprintf(2, "ERROR: only 2 arguments are needed\n");
    }
    char* target_path = argv[1];
    char* target_file = argv[2];
    find(target_path,target_file);
    exit(0);
}

char*
fmtname(char *path)
{
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--);

  // different from ls
  return p + 1;
}

void find(char *path, const char *filename) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;
    
    if((fd = open(path, 0)) < 0) {
        fprintf(2, "ERROR: find can not open %s\n", path);
        return; 
    }

    if(fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type) {
        case T_FILE:
            if(strcmp(filename, fmtname(path)) == 0) printf("%s\n", path);
            break;
        case T_DIR:
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
                printf("find: path too long\n");
                return;
            }
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';
            while(read(fd, &de, sizeof(de)) == sizeof(de)) {
                if(de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;
                memmove(p, de.name, DIRSIZ);
                // char* last '0',ensure it's a right string in C
                p[DIRSIZ] = 0;
                if(stat(buf, &st) < 0) {
                    printf("find: cannot stat %s\n", buf);
                }
                find(buf, filename);
            }
            break;
    }
    close(fd);
}