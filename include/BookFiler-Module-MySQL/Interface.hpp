/*
 * @name BookFiler Module - MySQL
 * @author Branden Lee
 * @version 1.00
 * @license MIT
 * @brief mySQL, SQLite3, and HTTP implementation
 */

#ifndef BOOKFILER_MODULE_MYSQL_INTERFACE_H
#define BOOKFILER_MODULE_MYSQL_INTERFACE_H

// c++17
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

/* rapidjson v1.1 (2016-8-25)
 * Developed by Tencent
 * License: MITs
 */
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>

/*
 * bookfiler - MySQL
 */
namespace bookfiler {
namespace MySQL {

class ModuleInterface {
public:
  /* module typical methods
   * init, registerSettings
   */
  virtual void init() = 0;
  /* registerSettings
   * @brief Registers a map of callbacks for handling data from the main
   * application settings file.
   */
  virtual void registerSettings(
      std::shared_ptr<rapidjson::Document>,
      std::shared_ptr<std::unordered_map<
          std::string,
          std::function<void(std::shared_ptr<rapidjson::Document>)>>>) = 0;
};

} // namespace MySQL
} // namespace bookfiler

#endif
// end BOOKFILER_MODULE_MYSQL_INTERFACE_H
