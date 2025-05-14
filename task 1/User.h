#ifndef USER_H
#define USER_H

#include <string>
#include <iostream>
#include <stdexcept>


class Group;

class User {
public:
    friend std::ostream& operator<<(std::ostream& os, const User& usr) {
        os << "User ID: " << usr.id << ", Name: " << usr.name;
        return os;
    }

    User() = default;
    ~User() = default;
    User(const std::string& name_, const unsigned int id_) : name(name_), id(id_) {}
    User& operator=(const User& other);
    User(const User& other);

    Group* GetGroupPtr() const;
    Group& GetGroupLink() const;
    unsigned int GetId() const {return id;}

    std::string name{};
private:
    unsigned int id{};
    Group* group_ptr_ = nullptr;
};


#endif //USER_H
