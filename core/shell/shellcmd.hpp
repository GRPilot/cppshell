#ifndef __SHELLCMD_H__
#define __SHELLCMD_H__

namespace env {

struct CmdInfo {
    std::string name;
    std::string shortcut;
    std::string description;
};

class Shell;

class Command {
public:
    explicit Command(const std::string &name,
                     const std::string &shortcut,
                     const std::string &description,
                     Shell &holder);

    virtual bool operator==(const std::string &other);

    virtual CmdInfo info() const;
    virtual int execute() = 0;
    virtual int setArgs(const std::string &args);
protected:
    CmdInfo information;
    std::vector<std::string> arguments;
    Shell &holder;
};

} // namespace env

#endif // __SHELLCMD_H__
