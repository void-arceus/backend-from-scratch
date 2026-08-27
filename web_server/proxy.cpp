#include <arpa/inet.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <mutex>
#include <thread>

#include "server.hpp"
#include "ThreadPool.hpp"

#define PORT 8080
#define BACKLOG 10
#define SERVER_ONE_PORT 3000
#define SERVER_TWO_PORT 3001
#define SERVER_THREE_PORT 3002

std::vector<int> server_fds(3);
std::mutex index_mutex;
int server_index = 0;
int req_count = 0;

void connect_servers()
{
    struct sockaddr_in server_one_addr, server_two_addr, server_three_addr;

    // server_one
    server_one_addr.sin_family = AF_INET;
    server_one_addr.sin_port = htons(SERVER_ONE_PORT);
    server_one_addr.sin_addr.s_addr = INADDR_ANY;
    memset(server_one_addr.sin_zero, '\0', 8);

    // server_two
    server_two_addr.sin_family = AF_INET;
    server_two_addr.sin_port = htons(SERVER_TWO_PORT);
    server_two_addr.sin_addr.s_addr = INADDR_ANY;
    memset(server_two_addr.sin_zero, '\0', 8);

    // server_three
    server_three_addr.sin_family = AF_INET;
    server_three_addr.sin_port = htons(SERVER_THREE_PORT);
    server_three_addr.sin_addr.s_addr = INADDR_ANY;
    memset(server_three_addr.sin_zero, '\0', 8);

    // connectin to each server
    int server_one_fd = socket(AF_INET, SOCK_STREAM, 0);
    int server_two_fd = socket(AF_INET, SOCK_STREAM, 0);
    int server_three_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_one_fd == -1 || server_two_fd == -1 || server_three_fd == -1)
    {
        perror("SOCKET ERROR:");
        return;
    }

    server_fds[0] = server_one_fd;
    server_fds[1] = server_two_fd;
    server_fds[2] = server_three_fd;

    int connect_one = connect(server_one_fd, (struct sockaddr *)&server_one_addr, sizeof(struct sockaddr_in));
    int connect_two = connect(server_two_fd, (struct sockaddr *)&server_two_addr, sizeof(struct sockaddr_in));
    int connect_three = connect(server_three_fd, (struct sockaddr *)&server_three_addr, sizeof(struct sockaddr_in));

    if (connect_one == -1)
    {
        perror("SERVER ONE CONNECT ERROR");
        return;
    }

    if (connect_two == -1)
    {
        perror("SERVER TWO CONNECT ERROR");
        return;
    }

    if (connect_three == -1)
    {
        perror("SERVER THREE CONNECT ERROR");
        return;
    }
    return;
}

void handle_proxy(int client_fd)
{
    char buffer[4096];
    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        size_t recv_buffer = recv(client_fd, buffer, sizeof(buffer), 0);

        if (recv_buffer == -1)
        {
            perror("REVEIVE ERROR");
            close(client_fd);
            break;
        }

        if (recv_buffer == 0)
        {
            close(client_fd);
            break;
        }

        /* create connection to the servers */
        int curr_backend_fd;

        {
            std::lock_guard<std::mutex> lock(index_mutex);
            curr_backend_fd = server_fds[(server_index % 3)];
            ++server_index;
        }

        int forward_request = send(curr_backend_fd, buffer, recv_buffer, 0);

        char server_response[4096];
        memset(server_response, 0, sizeof(server_response));
        int response_buffer = recv(curr_backend_fd, server_response, sizeof(server_response), 0);
        if (response_buffer == -1)
        {
            perror("SERVER RESPONSE BUFFER ERROR:");
        }

        int client_response = send(client_fd, server_response, response_buffer, 0);
        if (client_response == -1)
        {
            perror("CLIENT RESPONSE ERROR:");
        }
    }
    close(client_fd);
}

void proxy()
{
    int proxy_fd = create_and_bind_socket(PORT);
    if (proxy_fd == -1)
    {
        perror("FILE DESCRIPTOR ERROR:");
        return;
    }
    start_listening(proxy_fd, BACKLOG);

    std::cout << "Server is running of PORT: " << PORT << "\n";

    /* connecting to the servers*/
    connect_servers();

    unsigned int num_threads = std::thread::hardware_concurrency();
    ThreadPool Pool(num_threads);

    while (true)
    {
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(struct sockaddr_in);

        int client_fd = accept(proxy_fd, (struct sockaddr *)&client_addr, &addr_len);

        if (client_fd == -1)
        {
            continue;
        }
        std::cout << "Request count: " << req_count << "\n";
        ++req_count;
        Pool.addTask(handle_proxy, client_fd);
    }
}
