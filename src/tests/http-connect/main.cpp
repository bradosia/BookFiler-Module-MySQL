// c++17
#include <iostream>
#include <memory>

/* rapidjson v1.1 (2016-8-25)
 * Developed by Tencent
 * License: MITs
 */
#include <rapidjson/prettywriter.h>

// Local Project
#include "Module.hpp"

std::string testName = "HTTP Connection Test";

int main() {
  std::shared_ptr<bookfiler::MySQL::ModuleExport> BF_Module_MySQL =
      std::make_shared<bookfiler::MySQL::ModuleExport>();
  BF_Module_MySQL->init();

  curl_global_init(CURL_GLOBAL_DEFAULT);

  rapidjson::Document dataDoc = bookfiler::HTTP::HTTPS_GET_JSON(
      "http://data.nba.net/prod/v1/20170201/0021600732_boxscore.json");
  rapidjson::StringBuffer buffer;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
  dataDoc.Accept(writer);
  std::cout << "bookfiler::MySQL::HTTPS_GET_JSON:\n"
            << buffer.GetString() << std::endl;

  curl_global_cleanup();

  std::cout << testName << " END" << std::endl;
  system("pause");
  return 0;
}
