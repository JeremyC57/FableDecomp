// NThingMap::CThingMapManager::GetThingMapAt(const C2DCoordI& pos) const -- first active thing map whose area contains the cell position, else 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00637A80, length 77 bytes.
// No callees. Layout (the landed 0x00637AE0 GetMapsMemoryFootprint model): the retail
// ActiveThingMaps vector's {first, last} sit at +0x14/+0x18 (the engine header's PDB CArray at
// +0x18 is 4 bytes high: retail containers drop the editor word), each entry points at a
// CThingMapInfo whose first member is the CThingMap; the map's area (C2DBoxI, inclusive top-left,
// exclusive bottom-right) is at +0x20. The local model stands in for engine/CThingMapManager.h
// for that reason.
// Shape: `last` and pos.x are loop-invariant (no calls, no stores) and live in ebx/edi; the area
// test is an inline member on the box (`add eax,20h` materialises its `this`), pos.y re-read per
// test; the entry is a named local (esi) and the hit re-reads info->PMap through it.
struct C2DCoordI {
    long X;
    long Y;
};
struct C2DBoxI {
    long X0, Y0, X1, Y1;
    bool Contains(const C2DCoordI& pos) const
    {
        return pos.X >= X0 && pos.X < X1 && pos.Y >= Y0 && pos.Y < Y1;
    }
};
namespace NThingMap {
struct CThingMap {
    unsigned char _pad00[0x20];
    C2DBoxI       Area;                         // +0x20
};
struct CThingMapManager {
    struct CThingMapInfo {
        CThingMap* PMap;                        // +0x00
    };
    void*           __vftable;                  // +0x00
    void*           PWorld;                     // +0x04
    unsigned char   ThingMaps[12];              // +0x08 (retail 12-byte container)
    CThingMapInfo** ActiveThingMaps_First;      // +0x14
    CThingMapInfo** ActiveThingMaps_Last;       // +0x18
    CThingMap* GetThingMapAt(const C2DCoordI& pos) const;
};

CThingMap* CThingMapManager::GetThingMapAt(const C2DCoordI& pos) const
{
    for (CThingMapInfo* const* it = ActiveThingMaps_First; it != ActiveThingMaps_Last; ++it) {
        const CThingMapInfo* info = *it;
        if (info->PMap->Area.Contains(pos))
            return info->PMap;
    }
    return 0;
}
}
