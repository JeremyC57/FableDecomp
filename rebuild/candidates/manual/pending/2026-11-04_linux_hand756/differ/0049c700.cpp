// CThingSearchTools::ClipAndConvertWorldMapAreaToLocalArea(const NThingMap::CThingMap& map, const C2DBoxI& area) const -- clip a world area to the map's area and return it relative to the map's top-left corner
// Grade: DIFFER(ours v retail) -- DIFFER(107v107) (try n): one store of the local copy (BRX, [esp+14h]) is scheduled before 'add esi,20h' (retail after it); everything else matches. Tried 25 spellings: Initialise/4-arg ctor/in-place shifts, X-first order, copy-ctor forms, same-TU ClipTo body, map-area reference/pointer locals.
// Leaf: ?ClipAndConvertWorldMapAreaToLocalArea@CThingSearchTools@@IBE?AVC2DBoxI@@ABVCThingMap@NThingMap@@ABV2@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049C700, 107 bytes (`ret 0Ch`: hidden result + two arguments; `this` unused);
//   Ego_r.pdb ?ClipAndConvertWorldMapAreaToLocalArea@CThingSearchTools@@IBE?AVC2DBoxI@@ABVCThingMap@NThingMap@@ABV2@@Z (protected).
// Callees: C2DBoxI::ClipTo(const C2DBoxI&) 0x0049C810.
// Layout: NThingMap::CThingMap::Area (C2DBoxI) at retail +0x20 (Ego_r.pdb +0x28; the retail STLport vectors are 12 bytes), as the
//   landed CThingSearchTools::GetAllThingsInRoughArea 0x0075BC40 models it (no shared CThingMap header).
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_COPY
#include "engine/C2DBoxI.h"
#include "engine/CThingSearchTools.h"

class CThingMapManager;
namespace NThingMap {
class C2DExtentsI { public: long X, Y; };
class CMapwhoLevelInfo { public: long Size; C2DExtentsI Extents; long ScaleShift; long NoThings; };
struct CThingMapCell { void* PFirstNode; };
class CThingMap { public:
    long _padding_;                                                // +0x00
    std::vector<CMapwhoLevelInfo> MapwhoLevelInfo;                 // +0x04
    CThingMapManager* Manager;                                     // +0x10
    std::vector<std::vector<CThingMapCell> > Cells;                // +0x14
    C2DBoxI Area;                                                  // +0x20
    long NoThings;                                                 // +0x30
};
}

C2DBoxI CThingSearchTools::ClipAndConvertWorldMapAreaToLocalArea(const NThingMap::CThingMap& map, const C2DBoxI& area) const
{
    C2DBoxI local_area = area;
    local_area.ClipTo(map.Area);
    C2DCoordI top_left = map.Area.GetTLPos();
    local_area.TLX -= top_left.X;
    local_area.BRX -= top_left.X;
    local_area.TLY -= top_left.Y;
    local_area.BRY -= top_left.Y;
    return local_area;
}
