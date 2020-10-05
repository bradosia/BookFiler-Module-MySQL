/*
 * @name BookFiler Module - MySQL
 * @author Branden Lee
 * @version 1.00
 * @license MIT
 * @brief mySQL, SQLite3, and HTTP implementation
 */

#ifndef BOOKFILER_MODULE_MYSQL_HTTP_H
#define BOOKFILER_MODULE_MYSQL_HTTP_H

// config
#include "config.hpp"

// c++17
//#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>

/* rapidjson v1.1 (2016-8-25)
 * Developed by Tencent
 * License: MITs
 */
#include <rapidjson/document.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/reader.h> // rapidjson::ParseResult
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

/*
 * CURL 7.68.0
 * License: MIT/X inspired
 */
#define CURL_STATICLIB
#include <brotli/decode.h>
#include <curl/curl.h>

/*
 * bookfiler - MySQL
 */
namespace bookfiler {
namespace MySQL {

class HTTP {

public:
  HTTP(){};
  ~HTTP(){};
};

#if HTTPS_CURL_ENABLE
/* Writes data from HTTP request into a string buffer
 * @param ptr data address
 */
size_t writefunc(void *ptr, size_t size, size_t nmemb, std::string *s);
/* Makes an HTTPS GET request to the URI
 * @param URI The address
 */
rapidjson::Document HTTPS_GET_JSON(std::string URI);
void printJSON_value(const rapidjson::Value &a, unsigned int depth);
void printJSON_iterator(rapidjson::Value::ConstMemberIterator &itr,
                        unsigned int depth);
void getSettingsFile(std::string settingsFileString,
                     std::string &inputURIString,
                     std::string &outputFileString);
#endif

} // namespace MySQL
} // namespace bookfiler

#endif
// end BOOKFILER_MODULE_MYSQL_HTTP_H
