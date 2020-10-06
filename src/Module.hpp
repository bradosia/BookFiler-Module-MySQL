/*
 * @name BookFiler Module - MySQL
 * @author Branden Lee
 * @version 1.00
 * @license MIT
 * @brief mySQL, SQLite3, and HTTP implementation
 */

#ifndef BOOKFILER_MODULE_MYSQL_EXPORT_H
#define BOOKFILER_MODULE_MYSQL_EXPORT_H

// config
#include "core/config.hpp"

// c++17
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>

/* boost 1.72.0
 * License: Boost Software License (similar to BSD and MIT)
 */
#include <boost/config.hpp> // for BOOST_SYMBOL_EXPORT

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

// Local Project
#include "BookFiler-Module-MySQL/Interface.hpp"
#include "core/git.hpp"
#include "core/httpCurl.hpp"
#include "core/imapCurl.hpp"
#include "core/mySQL.hpp"
#include "core/smtpCurl.hpp"

/*
 * bookfiler - MySQL
 */
namespace bookfiler {
namespace MySQL {

std::string moduleName = "BookFiler Module MySQL";

/*
 * This widget uses the MVC design pattern.
 * This is the controller, the view is a QT widget, and the model is the API
 * storage
 */
class ModuleExport : public ModuleInterface {
public:
  ModuleExport(){};
  ~ModuleExport(){};

  /* module typical methods
   * init, registerSettings
   */
  void init() { std::cout << moduleName << ": init()" << std::endl; }
  /* registerSettings
   * @brief Registers a map of callbacks for handling data from the main
   * application settings file.
   */
  void registerSettings(
      std::shared_ptr<rapidjson::Document> moduleRequest,
      std::shared_ptr<std::unordered_map<
          std::string,
          std::function<void(std::shared_ptr<rapidjson::Document>)>>>);
  void setSettings(std::shared_ptr<rapidjson::Value> data);
};

// Exporting `my_namespace::module` variable with alias name `module`
// (Has the same effect as `BOOST_DLL_ALIAS(my_namespace::module, module)`)
// extern "C" BOOST_SYMBOL_EXPORT ModuleExport mySQLModule;
// ModuleExport mySQLModule;

} // namespace MySQL
} // namespace bookfiler

#endif
// end BOOKFILER_MODULE_MYSQL_EXPORT_H
