#include <cstdio>
#include <cstdlib>
#include <cstring>
// Behaviour test for 0x00AF5900: particles/decals deleted, then Name, UpdateParameters, Decals, Particles, Components, base.
static char g_log[32]; static int g_n = 0;
static void Log(char c) { g_log[g_n++] = c; g_log[g_n] = 0; }
namespace NParticleEngine {
struct SScalarTag { SScalarTag() {} };
struct CComponentEntry { int type; };
static void _Destroy_range(CComponentEntry*, CComponentEntry*, const SScalarTag&) { Log('R'); }
struct SComponentMap {
    CComponentEntry* _M_start; CComponentEntry* _M_finish; CComponentEntry* _M_end_of_storage; unsigned long m_compare;
    ~SComponentMap() { _Destroy_range(_M_start, _M_finish, SScalarTag()); if (_M_start) { Log('F'); std::free(_M_start); } }
};
struct SParticleList { void* head; ~SParticleList() { Log('P'); } };
struct SDecalList { void* head; ~SDecalList() { Log('D'); } };
struct SUpdateParameters { int body; ~SUpdateParameters() { Log('U'); } };
struct CCharString { void* m_pStringData; ~CCharString() { Log('N'); } };
struct CBase { virtual void v00() {} ~CBase() { Log('B'); } };
struct CParticleSystem : CBase {
    SComponentMap Components;
    SParticleList Particles;
    SDecalList Decals;
    SUpdateParameters UpdateParameters;
    CCharString Name;
    virtual void v00() {}
    void DeleteAllParticles() { Log('a'); }
    void DestroyDecals() { Log('d'); }
    ~CParticleSystem();
};
CParticleSystem::~CParticleSystem()
{
    DeleteAllParticles();
    DestroyDecals();
}
}
int main()
{
    { NParticleEngine::CParticleSystem s; s.Components._M_start = (NParticleEngine::CComponentEntry*)std::malloc(8); s.Components._M_finish = s.Components._M_start; }
    if (std::strcmp(g_log, "adNUDPRFB") != 0) { std::printf("00af5900_TEST FAIL\n"); return 1; }
    std::printf("00af5900_TEST PASS\n");
    return 0;
}
