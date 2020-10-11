/*
 * @name BookFiler Module - MySQL
 * @author Branden Lee
 * @version 1.00
 * @license MIT
 * @brief mySQL, SQLite3, and HTTP implementation
 */

#ifndef BOOKFILER_MODULE_MYSQL_CONFIG_H
#define BOOKFILER_MODULE_MYSQL_CONFIG_H

#define MODULE_EXPORT_SET_SETTINGS_DEBUG 1
#define MODULE_EXPORT_SET_ACCOUNTS_DEBUG 1

#include <string>

namespace bookfiler {
namespace MySQL {

static std::string moduleName = "BookFiler Module MySQL";
static std::string moduleCode = "bookfiler::MySQL";

} // namespace MySQL
} // namespace bookfiler

#endif // BOOKFILER_MODULE_MYSQL_CONFIG_H
