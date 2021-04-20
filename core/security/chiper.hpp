#ifndef __CHIPER_H__
#define __CHIPER_H__

namespace security {

using ChiperData = std::vector<core::byte_t>;

class ChiperMethod {
public:
    virtual ChiperData encrypt(const ChiperData &data) = 0;
};

class Chiper {
public:
    enum class Method {
        SHA256,
        MagicCube,
    };

    explicit Chiper(Method method = Method::SHA256);

    ChiperData encrypt(const ChiperData &data);

private:
    std::unique_ptr<ChiperMethod> method;
};

} // namespace security

namespace std {
    string to_string(const security::ChiperData &data);
} // namespace std

#endif // __CHIPER_H__
