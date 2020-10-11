#define loadModules_DEBUG 1
#define SETTINGS_FILE "settings.json"

// C++17
#include <iostream>
#include <memory>

/* rapidjson v1.1 (2016-8-25)
 * Developed by Tencent
 * License: MITs
 */
#include <rapidjson/error/en.h>
#include <rapidjson/prettywriter.h>

// bradosia libraries 1.0
#include <ModuleManager/ModuleManager.hpp>
#include <SettingsManager/SettingsManager.hpp>

// Modules
#include <BookFiler-Module-MySQL/Interface.hpp>

// Local Project
// #include ""

int loadModules();
int moduleLoaded(std::shared_ptr<bookfiler::MySQL::ModuleInterface>);
int allModulesLoaded();

std::string testName = "MySQL Connection Test";
std::shared_ptr<bookfiler::MySQL::ModuleInterface> mySQL_Module;
std::shared_ptr<bradosia::ModuleManager> moduleManagerPtr;
std::shared_ptr<bradosia::SettingsManager> settingsManagerPtr;

int main() {
  std::cout << testName << " BEGIN" << std::endl;

  loadModules();

  std::cout << testName << " END" << std::endl;
  system("pause");
  return 0;
}

int loadModules() {
#if loadModules_DEBUG
  std::cout << "loadModules() BEGIN\n";
#endif
  settingsManagerPtr = std::make_shared<bradosia::SettingsManager>();
  /* Module Load
   */
  moduleManagerPtr = std::make_shared<bradosia::ModuleManager>();
  moduleManagerPtr->addModule<bookfiler::MySQL::ModuleInterface>("mySQLModule");
  moduleManagerPtr->getCallbackLoadSignal<bookfiler::MySQL::ModuleInterface>("mySQLModule")
      ->connect(std::bind(&moduleLoaded, std::placeholders::_1));
  moduleManagerPtr->callbackLoadAllSignal.connect(std::bind(&allModulesLoaded));
  moduleManagerPtr->loadModules("modules");
#if loadModules_DEBUG
  std::cout << "loadModules() END\n";
#endif
  return 0;
}

int moduleLoaded(std::shared_ptr<bookfiler::MySQL::ModuleInterface> module) {
  mySQL_Module = module;
  /* register widgets
   */
  mySQL_Module->init();
  /* register setting deploy
   */
  std::shared_ptr<rapidjson::Document> moduleRequest =
      std::make_shared<rapidjson::Document>();
  std::shared_ptr<std::unordered_map<
      std::string, std::function<void(std::shared_ptr<rapidjson::Document>)>>>
      moduleCallbackMap = std::make_shared<std::unordered_map<
          std::string,
          std::function<void(std::shared_ptr<rapidjson::Document>)>>>();
  mySQL_Module->registerSettings(moduleRequest, moduleCallbackMap);
  settingsManagerPtr->merge(moduleRequest, moduleCallbackMap);
  return 0;
}

int allModulesLoaded() {
  int rc;

  /* Get the settings
   */
  settingsManagerPtr->deployFile(SETTINGS_FILE);

  /* Example using the module */
  std::shared_ptr<bookfiler::MySQL::Connection> mySQL_connection =
      mySQL_Module->newConnection();
  rc = mySQL_connection->open();
  if (rc < 0) {
    std::cout << "database could not be opened\n";
    return 0;
  }
  mySQL_connection->tableCreate();
  return 0;
}
