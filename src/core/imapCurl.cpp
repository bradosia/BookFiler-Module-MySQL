/*
 * @name BookFiler Module - MySQL
 * @author Branden Lee
 * @version 1.00
 * @license MIT
 * @brief mySQL, SQLite3, and HTTP implementation
 */

// Local Project
#include "imapCurl.hpp"

/*
 * bookfiler - MySQL
 */
namespace bookfiler {
namespace IMAP {

int getEmails(std::shared_ptr<rapidjson::Document> settingsDoc) {
  CURL *curl;
  CURLcode res = CURLE_OK;
  std::string bufferString;

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

  curl = curl_easy_init();
  if (curl) {
    /* Set username and password */
    curl_easy_setopt(curl, CURLOPT_USERNAME, username.c_str());
    curl_easy_setopt(curl, CURLOPT_PASSWORD, password.c_str());

    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    /* setup callbacks */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, bookfiler::curl::writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &bufferString);

    url = "";
    url.append("imaps://").append(host).append(":").append(std::to_string(port)).append("/INBOX/;UID=1");

    /* This will fetch message 1 from the user's inbox */
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    /* Perform the fetch */
    res = curl_easy_perform(curl);

    /* Check for errors */
    if (res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    std::cout << bufferString << std::endl;

    /* Always cleanup */
    curl_easy_cleanup(curl);
  }

  return (int)res;
}

} // namespace MySQL
} // namespace bookfiler
