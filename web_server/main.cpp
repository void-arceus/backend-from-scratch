// main.cpp

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

#define MYPORT 8080
#define BACKLOG 10
#define h_addr h_addr_list[0]

/*
    -> netdb.h - definition for network database operations
     
    incomming connections sequence of system calls 
    -socket(); - socket(Address Family, Connection Type, Protocol);
    -bind(); - bind(socked file descriptor, pointer to the address struct, length of the address);
    -listen(); - listen(socket file descriptor, backlog);  
    -accept(); - accept(socket file descriptor, pointer to address, lenght of the address);
    ... 
*/

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

    // bind() -> binding the port  
    int bind_res; 
    bind_res = bind(sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)); 
    
    if (bind_res == -1) {
        perror("BIND ERROR"); 
        exit(1);
    }
   
    // listen() -> listening to the request  
    listen(sockfd, BACKLOG);

    // accept() -> accept the connection 
    sin_size = sizeof(struct sockaddr_in);
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
    if (new_fd == -1) {
        perror("ACCEPT ERROR"); 
        exit(1);
    }

    /*
        send and revieve data
        send(socket file descriptor, pointer to message, message length in bytes, int flags); 
        recv(socket file descriptor, buffer to read the information into, maximum length of the buffer, flag);
    */
   
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
    
    /*
        - next to implement
        - use recv() to read the data
    */
   
    char buffer[1000];
    int recieve = recv(new_fd, &buffer, 1000,0); 
    if (recieve == -1) {
        perror("RECIEVE ERROR");
        exit(1);
    }
    
    std::cout << "Recieved: " << buffer << "\n";

    // gethostname()
    char hostname[1000];
    int host_name = gethostname(hostname, 1000);

    if (host_name == -1) {
        perror("HOSTNAME ERROR"); 
        exit(1);
    } 
    
    // revc(), 0 -> success, -1 -> error
    close(new_fd);
    close(sockfd);
    
    return 0; 
}
