#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <unordered_map>

#include "tokenizer.h"
#include "command_helper.h"
#include "builtin.h"

extern std::unordered_map<std::string, Cmd> cmd_table;

void register_cmd(std::string name, std::function<void()> func, std::string doc) {
    cmd_table[name] = {name, [func](Tokenizer&) { func(); }, doc};
}

void register_cmd(std::string name, std::function<void(const std::string&)> func, std::string doc) {
    cmd_table[name] = {name, [func](Tokenizer& tok) { func(tok.input_string); }, doc};
}

void register_cmd(std::string name, std::function<void(const std::vector<std::string>&)> func, std::string doc) {
    cmd_table[name] = {name, [func](Tokenizer& tok) { func(tok.input_tokens); }, doc};
}

void init_table(void) {
    register_cmd("pwd", pwd_command, "Prints current working directory");
    register_cmd("exit", []() { exit(0); }, "Exit the shell");

    register_cmd("echo", echo_command, "Prints input to console");

    register_cmd("cd", cd_command, "Change directory");

    register_cmd("help", [](const std::vector<std::string>&) {
        for (auto const& [name, c] : cmd_table) printf("%-10s : %s\n", name.c_str(), c.doc.c_str());
    }, "Displays this table");

    register_cmd("hi", [](const std::string&) { std::cout << "Hello Neo...\n"; }, "???");
}