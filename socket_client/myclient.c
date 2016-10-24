/* myclient.c */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#define BUF 1024
#define PORT 6543


extern
int main (int argc, char **argv)
{
    int create_socket;
    char buffer[BUF];
    struct sockaddr_in address;
    FILE *fptr;
    int size;

    if( argc < 2 )
    {
        printf("Usage: %s ServerAdresse\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if ((create_socket = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket error");
        return EXIT_FAILURE;
    }

    memset(&address,0,sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons (PORT);
    inet_aton (argv[1], &address.sin_addr);

    if (connect ( create_socket, (struct sockaddr *) &address, sizeof (address)) == 0)
    {
        printf ("Connection with server (%s) established\n", inet_ntoa (address.sin_addr));
        size=recv(create_socket,buffer,BUF-1, 0);
        if (size>0)
        {
            buffer[size]= '\0';
            printf("%s",buffer);
        }
    }
    else
    {
        perror("Connect error - no server available");
        return EXIT_FAILURE;
    }


    /*DIR *dir;
    struct dirent *direntry;
    dir = opendir("text");
    if (dir)
     {
         while ((direntry = readdir(dir)) != NULL)
         {
             printf("%s\n", direntry->d_name);
         }
         closedir(dir);
     }*/

    if (!(fptr = fopen("./text", "r")))
    {
        perror("File error - could not open file!");
        close(create_socket);
        return EXIT_FAILURE;
    }
    do
    {
        printf ("Send a message: ");
        fgets (buffer, BUF, stdin);

        send(create_socket, buffer, strlen(buffer) - 1, 0);
    }
    while (strcmp (buffer, "quit\n") != 0);
    printf("Closing socket...");
    fclose(fptr);
    if(close(create_socket) == 0)
        printf("OK!\n");
    else
        printf("FAIL! - error code: %d\n", errno);

    return EXIT_SUCCESS;
}
