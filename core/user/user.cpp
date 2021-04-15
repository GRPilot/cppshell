#include "deps.hpp"

#include "user.hpp"
#include "permissions.hpp"

namespace security {

User::User(const std::string &name) : name(name) {}

std::string_view User::username() const {
    return name;
}

std::shared_ptr<Permissions> User::permissions() const {
    return perms;
}

} // namespace security
