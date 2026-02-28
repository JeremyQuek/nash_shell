#include <unistd.h>
#include <string>
#include <vector>
#include <filesystem>
#include <iostream>

#include <sys/wait.h>
#include "subprocess.h"

using namespace std;
namespace fs = filesystem;
extern "C" char **environ;
extern vector<fs::path> sys_paths;                  //paths as std::fs::path cause we need to validate dynamic path
extern unordered_map<string, string> hash_table;    //paths string since we validate it on initialisation

bool resolve_path(string&cmd, string& executable_path)  {
    auto table_iterator = hash_table.find(cmd);
    if (table_iterator != hash_table.end()) {
        executable_path = table_iterator->second;
        return true;
    } 

    for (const auto& path: sys_paths) { 
        try {
            fs::path target = path / cmd;;
            if ((fs::exists(target)) && !(fs::is_directory(target))) {
                executable_path = fs::absolute(target);
                return true;
            }
        }
        catch(exception) {
            continue;
        }
    }
    return false;
}


void start_process(string& executable_path, char** argv) {
    cout<<executable_path<<endl;
    pid_t pid = fork();

    if (pid == 0) {
        // CHILD PROCESS
        // Use our local copy's pointers. Since it's a child, 
        // it has its own copy of the 'args' vector memory.
        if (execve(executable_path.c_str(), argv, environ) == -1) {
            perror("[SYSTEM_ERR] (execve)");
            // If execve fails, we MUST exit the child. 
            // Otherwise, it returns to the while loop in main()!
            _exit(EXIT_FAILURE); 
        }
    } 
    else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
    } 
    else {
        perror("[SYSTEM_ERR] fork failed");
    }
}