// behaviour test for 0x00AF2620 NParticleEngine::CPSCBase::CPSCBase(), linked against the built constructor: the CBase
// base constructor runs once, no particle system, both state bits clear (the rest of the flag byte kept), vtable set
#include <cstdio>
#include <cstring>
#include <new>
static int g_baseCtors = 0;
struct CBase {
    virtual ~CBase();
    CBase();
};
CBase::CBase() { ++g_baseCtors; }
CBase::~CBase() { }
namespace NParticleEngine {
struct CParticleSystem;
class CPSCBase : public CBase {
public:
    CParticleSystem* ParticleSystem;
    bool             UpdateStarted : 1;
    bool             Initialised : 1;
    CPSCBase();
};
}
int main()
{
    static unsigned char storage[sizeof(NParticleEngine::CPSCBase) + 16];
    std::memset(storage, 0xFF, sizeof(storage));
    NParticleEngine::CPSCBase* p = new (storage) NParticleEngine::CPSCBase();
    bool ok = sizeof(NParticleEngine::CPSCBase) == 12 && g_baseCtors == 1 && p->ParticleSystem == 0 && !p->UpdateStarted
        && !p->Initialised && storage[8] == 0xFC && *(unsigned long*)storage != 0xFFFFFFFFu;
    if (!ok) { std::printf("00af2620_TEST FAIL\n"); return 1; }
    std::printf("00af2620_TEST PASS\n");
    return 0;
}
