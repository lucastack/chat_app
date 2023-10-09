#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 4444
#define BUFFER_SIZE 2048
#define MAX_CLIENTS 30
#define FALSE 0
#define TRUE 1

int client_sockets[MAX_CLIENTS] = {0};


void *client_handler(void *socket_desc) {
    	int new_sock = *(int *)socket_desc;
    	char buffer[BUFFER_SIZE];
    	int bytes_received;
	int is_name_set = 0;
	char *name = NULL;
    	
	while ((bytes_received = recv(new_sock, buffer, BUFFER_SIZE, 0)) > 0) {
        	buffer[bytes_received] = '\0';
		if (is_name_set == 0){
			name = (char *)malloc(strlen(buffer));
			strcpy(name, buffer);
			is_name_set = 1;
			printf("Client with name: %s\n", buffer);
		} else {
			printf("%s: %s\n", name, buffer);
			char *formatted_message = (char *)malloc(strlen(name) + strlen(buffer) + 3);
			if (formatted_message == NULL) {
                		perror("Memory allocation failed");
                		free(name);
                		close(new_sock);
                		free(socket_desc);
                		return 0;
            		}
			sprintf(formatted_message, "%s: %s", name, buffer);	
        		for (int i = 0; i < MAX_CLIENTS; i++) {
            			if (client_sockets[i] != 0 && client_sockets[i] != new_sock) {
                			send(client_sockets[i], formatted_message, strlen(formatted_message), 0);
            			}
        		}
    		}
	}
    	if (bytes_received == 0) {
        	printf("Client disconnected\n");
    	} else if (bytes_received == -1) {
        	perror("recv failed");
    	}
    	for (int i = 0; i < MAX_CLIENTS; i++) {
        	if (client_sockets[i] == new_sock) {
            		client_sockets[i] = 0;
            		break;
        	}
    	}
    	close(new_sock);
	free(name);
    	free(socket_desc);
    	return 0;
}


int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Can't create socket");
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        return 1;
    }

    if (listen(server_socket, 3) == -1) {
        perror("Listen failed");
        return 1;
    }

    printf("Server started and waiting for connections...\n");

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
        printf("Client connected\n");

        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (client_sockets[i] == 0) {
                client_sockets[i] = client_socket;
                pthread_t client_thread;
                int *new_sock = malloc(1);
                *new_sock = client_socket;

                if (pthread_create(&client_thread, NULL, client_handler, (void *)new_sock) < 0) {
                    perror("Could not create thread");
                    return 1;
                }
                break;
            }
        }
    }

    close(server_socket);
    return 0;
}

