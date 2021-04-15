#include "deps.hpp"

#include "shellcmd.hpp"

namespace env {

Command::Command(const std::string &name,
                 const std::string &shortcut,
                 const std::string &description)
    : information{name, shortcut, description} {
}

bool Command::operator==(const std::string &other) {
    if(other.empty()) {
        return false;
    }
    const size_t spacePos = other.find(' ');
    bool hasArguments = spacePos != std::string::npos;
    if(!hasArguments) {
        return information.name == other || information.shortcut == other;
    }
    std::string_view cmdname(other.c_str(), spacePos);
    return information.name == cmdname || information.shortcut == cmdname;
}

CmdInfo Command::info() const {
    return information;
}

} // namespace env
