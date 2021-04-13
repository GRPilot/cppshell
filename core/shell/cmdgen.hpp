#ifndef __CMDGEN_H__
#define __CMDGEN_H__

namespace env {

class Command;
namespace {
using CommandPtr = std::shared_ptr<Command>;
}
class Shell;

enum class CommandType {
    Help,
    HelloWorld,
    Exit,
    Echo,
};

class CommandGenerator {
public:
    static CommandPtr gen(CommandType type);
    static CommandPtr gen(CommandType type, Shell &holder);
};

} // namespace env

#endif // __CMDGEN_H__
