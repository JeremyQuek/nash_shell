#include <unistd.h>
#include <string>
#include <vector>
#include <filesystem>
#include <iostream>

#include <sys/wait.h>

using namespace std;
namespace fs = filesystem;
extern "C" char **environ;
extern vector<fs::path> sys_paths;                  //paths as std::fs::path cause we need to validate dynamic path
extern unordered_map<string, string> hash_table;    //paths string since we validate it on initialisation

bool resolve_path(string&cmd, string& executable_path)  {
    for (const auto& path: sys_paths) { 
        try {
            fs::path target = path / cmd;;
            if ((fs::exists(target)) && !(fs::is_directory(target))) {
                executable_path = fs::canonical(target);
                return true;
            }
        }
        catch(exception) {
            continue;
        }
    }
    return false;
}

bool start_process(string& cmd, char** argv) {
    string executable_path;

    auto table_iterator = hash_table.find(cmd);
    if (table_iterator != hash_table.end()) {
        executable_path = table_iterator->second;
    } else {
        if (!resolve_path(cmd,executable_path)) {
            cout << "[SYSTEM_ERR] UNKNOWN COMMAND: " << cmd<<endl;
            return false;
        }
    } 

    try {
        pid_t pid = fork();
        if (pid==0) {
            execve(executable_path.c_str(), argv, environ);
        } else {
            int status;
            waitpid(pid, &status, 0);
        }
    }
    catch(const exception & e) {
        cerr << "[SYSTEM_ERR]: " << e.what() <<endl;
        return false;
    }
    return true;
}