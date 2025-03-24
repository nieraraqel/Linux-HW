#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <poll.h>
#include <errno.h>

void *handle_client(void *arg) {
    int sock = *(int *)arg;
    char buffer[1024];
    int valread;
    
    while ((valread = read(sock, buffer, 1024)) > 0) {
        buffer[valread] = '\0';
        printf("Client: %s\n", buffer);
        send(sock, buffer, strlen(buffer), 0);
    }
    
    close(sock);
    printf("Client disconnected\n");
    return NULL;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    struct pollfd poll_fds[10 + 1];
    int client_sockets[10] = {0};
    pthread_t threads[10];
    
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    
    poll_fds[0].fd = server_fd;
    poll_fds[0].events = POLLIN;
    
    printf("Server listening on port %d\n", 8080);
    
    while (1) {
        int activity = poll(poll_fds, 10 + 1, -1);
        if (activity < 0 && errno != EINTR) {
            perror("poll error");
        }
        
        if (poll_fds[0].revents & POLLIN) {
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
                perror("accept");
                continue;
            }
            
            printf("New client connected\n");
            
            for (int i = 0; i < 10; i++) {
                if (client_sockets[i] == 0) {
                    client_sockets[i] = new_socket;
                    poll_fds[i + 1].fd = new_socket;
                    poll_fds[i + 1].events = POLLIN;
                    pthread_create(&threads[i], NULL, handle_client, (void*)&client_sockets[i]);
                    break;
                }
            }
        }
    }
    return 0;
}
