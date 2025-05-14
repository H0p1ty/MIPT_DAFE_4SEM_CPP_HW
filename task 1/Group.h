#ifndef GROUP_H
#define GROUP_H

#include <iostream>
#include <unordered_map>
#include <memory>

class User;

class Group {
public:
    friend std::ostream& operator << (std::ostream& os, const Group& group) {
        if (group.id != 0) {
            os << "Group ID: " << group.id << "\n";
        } else {
            os << "Users with no group:\n";
        }
        group.AllUsers(true);
        return os;
    }
    Group() = default;
    ~Group() = default;
    Group(const unsigned int id_) : id(id_) {}
    Group& operator=(const Group& other);

    bool ContainsUserById(unsigned int id) const;
    void AddUser(std::shared_ptr<User>& user_ptr);
    void RemoveUser(unsigned int id);
    std::shared_ptr<User>& GetUserById(unsigned int id);
    unsigned int GetId() const {return id;}
    void AllUsers(bool tabulate = false) const;

    void MoveUsersToOtherGroup(Group& other);
private:
    unsigned int id{};
    std::unordered_map<unsigned int, std::shared_ptr<User>> users_by_id_;
};



#endif //GROUP_H
