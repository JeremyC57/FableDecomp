// Behaviour test for CEngineLocalDetailGenerator::LoadNewScene @ 0x00B599C0.
#include <cstdio>
struct CEngineCamera { int id; };
static int g_seq = 0;
struct CLocalDetailCacheMap {
    int updated, rendered; double time; CEngineCamera* cam; float alpha;
    bool Update(double t) { updated = ++g_seq; time = t; return true; }
    void Render(CEngineCamera* c, float a) { rendered = ++g_seq; cam = c; alpha = a; }
};
struct CEngineMap { void* u; CLocalDetailCacheMap* m_pLocalDetailCache; };
struct SMapListNode { SMapListNode* m_pNext; SMapListNode* m_pPrev; CEngineMap* m_pMap; };
struct CMapList { SMapListNode* m_pHead; unsigned int m_size; };
struct SEngineState { CMapList m_loadedMaps; };
static SEngineState g_state; static SEngineState* g_pState = &g_state;
static CEngineCamera g_cam; static CEngineCamera* g_pCamera = &g_cam;
struct CEngineLocalDetailGenerator {
    bool LoadNewScene(int)
    {
        CMapList* maps = &g_pState->m_loadedMaps; SMapListNode* node;
        for (node = maps->m_pHead->m_pNext; node != maps->m_pHead; node = node->m_pNext) {
            CLocalDetailCacheMap* cache = node->m_pMap->m_pLocalDetailCache;
            if (cache) cache->Update(1.0e30f);
        }
        for (node = maps->m_pHead->m_pNext; node != maps->m_pHead; node = node->m_pNext) {
            CLocalDetailCacheMap* cache = node->m_pMap->m_pLocalDetailCache;
            if (cache) cache->Render(g_pCamera, 0.0f);
        }
        return false;
    }
};
int main()
{
    CLocalDetailCacheMap c1 = { 0 }, c3 = { 0 };
    CEngineMap m1 = { 0, &c1 }, m2 = { 0, 0 }, m3 = { 0, &c3 };
    SMapListNode head, n1, n2, n3;
    head.m_pNext = &n1; n1.m_pNext = &n2; n2.m_pNext = &n3; n3.m_pNext = &head;
    n1.m_pMap = &m1; n2.m_pMap = &m2; n3.m_pMap = &m3; head.m_pMap = 0;
    g_state.m_loadedMaps.m_pHead = &head;
    CEngineLocalDetailGenerator g;
    bool r = g.LoadNewScene(0);
    int ok = !r && c1.updated == 1 && c3.updated == 2 && c1.rendered == 3 && c3.rendered == 4 && c1.time == (double)1.0e30f
          && c1.cam == &g_cam && c3.alpha == 0.0f;
    head.m_pNext = &head; g_seq = 0;                       // empty list: nothing
    r = g.LoadNewScene(0);
    ok = ok && !r && g_seq == 0;
    if (!ok) { std::printf("00b599c0_TEST FAIL\n"); return 1; }
    std::printf("00b599c0_TEST PASS\n");
    return 0;
}