#include "parser.hpp"
#include <string> 

std::string getRequestType(const char *buffer, int length) 
{
    std::string res = "";
    for (int i = 0; i < length; ++i) {
        if (buffer[i] == ' ') break;
        res += buffer[i]; 
    }
    return res;
}



