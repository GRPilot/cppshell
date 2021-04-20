#ifndef __MAGICCUBE_H__
#define __MAGICCUBE_H__

namespace security {

class MagicCube : public ChiperMethod {
public:
    ChiperData encrypt(const ChiperData &data) override;
private:
    std::vector<uint32_t> getMagicCube(uint32_t demention) const;

    static std::map<uint32_t, std::vector<uint32_t>> cubeCache;
};

} // namespace security

#endif // __MAGICCUBE_H__
