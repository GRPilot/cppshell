#ifndef __SHELL_H__
#define __SHELL_H__

namespace env {

class Command;
namespace {
using CommandPtr = std::shared_ptr<Command>;
}

class Shell {
public:

    void addCommand(const CommandPtr &cmd);

    int start() const;

    void print(const std::string &str) const;

    const std::vector<CommandPtr> &getCommands() const;
private:
    std::vector<CommandPtr> commands;

    CommandPtr find_cmd(const std::string &cmd) const;
};

} // namespace env

#endif // __SHELL_H__
