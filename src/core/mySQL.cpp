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

int ssh_tunnel(int localPort, std::string remoteHost, int remotePort) {
  /* ssh tunnel */
  ssh_session ssh_session;
  int rc;
  int verbosity = SSH_LOG_PROTOCOL;
  int port = 21098;

  ssh_session = ssh_new();
  if (ssh_session == NULL) {
    std::cout << "ssh_new ERROR" << std::endl;
    return -1;
  }

  ssh_options_set(ssh_session, SSH_OPTIONS_HOST, "SERVER");
  ssh_options_set(ssh_session, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
  ssh_options_set(ssh_session, SSH_OPTIONS_PORT, &port);
  ssh_options_set(ssh_session, SSH_OPTIONS_USER, "USERNAME");

  rc = ssh_connect(ssh_session);
  if (rc != SSH_OK) {
    std::cout << "ssh_connect ERROR: " << ssh_get_error(ssh_session)
              << std::endl;
    return rc;
  }

  enum ssh_known_hosts_e state;
  unsigned char *hash = NULL;
  size_t hlen;
  ssh_key srv_pubkey = NULL;

  rc = ssh_get_server_publickey(ssh_session, &srv_pubkey);
  if (rc < 0) {
    std::cout << "ssh_get_server_publickey ERROR" << std::endl;
    return rc;
  }

  rc =
      ssh_get_publickey_hash(srv_pubkey, SSH_PUBLICKEY_HASH_SHA1, &hash, &hlen);
  ssh_key_free(srv_pubkey);
  if (rc < 0) {
    std::cout << "ssh_get_publickey_hash ERROR" << std::endl;
    return rc;
  }

  state = ssh_session_is_known_server(ssh_session);
  std::cout << "ssh_session_is_known_server STATE: " << state << std::endl;

  rc = ssh_userauth_password(ssh_session, "USERNAME", "PASSWORD");
  if (rc != SSH_AUTH_SUCCESS) {
    std::cout << "ssh_userauth_password ERROR: " << ssh_get_error(ssh_session)
              << std::endl;
    ssh_disconnect(ssh_session);
    ssh_free(ssh_session);
    return rc;
  }

  ssh_channel forwarding_channel;
  char *http_get = "GET / HTTP/1.1\nHost: www.google.com\n\n";
  int nbytes, nwritten;

  forwarding_channel = ssh_channel_new(ssh_session);
  if (forwarding_channel == NULL) {
    std::cout << "ssh_channel_new ERROR" << std::endl;
    return rc;
  }

  rc = ssh_channel_open_forward(forwarding_channel, "localhost", 3306,
                                "localhost", 3306);
  if (rc != SSH_OK) {
    std::cout << "ssh_channel_open_forward ERROR" << std::endl;
    ssh_channel_free(forwarding_channel);
    return rc;
  }

  nbytes = strlen(http_get);
  nwritten = ssh_channel_write(forwarding_channel, http_get, nbytes);
  if (nbytes != nwritten) {
    ssh_channel_free(forwarding_channel);
    return SSH_ERROR;
  }

  ssh_channel_free(forwarding_channel);
  ssh_free(ssh_session);
  return 0;
}

int create_db(std::string repoPath, rapidjson::Document settings) {
  int port;
  std::string localhost, username, password;
  /* mysql */
  MYSQL *con = mysql_init(NULL);

  if (con == NULL) {
    fprintf(stderr, "%s\n", mysql_error(con));
    exit(1);
  }

  if (mysql_real_connect(con, localhost.c_str(), username.c_str(),
                         password.c_str(), "midtelci_test", port, NULL,
                         0) == NULL) {
    std::cout << mysql_error(con) << std::endl;
    mysql_close(con);
    return -1;
  }

  std::string query = "CREATE TABLE `test_table2` (`id` INT NOT NULL "
                      "AUTO_INCREMENT,`text` BLOB "
                      "NULL DEFAULT NULL,INDEX `Index 1` "
                      "(`id`)) COLLATE='utf8_general_ci' ENGINE=InnoDB;";
  if (mysql_query(con, query.c_str())) {
    std::cout << mysql_error(con) << std::endl;
    mysql_close(con);
    return -1;
  }

  mysql_close(con);
  return 0;
}

} // namespace MySQL
} // namespace bookfiler
