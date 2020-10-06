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

std::string testName = "SMTP Connection Test";

std::string readFile(std::fstream &fileStream);
int readJson(std::string path,
             std::shared_ptr<rapidjson::Document> settingsDoc);

int main() {
  int rc;
  std::shared_ptr<bookfiler::MySQL::ModuleExport> BF_Module_MySQL =
      std::make_shared<bookfiler::MySQL::ModuleExport>();
  BF_Module_MySQL->init();

  /* Get JSON settings file */
  std::shared_ptr<rapidjson::Document> settingsDoc =
      std::make_shared<rapidjson::Document>();
  rc = readJson("imap-connect.json", settingsDoc);
  if (rc < 0) {
    return 0;
  }

  curl_global_init(CURL_GLOBAL_DEFAULT);

  bookfiler::IMAP::curl::getEmails(settingsDoc);

  curl_global_cleanup();

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
