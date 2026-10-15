#include <cstdio>
#include <cstdlib>
// Behaviour test for NParticleEngine::CPSCUpdateNormal scalar deleting destructor (0x00B0F990):
// KillAllDecalEmitters, the decal array (teardown + free), the +0x14 string, then the CBase root.
static int g_order[8]; static int g_n = 0; static int g_freed = 0;
struct CCharString { void* m_pData; ~CCharString() { g_order[g_n++] = 2; } };
struct CBase { virtual ~CBase() { g_order[g_n++] = 1; } };
struct CDecalEmitterArray {
    void* m_pData; long m_size; long m_capacity;
    void DestroyElements() { g_order[g_n++] = 3; }
    ~CDecalEmitterArray() { DestroyElements(); if (m_pData) { std::free(m_pData); ++g_freed; } }
};
struct CPSCUpdateNormal : CBase {
    CCharString m_name; CDecalEmitterArray m_decalEmitters;
    void KillAllDecalEmitters() { g_order[g_n++] = 4; }
    virtual ~CPSCUpdateNormal() { KillAllDecalEmitters(); }
};
int main()
{
    CPSCUpdateNormal* u = new CPSCUpdateNormal;
    u->m_decalEmitters.m_pData = std::malloc(4);
    CBase* p = u;
    delete p;
    if (g_n != 4 || g_order[0] != 4 || g_order[1] != 3 || g_order[2] != 2 || g_order[3] != 1 || g_freed != 1) { std::printf("00b0f990_TEST FAIL\n"); return 1; }
    std::printf("00b0f990_TEST PASS\n");
    return 0;
}