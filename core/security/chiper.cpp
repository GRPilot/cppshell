#include "deps.hpp"
#include "core.hpp"

#include "chiper.hpp"

#include "magiccube.hpp"
#include "sha256.hpp"

namespace security {

Chiper::Chiper(Chiper::Method method) {
    ChiperMethod *chipmethod = nullptr;

    switch(method) {
        case Method::SHA256:
            chipmethod = new SHA256();
            break;
        case Method::MagicCube:
            chipmethod = new MagicCube();
            break;
    }

    this->method.reset(chipmethod);
}

ChiperData Chiper::encrypt(const ChiperData &data) {
    if(method) {
        return method->encrypt(data);
    }
    return ChiperData{};
}

} // namespace security

namespace std {
    string to_string(const security::ChiperData &data) {
        stringstream out;
        for(core::byte_t byte : data) {
            out.width(2);
            out.fill('0');
            out << std::hex << std::uppercase << (int)(byte);
        }
        return out.str();
    }
} // namespace std
