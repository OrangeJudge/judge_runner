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
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/syscall.h>

#include "common.cc"

// Problem data
int time_limit = 1000;  // ms
int memory_limit = 128;  // MB
std::string input_path;

int execute() {
    // copy runtime
    // limit system call
    _copy_file(input_path, working_dir + "/data.in");
    pid_t pid = fork();
    if (pid == 0) {  // child process
        _log("Child process starts.");
        chdir(working_dir.c_str());
        freopen("data.in", "r", stdin);
        freopen("user.out", "w", stdout);
        freopen("error.out", "a+", stderr);
        // trace the child process
        ptrace(PT_TRACE_ME, 0, NULL, 0);
        // chroot(working_dir.c_str());
        // struct rlimit limit;  // an rlimit struct variable for multiple use.
        // limit.rlim_cur = limit.rlim_max = 1;
        // setrlimit(RLIMIT_CPU, &limit);
        alarm(0);
        alarm(1);
        execl("./main", "./main", NULL);
        exit(0);
    } else {  // parent process
        _log("Judging process: " + std::to_string(pid));
        int status;
        struct rusage ruse;
        while (1) {
            _log("Executing...");
            // check the usage
            wait4(pid, &status, 0, &ruse);
            // check memory
            if (WIFEXITED(status)) {
                _log("Break 1");
                break;
            }
            // if error.out has content exit
            // if user file too big, exit
            int exitcode = WEXITSTATUS(status);
            if (exitcode != 0 && exitcode != 5) {
                _log(strsignal(exitcode));
                switch (exitcode) {
                case SIGCHLD:
                case SIGALRM:
                    alarm(0);
                }
                ptrace(PT_KILL, pid, NULL, 0);
                break;
            }
            if (WIFSIGNALED(status)) {
                int sig = WTERMSIG(status);
                _log("signal " + std::to_string(sig));
                _log(strsignal(sig));
                break;
            }
            // struct user_regs_struct reg;
            // ptrace(PT_GETREGS, pid, NULL, &reg);
            // check system call
            // ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
            _log("Finish loop");
        }
        _log("Finish execution.");
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