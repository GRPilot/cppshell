#ifndef __CMDGEN_H__
#define __CMDGEN_H__

namespace env {

class Command;
namespace {
using CommandPtr = std::shared_ptr<Command>;
}
class Shell;

/**
 * @todo:
 * [*] ls;
 * [ ] cd;
 * [ ] touch;
 * [ ] cat;
 * [ ] chmod;
 * [ ] whoami;
 * [ ] pwd;
 */

enum class CommandType {
    Help,
    Exit,
    Echo,
    Ls,
};

class CommandGenerator {
public:
    static CommandPtr gen(CommandType type);
    static CommandPtr gen(CommandType type, Shell &holder);
};

} // namespace env

#endif // __CMDGEN_H__
