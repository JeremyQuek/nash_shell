#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unordered_map>
#include <cctype>
#include <unistd.h>
#include <filesystem>

#include "builtin.h"
#include "tokenizer.h"
#include "command_helper.h"

using namespace std;
namespace fs = filesystem;

fs::path WORK_DIR = fs::current_path();
unordered_map<string, Cmd> cmd_table;

int main() {setvbuf(stdout, NULL, _IONBF, 0);   
    init_table();

    string line;
    while (true) {
        if (!getline(cin, line)) break;  

        for (char &c : line) c = tolower(c);
        Tokenizer tok(line);
        tok.tokenize();
        vector<string>& tokens = tok.input_tokens;
        string& cmd = tokens[0];


        if (cmd=="ls") {
            pid_t pid = fork();
            if (pid==0) {
                char *argv[] = { "ls", NULL };
                char *envp[] = { NULL };
                execve("/bin/ls", argv, envp);
            } else {
                int status;
                waitpid(pid, &status, 0);
            }
            continue;
        }
        if (cmd=="git") {
            pid_t pid = fork();
            if (pid==0) {
                char *argv[] = { "git", NULL };
                char *envp[] = { NULL };
                execve("/usr/bin/git", argv, envp);
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
            cout << "[SYSTEM_ERR] UNKNOWN SIGNAl:" << cmd << endl;
        }
    }

    return 0;
}
