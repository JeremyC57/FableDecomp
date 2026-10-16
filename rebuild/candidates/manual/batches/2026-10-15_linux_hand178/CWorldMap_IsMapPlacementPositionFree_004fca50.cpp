// CWorldMap::IsMapPlacementPositionFree(const C2DBoxI&) -- are all placement-grid cells under a world box still unassigned (0)?
// Retail: Fable.exe @ 0x004FCA50, 109 bytes, `ret 4`. PDB ?IsMapPlacementPositionFree@CWorldMap@@IAE_NABVC2DBoxI@@@Z (protected). No callees.
// Retail CWorldMap (the engine header engine/CWorldMap.h is the PDB layout with 16-byte containers; retail STLport
// vectors / CArrays are 12 bytes and the map member is 12 bytes, so everything after +0x14 sits lower):
//   +0x20 Maps (CArray<CMapInfo>: start +0x20, finish +0x24; PDB +0x24), +0x2C Regions (vector<CRegion>, 0x58-byte
//   elements; PDB +0x34), +0x38 RegionTime, +0x44 PRegionGraph, +0x4C SoundThemesDisabled, +0x50 Granularity,
//   +0x54 GranularityShift (PDB +0x64), +0x58 MapPlacementPitch (PDB +0x68), +0x5C Area {TLX, TLY, BRX, BRY}
//   (PDB +0x6C), +0x6C MapPlacement (vector<unsigned short>, PDB +0x7C), +0x9C LoadedRegion (PDB +0xBC).
// CMapInfo (0x48 bytes, PDB = retail): +0x00 Area, +0x10 PMap, +0x18 LevelName, +0x1C LevelScriptName,
// +0x20 MapUID, +0x24 Used ... +0x40 IsSea.
// Shape: a read-only twin of SetMapPlacement 0x004FC9C0: with no stores in the loops the divided bounds are hoisted
// and the row pointer is strength-reduced (step 2 * pitch); the first used cell returns false.
struct C2DBoxI { long TLX, TLY, BRX, BRY; };

struct CWorldMap {
    unsigned char   _pad[0x50];
    long            Granularity;                // +0x50 (PDB +0x60)
    long            GranularityShift;           // +0x54
    long            MapPlacementPitch;          // +0x58
    C2DBoxI         Area;                       // +0x5C
    unsigned short* MapPlacement;               // +0x6C vector<unsigned short> start (PDB +0x7C)
    unsigned short* MapPlacementFinish;         // +0x70
    unsigned short* MapPlacementEnd;            // +0x74
    bool IsMapPlacementPositionFree(const C2DBoxI& box);
};

bool CWorldMap::IsMapPlacementPositionFree(const C2DBoxI& box)
{
    for (long y = box.TLY / Granularity; y < box.BRY / Granularity; ++y)
    {
        const unsigned short* row = &MapPlacement[y * MapPlacementPitch];
        for (long x = box.TLX / Granularity; x < box.BRX / Granularity; ++x)
            if (row[x] != 0)
                return false;
    }
    return true;
}
