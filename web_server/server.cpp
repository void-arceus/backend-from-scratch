#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <iostream>
#include <unordered_map>

#include "parser.hpp"
#include "router.hpp"

struct Request
{
    std::string type;
    std::string uri;
    std::string version;
    std::unordered_map<std::string, std::string> headers;
    std::string body;
};

int create_and_bind_socket(const int PORT)
{
    struct sockaddr_in my_addr;

    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1)
    {
        perror("SOCKET FD ERROR:");
        exit(1);
    }

    std::cout << "Socket FD: " << sock_fd << "\n";

    int opt = 1;
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(PORT);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    memset(&my_addr.sin_zero, '\0', 8);

    if (bind(sock_fd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("BIND ERROR:");
        exit(1);
    }

    return sock_fd;
}

void start_listening(int server_fd, const int BACKLOG)
{
    if (listen(server_fd, BACKLOG) == -1)
    {
        perror("LISTEN ERROR:");
        exit(1);
    }
}

void handle_client(int client_fd)
{
    char buffer[4096];
    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        size_t recv_buffer = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

        if (recv_buffer == -1)
        {
            perror("RECEIVE ERROR:");
            close(client_fd);
            return;
        }
        if (recv_buffer == 0)
            break;

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

        // send response
        const char *body;
        std::string res = "";
        if (reqData.type == "GET")
        {
            res = getRequestHandler();
        }
        else if (reqData.type == "POST")
        {
            res = postRequestHandler(reqData.uri);
        }
        else
        {
            res = "Hello from nowhere";
        }

        body = res.c_str();

        const std::string response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: " +
            std::to_string(strlen(body)) + "\r\n"
                                           "\r\n" +
            body;
        int bytes_sent = send(client_fd, response.c_str(), (int)response.size(), 0);
    }
    close(client_fd);
}
