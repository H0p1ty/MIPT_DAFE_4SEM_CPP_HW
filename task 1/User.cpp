#include "User.h"

User& User::operator=(const User& other) {
    if (this == &other)
        return *this;
    group_ptr_ = other.GetGroupPtr();
    name = other.name;
    id = other.id;
    return *this;
}

Group* User::GetGroupPtr() const {
    return group_ptr_;
}

Group& User::GetGroupLink() const {
    if (group_ptr_ == nullptr)
        throw std::logic_error("User is not a member of a group");
    return *group_ptr_;
}

User::User(const User &other) {
    id = other.id;
    group_ptr_ = other.group_ptr_;
    name = other.name;
}
