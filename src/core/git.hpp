/*
 * @name BookFiler Module - MySQL
 * @author Branden Lee
 * @version 1.00
 * @license MIT
 * @brief mySQL, SQLite3, and HTTP implementation
 */

#ifndef BOOKFILER_MODULE_MYSQL_GIT_H
#define BOOKFILER_MODULE_MYSQL_GIT_H

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

/* libgit2
 * License: GPL2
 */
#include "git2.h"

/*
 * bookfiler - MySQL
 */
namespace bookfiler {
namespace MySQL {

int walk_repo(std::string repoPath);

} // namespace MySQL
} // namespace bookfiler

#endif
// end BOOKFILER_MODULE_MYSQL_GIT_H