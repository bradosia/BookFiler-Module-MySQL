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
namespace MySQL {

int getEmails(rapidjson::Document settings) {
  CURL *curl;
  CURLcode res = CURLE_OK;
  std::string bufferString;

  curl = curl_easy_init();
  if (curl) {
    /* Set username and password */
    curl_easy_setopt(curl, CURLOPT_USERNAME, "USERNAME");
    curl_easy_setopt(curl, CURLOPT_PASSWORD, "PASSWORD");

    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    /* setup callbacks */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &bufferString);

    /* This will fetch message 1 from the user's inbox */
    curl_easy_setopt(curl, CURLOPT_URL, "imaps://imap.gmail.com:993/INBOX/;UID=1");

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
