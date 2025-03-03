#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#define REQUEST "GET / HTTP/1.1\r\nHost: httpforever.com\r\nConnection: close\r\n\r\n"

int main() {
    struct addrinfo hints{}, *res;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo("httpforever.com", "80", &hints, &res) != 0) {
        std::cerr << "Address error: " << strerror(errno) << std::endl;
        return 1;
    }

    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    
    if (sockfd == -1) {
        std::cerr << "Socket error: " << strerror(errno) << std::endl;
        freeaddrinfo(res);
        return 1;
    }

    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        std::cerr << "Connection error: " << strerror(errno) << std::endl;
        close(sockfd);
        freeaddrinfo(res);
        return 1;
    }

    freeaddrinfo(res);

    if (send(sockfd, REQUEST, strlen(REQUEST), 0) == -1) {
        std::cerr << "Error2: " << strerror(errno) << std::endl;
        close(sockfd);
        return 1;
    }

    std::ofstream outfile("httpforever.html", std::ios::binary);
    
    char buffer[4096];
    ssize_t bytes_received;

    while ((bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0)) > 0) {
        outfile.write(buffer, bytes_received);
    }

    if (bytes_received == -1) {
        std::cerr << "Error1: " << strerror(errno) << std::endl;
    }

    close(sockfd);

    outfile.close();
    std::cout << "saved in httpforever.html" << std::endl;

    return 0;
}

