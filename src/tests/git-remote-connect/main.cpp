// c++17
#include <iostream>
#include <memory>

/* rapidjson v1.1 (2016-8-25)
 * Developed by Tencent
 * License: MITs
 */
#include <rapidjson/error/en.h>
#include <rapidjson/prettywriter.h>

// Local Project
#include "Module.hpp"

std::string readFile(std::fstream &fileStream);
int readJson(std::string path,
             std::shared_ptr<rapidjson::Document> settingsDoc);

std::string testName = "GIT Connection Test";

int main() {
  int rc;
  std::shared_ptr<bookfiler::MySQL::ModuleExport> BF_Module_MySQL =
      std::make_shared<bookfiler::MySQL::ModuleExport>();
  BF_Module_MySQL->init();

  std::shared_ptr<rapidjson::Document> settingsDoc =
      std::make_shared<rapidjson::Document>();
  rc = readJson("git-remote-connect.json", settingsDoc);
  if (rc < 0) {
    return 0;
  }

  git_libgit2_init();
  bookfiler::GIT::Repository repo;
  rc = repo.openRemote(settingsDoc);
  if (rc < 0) {
    std::cout << "repository could not be opened" << std::endl;
    return 0;
  }
  repo.walk();

  git_libgit2_shutdown();

  std::cout << testName << " END" << std::endl;
  system("pause");
  return 0;
}

std::string readFile(std::fstream &fileStream) {
  fileStream.seekg(0, std::ios::end);
  size_t size = fileStream.tellg();
  std::string contentStr(size, ' ');
  fileStream.seekg(0);
  fileStream.read(&contentStr[0], size);
  return contentStr;
}

int readJson(std::string path,
             std::shared_ptr<rapidjson::Document> settingsDoc) {
  std::fstream settingsFile;
  settingsFile.open(path, std::fstream::in);
  if (!settingsFile) {
    std::cout << "settings file not found" << std::endl;
    return -1;
  }
  std::string contentStr = readFile(settingsFile);
  bool flag = settingsDoc
                  ->Parse<rapidjson::kParseCommentsFlag |
                          rapidjson::kParseTrailingCommasFlag |
                          rapidjson::kParseStopWhenDoneFlag>(contentStr.c_str())
                  .HasParseError();
  if (flag) {
    std::cout << "settings file could not be parsed: "
              << GetParseError_En(settingsDoc->GetParseError()) << std::endl;
    return -1;
  }
  return 0;
}
