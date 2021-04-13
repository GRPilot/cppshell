#ifndef __SHELLCMD_H__
#define __SHELLCMD_H__

namespace env {

struct CmdInfo {
    std::string name;
    std::string shortcut;
    std::string description;
};

class Command {
public:
    explicit Command(const std::string &name,
                     const std::string &shortcut,
                     const std::string &description);

    virtual bool operator==(const std::string &other);

    virtual CmdInfo info() const;
    virtual int execute() = 0;
protected:
    CmdInfo information;
};

} // namespace env

#endif // __SHELLCMD_H__
