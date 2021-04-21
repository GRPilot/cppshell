#include "deps.hpp"
#include "core.hpp"

#include "userstorage.hpp"
#include "user.hpp"
#include "permissions.hpp"
#include "chiper.hpp"

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

UserPtr UserStorage::authorize(const std::string &username, const std::string &password) const {
    if(password.size() < MIN_PASSWORD_LEN) {
        return nullptr;
    }

    auto found = std::find(users.begin(), users.end(), username);
    if(found == users.end() || found->authorized) {
        return nullptr;
    }

    Chiper chiper(Chiper::Method::SHA256);
    ChiperData passwordHash = chiper.encrypt(ChiperData(password.begin(), password.end()));
    if(found->hash == std::to_string(passwordHash)) {
        return found->user;
    }
    return nullptr;
}

bool UserStorage::signIn(const std::string &username, const std::string &password) {
    if(password.size() < MIN_PASSWORD_LEN) {
        return false;
    }

    auto found = std::find(users.begin(), users.end(), username);
    if(found != users.end()) {
        return false;
    }

    Chiper chiper(Chiper::Method::SHA256);
    ChiperData passwordHash = chiper.encrypt(ChiperData(password.begin(), password.end()));
    users.emplace_back(
        std::make_shared<User>(username, "---"),
        false,
        std::to_string(passwordHash)
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
    if(password.size() < MIN_PASSWORD_LEN) {
        return false;
    }

    auto found = std::find(users.begin(), users.end(), user);
    if(found == users.end() || !found->authorized) {
        return false;
    }
    Chiper chiper(Chiper::Method::SHA256);
    ChiperData passwordHash = chiper.encrypt(ChiperData(password.begin(), password.end()));
    found->hash = std::to_string(passwordHash);
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
    auto make_record = [](const StoredUser &sUser) {
        std::stringstream out;
        out << sUser.user->username() << ";"
            << sUser.user->permissions() << ";"
            << sUser.hash;
        return out.str();
    };

    std::ofstream storage(storageFilename);
    for(const auto &stored : users) {
        storage << make_record(stored) << std::endl;
    }
}

} // namespace security
