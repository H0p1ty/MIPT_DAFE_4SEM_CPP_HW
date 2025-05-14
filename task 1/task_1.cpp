#include "Observer.h"
#include "Group.h"
#include "User.h"
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <algorithm>
#include <limits>
#include <vector>


bool CheckNum(std::string word) {
    if (std::ranges::all_of(word, ::isdigit))
        return std::stoul(word) <= std::numeric_limits<unsigned int>::max();
    return false;
}


int main() {
    std::string command, word;
    Observer observer;
    std::map<std::string, int> names{
        {"createUser", 2}, {"deleteUser", 1}, {"allUsers", 0}, {"getUser", 1},
         {"createGroup", 1}, {"deleteGroup", 1}, {"allGroups", 0}, {"getGroup", 1},
         {"attachUserToGroup", 2}, {"detachUserFromGroup", 1}
    };
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);
        auto input = std::istringstream(command);

        std::vector<std::string> tokens;

        for (; getline(input, word, ' ');) {
            tokens.push_back(word);
        }
        if (tokens.empty() || !names.contains(tokens[0]) || ((tokens.size() - 1) < names[tokens[0]])) {
            std::cerr << "Invalid input" << std::endl;
            continue;
        }

        if (names[tokens[0]] > 0 && !CheckNum(tokens[1])) {
            std::cerr << "Bad id (too big or <0 or even not an int)" << std::endl;
            continue;
        }
        if (tokens[0] == "createUser") {
            observer.CreateUser(std::stoul(tokens[1]), tokens[2]);
        } else if (tokens[0] == "deleteUser") {
            observer.DeleteUser(std::stoul(tokens[1]));
        } else if (tokens[0] == "allUsers") {
            observer.AllUsers();
        } else if (tokens[0] == "getUser") {
            observer.GetUser(std::stoul(tokens[1]));
        } else if (tokens[0] == "createGroup") {
            observer.CreateGroup(std::stoul(tokens[1]));
        } else if (tokens[0] == "deleteGroup") {
            observer.DeleteGroup(std::stoul(tokens[1]));
        } else if (tokens[0] == "allGroups") {
            observer.AllGroups();
        } else if (tokens[0] == "getGroup") {
            observer.GetGroup(std::stoul(tokens[1]));
        } else if (tokens[0] == "attachUserToGroup") {
            if (!CheckNum(tokens[2])) {
                std::cerr << "Bad id (too big or <0 or even not an int)" << std::endl;
                continue;
            }
            observer.AttachUserToGroup(std::stoul(tokens[1]), std::stoul(tokens[2]));
        } else if (tokens[0] == "detachUserFromGroup") {
            observer.DetachUserFromGroup(std::stoul(tokens[1]));
        } else {
            std::cerr << "unknown command" << std::endl;
        }
    }
}
/*
createUser 1 xj9
createUser 2 abc
createUser 3 gtfo
createGroup 1
attachUserToGroup 1 1
attachUserToGroup 2 1
allUsers
deleteGroup 1
allUsers
 */
