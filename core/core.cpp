#include "deps.hpp"

#include "core.hpp"

namespace core {

std::vector<std::string> split(const std::string &str, char sep) {
    if(str.empty()) {
        return {};
    }
    std::vector<std::string> splitted;
    size_t last_pos = 0;
    for(size_t i = 0; i < str.size(); ++i) {
        bool isPreLastId = i + 1 == str.size();
        if(str[i] == sep || isPreLastId) {
            if(!isPreLastId) {
                splitted.emplace_back(str.substr(last_pos, i - last_pos));
            } else {
                splitted.emplace_back(str.substr(last_pos));
            }
            last_pos = i + 1;
        }
    }
    if(splitted.empty()) {
        splitted.emplace_back(str);
    }
    return splitted;
}

} // namespace core
