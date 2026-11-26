// std::vector<NParticleEngine::CParticleEmitterInfo>::resize(unsigned int) -- value-initialise a 12-byte element and forward it to the two-argument resize
// Leaf: ?resize@?$vector@VCParticleEmitterInfo@NParticleEngine@@V?$allocator@VCParticleEmitterInfo@NParticleEngine@@@std@@@std@@QAEXI@Z
// Identity: retail 0x00AF2200, 36 bytes. Loads n, xor-zeroes a 12-byte CParticleEmitterInfo (Data, Bank, Index), calls resize(n, value) at 0x00AF2170. The catalog label vector<EWeaponClass>::_Umove is propagated.
namespace NParticleEngine {
class CParticleEmitterInfo {
public:
    unsigned long Data;
    unsigned long Bank;
    unsigned long Index;
};
}
namespace std {
template<class T> class allocator {};
template<class T, class Al=allocator<T> > class vector {
public:
    void resize(unsigned int, const T&);
    void resize(unsigned int n) {
        unsigned long z = 0;
        unsigned int count = n;
        T value;
        value.Data = z;
        value.Bank = z;
        value.Index = z;
        resize(count, value);
    }
};
}
template void std::vector<NParticleEngine::CParticleEmitterInfo>::resize(unsigned int);