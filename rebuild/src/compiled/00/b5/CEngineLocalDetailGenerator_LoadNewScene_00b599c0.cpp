// CEngineLocalDetailGenerator::LoadNewScene(const CEngineComponentLoadNewSceneParams& params) -- for every loaded engine map with a local-detail cache: force a full cache update, then render each cache once from the engine camera; reports false
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B599C0, length 101 bytes (112 with padding).
// Callees: CLocalDetailCacheMap::Update(double time) @ 0x00BDED90 (called with (double)1.0e30f = 1.0000000150474662e+30, the
// pool constant @ 0x012A1B28), CLocalDetailCacheMap::Render(CEngineCamera*, float) @ 0x00BDDD70 (with the engine camera
// @ 0x01436EA0 and 0.0f). Globals: the engine state @ 0x01436E8C (its loaded-map list at +0x2C).
// Shape: the list is a VC6-era std::list {head}: a pointer local to it (`add edi,2Ch`) gives begin = head->next and the end
// test re-reads head after every call; each node's value is a map whose +4 word is its detail cache (null-tested). The
// parameter is unused (`ret 4`), the result is `xor al,al`.
class CEngineCamera;
class CEngineComponentLoadNewSceneParams;

class CLocalDetailCacheMap { public:
    bool Update(double time);                      // 0x00BDED90
    void Render(CEngineCamera* camera, float alpha);   // 0x00BDDD70
};

class CEngineMap { public:
    void*                 LandscapeMap;             // +0x00
    CLocalDetailCacheMap* m_pLocalDetailCache;     // +0x04
};

struct SMapListNode {                              // VC6 std::list node
    SMapListNode* m_pNext;                         // +0x00
    SMapListNode* m_pPrev;                         // +0x04
    CEngineMap*   m_pMap;                          // +0x08
};

struct CMapList {
    SMapListNode* m_pHead;                         // +0x00 sentinel
    unsigned long m_size;                          // +0x04
};

struct SEngineState_01436e8c {
    unsigned char _pad[0x2c];
    CMapList      m_loadedMaps;                    // +0x2C
};
extern SEngineState_01436e8c* g_pEngineState_01436e8c;   // 0x01436E8C
extern CEngineCamera* g_pEngineCamera_01436ea0;          // 0x01436EA0

class CEngineLocalDetailGenerator { public:
    bool LoadNewScene(const CEngineComponentLoadNewSceneParams& params);
};

bool CEngineLocalDetailGenerator::LoadNewScene(const CEngineComponentLoadNewSceneParams& params)
{
    CMapList* maps = &g_pEngineState_01436e8c->m_loadedMaps;
    SMapListNode* node;
    for (node = maps->m_pHead->m_pNext; node != maps->m_pHead; node = node->m_pNext) {
        CLocalDetailCacheMap* cache = node->m_pMap->m_pLocalDetailCache;
        if (cache)
            cache->Update(1.0e30f);
    }
    for (node = maps->m_pHead->m_pNext; node != maps->m_pHead; node = node->m_pNext) {
        CLocalDetailCacheMap* cache = node->m_pMap->m_pLocalDetailCache;
        if (cache)
            cache->Render(g_pEngineCamera_01436ea0, 0.0f);
    }
    return false;
}