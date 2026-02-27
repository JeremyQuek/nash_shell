#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unordered_map>
#include <cctype>
#include <unistd.h>
#include <filesystem>

#include "builtin.h"
#include "tokenizer.h"
#include "command_table.h"

using namespace std;
namespace fs = filesystem;

extern "C" char **environ;
fs::path WORK_DIR = fs::current_path();
unordered_map<string, Cmd> cmd_table;

int main() {setvbuf(stdout, NULL, _IONBF, 0);   
    init_table(cmd_table);

    string line;
    while (true) {
        if (!getline(cin, line)) break;  
        Tokenizer tok(line);
        tok.ctokenize();
        vector<string>& tokens = tok.input_tokens;
        string& cmd = tokens[0];

        if (cmd=="python") {
            pid_t pid = fork();
            if (pid==0) {
                execve("/usr/local/bin/python", tok.argv, environ);
            } else {
                int status;
                waitpid(pid, &status, 0);
            }
            continue;
        }

        if (cmd=="ssh") {
            pid_t pid = fork();
            if (pid==0) {
                execve("/usr/bin/ssh", tok.argv, environ);
            } else {
                int status;
                waitpid(pid, &status, 0);
            }
            continue;
        }
        if (cmd=="top") {
            pid_t pid = fork();
            if (pid==0) {
                execve("/usr/bin/top", tok.argv, environ);
            } else {
                int status;
                waitpid(pid, &status, 0);
            }
            continue;
        }

        if (cmd=="ls") {
            pid_t pid = fork();
            if (pid==0) {
                execve("/bin/ls", tok.argv, environ);
            } else {
                int status;
                waitpid(pid, &status, 0);
            }
            continue;
        }
        
        if (cmd=="git") {
            pid_t pid = fork();
            if (pid==0) {
                execve("/usr/bin/git", tok.argv, environ);
            } else {
                int status;
                waitpid(pid, &status, 0);
            }
            continue;
        }

        if (cmd=="grep") {
            pid_t pid = fork();
            if (pid==0) {
                execve("/usr/bin/grep", tok.argv, environ);
            } else {
                int status;
                waitpid(pid, &status, 0);
            }
            continue;
        }

        if (cmd=="cat") {
            pid_t pid = fork();
            if (pid==0) {
                execve("/bin/cat", tok.argv, environ);
            } else {
                int status;
                waitpid(pid, &status, 0);
            }
            continue;
        }
        if (cmd=="mv") {
            pid_t pid = fork();
            if (pid==0) {
                execve("/bin/mv", tok.argv, environ);
            } else {
                int status;
                waitpid(pid, &status, 0);
            }
            cout<<endl;
            continue;
        }

        if (cmd=="rm") {
            pid_t pid = fork();
            if (pid==0) {
                execve("/bin/rm", tok.argv, environ);
            } else {
                int status;
                waitpid(pid, &status, 0);
            }
            cout<<endl;
            continue;
        }
        if (cmd=="mkdir") {
            pid_t pid = fork();
            if (pid==0) {
                execve("/bin/mkdir", tok.argv, environ);
            } else {
                int status;
                waitpid(pid, &status, 0);
            }
            cout<<endl;
            continue;
        }

        auto it = cmd_table.find(cmd);
        if (it != cmd_table.end()) {
            it->second.handler(tok);
        } else {
            cout << "[SYSTEM_ERR] UNKNOWN SIGNAl:" << cmd << endl;
        }
        cout<<"\0";
    }

    return 0;
}
