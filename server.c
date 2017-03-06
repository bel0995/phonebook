#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>  // htons()
#include <netinet/in.h> // struct sockaddr_in
#include <sys/socket.h>


#include "common.h"


void connection_handler(int socket_desc) {
    int ret;

    char send_buf[256];

    // receive command from client OR READ
    char recv_buf[256];
    size_t recv_buf_len = sizeof(recv_buf);
   
	
	if (fork()==0){
		do{
			ret= read(socket_desc, recv_buf, recv_buf_len); 
			if (ret==0) break;
			if(ret== -1){
				if (errno == EINTR) continue;
				exit(EXIT_FAILURE);
			}
		printf("  messaggio = %s \n", recv_buf);
    }while(strcmp(recv_buf, "quit"));     

 
    

    // send reply
    size_t server_message_len = strlen(send_buf);
 // SEND OR WRITE
	ret=write(socket_desc,send_buf,server_message_len);
	/*	if(ret== -1){
			if (errno == EINTR) continue;
			exit(EXIT_FAILURE);
			}
			*/
		}
    // close socket
    else 
    ret = close(socket_desc);
    ERROR_HELPER(ret, "Cannot close socket for incoming connection");
	}



int main(int argc, char* argv[]) {
    int ret;

    int socket_desc, client_desc;
    
    struct sockaddr_in server_addr = {0}, client_addr = {0};
    

    int sockaddr_len = sizeof(struct sockaddr_in); // we will reuse it for accept()

    
	server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_family      = AF_INET;
    server_addr.sin_port        = htons(SERVER_PORT); 
    
    
    // inizializzazione socket per listen()
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    ERROR_HELPER(socket_desc, "Could not create socket");

    /* We enable SO_REUSEADDR to quickly restart our server after a crash:
     * for more details, read about the TIME_WAIT state in the TCP protocol */
    int reuseaddr_opt = 1;
    ret = setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &reuseaddr_opt, sizeof(reuseaddr_opt));
    ERROR_HELPER(ret, "Cannot set SO_REUSEADDR option");

    // BIND address to socket
    ret = bind(socket_desc, (struct sockaddr*) &server_addr, sockaddr_len);
    ERROR_HELPER(ret, "Cannot bind address to socket");

    // LISTEN()
    ret = listen(socket_desc, MAX_CODA);
    ERROR_HELPER(ret, "Cannot listen on socket");

    // loop to handle incoming connections serially
    while (1) {
        client_desc = accept(socket_desc, (struct sockaddr*) &client_addr, (socklen_t*) &sockaddr_len);
        ERROR_HELPER(client_desc, "Cannot open socket for incoming connection");

        if (DEBUG) fprintf(stderr, "Incoming connection accepted...\n");

        connection_handler(client_desc);

        if (DEBUG) fprintf(stderr, "Done!\n");
    }

    exit(EXIT_SUCCESS); // this will never be executed
    
}
