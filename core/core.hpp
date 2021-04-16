#ifndef __CORE_H__
#define __CORE_H__

namespace core {

std::vector<std::string> split(const std::string &str, char sep);

} // namespace core

template<class T>
class singleton {
public:
    static T &instance() {
        static T single;
        return single;
    }

protected:
    explicit singleton() {}
    virtual ~singleton() {}
};

#endif // __CORE_H__
