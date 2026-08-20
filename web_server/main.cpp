#include "server.hpp"
#include <thread> 
#include <iostream> 
#include <arpa/inet.h>

#define PORT 8080
#define BACKLOG 10

int main() {
    int server_fd = create_and_bind_socket(PORT);  
    start_listening(server_fd, BACKLOG); 

    std::cout << "SERVER is runnin on PORT: " << PORT << "\n";
    
    while (true) {
        struct sockaddr_in client_addr; 
        socklen_t len = sizeof(struct sockaddr_in);

        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &len);
        if (client_fd == -1) continue;

        std::thread worker(handle_client, client_fd);
        worker.detach(); 
    }
    return 0;
}

