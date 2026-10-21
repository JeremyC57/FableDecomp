// behaviour test for 0x00469435
#include <stdio.h>
#include <string.h>
#include <new>
class CBlendedParticleEffectSet {
public:
    long  Particle1Index;                                // +0x00
    float Particle1Value;                                // +0x04
    long  Particle2Index;                                // +0x08
    float Particle2Value;                                // +0x0c
    CBlendedParticleEffectSet() : Particle1Index(0), Particle1Value(0.0f), Particle2Index(0), Particle2Value(0.0f) {}
};
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair() : first(T1()), second(T2()) {}
};
template struct pair<long, CBlendedParticleEffectSet>;
}
int main()
{
    unsigned char buf[0x14]; memset(buf, 0xAB, sizeof(buf));
    std::pair<long, CBlendedParticleEffectSet>* p = new (buf) std::pair<long, CBlendedParticleEffectSet>();
    int ok = p->first == 0 && p->second.Particle1Index == 0 && p->second.Particle1Value == 0.0f
          && p->second.Particle2Index == 0 && p->second.Particle2Value == 0.0f && sizeof(*p) == 0x14;
    printf(ok ? "00469435_TEST PASS\n" : "00469435_TEST FAIL\n");
    return ok ? 0 : 1;
}