#ifndef __USER_H__
#define __USER_H__

namespace security {

class Permissions;

class User {
    friend class UserStorage;
public:
    explicit User(const std::string &name);

    std::string_view username() const;
    std::shared_ptr<Permissions> permissions() const;
private:
    std::string name;
    std::shared_ptr<Permissions> perms;
};

} // namespace security

#endif // __USER_H__
