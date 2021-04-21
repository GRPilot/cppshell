#include "deps.hpp"
#include "core.hpp"

#include "chiper.hpp"
#include "magiccube.hpp"

namespace security {

std::map<uint32_t, std::vector<uint32_t>> MagicCube::cubeCache{
    std::pair<uint32_t, std::vector<uint32_t>>{
        4u, std::vector<uint32_t>{
             7, 12,  1, 14,
             2, 13,  8, 11,
            16,  3, 10,  5,
             9,  6, 15,  4
        }
    }
};

ChiperData MagicCube::encrypt(const ChiperData &data) {
    if(data.empty()) {
        return data;
    }

    uint32_t demention = sqrt(data.size()) + 1;
    std::vector<uint32_t> cube = getMagicCube(demention);
    ChiperData result(data.size(), ' ');

    const size_t cs = cube.size();
    int skipCount = 0;
    for(int id = 0; id < cs; ++id) {
        if(data[id] == '\n') {
            continue;
        }

        int chr_id = (cube[id % cs] - 1) + cs * (id / cs);

        if(chr_id >= data.size()) {
            ++skipCount;
            continue;
        }

        result[id - skipCount] = data[chr_id];
    }

    return result;
}

std::vector<uint32_t> MagicCube::getMagicCube(uint32_t demention) const {
    const auto &found = cubeCache.find(demention);
    if(found != cubeCache.end()) {
        return found->second;
    }

    if(demention == 2) {
        ++demention;
    }

    std::vector<uint32_t> &cube = cubeCache[demention];
    const uint32_t cells = demention * demention;
    cube.assign(cells, 0);

    int h = demention / 2; 
    int w = demention  - 1; 
    for(int num = 1; num <= cells; ) {
        if(h == -1 && w == demention) {
            w = demention - 2;
            h = 0;
        } else {
            if(w == demention) w = 0;
            if(h < 0) h = demention - 1;
        }

        if(cube[w + (h * demention)]) {
            w -= 2;
            h++;
            continue;
        } else {
            cube[w + (h * demention)] = num++;
        }

        w++; h--;
    }

    return cube;
}

} // namespace security
