#include "deps.hpp"

#include "cmdgen.hpp"
#include "shell.hpp"
#include "shellcmd.hpp"

namespace env {

class CmdHelp : public Command {
public:
    CmdHelp(Shell &shell) : Command("help", "h", "Show all commands", shell) {}
    int execute() override {
        const std::vector<std::shared_ptr<Command>> &commands = holder.getCommands();
        if(commands.empty()) {
            holder.print("Sorry, but there is no one registered command");
            return -1;
        }
        std::stringstream ss;
        ss.setf(std::ios::left);
        ss << "Commands:" << std::endl;

        ss.width(sizeof("Command: "));
        ss << "Command: ";
        ss.width(sizeof("Shortcut:"));
        ss << "Shortcut:";
        ss << "Description:" << std::endl;
        for(const std::shared_ptr<Command> &cmd : commands) {
            CmdInfo info = cmd->info();
            std::cout << " ";
            ss.width(sizeof("Command: "));
            ss << info.name;
            ss.width(sizeof("Shortcut:"));
            ss << info.shortcut << "- ";
            ss << info.description << std::endl;
        }
        ss.clear();
        holder.print(ss.str(), false);
        return 0;
    }
};

class CmdExit : public Command {
public:
    CmdExit(Shell &holder) : Command("exit", "e", "Exit shell", holder) {}
    int execute() override {
        exit(0);
        return 0;
    }
};

class CmdEcho : public Command {
public:
    CmdEcho(Shell &shell) : Command("echo", "", "Just echo. Nothing more, but a lil less", shell) {}
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
        : Command("ls", "l", "List information about the FILEs (the current directory by default)",
                  holder) {
    }

    int execute() override {
        ParsedArguments args(arguments);
        return ls(args);
    }

private:
    int ls(const ParsedArguments &args) const {
        namespace fs = std::filesystem;
        try {
            std::stringstream out;
            out << std::endl;
            auto found = std::find(args.params.begin(), args.params.end(), ParsedArguments::recursive);
            if(found == args.params.end()) {
                for(const auto &entry : fs::directory_iterator(args.path)) {
                    out << entry.path() << std::endl;
                }
            } else {
                for(const auto &entry : fs::recursive_directory_iterator(args.path)) {
                    out << entry.path() << std::endl;
                }
            }
            holder.print(out.str(), false);
        } catch(fs::filesystem_error &err) {
            holder.print(std::string("[LS] Error: ").append(err.what()));
            return err.code().value();
        }
        return 0;
    }
};

class CmdCd : public Command {
public:
    explicit CmdCd(Shell &holder) : Command("cd", "", "Change dirrectory", holder) {}

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
};

class CmdPwd : public Command {
public:
    explicit CmdPwd(Shell &holder)
        : Command("pwd", "", "Show the current working directory", holder) {}

    int execute() override try {
        holder.print(std::string("[PWD] ").append(std::filesystem::current_path()));
        return 0;
    } catch(std::filesystem::filesystem_error &err) {
        holder.print(std::string("[PWD] ").append(err.what()));
        return err.code().value();
    }
};

class CmdCat : public Command {
public:
    explicit CmdCat(Shell &holder)
        : Command("cat", "ct", "Concatenate FILE(s) to standard output", holder) {}

    int execute() override {
        const std::string &filename = arguments.back();
        std::ifstream file(filename);
        if(!file.good()) {
            holder.print("[CAT] Error: the argument is not a filename");
            return -1;
        }
        std::stringstream content;
        constexpr size_t LINE_SIZE = 60;
        content << std::endl << std::string(LINE_SIZE, '-') << std::endl
                << file.rdbuf()
                << std::endl << std::string(LINE_SIZE, '-');
        holder.print(content.str());
        return 0;
    };
};

class CmdTouch : public Command {
public:
    explicit CmdTouch(Shell &holder)
        : Command("touch", "tc", "Create a file", holder) {}

    int execute() override {
        if(arguments.size() < 2) {
            holder.print("[TOUCH] Wrong arguments");
            return -1;
        }

        std::ofstream newfile;
        newfile.exceptions(std::ios::badbit | std::ios::failbit);
        try {
            const std::string &filename = arguments.at(1);
            newfile.open(filename);
            newfile.close();
            return 0;
        } catch(std::fstream::failure &err) {
            holder.print(std::string("[TOUCH] Error: ") + err.what());
            return err.code().value();
        } catch(...) {
            return -2;
        }
    }
};

class CmdRemove : public Command {
public:
    explicit CmdRemove(Shell &holder)
        : Command("rm", "", "Remove file of folder", holder) {}

    int execute() override {
        if(arguments.size() < 2) {
            holder.print("[RM] Wrong arguments");
            return -1;
        }
        if(arguments[1] != "-r") {
            return remove(arguments[1]);
        }
        if(arguments.size() < 3) {
            holder.print("[RM] Wrong arguments");
            return -2;
        }
        return removeRecursive(arguments[2]);
    }
private:
    int remove(const std::string &path) try {
        if(std::filesystem::remove(path)) {
            return 0;
        }
        return -1;
    } catch(std::filesystem::filesystem_error &err) {
        holder.print(std::string("[RM] Error: ") + err.what());
        holder.print(std::string("[RM] Use -r for directories"));
        return err.code().value();
    }

    int removeRecursive(const std::string &path) try {
        if(std::filesystem::remove_all(path)) {
            return 0;
        }
        return -1;
    } catch(std::filesystem::filesystem_error &err) {
        holder.print(std::string("[RM] Error: ") + err.what());
        return err.code().value();
    }
};

//= == == == == == == == == == == == == = Generators = == == == == == == == == == == == == ==//

CommandPtr CommandGenerator::gen(CommandType type, Shell &holder) {
    switch(type) {
        case CommandType::Exit  : return CommandPtr(new CmdExit(holder));
        case CommandType::Help  : return CommandPtr(new CmdHelp(holder));
        case CommandType::Echo  : return CommandPtr(new CmdEcho(holder));
        case CommandType::Ls    : return CommandPtr(new CmdLs(holder));
        case CommandType::Cd    : return CommandPtr(new CmdCd(holder));
        case CommandType::Pwd   : return CommandPtr(new CmdPwd(holder));
        case CommandType::Cat   : return CommandPtr(new CmdCat(holder));
        case CommandType::Touch : return CommandPtr(new CmdTouch(holder));
        case CommandType::Remove: return CommandPtr(new CmdRemove(holder));
    }
    return nullptr;
}

} // namespace env
