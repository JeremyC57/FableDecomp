// CEngineWorldMap::SetMapVisibilityAllMaps(bool visible) -- show or hide every engine map of the world map (each non-null map links itself into or out of the visible-map list)
// Leaf: ?SetMapVisibilityAllMaps@CEngineWorldMap@@QAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B423B0, length 64 bytes.
// Identity (NF160, 2026-11-20, queue q4593): Ego_r.pdb 0x009E2520 / FableWin.pdb 0x02D62820 ?SetMapVisibilityAllMaps@CEngineWorldMap@@QAEX_N@Z,
//   the tail-jump target of CEngine::SetMapVisibilityAllMaps 0x00B23E50 on the CEngineWorldMap component (which already binds this
//   spelling); next after SetMapVisibility 0x00B42390 in both PDBs. `this`+0x20 is MapList (CArray<CEngineMap*>), +0x2C VisibleMapList
//   (std::list<CEngineMap*>; engine/CEngineWorldMap.h); the callee 0x00B41DB0 is Ego_r.pdb 0x009E2440 / FableWin.pdb 0x02D5B370
//   ?SetMapVisibility@CEngineMap@@QAEXAAV?$list@PAVCEngineMap@@V?$allocator@PAVCEngineMap@@@std@@@std@@_N@Z. Was the invented
//   CMapOwner_00b423b0::SetMapVisibility over local CRetailVector / SMapRecord / CEngineMap models (the catalog label
//   CMeshDataBank::FreeDeviceSpecificResources was propagated).
// Shape: `this` in esi, the index in edi, the argument hoisted into ebx; the count is the unsigned element count (`jb`); `ret 4`.
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_VISIBLEMAPLIST
#define FABLE_RETAIL_CENGINEWORLDMAP_SETMAPVISIBILITYALLMAPS
#define FABLE_RETAIL_CENGINEMAP_SETMAPVISIBILITY
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CEngineWorldMap.h"
#include "engine/CEngineMap.h"

void CEngineWorldMap::SetMapVisibilityAllMaps(bool visible)
{
    for (unsigned long i = 0; i < MapList.size(); ++i) {
        if (MapList[i])
            MapList[i]->SetMapVisibility(VisibleMapList, visible);
    }
}
