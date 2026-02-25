#ifndef COMMAND_HELPER_H
#define COMMAND_HELPER_H

#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <unordered_map>

#include "tokenizer.h"

struct Cmd {
    std::string name;
    std::function<void(Tokenizer&)> handler; 
    std::string doc;
};

void register_cmd(std::string name, std::function<void()> func, std::string doc);
void register_cmd(std::string name, std::function<void(const std::string&)> func, std::string doc);
void register_cmd(std::string name, std::function<void(const std::vector<std::string>&)> func, std::string doc);

void init_table(void);

#endif