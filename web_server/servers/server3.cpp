#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <string>
#include <unistd.h>

#include "../server.hpp"

#define PORT 3002
#define BACKLOG 10

void server_three() 
{
    int server_three_fd = create_and_bind_socket(PORT);
    if (server_three_fd == -1) 
    {   
        perror("SERVER THREE FD:");
        return;
    }

    std::cout << "Server three fd: " << server_three_fd << "\n";
    start_listening(server_three_fd, BACKLOG);

    struct sockaddr_in client_addr;
    socklen_t len = sizeof(struct sockaddr_in);
    
    int server_three_client_fd = accept(server_three_fd, (struct sockaddr *) &client_addr, &len);
    
    if (server_three_client_fd == -1) 
    {
        perror("CLIENT ERROR"); 
        return; 
    }

    while (true) 
    {
    /* read request */
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer)); 
    size_t recv_buffer = recv(server_three_client_fd, buffer, sizeof(buffer)-1, 0);
    
    if (recv_buffer == -1) 
    {
        perror("SERVER THREE RECIEVE ERROR");
        continue; 
    }

    // send response 
    const char *body;
    std::string res = "Response from SERVER 3";
    
    body = res.c_str();
    
    const std::string response = 
        "HTTP/1.1 200 OK\r\n" 
        "Content-Type: text/plain\r\n"
        "Content-Length: " + 
        std::to_string(strlen(body)) + "\r\n\r\n"
        + body;
    
    int bytes_send = send(server_three_client_fd, response.c_str(), (int)response.size(), 0);

    std::cout << "Bytes send from SERVER 3: " << bytes_send << "\n";
    }
    close(server_three_client_fd);
}

