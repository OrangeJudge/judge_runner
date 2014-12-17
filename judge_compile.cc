/*
 * Judge Runner
 *
 * Author: Zhou Xinzi
 * 
 * The program is heavily influenced by HUSTOJ.
 *
 */

#include <iostream>
#include <dirent.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <sys/stat.h>

#include "common.cc"

// Compile source
std::string solution_path;

int compile() {
    _log("Compiling " + solution_path);
    _copy_file(solution_path, working_dir + "/main.c");
    pid_t pid = fork();
    if (pid == 0) {  // child process
        chdir(working_dir.c_str());
        freopen("ce.txt", "w", stderr);
        execvp(LANG_C[0], (char * const *) LANG_C);
        _log("Done compile.");
        exit(0);
    } else {  // parent process
        int status;
        waitpid(pid, &status, 0);
        if (_get_filesize(working_dir + "/ce.txt") == 0) {
            _log("Compiled without error.");
        } else {
            _log("Compile error");
            return 1;
        }
    }
    return 0;
}

int main(int argc, char* argv[]) {
    _log("Judge compiler starts.");

    working_dir = JUDGE_HOME + "/solution_" + std::to_string(solution_id);
    if (_dir_exist(working_dir)) {
        system(("rm -R " + working_dir).c_str());
    }
    system(("mkdir " + working_dir).c_str());
    
    // initialize
    if (argc < 2) {
        _log("Invalid arguments");
        exit(1);
    }
    solution_path = argv[1];

    int compile_status = compile();

    _log("Compile status: " + std::to_string(compile_status));

    return 0;
}