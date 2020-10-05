#include <iostream>
#include <memory>

// Options
#define CURL_TEST 0
#define GIT_TEST 1
#define MYSQL_TEST 0

/* rapidjson v1.1 (2016-8-25)
 * Developed by Tencent
 * License: MITs
 */
#include <rapidjson/prettywriter.h>

// Local Project
#include "Module.hpp"

int main() {
  std::shared_ptr<bookfiler::MySQL::ModuleExport> BF_Module_MySQL =
      std::make_shared<bookfiler::MySQL::ModuleExport>();
  BF_Module_MySQL->init();

  /* test curl
   */
#if CURL_TEST
  rapidjson::Document dataDoc = bookfiler::MySQL::HTTPS_GET_JSON(
      "http://data.nba.net/prod/v1/20170201/0021600732_boxscore.json");
  rapidjson::StringBuffer buffer;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
  dataDoc.Accept(writer);
  std::cout << "bookfiler::MySQL::HTTPS_GET_JSON:\n"
            << buffer.GetString() << std::endl;
#endif

  /* test git
   */
#if GIT_TEST
  bookfiler::MySQL::walk_repo("C:/github/BookFiler-Module-MySQL");
#endif

  /* test mysql
   */
#if MYSQL_TEST

#endif

  std::cout << "end" << std::endl;
  return 0;
}
