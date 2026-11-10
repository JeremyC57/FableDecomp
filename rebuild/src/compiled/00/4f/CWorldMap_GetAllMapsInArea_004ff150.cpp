// CWorldMap::GetAllMapsInArea(const C2DBoxI&, vector<unsigned long>&) const -- clamp the box to the world and refill `maps` with the map numbers it touches: the one map from the placement grid when the box lies in one level, else every used map whose area overlaps it
// Leaf: ?GetAllMapsInArea@CWorldMap@@UBEXABVC2DBoxI@@AAV?$vector@KV?$allocator@K@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004FF150, 434 bytes, thiscall virtual (CWorldMap slot 20, +0x50), `ret 8`, frame 0x10.
// Identity: Ego_r.pdb / FableWin.pdb ?GetAllMapsInArea@CWorldMap@@UBEXABVC2DBoxI@@AAV?$vector@KV?$allocator@K@std@@@std@@@Z.
// Callees: memmove 0x00BFEAE6 (STLport's inline clear()), CWorldMap::AreaCoversOneLevel 0x004FB160, C2DBoxI::Overlapping
//   0x004FAB40, vector<unsigned long>::_M_insert_overflow 0x00464ACE (push_back's overflow path).
// Layout: engine/CWorldMap.h (Maps +0x20, GranularityShift +0x54, MapPlacementPitch +0x58, Area +0x5C, MapPlacement +0x6C).
// Shape: C2DBoxI's user copy constructor (FableWin.pdb ??0C2DBoxI@@QAE@ABV0@@Z, FABLE_RETAIL_C2DBOXI_COPY) gives retail's load order
//   for the box copy (max in ecx, BRX in edx, BRY in eax); C2DBoxI::Overlapping's body (0x004FAB40, in the CWorldMap.cpp object) and
//   CWorldMap::AreaCoversOneLevel's (0x004FB160, same TU) visible and out of line keep the clamped TLX / TLY in ebx / edi across AreaCoversOneLevel; a signed `long i` index gives the
//   strength-reduced `Maps[i]` offset in ebx (0x004FF285 `mov ebx,48h`).
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_AREACOVERSONELEVEL_BODY
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_OVERLAPPING_BODY
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CMapInfo.h"
#include "engine/CWorldMap.h"

void CWorldMap::GetAllMapsInArea(const C2DBoxI& area, std::vector<unsigned long>& maps) const
{
    C2DBoxI box = area;
    long max_x = Area.BRX - 1;
    if (box.TLX < 0) box.TLX = 0; else if (box.TLX > max_x) box.TLX = max_x;
    if (box.BRX < 0) box.BRX = 0; else if (box.BRX > max_x) box.BRX = max_x;
    long max_y = Area.BRY - 1;
    if (box.TLY < 0) box.TLY = 0; else if (box.TLY > max_y) box.TLY = max_y;
    if (box.BRY < 0) box.BRY = 0; else if (box.BRY > max_y) box.BRY = max_y;
    maps.clear();
    if (AreaCoversOneLevel(box))
    {
        maps.push_back(MapPlacement[(box.TLY >> GranularityShift) * MapPlacementPitch + (box.TLX >> GranularityShift)]);
        return;
    }
    for (long i = 1; i < Maps.size(); ++i)
        if (Maps[i].Used && Maps[i].Area.Overlapping(box))
            maps.push_back(i);
}