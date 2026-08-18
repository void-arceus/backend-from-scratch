#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <netdb.h>
#include <cstdint>

#include "parser.hpp" 

#define MYPORT 8080
#define BACKLOG 10
#define h_addr h_addr_list[0]

int main(int argc, char **argv) {  
    int sockfd, new_fd; 
    struct sockaddr_in my_addr; 
    struct sockaddr_in their_addr;
    unsigned int sin_size; 

    sockfd = socket(AF_INET, SOCK_STREAM, 0);  // TCP connection 
    if (sockfd == -1) {
        perror("SOCKET ERROR"); 
        exit(1);
    }
     
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(MYPORT);  
    my_addr.sin_addr.s_addr = INADDR_ANY;
    memset(&(my_addr.sin_zero), '\0', 8);

    int bind_res; 
    bind_res = bind(sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)); 
    
    if (bind_res == -1) {
        perror("BIND ERROR"); 
        exit(1);
    }
   
    listen(sockfd, BACKLOG);
    
    sin_size = sizeof(struct sockaddr_in);
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
    if (new_fd == -1) {
        perror("ACCEPT ERROR"); 
        exit(1);
    }

    const char *body = "Hello, World!\n"; 
    const std::string response = 
        "HTTP/1.1 200 OK\r\n"  
        "Content-Type: text/plain\r\n"
        "Content-Length:" + std::to_string(strlen(body)) + "\r\n\r\n"  // double \r\n means headers finished and actual data body begins
        +  
        body; 

    uint32_t len;
    int bytes_sent; 
    len = response.size(); 
    bytes_sent = send(new_fd, response.c_str(),len, 0);  
    std::cout << "Bytes sent: " << bytes_sent << "\n"; 
    
    char buffer[1000];
    int receive = recv(new_fd, buffer, 1000,0); 
    if (receive == -1) {
        perror("RECIEVE ERROR");
        exit(1);
    }
    
    std::cout << "Recieved:\n" << buffer << "\n";
    std::string reqType = getRequestType(buffer, receive);
    
    std::cout << "Request type: " << reqType << "\n";

    char hostname[1000];
    int host_name = gethostname(hostname, 1000);

    if (host_name == -1) {
        perror("HOSTNAME ERROR"); 
        exit(1);
    } 
    
    close(new_fd);
    close(sockfd);
    
    return 0; 
}
