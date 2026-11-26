#include <stdio.h>
namespace NParticleEngine {
class CParticleEmitterInfo {
public:
    unsigned long Data;
    unsigned long Bank;
    unsigned long Index;
};
}
static unsigned int g_n = 0;
static unsigned long g_data = 1, g_bank = 1, g_index = 1;
namespace std {
template<class T> class allocator {};
template<class T, class Al=allocator<T> > class vector {
public:
    void resize(unsigned int n);
    void resize(unsigned int n, const T& value);
};
template<class T, class Al>
void vector<T,Al>::resize(unsigned int n, const T& value) {
    g_n = n;
    g_data = value.Data;
    g_bank = value.Bank;
    g_index = value.Index;
}
}
template void std::vector<NParticleEngine::CParticleEmitterInfo>::resize(unsigned int, const NParticleEngine::CParticleEmitterInfo&);
int main() {
    std::vector<NParticleEngine::CParticleEmitterInfo> values;
    values.resize(7);
    if (g_n == 7 && g_data == 0 && g_bank == 0 && g_index == 0) {
        printf("00af2200_TEST PASS\n");
        return 0;
    }
    printf("00af2200_TEST FAIL %u %lu %lu %lu\n", g_n, g_data, g_bank, g_index);
    return 1;
}