#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <unordered_map>

std::string getRequestType(const char *buffer, int length);
std::string getHTTPVersion(const char *buffer, int length);
std::string getURI(const char *buffer, int length);
std::unordered_map<std::string, std::string> getReqHeaders(const char *buffer, int length);
std::string getReqBody(const char *buffer, int length, int contentLength);

#endif
