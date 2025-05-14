#ifndef OBSERVER_H
#define OBSERVER_H

#include <string>
#include <unordered_map>
#include <memory>

#include "Group.h"


class Observer {
public:
    Observer() {
        // 0 <=> no group
        group_map_by_id_.insert({0, Group(0)});
    }

    void CreateUser(unsigned int id, std::string name);
    void DeleteUser(unsigned int id);
    void AllUsers() const;
    void GetUser(unsigned int id);
    void CreateGroup(unsigned int id);
    void DeleteGroup(unsigned int id);
    void AllGroups() const;
    void GetGroup(unsigned int id);
    void AttachUserToGroup(unsigned int user_id, unsigned int group_id);
    void DetachUserFromGroup(unsigned int user_id);
    bool ContainsUser(unsigned int user_id) const;
    bool ContainsGroup(unsigned int group_id) const;
private:
    std::unordered_map<unsigned int, Group> group_map_by_id_;
};



#endif //OBSERVER_H
