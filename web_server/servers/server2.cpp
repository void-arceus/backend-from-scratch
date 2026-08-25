#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <string>
#include <unistd.h>

#include "../server.hpp"

#define PORT 3001
#define BACKLOG 10

void server_two() 
{
    int server_two_fd = create_and_bind_socket(PORT);
    if (server_two_fd == -1) 
    {   
        perror("SERVER ONE FD:");
        return;
    }

    std::cout << "Server two fd: " << server_two_fd << "\n";
    start_listening(server_two_fd, BACKLOG);

    struct sockaddr_in client_addr;
    socklen_t len = sizeof(struct sockaddr_in);
    
    int server_two_client_fd = accept(server_two_fd, (struct sockaddr *) &client_addr, &len);
    
    if (server_two_client_fd == -1) 
    {
        perror("CLIENT ERROR"); 
        return; 
    }

    while (true) 
    {
    /* read request */
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer)); 
    size_t recv_buffer = recv(server_two_client_fd, buffer, sizeof(buffer)-1, 0);
    
    if (recv_buffer == -1) 
    {
        perror("SERVER ONE RECIEVE ERROR");
        continue; 
    }

    // send response 
    const char *body;
    std::string res = "Response from SERVER 2";
    
    body = res.c_str();
    
    const std::string response = 
        "HTTP/1.1 200 OK\r\n" 
        "Content-Type: text/plain\r\n"
        "Content-Length: " + 
        std::to_string(strlen(body)) + "\r\n\r\n"
        + body;
    
    int bytes_send = send(server_two_client_fd, response.c_str(), (int)response.size(), 0);

    std::cout << "Bytes send from SERVER 2: " << bytes_send << "\n";
    }
    close(server_two_client_fd);
}

