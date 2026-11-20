// CEngineWorldMap::SetMapVisibility(long index, bool visible) -- show or hide the engine map `index`: it links itself into (or out of) the world map's visible-map list
// Leaf: ?SetMapVisibility@CEngineWorldMap@@QAEXJ_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B42390, length 25 bytes.
// Identity (NF160, 2026-11-20, queue q4592): Ego_r.pdb 0x009E2500 / FableWin.pdb 0x02D625D0 ?SetMapVisibility@CEngineWorldMap@@QAEXJ_N@Z,
//   the tail-jump target of CEngine::SetMapVisibility 0x00B23E40 on the CEngineWorldMap component (which already binds this spelling).
//   `this`+0x20 is MapList (CArray<CEngineMap*>), +0x2C VisibleMapList (std::list<CEngineMap*>, the 4-byte retail STLport list;
//   engine/CEngineWorldMap.h); the callee 0x00B41DB0 is Ego_r.pdb 0x009E2440 / FableWin.pdb 0x02D5B370
//   ?SetMapVisibility@CEngineMap@@QAEXAAV?$list@PAVCEngineMap@@V?$allocator@PAVCEngineMap@@@std@@@std@@_N@Z. Was the stand-in
//   CEngineMapManager::SetMapVisibility over local SMapState / CRetailMapList models.
// Shape: the bool is pushed first (right to left), the list address second; `ret 8`.
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_VISIBLEMAPLIST
#define FABLE_RETAIL_CENGINEWORLDMAP_SETMAPVISIBILITY
#define FABLE_RETAIL_CENGINEMAP_SETMAPVISIBILITY
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CEngineWorldMap.h"
#include "engine/CEngineMap.h"

void CEngineWorldMap::SetMapVisibility(long index, bool visible)
{
    MapList[index]->SetMapVisibility(VisibleMapList, visible);
}
