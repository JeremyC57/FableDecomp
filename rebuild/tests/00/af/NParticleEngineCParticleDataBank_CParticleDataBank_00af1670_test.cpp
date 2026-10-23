// behaviour test for 0x00AF1670 CParticleDataBank::CParticleDataBank: bases, empty emitter array, symbol maps, resource list reset + 64 KB budget
#include <cstdio>
static int g_order[8]; static int g_n = 0;
namespace N_009d5f80 { struct CBankFile { CBankFile() { g_order[g_n++] = 1; } }; } using N_009d5f80::CBankFile;
struct CResourceList { unsigned int m_maxSize; unsigned int m_usedSize; CResourceList() : m_maxSize(1), m_usedSize(5) {}
    void SetMaxSize(unsigned int s) { m_maxSize = s; g_order[g_n++] = 4; } };
struct CResourceBank { CResourceList ResourceList, LoadingQueue; CResourceBank() { g_order[g_n++] = 2; } };
struct CSymbolMap { CSymbolMap() { g_order[g_n++] = 3; } };
template <class T> struct CArray { T* a; T* b; T* c; CArray() : a(0), b(0), c(0) {} };
struct CParticleDataBank : CBankFile, CResourceBank {
    CArray<int> ParticleEmitters; CSymbolMap GraphicSymbols, MeshSymbols;
    CParticleDataBank() { ResourceList.m_usedSize = 0; ResourceList.SetMaxSize(0x10000); }
};
int main()
{
    CParticleDataBank b;
    if (g_n != 5 || g_order[0] != 1 || g_order[1] != 2 || g_order[2] != 3 || g_order[3] != 3 || g_order[4] != 4 ||
        b.ResourceList.m_usedSize != 0 || b.ResourceList.m_maxSize != 0x10000 || b.ParticleEmitters.a != 0 || b.ParticleEmitters.c != 0)
    { std::printf("00af1670_TEST FAIL\n"); return 1; }
    std::printf("00af1670_TEST PASS\n");
    return 0;
}