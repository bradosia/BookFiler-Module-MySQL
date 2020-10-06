/*
 * @name BookFiler Module - MySQL
 * @author Branden Lee
 * @version 1.00
 * @license MIT
 * @brief mySQL, SQLite3, and HTTP implementation
 */

// Local Project
#include "imapMailio.hpp"

/*
 * bookfiler - MySQL
 */

namespace bookfiler {
namespace IMAP {
namespace mailio {

int getEmails(std::shared_ptr<rapidjson::Document> settingsDoc) {
  int port;
  std::string host, username, password, url;

  /* Get settings from JSON document */
  if (!settingsDoc->IsObject()) {
    std::cout << "settings document invalid" << std::endl;
    return -1;
  }
  char memberName1[] = "host";
  if (settingsDoc->HasMember(memberName1) &&
      (*settingsDoc)[memberName1].IsString()) {
    host = (*settingsDoc)[memberName1].GetString();
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

  try {
    // connect to server
    ::mailio::imaps conn(host, port);
    // modify to use an existing zoho account
    conn.authenticate(username, password, ::mailio::imaps::auth_method_t::LOGIN);
    // query inbox statistics
    ::mailio::imaps::mailbox_stat_t stat = conn.statistics("inbox");
    std::cout << "Number of messages in mailbox: " << stat.messages_no
              << std::endl;
  } catch (::mailio::imap_error &exc) {
    std::cout << exc.what() << std::endl;
  } catch (::mailio::dialog_error &exc) {
    std::cout << exc.what() << std::endl;
  }

  return 0;
}

} // namespace mailio
} // namespace IMAP
} // namespace bookfiler
