/*
 * @name BookFiler Module - MySQL
 * @author Branden Lee
 * @version 1.00
 * @license MIT
 * @brief mySQL, SQLite3, and HTTP implementation
 */

#ifndef BOOKFILER_MODULE_IMAP_CURL_H
#define BOOKFILER_MODULE_IMAP_CURL_H

// Local Project
#include "Curl.hpp"

/*
 * bookfiler - MySQL
 */
namespace bookfiler {
namespace IMAP {
namespace curl {

int getEmails(std::shared_ptr<rapidjson::Document> settings);

} // namespace curl
} // namespace IMAP
} // namespace bookfiler

#endif
// end BOOKFILER_MODULE_MYSQL_HTTP_H
