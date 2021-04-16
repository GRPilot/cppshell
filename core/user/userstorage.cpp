#include "deps.hpp"
#include "core.hpp"

#include "userstorage.hpp"
#include "user.hpp"
#include "permissions.hpp"

namespace security {

UserStorage::UserStorage() {
    loadUserStorage();
}

UserStorage::~UserStorage() {
    updateUserStorage();
}

UserStorage::StoredUser::StoredUser(UserPtr user, AuthorizedFlag authorized, const std::string &hash)
    : user(user), authorized(authorized), hash(hash) {
    assert(user && "Cannot initialize user by null pointer");
}

bool UserStorage::StoredUser::operator==(const std::string &name) const {
    return *user == name;
}

bool UserStorage::StoredUser::operator==(const UserPtr &name) const {
    return user.get() == name.get();
}

UserPtr UserStorage::authorize(const std::string &username, const std::string &password) {
    auto found = std::find(users.begin(), users.end(), username);
    if(found == users.end() || found->authorized) {
        return nullptr;
    }
    /// @todo: hash password
    if(found->hash == password) {
        return found->user;
    }
    return nullptr;
}

bool UserStorage::signIn(const std::string &username, const std::string &password) {
    auto found = std::find(users.begin(), users.end(), username);
    if(found != users.end()) {
        return false;
    }

    /// @todo: hash password
    users.emplace_back(
        std::make_shared<User>(username, "---"), false, password
    );
    updateUserStorage();
    return true;
}

bool UserStorage::logOut(UserPtr user) {
    auto found = std::find(users.begin(), users.end(), user);
    if(found != users.end() && found->authorized) {
        found->authorized = false;
        return true;
    }
    return false;
}

bool UserStorage::rename(UserPtr user, const std::string &newName) {
    bool incorrectName = newName.size() < 4
        || std::find(users.begin(), users.end(), newName) == users.end();
    if(incorrectName) {
        return false;
    }

    auto found = std::find(users.begin(), users.end(), user);
    if(found == users.end() || found->authorized) {
        return false;
    }
    found->user->name = newName;
    updateUserStorage();
    return true;
}

bool UserStorage::changePassword(UserPtr user, const std::string &password) {
    auto found = std::find(users.begin(), users.end(), user);
    if(found == users.end() || !found->authorized) {
        return false;
    }
    /// @todo: hash password
    found->hash = password;
    updateUserStorage();
    return true;
}

void UserStorage::loadUserStorage() {
    users.clear();
    std::ifstream fileStorage(storageFilename);
    if(!fileStorage.good()) {
        return;
    }
    std::istream_iterator<std::string> cur(fileStorage);
    std::istream_iterator<std::string> end;
    for(; cur != end; ++cur) {
        std::vector<std::string> splitted = core::split(*cur, ';');
        if(splitted.size() < BLOCK_COUNT) {
            continue;
        }
        std::cout << "Load user: " << std::endl;
        std::cout << "    Username: " << splitted[0] << std::endl;
        std::cout << "    Permissions: " << splitted[1] << std::endl;
        std::cout << "    hash: " << splitted[2] << std::endl;
        /**
         * splitted[0] -> username
         * splitted[1] -> permissions
         * splitted[2] -> hash
         */
        users.emplace_back(
            std::make_shared<User>(splitted[0], splitted[1]), false, splitted[2]
        );
    }
}

void UserStorage::updateUserStorage() const {
    std::ofstream storage(storageFilename);
    auto make_record = [](const StoredUser &sUser) {
        std::stringstream out;
        out << sUser.user->username() << ";"
            << sUser.user->permissions() << ";"
            << sUser.hash;
        return out.str();
    };

    for(const auto &stored : users) {
        std::string debug = make_record(stored);
        std::cout << "Add record: [" << debug << "]\n";
        storage << debug << std::endl;
    }
}

} // namespace security
