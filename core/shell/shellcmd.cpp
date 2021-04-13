#include "deps.hpp"

#include "shellcmd.hpp"

namespace env {

Command::Command(const std::string &name,
                 const std::string &shortcut,
                 const std::string &description)
    : information{name, shortcut, description} {
}

bool Command::operator==(const std::string &other) {
    return information.name == other || information.shortcut == other;
}

CmdInfo Command::info() const {
    return information;
}

} // namespace env
