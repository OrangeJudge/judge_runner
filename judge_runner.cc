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
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "common.cc"

// Problem data
int time_limit = 1000;  // ms
int memory_limit = 128;  // MB
std::string input_path;

int execute() {
    // copy runtime
    // limit system call
    _copy_file(input_path, working_dir + "/data.in");
    int time_limit_s = time_limit / 1000 + 1;
    pid_t pid = fork();
    if (pid == 0) {  // child process
        _log("Child process starts.");
        chdir(working_dir.c_str());
        freopen("data.in", "r", stdin);
        freopen("user.out", "w", stdout);
        freopen("error.out", "a+", stderr);
        // trace the child process
        ptrace(PTRACE_TRACEME, 0, NULL, 0);
        chroot(working_dir.c_str());  // cannot do so for java
        // set limits
        struct rlimit limit;  // an rlimit struct variable for multiple use.
        // Limit CPU time
        limit.rlim_cur = limit.rlim_max = time_limit_s;
        setrlimit(RLIMIT_CPU, &limit);
        // Limit file size
        limit.rlim_cur = MAX_FILE_SIZE * MEGABYTE;
        limit.rlim_max = (MAX_FILE_SIZE + 1) * MEGABYTE;
        setrlimit(RLIMIT_FSIZE, &limit);
        // Limit number of process
        if (lang == LANG_JAVA) {
            limit.rlim_cur = limit.rlim_max = 50;
        } else { // C, C++, Python
            limit.rlim_cur = limit.rlim_max = 1;
        }
        setrlimit(RLIMIT_NPROC, &limit);
        // Limit the stack size
        limit.rlim_cur = limit.rlim_max = MAX_STACK_SIZE * MEGABYTE;
        setrlimit(RLIMIT_STACK, &limit);
        // Limit the memory
        if (lang == LANG_C || lang == LANG_CPP) {
            limit.rlim_cur = memory_limit * MEGABYTE / 2 * 3;
            limit.rlim_max = memory_limit * MEGABYTE * 2;
            setrlimit(RLIMIT_AS, &limit);
        }
        // set alarm in case the CPU time limit does not work
        alarm(0); // clear alarm
        alarm(time_limit_s + 10); // set alarm in second
        // execute the program
        switch(lang) {
        case LANG_C:
        case LANG_CPP:
            execl("./main", "./main", NULL);
            break;
        case LANG_JAVA:
            {
                std::string java_xms = "-Xms32m";
                std::string java_xmx = "-Xmx" + std::to_string(memory_limit * 2) + "m";
                execl("/usr/bin/java", "/usr/bin/java", java_xms.c_str(), java_xmx.c_str(),
                    "-Djava.security.manager",
                    "-Djava.security.policy=./java.policy", "Main", (char *) NULL);
            }
            break;
        case LANG_PYTHON:
            execl("/python", "/python", "main.py", (char *) NULL);
            break;
        }
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
                // some kind of runtime error.
                _log("Runtime error.");
                _log(strsignal(exitcode));
                switch (exitcode) {
                case SIGCHLD:
                case SIGALRM:
                    alarm(0);
                }
                ptrace(PTRACE_KILL, pid, NULL, 0);
                break;
            }
            if (WIFSIGNALED(status)) {
                int sig = WTERMSIG(status);
                _log("signal " + std::to_string(sig));
                _log(strsignal(sig));
                break;
            }
            // check system call
            struct user_regs_struct reg;
            ptrace(PTRACE_GETREGS, pid, NULL, &reg);
            // to make it continue
            ptrace(PTRACE_SYSCALL, pid, NULL, 0);
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