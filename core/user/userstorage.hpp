#ifndef __USERSTORAGE_H__
#define __USERSTORAGE_H__

namespace security {

class User;
using UserPtr = std::shared_ptr<User>;

class UserStorage : public singleton<UserStorage> {
    friend class singleton<UserStorage>;

    static const char * storageFilename;
    static const size_t UserStorage::BLOCK_COUNT;
public:

    UserPtr authorize(const std::string &username, const std::string &password);
    bool logOut(UserPtr user);

    bool rename(UserPtr user, const std::string &newName);
    bool changePassword(UserPtr user, const std::string &password);

private:
    using AuthorizedFlag = bool;
    struct StoredUser {
        explicit StoredUser(UserPtr user, AuthorizedFlag authorized, const std::string &hash);

        UserPtr user;
        AuthorizedFlag authorized;
        std::string hash;
        bool operator==(const std::string &name) const;
        bool operator==(const UserPtr &name) const;
    };
    std::vector<StoredUser> users;

    void loadUserStorage();
    void updateUserStorage() const;
};

} // namespace security

#endif // __USERSTORAGE_H__
