#include "parser.hpp"
#include <string> 
#include <unordered_map>
#include <algorithm> 
#include <cctype>

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

std::unordered_map<std::string, std::string> getReqHeaders(const char *buffer, int length) { 
    std::unordered_map<std::string, std::string> reqHeaders;
   
    int i = 0; 

    while (i < length && buffer[i] != '\n') ++i;
    ++i;
    
    while (i < length) {
        if (buffer[i] == '\r') break;
        std::string currLine = ""; 
        while (i < length) {
            if (buffer[i] == '\n') {
                break; 
            }
            if (buffer[i] != '\r') { 
                currLine += buffer[i];
            }
            ++i; 
        }
  
        // extract the key and value from here 
        std::string key = "", value = "";
        int j = 0; 
        while (j < (int)currLine.size()) {
            if (currLine[j] == ':') {
                ++j; 
                break;
            } 
            key += currLine[j];
            ++j;
        }
        while (j < (int)currLine.size()) {
            value += currLine[j]; 
            ++j;
        }
        std::transform(key.begin(), key.end(), key.begin(), [](unsigned char c) {
            return std::tolower(c);
        });
        reqHeaders[key] = value;
        ++i;
    }

    return reqHeaders;
}


