#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define SERVER_IP "127.0.0.1"
#define PORT 4444
#define BUFFER_SIZE 2048

int client_socket;
char buffer[BUFFER_SIZE];

void *receive_messages(void *param) {
	while (1) {
		int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);

        	if (bytes_received > 0) {
			buffer[bytes_received] = '\0';
            		printf("\r%s\n", buffer);
        	} 
        	else if (bytes_received == 0) {
            		printf("Server disconnected.\n");
            		exit(1);
        	}
        	else {
            		perror("recv failed");
            		exit(1);
        	}
    	}
	return 0;
}

int main() {
    	struct sockaddr_in server_addr;
    	char buffer[BUFFER_SIZE];

    	client_socket = socket(AF_INET, SOCK_STREAM, 0);
    	if (client_socket == -1) {
        	perror("Can't create socket");
        	return 1;
    	}

    	memset(&server_addr, 0, sizeof(server_addr));
    	server_addr.sin_family = AF_INET;
    	server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
  	server_addr.sin_port = htons(PORT);

    	if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        	perror("Connect failed");
        	return 1;
    	}

    	printf("Connected to server\n");

	printf("Insert yout name: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strlen(buffer) - 1] = '\0';  

        send(client_socket, buffer, strlen(buffer), 0);
	
	pthread_t receive_thread;
    	if (pthread_create(&receive_thread, NULL, receive_messages, NULL) < 0) {
        	perror("Could not create thread");
        	return 1;
    	}
    	while (1) {
        	fgets(buffer, BUFFER_SIZE, stdin);
        	buffer[strlen(buffer) - 1] = '\0';  // Remove newline

        	send(client_socket, buffer, strlen(buffer), 0);
    	}
    	close(client_socket);
    	return 0;
}

