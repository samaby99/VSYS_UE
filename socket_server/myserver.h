#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <dirent.h>
#include <sys/stat.h>
#include <strings.h>
#define BUF 1024
#define PORT 6543

int create_socket;
void signal_handler(int);
int put_file(int, char*, char*);
