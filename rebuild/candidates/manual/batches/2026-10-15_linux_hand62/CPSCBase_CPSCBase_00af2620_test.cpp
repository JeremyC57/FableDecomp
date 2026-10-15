#include <cstdio>
#include <cstring>
#include <new>

// NParticleEngine::CPSCBase::CPSCBase(): CBase base, no particle system, both state bits clear.
static int g_baseCtors = 0;
struct CBase {
    virtual ~CBase() { }
    CBase() { ++g_baseCtors; }
};
namespace NParticleEngine {
struct CParticleSystem;
struct CPSCBase : CBase {
    CParticleSystem* ParticleSystem;
    bool             UpdateStarted : 1;
    bool             Initialised : 1;
    CPSCBase();
};
CPSCBase::CPSCBase()
    : ParticleSystem(0)
{
    UpdateStarted = false;
    Initialised = false;
}
}

int main()
{
    static unsigned char storage[sizeof(NParticleEngine::CPSCBase) + 16];
    std::memset(storage, 0xFF, sizeof(storage));
    NParticleEngine::CPSCBase* p = new (storage) NParticleEngine::CPSCBase();
    if (g_baseCtors != 1 || p->ParticleSystem != 0 || p->UpdateStarted || p->Initialised) {
        std::printf("00af2620_TEST FAIL\n");
        return 1;
    }
    std::printf("00af2620_TEST PASS\n");
    return 0;
}
