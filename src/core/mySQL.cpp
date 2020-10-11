/*
 * @name BookFiler Module - MySQL
 * @author Branden Lee
 * @version 1.00
 * @license MIT
 * @brief mySQL, SQLite3, and HTTP implementation
 */

// Local Project
#include "mySQL.hpp"

/*
 * bookfiler - MySQL
 */
namespace bookfiler {
namespace MySQL {

ConnectionImpl::ConnectionImpl() { con = mysql_init(NULL); }
ConnectionImpl::~ConnectionImpl() {
  if (con) {
    mysql_close(con);
  }
};
int ConnectionImpl::setAccountsDoc(
    std::shared_ptr<rapidjson::Value> accountsDoc_) {
  accountsDoc = accountsDoc_;
}
int ConnectionImpl::setSettingsDoc(
    std::shared_ptr<rapidjson::Value> settingsDoc_) {
  settingsDoc = settingsDoc_;
}
int ConnectionImpl::open() {
  int port;
  std::string host, username, password, pluginDir;

  /* Get settings from JSON document */
  if (!settingsDoc->IsObject()) {
    std::cout << moduleCode
              << "::ConnectionImpl::open ERROR:\nSettings document invalid"
              << std::endl;
    return -1;
  }
  char memberName10[] = "pluginDir";
  if (settingsDoc->HasMember(memberName10) &&
      (*settingsDoc)[memberName10].IsString()) {
    pluginDir = (*settingsDoc)[memberName10].GetString();
  }

  /* Get accounts from JSON document */
  if (!accountsDoc->IsObject()) {
    std::cout << moduleCode
              << "::ConnectionImpl::open ERROR:\nAccounts document invalid"
              << std::endl;
    return -1;
  }
  char memberName1[] = "host";
  if (accountsDoc->HasMember(memberName1) &&
      (*accountsDoc)[memberName1].IsString()) {
    host = (*accountsDoc)[memberName1].GetString();
  }
  char memberName2[] = "username";
  if (accountsDoc->HasMember(memberName2) &&
      (*accountsDoc)[memberName2].IsString()) {
    username = (*accountsDoc)[memberName2].GetString();
  }
  char memberName3[] = "password";
  if (accountsDoc->HasMember(memberName3) &&
      (*accountsDoc)[memberName3].IsString()) {
    password = (*accountsDoc)[memberName3].GetString();
  }
  char memberName4[] = "port";
  if (accountsDoc->HasMember(memberName4) &&
      (*accountsDoc)[memberName4].IsInt()) {
    port = (*accountsDoc)[memberName4].GetInt();
  }

  mysql_optionsv(con, MYSQL_PLUGIN_DIR,
                 (void *)pluginDir.c_str());

  std::cout << moduleCode
            << "::ConnectionImpl::open Connection Settings:\nHost: " << host
            << "\nPort: " << port << "\nUsername: " << username
            << "\nPassword: " << password << "\nPlugin Dir: " << pluginDir << std::endl;

  if (con == NULL) {
    std::cout << moduleCode << "::ConnectionImpl::open ERROR:\n"
              << mysql_error(con) << std::endl;
    return -1;
  }

  if (mysql_real_connect(con, host.c_str(), username.c_str(), password.c_str(),
                         "test", port, NULL, 0) == NULL) {
    std::cout << moduleCode
              << "::ConnectionImpl::open mysql_real_connect ERROR:\n"
              << mysql_error(con) << std::endl;
    return -1;
  }

  return 0;
}

int ConnectionImpl::tableCreate() {
  std::string query = "CREATE TABLE `test_table2` (`id` INT NOT NULL "
                      "AUTO_INCREMENT,`text` BLOB "
                      "NULL DEFAULT NULL,INDEX `Index 1` "
                      "(`id`)) COLLATE='utf8_general_ci' ENGINE=InnoDB;";
  if (mysql_query(con, query.c_str())) {
    std::cout << mysql_error(con) << std::endl;
    return -1;
  }
  return 0;
}

} // namespace MySQL
} // namespace bookfiler
