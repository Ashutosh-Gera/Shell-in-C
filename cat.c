#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {

    char ch;
    int fd;

    fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("aSh");
        return errno;
    }
    while (read(fd, &ch, 1)) {
        putchar(ch);
    }
    printf("\n");
    close(fd);
    return 0;
}