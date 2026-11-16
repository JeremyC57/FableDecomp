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

// NF46 (round 7): the 0x01436E8C global is NEngineInternal::GPEngineWorldMap (Ego_r.pdb / FableWin.pdb
// ?GPEngineWorldMap@NEngineInternal@@3PAVCEngineWorldMap@@A) and its +0x2C list the Ego_r.pdb member VisibleMapList
// (std::list<CEngineMap*>, engine/CEngineWorldMap.h overlay opt-in _VISIBLEMAPLIST); were the SEngineState_01436e8c /
// g_pEngineState_01436e8c, CMapList and SMapListNode stand-ins.
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_VISIBLEMAPLIST
#include "engine/CEngineWorldMap.h"
namespace NEngineInternal { extern CEngineWorldMap* GPEngineWorldMap; }   // 0x01436E8C
namespace NEngineInternal { extern CEngineCamera* GPCamera; }          // 0x01436EA0

class CEngineLocalDetailGenerator { public:
    bool LoadNewScene(const CEngineComponentLoadNewSceneParams& params);
};

bool CEngineLocalDetailGenerator::LoadNewScene(const CEngineComponentLoadNewSceneParams& params)
{
    std::list<CEngineMap*>* maps = &NEngineInternal::GPEngineWorldMap->VisibleMapList;
    std::list<CEngineMap*>::iterator it;
    for (it = maps->begin(); it != maps->end(); ++it) {
        CLocalDetailCacheMap* cache = (*it)->m_pLocalDetailCache;
        if (cache)
            cache->Update(1.0e30f);
    }
    for (it = maps->begin(); it != maps->end(); ++it) {
        CLocalDetailCacheMap* cache = (*it)->m_pLocalDetailCache;
        if (cache)
            cache->Render(NEngineInternal::GPCamera, 0.0f);
    }
    return false;
}