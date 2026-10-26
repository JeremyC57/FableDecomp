#include <cstdio>
#include <cstring>
static int g_clear = 0, g_close = 0, g_destroyed = 0;
struct CEngineScene_00bdc4f0 { void Clear() { ++g_clear; } };
class CLocalDetailCacheMap { public: void CloseStaticMap() { ++g_close; } };
struct CCountedItem_00b3ef40 { void Destroy() { ++g_destroyed; } };
struct SCountedBlock_00b3ef40 { long m_count; void (CCountedItem_00b3ef40::*m_pfnDestroy)(); CCountedItem_00b3ef40* m_pItem; };
class CEngineMap { public: CEngineScene_00bdc4f0* m_pScene; CLocalDetailCacheMap* m_pDetailCache; unsigned char _pad08[0x18]; void* m_pStaticMap; unsigned char _pad24[0x18]; void* m_pStaticMapItem; SCountedBlock_00b3ef40* m_pStaticMapBlock;
    void CloseStaticMap() { if (m_pStaticMap) { if (m_pScene) m_pScene->Clear(); if (m_pDetailCache) m_pDetailCache->CloseStaticMap();
        if (m_pStaticMapBlock) { --m_pStaticMapBlock->m_count; if (m_pStaticMapBlock->m_count == 0) { (m_pStaticMapBlock->m_pItem->*m_pStaticMapBlock->m_pfnDestroy)(); delete m_pStaticMapBlock; } }
        m_pStaticMapBlock = 0; m_pStaticMapItem = 0; m_pStaticMap = 0; } } };
int main() {
    CEngineMap m; std::memset(&m, 0, sizeof m); CEngineScene_00bdc4f0 s; CLocalDetailCacheMap d; SCountedBlock_00b3ef40* b = new SCountedBlock_00b3ef40; CCountedItem_00b3ef40 item;
    b->m_count = 1; b->m_pfnDestroy = &CCountedItem_00b3ef40::Destroy; b->m_pItem = &item;
    m.m_pScene = &s; m.m_pDetailCache = &d; m.m_pStaticMap = &m; m.m_pStaticMapItem = &item; m.m_pStaticMapBlock = b;
    m.CloseStaticMap(); if (g_clear != 1 || g_close != 1 || g_destroyed != 1 || m.m_pStaticMapBlock || m.m_pStaticMapItem || m.m_pStaticMap) { std::printf("00b3ef40_TEST FAIL\n"); return 1; }
    m.CloseStaticMap(); if (g_clear != 1) { std::printf("00b3ef40_TEST FAIL\n"); return 1; }
    SCountedBlock_00b3ef40* b2 = new SCountedBlock_00b3ef40; b2->m_count = 2; b2->m_pfnDestroy = &CCountedItem_00b3ef40::Destroy; b2->m_pItem = &item; m.m_pStaticMap = &m; m.m_pStaticMapBlock = b2; m.m_pScene = 0;
    m.CloseStaticMap(); if (g_destroyed != 1 || b2->m_count != 1 || g_close != 2) { std::printf("00b3ef40_TEST FAIL\n"); return 1; }
    delete b2;
    std::printf("00b3ef40_TEST PASS\n"); return 0;
}