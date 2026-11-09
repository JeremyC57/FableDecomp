// Behaviour test for CEngineClothSimulationAnimatedMesh's scalar deleting destructor (0x00B84190): the empty derived destructor, then the
// base CEngineClothSimulationMeshBase destructor (ClearParticleSims, the particle array destroyed, ~CBaseClassNonCopyable), memory freed only
// on flag bit 0 (a virtual `delete` through the base pointer frees; an explicit destructor call does not).
#include <new>
#include <cstdio>
#include <cstdlib>
static int g_log[8]; static int g_n; static int g_frees;
struct CBaseClassNonCopyable { virtual ~CBaseClassNonCopyable() { g_log[g_n++] = 3; } };
struct CParticleArray { ~CParticleArray() { g_log[g_n++] = 2; } };
struct CEngineClothSimulationMeshBase : CBaseClassNonCopyable {
    CParticleArray ParticleSims;
    void ClearParticleSims() { g_log[g_n++] = 1; }
    virtual ~CEngineClothSimulationMeshBase() { ClearParticleSims(); }
    static void* operator new(std::size_t n) { return std::malloc(n); }
    static void operator delete(void* p) { ++g_frees; std::free(p); }
};
struct CEngineClothSimulationAnimatedMesh : CEngineClothSimulationMeshBase { virtual ~CEngineClothSimulationAnimatedMesh() { } };
static int Check(int frees) { return g_n == 3 && g_log[0] == 1 && g_log[1] == 2 && g_log[2] == 3 && g_frees == frees; }
int main()
{
    CBaseClassNonCopyable* p = new CEngineClothSimulationAnimatedMesh;
    g_n = 0; delete p;                                       // ??_G with flag 1
    int ok = Check(1);
    unsigned char storage[sizeof(CEngineClothSimulationAnimatedMesh)];
    CEngineClothSimulationAnimatedMesh* m = ::new (storage) CEngineClothSimulationAnimatedMesh;
    g_n = 0; m->~CEngineClothSimulationAnimatedMesh();       // flag 0: destroyed, not freed
    ok = ok && Check(1);
    std::printf(ok ? "00b84190_TEST PASS\n" : "00b84190_TEST FAIL\n");
    return ok ? 0 : 1;
}