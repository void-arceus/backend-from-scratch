#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>

std::string getRequestType(const char *buffer, int length);
std::string getHTTPVersion(const char *buffer, int length); 
std::string getURI(const char *buffer, int length);

#endif

