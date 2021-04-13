#include "deps.hpp"
#include "shell.hpp"
#include "cmdgen.hpp"
#include "shellcmd.hpp"


// struct SomeOtherCmd : public Shell::Command {
//     SomeOtherCmd() : Command("some_other", "so", "Do some other hard work") {}
//     int execute() override {
//         printf("SomeOTHER\n");
//         return 0;
//     }
// };

int main() {
    env::Shell shell;
    shell.addCommand(env::CommandGenerator::gen(env::CommandType::Help, shell));
    shell.addCommand(env::CommandGenerator::gen(env::CommandType::Exit));
    shell.addCommand(env::CommandGenerator::gen(env::CommandType::HelloWorld));
    shell.addCommand(env::CommandGenerator::gen(env::CommandType::Echo, shell));
    return shell.start();
}

//================================================================================================//
