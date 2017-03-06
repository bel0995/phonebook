#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>  // htons() and inet_addr()
#include <netinet/in.h> // struct sockaddr_in
#include <sys/socket.h>

#include "common.h"

int main(int argc, char* argv[]) {
    int ret;

    // variables for handling a socket
    int socket_desc;
    struct sockaddr_in server_addr = {0}; // some fields are required to be filled with 0

    // create a socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_HELPER(socket_desc, "Could not create socket");

    // set up parameters for the connection
    server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    server_addr.sin_family      = AF_INET;
    server_addr.sin_port        = htons(SERVER_PORT); // don't forget about network byte order!

    // initiate a connection on the socket
    ret = connect(socket_desc, (struct sockaddr*) &server_addr, sizeof(struct sockaddr_in));
    ERROR_HELPER(ret, "Could not create connection");

    if (DEBUG) fprintf(stderr, "Connection established!\n");
    char recv_buf[256];
    size_t recv_buf_len = sizeof(recv_buf);
       
   
   printf("digita la stringa da inviare (quit per terminare)");
   do{
	   scanf("%s", recv_buf);
	   printf ("%s\n %d\n", recv_buf,strlen(recv_buf));
	   write(socket_desc, recv_buf, recv_buf_len);
   }
   while(strcmp(recv_buf,"quit")!=0);
   
	   read(socket_desc, recv_buf, recv_buf_len);
	   printf("Messaggio del server %s \n",recv_buf );
    // close the socket
    ret = close(socket_desc);
    ERROR_HELPER(ret, "Cannot close socket");

    if (DEBUG) fprintf(stderr, "Exiting...\n");

    exit(EXIT_SUCCESS);
}
