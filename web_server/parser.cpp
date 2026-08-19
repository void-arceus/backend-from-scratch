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

std::string getHTTPVersion(const char *buffer, int length) {
    std::string res = "";
    int i = 0; 
    while (i < length && buffer[i] != 'H') ++i;
    while (i < length) {
        if (buffer[i] == '\r' || buffer[i] == '\n') break;
        res += buffer[i]; 
        ++i;
    }
    return res;
}

std::string getURI(const char *buffer, int length) {
    std::string res = ""; 
    int i = 0; 
    while (i < length && buffer[i] != '/') ++i; 
    while (i < length && buffer[i] != ' ') {
        res += buffer[i]; 
        ++i; 
    }
    return res;
}

