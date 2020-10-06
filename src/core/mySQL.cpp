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

int Db::open(std::shared_ptr<rapidjson::Document> settingsDoc) {
  int port;
  std::string localhost, username, password;

  /* Get settings from JSON document */
  if (!settingsDoc->IsObject()) {
    std::cout << "settings document invalid" << std::endl;
    return -1;
  }
  char memberName1[] = "localhost";
  if (settingsDoc->HasMember(memberName1) &&
      (*settingsDoc)[memberName1].IsString()) {
    localhost = (*settingsDoc)[memberName1].GetString();
  }
  char memberName2[] = "username";
  if (settingsDoc->HasMember(memberName2) &&
      (*settingsDoc)[memberName2].IsString()) {
    username = (*settingsDoc)[memberName2].GetString();
  }
  char memberName3[] = "password";
  if (settingsDoc->HasMember(memberName3) &&
      (*settingsDoc)[memberName3].IsString()) {
    password = (*settingsDoc)[memberName3].GetString();
  }
  char memberName4[] = "port";
  if (settingsDoc->HasMember(memberName4) &&
      (*settingsDoc)[memberName4].IsInt()) {
    port = (*settingsDoc)[memberName4].GetInt();
  }

  if (con == NULL) {
    std::cout << mysql_error(con) << std::endl;
    return -1;
  }

  if (mysql_real_connect(con, localhost.c_str(), username.c_str(),
                         password.c_str(), "midtelci_test", port, NULL,
                         0) == NULL) {
    std::cout << mysql_error(con) << std::endl;
    mysql_close(con);
    return -1;
  }

  return 0;
}

int Db::tableCreate() {
  std::string query = "CREATE TABLE `test_table2` (`id` INT NOT NULL "
                      "AUTO_INCREMENT,`text` BLOB "
                      "NULL DEFAULT NULL,INDEX `Index 1` "
                      "(`id`)) COLLATE='utf8_general_ci' ENGINE=InnoDB;";
  if (mysql_query(con, query.c_str())) {
    std::cout << mysql_error(con) << std::endl;
    mysql_close(con);
    return -1;
  }
}

} // namespace MySQL
} // namespace bookfiler
