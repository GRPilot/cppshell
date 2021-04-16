#ifndef __USER_H__
#define __USER_H__

namespace security {

class Permissions;

class User {
    friend class UserStorage;
public:
    explicit User(const std::string &name, const std::string &permissions);

    std::string_view username() const;
    const Permissions &permissions() const;

    bool operator==(const std::string &name) const;
private:
    std::string name;
    std::unique_ptr<Permissions> perms;
};

} // namespace security

#endif // __USER_H__
