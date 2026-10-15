#include <cstdio>
#include <cstdlib>
#include <cstring>
// Behaviour test for 0x00b81e60: ClearParticleSims, then the ParticleSims range destroyed and freed, then ~CBase.
static char g_log[16]; static int g_n = 0;
static void Log(char c) { g_log[g_n++] = c; g_log[g_n] = 0; }
struct SScalarTag { SScalarTag() {} };
struct CParticleSimulationPointer { void* m_pObject; void* m_pCount; };
static void _Destroy_range(CParticleSimulationPointer*, CParticleSimulationPointer*, const SScalarTag&) { Log('R'); }
struct SParticleSimArray {
    CParticleSimulationPointer* _M_start; CParticleSimulationPointer* _M_finish; CParticleSimulationPointer* _M_end_of_storage;
    ~SParticleSimArray() { _Destroy_range(_M_start, _M_finish, SScalarTag()); if (_M_start) { Log('F'); std::free(_M_start); } }
};
struct CBase { virtual void v00() {} void* m_04; void* m_08; ~CBase() { Log('B'); } };
struct CEngineClothSimulationMeshBase : CBase {
    SParticleSimArray ParticleSims;
    virtual void v00() {}
    void ClearParticleSims() { Log('C'); }
    ~CEngineClothSimulationMeshBase() { ClearParticleSims(); }
};
struct CEngineClothSimulationStaticMesh : CEngineClothSimulationMeshBase {
    virtual void v00() {}
    ~CEngineClothSimulationStaticMesh();
};
CEngineClothSimulationStaticMesh::~CEngineClothSimulationStaticMesh()
{
}
int main()
{
    { CEngineClothSimulationStaticMesh m; m.ParticleSims._M_start = (CParticleSimulationPointer*)std::malloc(16); m.ParticleSims._M_finish = m.ParticleSims._M_start; }
    if (std::strcmp(g_log, "CRFB") != 0) { std::printf("00b81e60_TEST FAIL\n"); return 1; }
    g_n = 0; g_log[0] = 0;
    { CEngineClothSimulationStaticMesh m; m.ParticleSims._M_start = 0; m.ParticleSims._M_finish = 0; }
    if (std::strcmp(g_log, "CRB") != 0) { std::printf("00b81e60_TEST FAIL\n"); return 1; }
    std::printf("00b81e60_TEST PASS\n");
    return 0;
}
