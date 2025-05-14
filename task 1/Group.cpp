#include "Group.h"

#include "User.h"
#include <iostream>

Group &Group::operator=(const Group &other) {
    if (&other == this)
        return *this;

    id = other.id;
    return *this;
}

bool Group::ContainsUserById(unsigned int id) const {
    return users_by_id_.contains(id);
}

// copies shared ptr
void Group::AddUser(std::shared_ptr<User> &user_ptr) {
    if (ContainsUserById(user_ptr->GetId())) {
        throw std::logic_error("User already exists");
    }
    users_by_id_[user_ptr->GetId()] = user_ptr;
    //std::cout << user_ptr.use_count() << std::endl;
}

void Group::RemoveUser(unsigned int id) {
    if (!ContainsUserById(id)) {
        throw std::logic_error("User does not exist");
    }

    users_by_id_.erase(id);
}

std::shared_ptr<User> &Group::GetUserById(unsigned int id) {
    if (!ContainsUserById(id)) {
        throw std::logic_error("User does not exist");
    }
    return users_by_id_[id];
}

void Group::AllUsers(const bool tabulate) const {
    const char tabulation = tabulate ? '\t' : ' ';
    for (auto iter = users_by_id_.begin(); iter != users_by_id_.end(); ++iter) {
        std::cout << tabulation << *iter->second << std::endl;
    }
}

void Group::MoveUsersToOtherGroup(Group &other) {
    if (&other == this) {
        throw std::logic_error("cant move group to itself");
    }
    for (auto iter = users_by_id_.begin(); iter != users_by_id_.end(); ++iter) {
        other.AddUser(iter->second);
    }
    // for (auto iter = users_by_id_.begin(); iter != users_by_id_.end(); ++iter) {
    //     RemoveUser(iter->first);
    // }
}
