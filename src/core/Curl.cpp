/*
 * @name BookFiler Module - MySQL
 * @author Branden Lee
 * @version 1.00
 * @license MIT
 * @brief mySQL, SQLite3, and HTTP implementation
 */

// Local Project
#include "smtpCurl.hpp"

/*
 * bookfiler - MySQL
 */
namespace bookfiler {
namespace curl {


size_t writefunc(void *ptr, size_t size, size_t nmemb, std::string *s) {
  s->append((char *)ptr, size * nmemb);
  return size * nmemb;
}

} // namespace MySQL
} // namespace bookfiler
