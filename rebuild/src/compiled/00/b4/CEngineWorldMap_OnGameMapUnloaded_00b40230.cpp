// CEngineWorldMap::OnGameMapUnloaded(long index) -- the game map `index` was unloaded: its engine map tells its landscape map and forgets the game map pointer
// Leaf: ?OnGameMapUnloaded@CEngineWorldMap@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B40230, length 33 bytes.
// Identity (NF160, 2026-11-20, queue q4591): Ego_r.pdb 0x009E05B0 / FableWin.pdb 0x02D61810 ?OnGameMapUnloaded@CEngineWorldMap@@QAEXJ@Z,
//   the tail-jump target of CEngine::SetMapAsUnloaded 0x00B23E60 on the CEngineWorldMap component (which already binds this spelling);
//   the unloaded twin of the landed OnGameMapLoaded 0x00B401E0 (next in both PDBs). `this`+0x20 is MapList (CArray<CEngineMap*>,
//   engine/CEngineWorldMap.h); the body is CEngineMap::OnGameMapUnloaded() (Ego_r.pdb 0x009DD2B0 / FableWin.pdb 0x02D5C630, expanded here):
//   LandscapeMap (+0x00) tested and told through ?OnGameMapUnloaded@CEngineLandscapeMap@@QAEXXZ (Ego_r 0x009B4730 / FableWin 0x02CC75D0,
//   retail 0x00BDBFF0, a bare `ret` after OnGameMapLoaded 0x00BDBFE0), then GameMap (+0x08) cleared. Was the stand-in
//   CEngineMapManager::MapState_SetUnloaded over local CEngineMap / SMapState models.
// Shape: the map pointer stays in esi across the call; the GameMap store is an immediate zero.
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_ONGAMEMAPUNLOADED
#define FABLE_RETAIL_CENGINEMAP_ONGAMEMAPUNLOADED
#define FABLE_RETAIL_CENGINELANDSCAPEMAP_ONGAMEMAPUNLOADED
#include "fable_string.h"
#include "engine/CEngineWorldMap.h"
#include "engine/CEngineMap.h"
#include "engine/_quarantine/CEngineLandscapeMap.h"

inline void CEngineMap::OnGameMapUnloaded()
{
    if (LandscapeMap)
        LandscapeMap->OnGameMapUnloaded();
    GameMap = 0;
}

void CEngineWorldMap::OnGameMapUnloaded(long index)
{
    MapList[index]->OnGameMapUnloaded();
}
