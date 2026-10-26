#include <stdio.h>
class CMovableResource { public: int x; };
static long g_size; static CMovableResource* g_owner; static const void* g_mgr;
static char g_block[64];
class CEngineLandscapeMemoryManager { public:
    void* AllocVirtual(long size, CMovableResource* owner) { g_size = size; g_owner = owner; g_mgr = this; return g_block; }
};
struct SEngine_01436ea8 {
    unsigned char                  _pad[0x6b0];
    CEngineLandscapeMemoryManager* m_pLandscapeMemory;
};
static SEngine_01436ea8 g_engine;
SEngine_01436ea8* g_pEngine_01436ea8 = &g_engine;
class CEngineLandscapePatch {
public:
    static void* __fastcall operator new[](unsigned int size, CMovableResource* owner);
};

void* __fastcall CEngineLandscapePatch::operator new[](unsigned int size, CMovableResource* owner)
{
    CEngineLandscapeMemoryManager* manager = g_pEngine_01436ea8->m_pLandscapeMemory;
    return manager->AllocVirtual(size, owner);
}

int main()
{
    CEngineLandscapeMemoryManager m; CMovableResource r;
    g_engine.m_pLandscapeMemory = &m;
    void* p = CEngineLandscapePatch::operator new[](0x11c, &r);
    bool ok = p == g_block && g_size == 0x11c && g_owner == &r && g_mgr == &m;
    p = CEngineLandscapePatch::operator new[](4, 0);
    ok = ok && g_size == 4 && g_owner == 0;
    printf(ok ? "00bf36c0_TEST PASS\n" : "00bf36c0_TEST FAIL\n");
    return ok ? 0 : 1;
}