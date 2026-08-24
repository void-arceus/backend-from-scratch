#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <iostream>
#include <string.h>

#include "../server.hpp"
#include "../parser.hpp"

#define PORT 3000
#define BACKLOG 10

struct Request
{
    std::string type;
    std::string uri;
    std::string version;
    std::unordered_map<std::string, std::string> headers;
    std::string body;
};

void server_one() 
{
    int server_one_fd = create_and_bind_socket(PORT);
    
    start_listening(server_one_fd, BACKLOG);

    struct sockaddr_in client_addr;
    socklen_t len = sizeof(struct sockaddr_in);
    int server_one_client_fd = accept(server_one_fd, (struct sockaddr *) &client_addr, &len);
    if (server_one_client_fd == -1) 
    {
        perror("CLIENT ERROR"); 
        return;
    }
   
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer)); 
    size_t recv_buffer = recv(server_one_client_fd, buffer, sizeof(buffer)-1, 0);
    
    if (recv_buffer == -1) 
    {
        perror("RECIEVE ERROR");
        return;
    }

 // parsing the request data
        struct Request reqData;
        reqData.type = getRequestType(buffer, recv_buffer);
        reqData.version = getHTTPVersion(buffer, recv_buffer);
        reqData.uri = getURI(buffer, recv_buffer);
        reqData.headers = getReqHeaders(buffer, recv_buffer);

        if (reqData.type == "POST" && reqData.headers.count("content-length"))
        {
            reqData.body = getReqBody(buffer, recv_buffer, stoi(reqData.headers.at("content-length")));
        }
        else
        {
            reqData.body = "";
        }

        // testing the parsed data
        std::cout << "------------------------------------------------------------------------\n";
        std::cout << "TYPE: " << reqData.type << "\n";
        std::cout << "VERSION: " << reqData.version << "\n";
        std::cout << "URI: " << reqData.uri << "\n";
        std::cout << "HEADERS:\n";
        for (auto &[key, value] : reqData.headers)
        {
            std::cout << key << ":" << value << "\n";
        }
        if (reqData.body != "")
        {
            std::cout << "BODY: " << reqData.body << "\n";
        }
        std::cout << "------------------------------------------------------------------------\n";


    
}

