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
    int rc = 0;
    do {
        std::cout << "username >> ";
        std::getline(std::cin, cmd, '\n');

        CommandPtr found = find_cmd(cmd);
        if(nullptr == found) {
            print("Command '" + cmd + "' not found");
            continue;
        }
        found->setArgs(cmd);
        rc = found->execute();
        if(0 != rc) {
            printErr(found->info().name, rc);
        }
    } while(true);
}

void Shell::print(const std::string &str, bool printShell) const {
    std::cout << (printShell ? "[Shell] " : "") << str << std::endl;
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

void Shell::printErr(const std::string &cmdname, int rc) const {
    print("Error: command '" + cmdname + "' return " + std::to_string(rc));
}

} // namespace env
