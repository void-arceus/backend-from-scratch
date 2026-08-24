#include <arpa/inet.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "server.hpp"

#define PORT 8080
#define BACKLOG 10

void proxy () 
{
    int proxy_fd = create_and_bind_socket(PORT);
    start_listening(proxy_fd, BACKLOG);
    std::cout << "Server is runnign on PORT: " << PORT << "\n";

    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(struct sockaddr_in);

    int client_fd = accept(proxy_fd, (struct sockaddr*) &client_addr, &addr_len);
    
    if (client_fd == -1) 
    {
        perror("CLIENT ERROR");
        return; 
    }

    /* receive data */
    char buffer[4096];  
    memset(buffer, 0, sizeof(buffer));
    size_t recv_buffer = recv(client_fd, buffer, sizeof(buffer)-1, 0);

    if (recv_buffer == -1) 
    {
        perror("REVEIVE ERROR");
        return; 
    }

    /* server one */
    struct sockaddr_in server_one;
    
    server_one.sin_family = AF_INET;
    server_one.sin_port = htons(3000); 
    server_one.sin_addr.s_addr = INADDR_ANY;
    memset(&server_one.sin_zero, '\0', 8);
    
    int backend_one_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (backend_one_fd == -1) 
    {
        perror("BACKEND ONE FILE DESCRIPTOR,"); 
    }
    int connect_ = connect(backend_one_fd, (struct sockaddr*)& server_one, sizeof(struct sockaddr_in));
    if (connect_ == -1) 
    {
        perror("CONNECT ERROR");
        return; 
    }


    /* send data to server one */
    size_t send_buffer = send(backend_one_fd, buffer, recv_buffer, 0);      

    close(client_fd);
}
