#include <iostream>
#include <string.h>
#include <cctype>
#include <filesystem>
#include <unistd.h>
#include <stdexcept>

#include "builtin.h"
#include "tokenizer.h"

using namespace std;
namespace fs = filesystem;
extern fs::path WORK_DIR;

void pwd_command() {
    cout << WORK_DIR.string() << endl;
}

void echo_command(const string& input) {
    cout<<input.substr(5,input.size()-5)<<endl;
}

void cd_command(const vector<string>& input) {
    if (input.size()==1) {
        cout<<endl;
        return;
    }

    try {
        const string& cd_path = input[1];

        fs::path target = (WORK_DIR / cd_path);
        if (fs::exists(target)) {
            if (!fs::is_directory(target)) {
                cout<<"[SYSTEM_ERR] NOT A DIRECTORY: "<<cd_path<<endl;
            }
            else{
                WORK_DIR = fs::canonical(target);
                chdir(target.c_str());
                cout<<endl;
            }
        } else {
            cout<<"[SYSTEM_ERR] NO SUCH FILE OR DIRECTORY: "<<cd_path<<endl;
        }
    } 
    catch(exception) {
        cout<<endl;
    }
}


