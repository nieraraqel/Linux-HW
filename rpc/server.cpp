#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8888
#define BUFFER_SIZE 1024

struct ClientData {
    int client_socket;
};

pthread_mutex_t mutex;

std::string processCommand(const std::string& command) {
    std::istringstream iss(command);
    std::string operation;
    double num1, num2;

    if (!(iss >> operation >> num1 >> num2)) {
        return "ERROR: Invalid command format";
    }

    double result;
    if (operation == "ADD") {
        result = num1 + num2;
    } else if (operation == "SUB") {
        result = num1 - num2;
    } else if (operation == "MUL") {
        result = num1 * num2;
    } else if (operation == "DIV") {
        if (num2 == 0) return "ERROR: Division by zero";
        result = num1 / num2;
    } else {
        return "ERROR: Unknown command";
    }

    return std::to_string(result);
}

void* clientHandler(void* args) {
    ClientData* data = static_cast<ClientData*>(args);
    int client_socket = data->client_socket;
    delete data;

    char buffer[BUFFER_SIZE];

    while (true) {
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received <= 0) {
            break;
        }

        buffer[bytes_received] = '\0';
        std::string command(buffer);
        
        if (command == "exit") {
            break;
        }

        std::string response = processCommand(command);
        send(client_socket, response.c_str(), response.size(), 0);
    }

    close(client_socket);
    return nullptr;
}

int main() {
    pthread_mutex_init(&mutex, nullptr);

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        return EXIT_FAILURE;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        return EXIT_FAILURE;
    }

    if (listen(server_socket, 10) < 0) {
        perror("Listen failed");
        close(server_socket);
        return EXIT_FAILURE;
    }

    std::cout << "RPC Server is running on port " << PORT << "...\n";

    while (true) {
        sockaddr_in client_addr{};
        socklen_t client_addr_len = sizeof(client_addr);
        int client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_addr_len);

        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        std::cout << "Client connected: " << inet_ntoa(client_addr.sin_addr) << "\n";

        ClientData* client_data = new ClientData{client_socket};
        pthread_t thread;
        pthread_create(&thread, nullptr, clientHandler, client_data);
        pthread_detach(thread);
    }

    pthread_mutex_destroy(&mutex);
    close(server_socket);
    return 0;
}
