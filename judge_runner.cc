/*
 * Judge Runner
 *
 * Author: Zhou Xinzi
 *
 */

#include <iostream>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <sys/stat.h>

// Config
bool DEBUG = true;
std::string JUDGE_HOME = "/usr/local/judge_runner";
const char * LANG_C[] = {"gcc", "main.c", "-o", "main.out", NULL};

// Global variable
std::string working_dir;

// Problem data
int time_limit = 1000;  // ms
int memory_limit = 128;  // MB

// Solution data
int solution_id = 0;
std::string solution_path;
int lang = 0;

void log(std::string s) {
    if (DEBUG) {
        std::cout << s << std::endl;
    }
}

void _copy_file(std::string source, std::string target) {
    system(("cp " + source + " " + target).c_str());
}

size_t _get_filesize(std::string filename) {
    struct stat st;
    if(stat(filename.c_str(), &st) != 0) {
        return 0;
    }
    return st.st_size;   
}

int compile() {
    log("Compiling " + solution_path);
    _copy_file(solution_path, working_dir + "/main.c");
    int pid = fork();
    if (pid == 0) {  // child process
        chdir(working_dir.c_str());
        freopen("ce.txt", "w", stderr);
        execvp(LANG_C[0], (char * const *) LANG_C);
        log("Done compile.");
        exit(0);
    } else {  // parent process
        int status;
        waitpid(pid, &status, 0);
        if (_get_filesize(working_dir + "/ce.txt") == 0) {
            log("Compiled without error.");
        } else {
            log("Compile error");
        }
    }
    return 0;
}

int main(int argc, char* argv[]) {
    log("Judge runner starts.");

    working_dir = JUDGE_HOME + "/solution_" + std::to_string(solution_id);
    system(("mkdir " + working_dir).c_str());
    
    // initialize
    if (argc < 2) {
        std::cout << "Invalid arguments" << std::endl;
        exit(1);
    }
    solution_path = argv[1];

    int compile_status = compile();
    return 0;
}