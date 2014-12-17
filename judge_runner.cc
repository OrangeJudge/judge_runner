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

int execute() {
    // copy runtime
    // limit system call
    _copy_file(input_path, working_dir + "/data.in");
    pid_t pid = fork();
    if (pid == 0) {  // child process
        chdir(working_dir.c_str());
        freopen("data.in", "r", stdin);
        freopen("user.out", "w", stdout);
        freopen("error.out", "a+", stderr);
        execl("./main", "./main", NULL);
        exit(0);
    } else {  // parent process

    }
    return 0;
}

int main(int argc, char* argv[]) {
    _log("Judge runner starts.");

    working_dir = JUDGE_HOME + "/solution_" + std::to_string(solution_id);
    if (!_dir_exist(working_dir)) {
        _log("Working directory does not exist.");
        exit(1);
    }
    
    // initialize
    if (argc < 2) {
        std::cout << "Invalid arguments" << std::endl;
        exit(1);
    }
    input_path = argv[1];

    int execute_status = execute();

    if (execute_status) {

    } else {

    }

    return 0;
}