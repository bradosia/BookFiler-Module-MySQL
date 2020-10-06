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

std::string testName = "GIT Connection Test";

int main() {
  std::shared_ptr<bookfiler::MySQL::ModuleExport> BF_Module_MySQL =
      std::make_shared<bookfiler::MySQL::ModuleExport>();
  BF_Module_MySQL->init();

  bookfiler::MySQL::walk_repo("C:/github/BookFiler-Module-MySQL");

  std::cout << testName << " END" << std::endl;
  system("pause");
  return 0;
}
