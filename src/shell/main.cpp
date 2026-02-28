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
#include "system/sys_setup.h"
#include "system/subprocess.h"

// Set namespace
using namespace std;
namespace fs = filesystem;

// Initialise globals
extern "C" char **environ;
fs::path WORK_DIR = fs::current_path();
unordered_map<string, Cmd> cmd_table;         //builtin commands
vector<fs::path> sys_paths;                   //paths as std::fs::path cause we need to validate dynamic path
unordered_map<string, string> hash_table;     //paths string since we validate it on initialisation

int main() {setvbuf(stdout, NULL, _IONBF, 0);   
    init_table(cmd_table);
    init_sys_cache();

    string line;
    while (true) {
        if (!getline(cin, line)) break;  

        Tokenizer tok(line);
        tok.ctokenize();
        vector<string>& tokens = tok.input_tokens;
        string& cmd = tokens[0];

        if (cmd=="git") {
            pid_t pid = fork();
            if (pid==0) {
                execve("/opt/homebrew/bin/git", tok.argv, environ);
            } else {
                int status;
                waitpid(pid, &status, 0);
            }
            continue;
            }
        auto it = cmd_table.find(cmd);
        if (it != cmd_table.end()) {
            it->second.handler(tok);
        } else {
            string executable_path;
            if (resolve_path(cmd, executable_path)) {
                start_process(executable_path, tok.argv);
            }
            else {
                cout << "[SYSTEM_ERR] UNKNOWN SIGNAl:" << cmd << endl;
            }
        }
    }
    return 0;
}









// if (cmd=="python") {
//     pid_t pid = fork();
//     if (pid==0) {
//         execve("/usr/local/bin/python", tok.argv, environ);
//     } else {
//         int status;
//         waitpid(pid, &status, 0);
//     }
//     continue;
// }

// if (cmd=="ssh") {
//     pid_t pid = fork();
//     if (pid==0) {
//         execve("/usr/bin/ssh", tok.argv, environ);
//     } else {
//         int status;
//         waitpid(pid, &status, 0);
//     }
//     continue;
// }
// if (cmd=="top") {
//     pid_t pid = fork();
//     if (pid==0) {
//         execve("/usr/bin/top", tok.argv, environ);
//     } else {
//         int status;
//         waitpid(pid, &status, 0);
//     }
//     continue;
// }

// if (cmd=="ls") {
//     pid_t pid = fork();
//     if (pid==0) {
//         execve("/bin/ls", tok.argv, environ);
//     } else {
//         int status;
//         waitpid(pid, &status, 0);
//     }
//     continue;
// }

// if (cmd=="git") {
//     pid_t pid = fork();
//     if (pid==0) {
//         execve("/usr/bin/git", tok.argv, environ);
//     } else {
//         int status;
//         waitpid(pid, &status, 0);
//     }
//     continue;
// }

// if (cmd=="grep") {
//     pid_t pid = fork();
//     if (pid==0) {
//         execve("/usr/bin/grep", tok.argv, environ);
//     } else {
//         int status;
//         waitpid(pid, &status, 0);
//     }
//     continue;
// }

// if (cmd=="cat") {
//     pid_t pid = fork();
//     if (pid==0) {
//         execve("/bin/cat", tok.argv, environ);
//     } else {
//         int status;
//         waitpid(pid, &status, 0);
//     }
//     continue;
// }
// if (cmd=="mv") {
//     pid_t pid = fork();
//     if (pid==0) {
//         execve("/bin/mv", tok.argv, environ);
//     } else {
//         int status;
//         waitpid(pid, &status, 0);
//     }
//     cout<<endl;
//     continue;
// }

// if (cmd=="rm") {
//     pid_t pid = fork();
//     if (pid==0) {
//         execve("/bin/rm", tok.argv, environ);
//     } else {
//         int status;
//         waitpid(pid, &status, 0);
//     }
//     cout<<endl;
//     continue;
// }
// if (cmd=="mkdir") {
//     pid_t pid = fork();
//     if (pid==0) {
//         execve("/bin/mkdir", tok.argv, environ);
//     } else {
//         int status;
//         waitpid(pid, &status, 0);
//     }
//     cout<<endl;
//     continue;
// }