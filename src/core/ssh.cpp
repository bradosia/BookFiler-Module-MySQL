/*
 * @name BookFiler Module - MySQL
 * @author Branden Lee
 * @version 1.00
 * @license MIT
 * @brief mySQL, SQLite3, and HTTP implementation
 */

// Local Project
#include "ssh.hpp"

/*
 * bookfiler - MySQL
 */
namespace bookfiler {
namespace SSH {

int tunnel(rapidjson::Document settingsDoc) {
  /* ssh tunnel */
  ssh_session sshSession;
  int localPort, remotePort;
  std::string remoteHost, username, password;
  int rc;
  int verbosity = SSH_LOG_PROTOCOL;

  /* Get settings from JSON document */
  if (settingsDoc.IsObject()) {
    char memberName1[] = "remoteHost";
    if (settingsDoc.HasMember(memberName1) &&
        settingsDoc[memberName1].IsString()) {
      remoteHost = settingsDoc[memberName1].GetString();
    }
    char memberName2[] = "username";
    if (settingsDoc.HasMember(memberName2) &&
        settingsDoc[memberName2].IsString()) {
      username = settingsDoc[memberName2].GetString();
    }
    char memberName3[] = "password";
    if (settingsDoc.HasMember(memberName3) &&
        settingsDoc[memberName3].IsString()) {
      password = settingsDoc[memberName3].GetString();
    }
    char memberName4[] = "localPort";
    if (settingsDoc.HasMember(memberName4) &&
        settingsDoc[memberName4].IsInt()) {
      localPort = settingsDoc[memberName4].GetInt();
    }
    char memberName5[] = "remotePort";
    if (settingsDoc.HasMember(memberName5) &&
        settingsDoc[memberName5].IsInt()) {
      remotePort = settingsDoc[memberName5].GetInt();
    }
  }

  sshSession = ssh_new();
  if (sshSession == NULL) {
    std::cout << "ssh_new ERROR" << std::endl;
    return -1;
  }

  ssh_options_set(sshSession, SSH_OPTIONS_HOST, "SERVER");
  ssh_options_set(sshSession, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
  ssh_options_set(sshSession, SSH_OPTIONS_PORT, &remotePort);
  ssh_options_set(sshSession, SSH_OPTIONS_USER, "USERNAME");

  rc = ssh_connect(sshSession);
  if (rc != SSH_OK) {
    std::cout << "ssh_connect ERROR: " << ssh_get_error(sshSession)
              << std::endl;
    return rc;
  }

  enum ssh_known_hosts_e state;
  unsigned char *hash = NULL;
  size_t hlen;
  ssh_key srv_pubkey = NULL;

  rc = ssh_get_server_publickey(sshSession, &srv_pubkey);
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

  state = ssh_session_is_known_server(sshSession);
  std::cout << "ssh_session_is_known_server STATE: " << state << std::endl;

  rc = ssh_userauth_password(sshSession, "USERNAME", "PASSWORD");
  if (rc != SSH_AUTH_SUCCESS) {
    std::cout << "ssh_userauth_password ERROR: " << ssh_get_error(sshSession)
              << std::endl;
    ssh_disconnect(sshSession);
    ssh_free(sshSession);
    return rc;
  }

  ssh_channel forwarding_channel;
  char *http_get = "GET / HTTP/1.1\nHost: www.google.com\n\n";
  int nbytes, nwritten;

  forwarding_channel = ssh_channel_new(sshSession);
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
  ssh_free(sshSession);
  return 0;
}

} // namespace SSH
} // namespace bookfiler
