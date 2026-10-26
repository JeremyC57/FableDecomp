// Behaviour test for 0x00AF67B0 pair<NParticleEngine::EPSCType, NParticleEngine::CComponentInstance>::pair().
#include <cstdio>
#include <cstring>

class CBaseClass {
public:
    CBaseClass() {}
    virtual ~CBaseClass() {}
};
template <class T> class CCountedPointer { public:
    T*    Data;
    void* m_pCount;
    CCountedPointer() : Data(0), m_pCount(0) {}
};
namespace NParticleEngine {
enum EPSCType { PSC_NONE = 0, PSC_SPAWNER = 1 };
class CPSCBase;
class CComponentInstance : public CBaseClass {
public:
    CCountedPointer<CPSCBase> Component;
    bool                      Enabled;
    unsigned long             InstanceName;
    CComponentInstance() : Enabled(true) {}
};
}
namespace stlp {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) {}
};
}

int main()
{
    stlp::pair<NParticleEngine::EPSCType, NParticleEngine::CComponentInstance> p;
    if (!(p.first == NParticleEngine::PSC_NONE && p.second.Component.Data == 0 && p.second.Component.m_pCount == 0 && p.second.Enabled)) {
        std::printf("00af67b0_TEST FAIL\n");
        return 1;
    }
    std::printf("00af67b0_TEST PASS\n");
    return 0;
}