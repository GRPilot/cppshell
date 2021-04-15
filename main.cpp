#include "deps.hpp"
#include "core.hpp"
#include "shell.hpp"
#include "cmdgen.hpp"
#include "shellcmd.hpp"

int main() {
    env::Shell shell;
    shell.addCommand(env::CommandGenerator::gen(env::CommandType::Help, shell));
    shell.addCommand(env::CommandGenerator::gen(env::CommandType::Exit, shell));
    shell.addCommand(env::CommandGenerator::gen(env::CommandType::Echo, shell));
    shell.addCommand(env::CommandGenerator::gen(env::CommandType::Ls, shell));
    shell.addCommand(env::CommandGenerator::gen(env::CommandType::Cd, shell));
    return shell.start();
}
