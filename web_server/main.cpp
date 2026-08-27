#include <thread>
#include <arpa/inet.h>
#include "proxy.hpp"

#define PORT 8080
#define BACKLOG 10

int main()
{
    proxy(); 
    return 0;
}
