/*
 * @name BookFiler Module - MySQL
 * @author Branden Lee
 * @version 1.00
 * @license MIT
 * @brief mySQL, SQLite3, and HTTP implementation
 */

#ifndef BOOKFILER_MODULE_MYSQL_HTTP_H
#define BOOKFILER_MODULE_MYSQL_HTTP_H

// Local Project
#include "Curl.hpp"

/*
 * bookfiler - MySQL
 */
namespace bookfiler {
namespace HTTP {

class Json {

public:
  Json(){};
  ~Json(){};
};

#if HTTPS_CURL_ENABLE
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

} // namespace HTTP
} // namespace bookfiler

#endif
// end BOOKFILER_MODULE_MYSQL_HTTP_H
