#include "deps.hpp"

#include "user.hpp"
#include "permissions.hpp"

namespace security {

User::User(const std::string &name, const std::string &permissions)
    : name(name), perms(std::make_shared<Permissions>(permissions)) {}

std::string_view User::username() const {
    return name;
}

std::shared_ptr<Permissions> User::permissions() const {
    return perms;
}

bool User::operator==(const std::string &name) const {
    return this->name == name;
}

} // namespace security
