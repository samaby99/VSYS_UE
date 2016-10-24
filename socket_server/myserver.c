/* myserver.c */
#include "myserver.h"

int create_socket;

int main (void) {
  int new_socket;
  socklen_t addrlen;
  char buffer[BUF];
  int size;
  struct sockaddr_in address, cliaddress;

  signal(SIGINT, signal_handler);

  create_socket = socket (AF_INET, SOCK_STREAM, 0);

  memset(&address,0,sizeof(address));
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons (PORT);

  if (bind ( create_socket, (struct sockaddr *) &address, sizeof (address)) != 0) {
     perror("bind error");
     return EXIT_FAILURE;
  }
  listen (create_socket, 1); // set to higher number later

  addrlen = sizeof (struct sockaddr_in);

  while (1) {
     printf("Waiting for connections...\n");
     new_socket = accept ( create_socket, (struct sockaddr *) &cliaddress, &addrlen );
     if (new_socket > 0)
     {
        printf ("Client connected from %s:%d...\n", inet_ntoa (cliaddress.sin_addr),ntohs(cliaddress.sin_port));
        strcpy(buffer,"Welcome to myserver, Please enter your command:\n");
        send(new_socket, buffer, strlen(buffer),0);
     }
     do {
        size = recv (new_socket, buffer, BUF-1, 0);
        if( size > 0)
        {
           buffer[size] = '\0';
           //printf ("Message received (%d): %s\n", size, buffer);
           if (strncmp (buffer, "put", 3) == 0)
           {
               if(put_file (new_socket, buffer + 4) != 0)
               {
                   //TODO error-handling
               }
           }

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
     } while (strncmp (buffer, "quit", 4)  != 0);
     close (new_socket);
  }
}

int put_file(int socket, char* filename)
{
    printf("-%s-\n", filename);

    // TODO create file, handshake, receive & write until done

    return 0;
}


void signal_handler(int sig_num)
{
    close (create_socket);
    printf("sigint");

}
