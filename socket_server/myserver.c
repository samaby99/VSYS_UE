/* myserver.c */

#include "myserver.h"

int main (void)
{
    int new_socket;
    socklen_t addrlen;
    char buffer[BUF];
    char DL_dir[11];
    int size;
    struct sockaddr_in address, cliaddress;

    strcpy(DL_dir, "./Downloads"); // set the destination directory

    create_socket = socket (AF_INET, SOCK_STREAM, 0);

    memset(&address,0,sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons (PORT);

    if (bind ( create_socket, (struct sockaddr *) &address, sizeof (address)) != 0)
    {
        perror("bind error");
        return EXIT_FAILURE;
    }
    listen (create_socket, 1); // set to higher number later

    addrlen = sizeof (struct sockaddr_in);

    while (1)
    {
        printf("Waiting for connections...\n");
        new_socket = accept ( create_socket, (struct sockaddr *) &cliaddress, &addrlen );
        signal(SIGINT, signal_handler);
        if (new_socket > 0)
        {
            printf ("Client connected from %s:%d...\n", inet_ntoa (cliaddress.sin_addr),ntohs(cliaddress.sin_port));
            strcpy(buffer,"Welcome to myserver, Please enter your command:\n");
            send(new_socket, buffer, strlen(buffer),0);
        }
        do
        {
            size = recv (new_socket, buffer, BUF-1, 0);
            if( size > 0)
            {
                buffer[size] = '\0';
                //printf ("Message received (%d): %s\n", size, buffer);
                if ((strncmp (buffer, "put ", 4) == 0) &&
                     (strlen(buffer) > 4))
                    put_file (new_socket, DL_dir, buffer + 4);

            }
            else if (size == 0)
            {
                printf("Client closed remote socket\n");
                break;
            }
            else
            {
                perror("recv error");
                return EXIT_FAILURE;
            }
        }
        while (strncmp (buffer, "quit", 4)  != 0);
        close (new_socket);
    }
}

int put_file(int socket, char *DL_dir, char *filename)
{
    DIR *dir;
    //struct dirent *direntry;
    FILE *file;
    char path_and_file[BUF];

    dir = opendir(DL_dir);

    if(!dir) // check if download-directory exists; if not, create one
    {
        mkdir(DL_dir, S_IRWXU); // permissions: read, write, execute/search by owner
        dir = opendir(DL_dir);
        if(!dir)
            perror("File access error - could not create Downloads-directory\n");
        return EXIT_FAILURE;
    }

    sprintf(path_and_file, "%s/%s", DL_dir, filename); // combine path and filename

    if(access(path_and_file, W_OK) == 0) // check if file to write already exists
    {
        printf("File access error - \"%s\" already exists.\n", path_and_file);
        return EXIT_FAILURE;
    }

    send(socket, "OK", 2, 0);
    file = fopen(path_and_file, "w");
    printf("opened file %s\n", path_and_file); // debug
    fwrite("test-text", sizeof(char), 8, file);

    fclose(file);

    /*if (d)
     {
         while ((dir = readdir(d)) != NULL)
         {
             printf("%s\n", dir->d_name);
         }
         closedir(d);
     }*/


    // TODO create file, handshake, receive & write until done

    return 0;
}


void signal_handler(int sig_num)
{
    printf("Interrupt! Server terminating! Closing Socket...");
    if (close (create_socket) == 0)
        printf("OK!\n");
    else
        printf("FAIL!\n");

}
