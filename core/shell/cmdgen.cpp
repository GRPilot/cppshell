#include "deps.hpp"

#include "cmdgen.hpp"
#include "shell.hpp"
#include "shellcmd.hpp"

namespace env {

class CmdHelp : public Command {
public:
    CmdHelp(Shell &shell) : Command("help", "h", "Show all commands"), holder(shell) {}
    int execute() override {
        const std::vector<std::shared_ptr<Command>> &commands = holder.getCommands();
        if(commands.empty()) {
            holder.print("Sorry, but there is no one registered command");
            return -1;
        }
        std::stringstream ss;
        ss << "Commands:" << std::endl;
        for(const std::shared_ptr<Command> &cmd : commands) {
            CmdInfo info = cmd->info();
            std::cout << " ";
            ss.width(5);
            ss.setf(std::ios::left);
            ss << info.shortcut;
            ss.width(10);
            ss << info.name << "  - ";
            ss << info.description << std::endl;
        }
        holder.print(ss.str());
        return 0;
    }
private:
    Shell &holder;
};

class CmdExit : public Command {
public:
    CmdExit() : Command("exit", "e", "Exit shell") {}
    int execute() override {
        exit(0);
        return 0;
    }
};

class CmdEcho : public Command {
public:
    CmdEcho(Shell &shell)
        : Command("echo", "", "Just echo. Nothing more, but a lil less"), holder(shell) {}
    int execute() override {
        holder.print(params);
        params.clear();
        return 0;
    }
    bool operator==(const std::string &other) override {
        if(other.empty()) {
            return false;
        }
        size_t space_pos = other.find(' ');
        if(space_pos == std::string::npos) {
            return Command::operator==(other);
        }
        std::string cmdname = other.substr(0, space_pos);
        if(!Command::operator==(cmdname)) {
            return false;
        }
        params = other.substr(space_pos + 1);
        return true;
    }
private:
    Shell &holder;
    std::string params;
};

class CmdLs : public Command {
    struct ParsedArguments {
        enum params {
            recursive,
        };
        std::string path = ".";
        std::vector<params> params;
        explicit ParsedArguments(const std::vector<std::string> &args) {
            if(args.empty()) {
                return;
            }
            for(const auto &a : args) {
                std::size_t npos = std::string::npos;
                if(a.find("-r") != npos) {
                    params.push_back(recursive);
                }
                if(a.find('/') != npos || a.find('.') != npos) {
                    path = a;
                    if(path.back() != '/') {
                        path += '/';
                    }
                }
            }
        }
    };
public:
    explicit CmdLs(Shell &holder)
        : Command("ls", "l", "List information about the FILEs (the current directory by default)"),
          holder(holder) {
    }

    int execute() override {
        printf("logi: arguments size: %zd\n", arguments.size());
        ParsedArguments args(arguments);
        return ls(args);
    }

private:
    Shell &holder;
    int ls(const ParsedArguments &args) const {
        namespace fs = std::filesystem;
        try {
            auto found = std::find(args.params.begin(), args.params.end(), ParsedArguments::recursive);
            if(found == args.params.end()) {
                for(const auto &entry : fs::directory_iterator(args.path)) {
                    holder.print(entry.path());
                }
            } else {
                for(const auto &entry : fs::recursive_directory_iterator(args.path)) {
                    holder.print(entry.path());
                }
            }
        } catch(fs::filesystem_error &err) {
            holder.print(std::string("[LS] Error: ").append(err.what()));
            return err.code().value();
        }
        return 0;
    }
};

class CmdCd : public Command {
public:
    explicit CmdCd(Shell &holder) : Command("cd", "", "Change dirrectory"), holder(holder) {}

    int execute() override try {
        const std::string_view path = arguments.back();
        if(!std::filesystem::is_directory(path)) {
            holder.print("[CD] Error: the path is not a directory");
        }
        std::filesystem::current_path(path);
        return 0;
    } catch(std::filesystem::filesystem_error &err) {
        holder.print(std::string("[CD] ").append(err.what()));
        return err.code().value();
    }

private:
    Shell &holder;
};

//= == == == == == == == == == == == == = Generators = == == == == == == == == == == == == ==//

CommandPtr CommandGenerator::gen(CommandType type) {
    switch(type) {
        case CommandType::Exit: return CommandPtr(new CmdExit);
    }
    return nullptr;
}

CommandPtr CommandGenerator::gen(CommandType type, Shell &holder) {
    switch(type) {
        case CommandType::Help: return CommandPtr(new CmdHelp(holder));
        case CommandType::Echo: return CommandPtr(new CmdEcho(holder));
        case CommandType::Ls  : return CommandPtr(new CmdLs(holder));
        case CommandType::Cd  : return CommandPtr(new CmdCd(holder));
    }
    return nullptr;
}

} // namespace env
