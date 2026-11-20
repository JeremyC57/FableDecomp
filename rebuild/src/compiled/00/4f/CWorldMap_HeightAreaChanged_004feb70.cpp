// CWorldMap::HeightAreaChanged(const C2DBoxI* pworld_box) -- a height-map area changed (null = the whole world): tell the engine manager, tell every overlapping map in its local coordinates, rebuild the map edges, then refresh each overlapping map's navigation area
// Leaf: ?HeightAreaChanged@CWorldMap@@UAEXPBVC2DBoxI@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004FEB70, 261 bytes, thiscall virtual (CWorldMap slot 28), `ret 4`, frame 0x2c.
//   Grade: RELOCATION_MATCH (261/261). No flag, no pragma, no Inferred line.
// Identity: Ego_r.pdb ?HeightAreaChanged@CWorldMap@@UAEXPBVC2DBoxI@@@Z (0x004D2DA0) and FableWin.pdb (0x01C83FF0).
// Callees: CWorldMap virtual slot 13 GetDimensions() const (+0x34), CTBaseSingleton<CEngineManager>::Get 0x00436FB0 (__fastcall),
//   CEngineManager::AddChangedArea(const C2DBoxI&) 0x00640560, CWorldMap virtual slot 20 GetAllMapsInArea (+0x50),
//   CWorldMap::ClipAndConvertWorldMapAreaToLocalArea(long, const C2DBoxI&) const 0x004FC4D0 (same TU, below), CMap::HeightAreaChanged
//   0x0081E810, CWorldMap::UpdateAllEdges 0x0063DFD0, CNavigatorManager::UpdateNavigationMapArea(long, const C2DBoxF&) 0x00A14E40,
//   free 0x00BFEA14.
// The old residue (DIFFER(260v261), drafts with `long map` / `CMap* pmap` locals; the Ego_d-faithful text 299v261): retail loads *it once
//   into ebx and still uses it after the ClipAndConvertWorldMapAreaToLocalArea call. VC7.1 does that only when it knows the call cannot
//   write the vector's buffer: ClipAndConvert is this file's TU-mate (Ego_d: CWorldMap.cpp, lines 1387..1402, before this function's
//   1524), and its one callee C2DBoxI::ClipTo is an inline header body (Ego_d: file id 2736, lines 97..102; out of line in retail as the
//   COMDAT 0x0049C810). With both bodies visible the row is RELOCATION_MATCH; with ClipAndConvert declaration-only, or ClipTo
//   declaration-only, 265v261 (evidence/ablation.txt).
// Text: Ego_d.pdb locals this, pworld_box, map_list (std::vector<unsigned long>), world_box, it (one vector iterator, both loops)
//   { local_box } { map_box, area_changed }. Line sizes at /Od: loop 1 body 1540 / 1542 / 1544 = 30 / 39 / 46, ours 22 / 31 / 38; loop 2
//   1557 / 1558 = 47 / 36, ours 47 / 28 -- every difference is Dinkumware's out-of-line iterator operator* (8 bytes per *it). area_changed
//   is the four-line C2DBoxF constructor (Ego_d records it at 1557, the statement's last line; ToC2DBoxF() is 12 bytes).
//   map_box (C2DBoxI, Ego_d block 2) is not written: it is dead in release (retail converts *pworld_box directly) and nothing shows its
//   initialiser.
// Layout: engine/CWorldMap.h retail branch (Maps +0x20, CMapInfo 0x48 bytes with PMap +0x10; World +0x08); CWorld::GetNavigatorManager
//   (+0x48).
// Define block: the CWorldMap.cpp rows' common block (0x004FEB70 HeightAreaChanged and its TU-mate 0x004FC4D0), so that the same-TU copy
//   compiles from the same text.
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_CLIPTO_BODY
#define FABLE_RETAIL_CWORLDMAP_HEIGHTAREA
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_NAVIGATORMANAGER
#define FABLE_RETAIL_CNAVIGATORMANAGER
#define FABLE_RETAIL_CNAVIGATORMANAGER_UPDATEAREA
#define FABLE_RETAIL_CMAP_HEIGHTAREACHANGED
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXI_TOC2DBOXF
#include "fable_string.h"
#include "engine/CMapInfo.h"
#include "engine/CMap.h"
#include "engine/CWorldMap.h"
#include "engine/CWorld.h"
#include "engine/CNavigatorManager.h"
#include "engine/CEngineManager.h"

// Fidelity: same-TU strong definition of 0x004FC4D0 (retail TU-mate)
C2DBoxI CWorldMap::ClipAndConvertWorldMapAreaToLocalArea(long map_number, const C2DBoxI& area) const
{
    const CMapInfo& map = Maps[map_number];
    C2DBoxI clipped_area(area);
    C2DBoxI clip_area(map.Area);
    clipped_area.ClipTo(clip_area);
    clipped_area.TLX -= map.Area.TLX;
    clipped_area.BRX -= map.Area.TLX;
    clipped_area.TLY -= map.Area.TLY;
    clipped_area.BRY -= map.Area.TLY;
    return clipped_area;
}

void CWorldMap::HeightAreaChanged(const C2DBoxI* pworld_box)
{
    C2DBoxI world_box = GetDimensions();
    std::vector<unsigned long> map_list;
    std::vector<unsigned long>::iterator it;
    if (!pworld_box)
        pworld_box = &world_box;
    CEngineManager::Get().AddChangedArea(*pworld_box);
    GetAllMapsInArea(*pworld_box, map_list);
    for (it = map_list.begin(); it < map_list.end(); ++it)
    {
        C2DBoxI local_box = ClipAndConvertWorldMapAreaToLocalArea(*it, *pworld_box);
        if (Maps[*it].PMap.Get())
            Maps[*it].PMap->HeightAreaChanged(&local_box);
    }
    UpdateAllEdges();
    for (it = map_list.begin(); it < map_list.end(); ++it)
    {
        C2DBoxF area_changed((float)pworld_box->TLX,
                             (float)pworld_box->TLY,
                             (float)pworld_box->BRX,
                             (float)pworld_box->BRY);
        World.GetNavigatorManager().UpdateNavigationMapArea(*it, area_changed);
    }
}