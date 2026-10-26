// Behaviour test for vector<CIntelligentPointer<CParticleEmitter>>::resize(n) (0x00657037): an empty pointer is passed with n and destroyed after.
#include <cstdio>
static unsigned int g_n = 0; static int g_emptyValue = 0; static int g_dtors = 0;
struct CBaseIntelligentPointer { void* PItem; CBaseIntelligentPointer() : PItem(0) {} ~CBaseIntelligentPointer() { ++g_dtors; } };
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public: CIntelligentPointer() {} };
class CParticleEmitter;
template<class T> struct TVector {
    void resize(unsigned int newSize, const T& value) { g_n = newSize; g_emptyValue = value.PItem == 0; }
    void resize(unsigned int newSize) { resize(newSize, T()); }
};
int main()
{
    TVector<CIntelligentPointer<CParticleEmitter> > v;
    v.resize(2);
    if (g_n != 2 || !g_emptyValue || g_dtors != 1) { std::printf("00657037_TEST FAIL\n"); return 1; }
    std::printf("00657037_TEST PASS\n");
    return 0;
}