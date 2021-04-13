#include "deps.hpp"

#include "shell.hpp"
#include "shellcmd.hpp"

namespace env {

void Shell::addCommand(const CommandPtr &cmd) {
    if(nullptr == cmd) {
        return;
    }
    commands.push_back(cmd);
}

int Shell::start() const {
    std::string cmd;
    do {
        std::cout << "username >> ";
        std::getline(std::cin, cmd, '\n');
        if(cmd.find("exit") != std::string::npos) {
            return 0;
        }

        CommandPtr found = find_cmd(cmd);
        if(nullptr == found) {
            print("SHELL: Command '" + cmd + "' not found");
            continue;
        }

        found->execute();
    } while(true);
}

void Shell::print(const std::string &str) const {
    std::cout << str << std::endl;
}

const std::vector<CommandPtr> &Shell::getCommands() const {
    return commands;
}

CommandPtr Shell::find_cmd(const std::string &cmd) const {
    for(const std::shared_ptr<Command> &c : commands) {
        if(*c == cmd) {
            return c;
        }
    }
    return nullptr;
}

} // namespace env
