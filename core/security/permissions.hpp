#ifndef __PERMISSIONS_H__
#define __PERMISSIONS_H__

namespace security {

class Permissions {
    enum {
        p_read_position,
        p_write_position,
        p_execute_position,

        PERMISSIONS_COUNT ///< Always last
    };
public:
    enum permission_t {
        p_read    = (1 << p_read_position),
        p_write   = (1 << p_write_position),
        p_execute = (1 << p_execute_position),
    };

    Permissions() = default;
    explicit Permissions(const std::string &strperms);

    bool read() const;
    bool write() const;
    bool execute() const;

    void read(bool allow);
    void write(bool allow);
    void execute(bool allow);

    operator std::string() const;

private:
    std::bitset<PERMISSIONS_COUNT> perms;
};

} // namespace security

namespace std {
    string to_string(const security::Permissions &permissions);
} // namespace std
std::ostream &operator<<(std::ostream &out, const security::Permissions &permissions);

#endif // __PERMISSIONS_H__
