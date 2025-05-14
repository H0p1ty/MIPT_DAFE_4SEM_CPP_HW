#include "Observer.h"
#include "User.h"
#include "Group.h"

#include <iostream>


bool Observer::ContainsUser(unsigned int user_id) const {
    for (auto iter = group_map_by_id_.begin(); iter != group_map_by_id_.end(); ++iter) {
        if (iter->second.ContainsUserById(user_id))
            return true;
    }
    return false;
}

bool Observer::ContainsGroup(unsigned int group_id) const {
    for (auto iter = group_map_by_id_.begin(); iter != group_map_by_id_.end(); ++iter) {
        if (iter->first == group_id)
            return true;
    }
    return false;
}


void Observer::CreateUser(unsigned int id, std::string name) {
    if (id == 0) {
        std::cerr << "User id should be more than 0!" << std::endl;
        return;
    }
    if (ContainsUser(id)) {
        std::cerr << "User already exists" << std::endl;
        return;
    }

    std::shared_ptr<User> tmp = std::make_shared<User>(name, id);
    group_map_by_id_[0].AddUser(tmp);
}

void Observer::DeleteUser(unsigned int id) {
    if (!ContainsUser(id)) {
        std::cerr << "User does not exist" << std::endl;
        return;
    }
    for (auto group_iter = group_map_by_id_.begin(); group_iter != group_map_by_id_.end(); ++group_iter) {
        if (group_iter->second.ContainsUserById(id)) {
            std::cout << "del user, shard ptrs: " << group_iter->second.GetUserById(id).use_count() << std::endl;
            group_iter->second.RemoveUser(id);
            return;
        }
    }
}

void Observer::AllUsers() const {
    for (auto group_iter = group_map_by_id_.begin(); group_iter != group_map_by_id_.end(); ++group_iter) {
        //std::cout << group_iter->second << std::endl;
        // if (group_iter->first == 0) {
        //     std::cout << "[USERS WITH NO GROUP]" << std::endl;
        // }
        group_iter->second.AllUsers();
    }
}

void Observer::GetUser(unsigned int id) {
    if (!ContainsUser(id)) {
        std::cerr << "User does not exist" << std::endl;
        return;
    }
    for (auto group_iter = group_map_by_id_.begin(); group_iter != group_map_by_id_.end(); ++group_iter) {
        if (group_iter->second.ContainsUserById(id)) {
            std::cout << *group_iter->second.GetUserById(id) << std::endl;
        }
    }
}

void Observer::CreateGroup(unsigned int id) {
    if (id == 0) {
        std::cerr << "Group num should be more than 0!" << std::endl;
        return;
    }
    if (ContainsGroup(id)) {
        std::cerr << "Group already exists" << std::endl;
        return;
    }
    group_map_by_id_[id] = Group(id);
}

void Observer::DeleteGroup(unsigned int id) {
    if (id == 0) {
        std::cerr << "Group num should be more than 0!" << std::endl;
        return;
    }
    if (!ContainsGroup(id)) {
        std::cerr << "Group does not exist" << std::endl;
        return;
    }

    group_map_by_id_[id].MoveUsersToOtherGroup(group_map_by_id_[0]);
    group_map_by_id_.erase(id);
}

void Observer::AllGroups() const {
    for (auto group_iter = group_map_by_id_.begin(); group_iter != group_map_by_id_.end(); ++group_iter) {
        // if (group_iter->first == 0) {
        //     std::cout << "[group 0 is for users with no group]" << std::endl;
        // }
        std::cout << group_iter->second << std::endl;
    }
}

void Observer::GetGroup(unsigned int id) {
    if (!ContainsGroup(id)) {
        std::cerr << "Group does not exist" << std::endl;
        return;
    }
    std::cout << group_map_by_id_[id] << std::endl;
}

void Observer::AttachUserToGroup(unsigned int user_id, unsigned int group_id) {
    if (!ContainsUser(user_id)) {
        std::cerr << "User does not exist" << std::endl;
        return;
    }
    if (!ContainsGroup(group_id)) {
        std::cerr << "Group does not exist" << std::endl;
        return;
    }

    // find user group
    unsigned int user_group_id = 0;
    for (auto group_iter = group_map_by_id_.begin(); group_iter != group_map_by_id_.end(); ++group_iter) {
        if (group_iter->second.ContainsUserById(user_id)) {
            user_group_id = group_iter->first;
            break;
        }
    }

    if (group_id == user_group_id) {
        if (group_id == 0) {
            std::cerr << "User is not a member of any group" << std::endl;
        } else {
            std::cerr << "User is already in this group" << std::endl;
        }
        return;
    }
    group_map_by_id_[group_id].AddUser(group_map_by_id_[user_group_id].GetUserById(user_id));
    group_map_by_id_[user_group_id].RemoveUser(user_id);
    //group_map_by_id_[user_group_id].AllUsers();
}

void Observer::DetachUserFromGroup(unsigned int user_id) {
    AttachUserToGroup(user_id, 0);
}
