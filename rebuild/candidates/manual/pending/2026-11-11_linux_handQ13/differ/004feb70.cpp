// CWorldMap::HeightAreaChanged(const C2DBoxI*) -- a height-map area changed (null = the whole world): tell the engine manager, tell every overlapping map in its local coordinates, rebuild the map edges, then refresh each overlapping map's navigation area
// Grade: DIFFER(ours v retail) -- DIFFER(260v261) measured 2026-11-11 with FABLE_HDR_OVERLAY=agentQ13/hdr (no overlay header used): register choice only: retail keeps the constant 0 in ebx (`xor ebx,ebx` at 0x004FEB92, rematerialised at 0x004FEC0B after loop 1, which borrows ebx for the map number) and the loop-2 iterator in ebp; ours puts the first zero in eax and the iterator in ebx. Not re-probed this pass (chunk 1013b already probed c2prio).
// Q13: regraded as it sits (chunk Q13), not reworked.
// Earlier grade (prior draft): DIFFER(260v261) -- register choice only: retail keeps the constant 0 in ebx (rematerialised `xor ebx,ebx` after the first loop, `cmp ebp,ebx` before the free) and the second-loop iterator in ebp; ours (c2prio) gives const 0 priority -12 against the iterator's 54, so the iterator takes ebx and the zero is an immediate (`test ebx,ebx`). Inert (free probes): vector declared first, unsigned map, const_iterators, a named map local in loop 2, one reused iterator Chunk 1013c (c2prio --blocks): const 0 gains +12 in the entry block and loses 24 over the blocks it lives through; it needs > 54 to take ebx before it2, which no reference-adding spelling reaches (== NULL tests, IsNull(), ternary area, named map in loop 2 all leave it at -12)
// Leaf: ?HeightAreaChanged@CWorldMap@@UAEXPBVC2DBoxI@@@Z
// Chunk 1013b: rebased onto the shared engine/CEngineManager.h / CTBaseSingleton.h (new in this overlay; the local singleton model is gone; Get stays a call to 0x00436FB0 here). Same residue: c2prio (free probes) shows it2 (priority 54) takes ebx because ebp costs it 200 while its neighbour `it` (48) has only ebp left; retail colours `it` first and keeps 0 in ebx (const 0 is -12 in ours). Using *it twice raised `it` to 72 but adds a reload; a single iterator, while loop, if-init, box local, continue, IsNull and == NULL are inert.
// Retail: Fable.exe (TLC, VC7.1) @ 0x004FEB70, 266 bytes, thiscall virtual (CWorldMap slot 28), `ret 4`, frame 0x2c.
// Identity: Ego_r.pdb ?HeightAreaChanged@CWorldMap@@UAEXPBVC2DBoxI@@@Z (0x004D2DA0) and FableWin.pdb (0x01C83FF0).
// Callees: CWorldMap virtual slot 13 GetDimensions() const (+0x34), CTBaseSingleton<CEngineManager>::Get 0x00436FB0 (__fastcall),
//   CEngineManager::AddChangedArea(const C2DBoxI&) 0x00640560, CWorldMap virtual slot 20 GetAllMapsInArea (+0x50),
//   CWorldMap::ClipAndConvertWorldMapAreaToLocalArea(long, const C2DBoxI&) const 0x004FC4D0, CMap::HeightAreaChanged 0x0081E810,
//   CWorldMap::UpdateAllEdges 0x0063DFD0, CNavigatorManager::UpdateNavigationMapArea(long, const C2DBoxF&) 0x00A14E40, free 0x00BFEA14.
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_HEIGHTAREA
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_NAVIGATORMANAGER
#define FABLE_RETAIL_CNAVIGATORMANAGER
#define FABLE_RETAIL_CNAVIGATORMANAGER_UPDATEAREA
#define FABLE_RETAIL_CMAP_HEIGHTAREACHANGED
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXI_TOC2DBOXF
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CMapInfo.h"
#include "engine/CMap.h"
#include "engine/CWorldMap.h"
#include "engine/CWorld.h"
#include "engine/CNavigatorManager.h"
#include "engine/CEngineManager.h"

void CWorldMap::HeightAreaChanged(const C2DBoxI* area)
{
    C2DBoxI whole_world = GetDimensions();
    std::vector<unsigned long> maps;
    if (!area)
        area = &whole_world;
    CEngineManager::Get().AddChangedArea(*area);
    GetAllMapsInArea(*area, maps);
    for (std::vector<unsigned long>::iterator it = maps.begin(); it < maps.end(); ++it)
    {
        long map = *it;
        C2DBoxI local_area = ClipAndConvertWorldMapAreaToLocalArea(map, *area);
        CMap* pmap = Maps[map].PMap.Get();
        if (pmap)
            pmap->HeightAreaChanged(&local_area);
    }
    UpdateAllEdges();
    for (std::vector<unsigned long>::iterator it2 = maps.begin(); it2 < maps.end(); ++it2)
        World.GetNavigatorManager().UpdateNavigationMapArea(*it2, area->ToC2DBoxF());
}
