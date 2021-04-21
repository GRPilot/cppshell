#include "deps.hpp"
#include "core.hpp"
#include "shell.hpp"
#include "cmdgen.hpp"
#include "shellcmd.hpp"
#include "userstorage.hpp"
#include "user.hpp"
#include "permissions.hpp"

int main() {
    env::Shell shell;
    shell.addCommand(env::CommandGenerator::gen(env::CommandType::Ls, shell));
    shell.addCommand(env::CommandGenerator::gen(env::CommandType::Cd, shell));
    shell.addCommand(env::CommandGenerator::gen(env::CommandType::Pwd, shell));
    shell.addCommand(env::CommandGenerator::gen(env::CommandType::Cat, shell));
    shell.addCommand(env::CommandGenerator::gen(env::CommandType::Help, shell));
    shell.addCommand(env::CommandGenerator::gen(env::CommandType::Exit, shell));
    shell.addCommand(env::CommandGenerator::gen(env::CommandType::Echo, shell));
    shell.addCommand(env::CommandGenerator::gen(env::CommandType::Touch, shell));
    shell.addCommand(env::CommandGenerator::gen(env::CommandType::Remove, shell));
    return shell.start();
}
