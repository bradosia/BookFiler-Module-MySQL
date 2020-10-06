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
namespace MySQL {

int walk_repo(std::string repoPath) {
  int error;
  int depth = -1;

  git_libgit2_init();
  git_repository *repo = NULL;
  std::cout << "git_repository_init" << std::endl;
  error = git_repository_init(&repo, repoPath.c_str(), false);
  std::cout << "git_repository_init done" << std::endl;
  if (error < 0) {
    const git_error *e = git_error_last();
    printf("Error %d/%d: %s\n", error, e->klass, e->message);
    return error;
  }

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

  git_repository_free(repo);
  git_libgit2_shutdown();
}

} // namespace MySQL
} // namespace bookfiler
