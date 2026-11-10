// CThingSearchTools::ClipAndConvertWorldMapAreaToLocalArea(const NThingMap::CThingMap& map, const C2DBoxI& area) const -- clip a world area to the map's area and return it relative to the map's top-left corner
// Leaf: ?ClipAndConvertWorldMapAreaToLocalArea@CThingSearchTools@@IBE?AVC2DBoxI@@ABVCThingMap@NThingMap@@ABV2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049C700, 107 bytes (`ret 0Ch`: hidden result + two arguments; `this` unused);
//   Ego_r.pdb ?ClipAndConvertWorldMapAreaToLocalArea@CThingSearchTools@@IBE?AVC2DBoxI@@ABVCThingMap@NThingMap@@ABV2@@Z (protected).
// Callees: C2DBoxI::ClipTo(const C2DBoxI&) 0x0049C810.
// Layout: shared NThingMap::CThingMap has retail Area at +0x20.
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_COPY
#include "engine/C2DBoxI.h"
#include "engine/CThingSearchTools.h"

#define FABLE_RETAIL_CTHINGMAP_GETAREA
#include "engine/NThingMap_CThingMap.h"

C2DBoxI CThingSearchTools::ClipAndConvertWorldMapAreaToLocalArea(const NThingMap::CThingMap& map, const C2DBoxI& area) const
{
    const C2DBoxI& clip_area = map.GetArea();
    C2DBoxI clipped_area(area);
    clipped_area.ClipTo(clip_area);
    clipped_area.TLX -= clip_area.TLX;
    clipped_area.BRX -= clip_area.TLX;
    clipped_area.TLY -= clip_area.TLY;
    clipped_area.BRY -= clip_area.TLY;
    return clipped_area;
}