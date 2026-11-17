// CEngineWorldMap::OnGameMapLoaded(long index) -- the game map `index` finished loading: its engine map takes the game map pointer from the world-map provider's info record and tells its landscape map
// Leaf: ?OnGameMapLoaded@CEngineWorldMap@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B401E0, length 67 bytes.
// Identity (NF56, round 7, queue q10420): Ego_r.pdb 0x009E0560 / FableWin.pdb 0x02D615D0 ?OnGameMapLoaded@CEngineWorldMap@@QAEXJ@Z, the
//   tail-jump target of CEngine::SetMapAsLoaded 0x00B23E30 on the CEngineWorldMap component (the catalog leaf already says so). `this`+0x20
//   is MapList (CArray<CEngineMap*>, engine/CEngineWorldMap.h); the entry's +0x08 store is CEngineMap::GameMap through Ego_r/FableWin
//   ?SetGameMap@CEngineMap@@QAEXPBVCIDrawMap@@@Z and the +0x00 call 0x00BDBFE0 is LandscapeMap->OnGameMapLoaded() (Ego_r.pdb 0x009B4720 /
//   FableWin.pdb 0x02CC75C0 ?OnGameMapLoaded@CEngineLandscapeMap@@QAEXXZ): the body of CEngineMap::OnGameMapLoaded(long) (Ego_r 0x009DD5D0,
//   expanded here). The record is the provider's slot-1 CIDrawWorldMap::DrawGetMapInfo(long) const (0x20-byte CDrawMapInfo by hidden
//   pointer; PMap read from +4, its StaticMeshName destroyed with ~CCharString 0x0099EAE0) through NEngineInternal::GPEngineWorldMap
//   (0x01436E8C) ->PeekGameWorldMap() (+0x19C). Was the stand-in CEngineMapManager::MapState_SetLoaded over local CEngineMap / SMapState
//   models. Shape: the record is a named local (PMap read back from its frame slot, destroyed after the landscape call).
//   CEngineLandscapeMap has no shared header: the one-member declaration below.
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_GAMEWORLD
#define FABLE_RETAIL_CENGINEWORLDMAP_GAMEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_ONGAMEMAPLOADED
#define FABLE_RETAIL_CENGINEMAP_ONGAMEMAPLOADED
#define FABLE_RETAIL_CDRAWMAPINFO_PASS23
#include "fable_string.h"
#include "engine/CDrawMapInfo.h"
#include "engine/CIDrawWorldMap.h"
#include "engine/CEngineWorldMap.h"
#include "engine/CEngineMap.h"

namespace NEngineInternal { extern CEngineWorldMap* GPEngineWorldMap; }   // 0x01436E8C

class CEngineLandscapeMap { public:
    void OnGameMapLoaded();                        // 0x00BDBFE0
};

inline void CEngineMap::OnGameMapLoaded(long index)
{
    CDrawMapInfo info = NEngineInternal::GPEngineWorldMap->PeekGameWorldMap()->DrawGetMapInfo(index);
    SetGameMap(info.PMap);
    LandscapeMap->OnGameMapLoaded();
}

void CEngineWorldMap::OnGameMapLoaded(long index)
{
    MapList[index]->OnGameMapLoaded(index);
}
