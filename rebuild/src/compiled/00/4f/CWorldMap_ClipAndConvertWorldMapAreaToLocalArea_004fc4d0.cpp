// CWorldMap::ClipAndConvertWorldMapAreaToLocalArea(long map_number, const C2DBoxI& area) const -- clip a world-map box to one map's area and express it relative to that map's corner
// Leaf: ClipAndConvertWorldMapAreaToLocalArea
// Retail: Fable.exe @ 0x004FC4D0, 142 bytes, `ret 0Ch` (hidden C2DBoxI result). PDB
// ?ClipAndConvertWorldMapAreaToLocalArea@CWorldMap@@QBE?AVC2DBoxI@@JABV2@@Z
// Callee: C2DBoxI::ClipTo(const C2DBoxI&) 0x0049C810.
// Re-port (p25N, 2026-11-20): onto the shared engine/CWorldMap.h / CMapInfo.h / C2DBoxI.h for the same-TU copy in 0x004FEB70
//   CWorldMap::HeightAreaChanged. The old file's local C2DBoxI / CMapInfo / CWorldMap models and its `#pragma auto_inline(off)` round a
//   local ClipTo copy are gone: C2DBoxI.h's add-only opt-in FABLE_RETAIL_C2DBOXI_CLIPTO_BODY gives ClipTo's inline header body (Ego_d.pdb:
//   ClipTo's lines 97..102 are in a header, file id 2736, not CWorldMap.cpp's 4704), which c2 keeps out of line (the call to 0x0049C810)
//   and whose effects the caller then knows (writes only `this`: the map corner's TLX stays in edi across the call and both copies are
//   built with all loads first). FABLE_RETAIL_C2DBOXI_COPY: the memberwise user copy constructor (without it: 148 v 142).
// Text: Ego_d.pdb (CWorldMap.cpp, file id 4704, lines 1387..1402): locals map_number, area, map, clipped_area, clip_area; line sizes at /Od
//   1389..1401 = 12 / 12 / 12 / 11 / 11 / 12 / 12 / 15, this text the same (1388 `map`: 15 v ours 18, Dinkumware's operator[] call v
//   the CArray's inline one).
// Layout: engine/CWorldMap.h retail branch (Maps CArray<CMapInfo> +0x20); engine/CMapInfo.h (0x48 bytes, Area +0x00).
// Shape: the box and a copy of the map's area are frame locals (memberwise copies through registers); the result is
// the clipped box shifted in place by the map corner (TLX, BRX, then TLY, BRY) and returned through the copy ctor.
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
