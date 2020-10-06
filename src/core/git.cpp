/*
 * @name BookFiler Module - MySQL
 * @author Branden Lee
 * @version 1.00
 * @license MIT
 * @brief mySQL, SQLite3, and HTTP implementation
 */

// Local Project
#include "git.hpp"

/*
 * bookfiler - MySQL
 */
namespace bookfiler {
namespace GIT {

int Repository::openRemote(std::shared_ptr<rapidjson::Document> settingsDoc) {
  int rc, port;
  std::string host, username, password, url, repositoryName;

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
  char memberName4[] = "repository";
  if (settingsDoc->HasMember(memberName4) &&
      (*settingsDoc)[memberName4].IsString()) {
    repositoryName = (*settingsDoc)[memberName4].GetString();
  }
  char memberName5[] = "port";
  if (settingsDoc->HasMember(memberName5) &&
      (*settingsDoc)[memberName5].IsInt()) {
    port = (*settingsDoc)[memberName5].GetInt();
  }

  url = "";
  url.append(host);

  git_remote **gitRemote = nullptr;
  git_repository *repo = nullptr;

  rc = git_repository_init(&repo, "/tmp/", false);
  if (rc < 0) {
    const git_error *e = git_error_last();
    std::cout << "git_repository_init Error " << rc << "/" << e->klass << ": " << e->message
              << std::endl;
    return rc;
  }

  rc = git_remote_create(gitRemote, repo, repositoryName.c_str(), url.c_str());
  if (rc < 0) {
    const git_error *e = git_error_last();
    std::cout << "git_remote_create Error " << rc << "/" << e->klass << ": " << e->message
              << std::endl;
    return rc;
  }

  return 0;
}

int Repository::openLocal(std::shared_ptr<rapidjson::Document> settingsDoc) {
  std::string repoPath;
  int rc;

  /* Get settings from JSON document */
  if (!settingsDoc->IsObject()) {
    std::cout << "settings document invalid" << std::endl;
    return -1;
  }
  if (settingsDoc->HasMember("path") && (*settingsDoc)["path"].IsString()) {
    repoPath = (*settingsDoc)["path"].GetString();
  }

  repo = NULL;
  std::cout << "git_repository_init" << std::endl;
  rc = git_repository_init(&repo, repoPath.c_str(), false);
  std::cout << "git_repository_init done" << std::endl;
  if (rc < 0) {
    const git_error *e = git_error_last();
    std::cout << "Error " << rc << "/" << e->klass << ": " << e->message
              << std::endl;
    return rc;
  }

  return 0;
}

int Repository::walk() {
  int error;
  int depth = -1;

  git_revwalk *walk;
  std::cout << "git_revwalk_new" << std::endl;
  error = git_revwalk_new(&walk, repo);
  if (error < 0) {
    const git_error *e = git_error_last();
    printf("Error %d/%d: %s\n", error, e->klass, e->message);
    return error;
  }

  git_revwalk_sorting(walk, GIT_SORT_TOPOLOGICAL | GIT_SORT_TIME);
  git_revwalk_push_head(walk);
  git_revwalk_hide_glob(walk, "tags/*");

  /* limits depth */
  if (depth > 0) {
    git_object *obj;
    error = git_revparse_single(&obj, repo, "HEAD~4"); // depth of 4
    if (error < 0) {
      const git_error *e = git_error_last();
      printf("Error %d/%d: %s\n", error, e->klass, e->message);
      return error;
    }
    git_revwalk_hide(walk, git_object_id(obj));
    git_object_free(obj);
  }

  git_oid oid;
  time_t time;
  while (git_revwalk_next(&oid, walk) == 0) {
    git_commit *commit;
    char oidstr[10] = {0};

    git_commit_lookup(&commit, repo, &oid);
    time = git_commit_time(commit);
    git_oid_tostr(oidstr, 9, &oid);
    std::cout << "Commit id: " << oidstr
              << ", Message: " << git_commit_message(commit)
              << ", time: " << ctime(&time) << std::endl;
    git_commit_free(commit);
  }
  return 0;
}

} // namespace GIT
} // namespace bookfiler
