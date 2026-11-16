// CEngineMapManager::MapState_SetLoaded(long index) -- fetch the map's info record (id + name) from the world-map provider for `index`, store its id in the map's state and run the map's loaded hook; the record's name is destroyed afterwards
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B401E0, length 67 bytes.
// Callees: virtual slot 1 of the provider at +0x19c of the engine state @ 0x01436E8C (hidden-pointer result, the index), the map's loaded hook @ 0x00BDBFE0
// (on the map object at +0 of the state entry), ~CCharString @ 0x0099EAE0 (on the record's name at +4: the record's inline destructor). Shape: the map
// state table at +0x20 holds pointers to 12-byte entries {map, ?, id}; the 0x20-byte frame holds the returned record (its id read from +4, its name destroyed at +8);
// the chunk label "CEngine::MapState_SetLoaded" names the method; `this` is the map manager (pointer table at +0x20).
// NF46 (round 7): the 0x01436E8C stand-in SEngineState_01436e8c / g_pEngineState_01436e8c is NEngineInternal::GPEngineWorldMap
//   (Ego_r.pdb / FableWin.pdb ?GPEngineWorldMap@NEngineInternal@@3PAVCEngineWorldMap@@A) on engine/CEngineWorldMap.h; +0x19c is its GameWorldMap
//   (CIDrawWorldMap*, FableWin.pdb inline PeekGameWorldMap; overlay opt-in _GAMEWORLDMAP) and the provider's slot 1 is
//   CIDrawWorldMap::DrawGetMapInfo(long) const returning the 0x20-byte CDrawMapInfo (engine/CIDrawWorldMap.h, engine/CDrawMapInfo.h): the id
//   read from +4 is its PMap (const CIDrawMap*), the name destroyed at +8 its StaticMeshName. The local CCharString / SMapInfo / CWorldMapProvider
//   moved to fable_string.h and those headers; the state entry's +8 field (local model) now holds that map pointer.
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_GAMEWORLD
#define FABLE_RETAIL_CENGINEWORLDMAP_GAMEWORLDMAP
#define FABLE_RETAIL_CDRAWMAPINFO_PASS23
#include "fable_string.h"
#include "engine/CDrawMapInfo.h"
#include "engine/CIDrawWorldMap.h"
#include "engine/CEngineWorldMap.h"
namespace NEngineInternal { extern CEngineWorldMap* GPEngineWorldMap; }   // 0x01436E8C

class CEngineMap { public:
    void OnLoaded();                               // 0x00BDBFE0
};

struct SMapState {
    CEngineMap*      m_pMap;                       // +0x00
    long             m_a;                          // +0x04
    const CIDrawMap* m_pDrawMap;                   // +0x08
};

struct CEngineMapManager {
    unsigned char _pad[0x20];
    SMapState**   m_ppStates;                      // +0x20
    void MapState_SetLoaded(long index);
};

void CEngineMapManager::MapState_SetLoaded(long index)
{
    SMapState* state = m_ppStates[index];
    CDrawMapInfo info = NEngineInternal::GPEngineWorldMap->PeekGameWorldMap()->DrawGetMapInfo(index);
    state->m_pDrawMap = info.PMap;
    state->m_pMap->OnLoaded();
}
