/*
 * @name BookFiler Module - MySQL
 * @author Branden Lee
 * @version 1.00
 * @license MIT
 * @brief mySQL, SQLite3, and HTTP implementation
 */

#ifndef BOOKFILER_MODULE_MYSQL_MYSQL_H
#define BOOKFILER_MODULE_MYSQL_MYSQL_H

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

/* libmariadbclient
 * Version: 3.1.7-2
 * License: LGPL
 */
#include <mariadb/mysql.h>

/* libssh
 * Version: 0.9.4-1
 * License: LGPL
 */
#include <libssh/libssh.h>

/*
 * bookfiler - MySQL
 */
namespace bookfiler {
namespace MySQL {

class Db {
private:
  MYSQL *con;

public:
  Db() { con = mysql_init(NULL); };
  ~Db() {
    if (con) {
      mysql_close(con);
    }
  };
  int open(std::shared_ptr<rapidjson::Document>);
  int tableCreate();
};

} // namespace MySQL
} // namespace bookfiler

#endif
// end BOOKFILER_MODULE_MYSQL_MYSQL_H
