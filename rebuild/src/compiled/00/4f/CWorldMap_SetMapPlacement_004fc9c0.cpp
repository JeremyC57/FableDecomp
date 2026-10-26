// CWorldMap::SetMapPlacement(const C2DBoxI&, long) -- stamp a map number over the placement-grid cells a world box covers
// Retail: Fable.exe @ 0x004FC9C0, 131 bytes, `ret 8`. PDB ?SetMapPlacement@CWorldMap@@IAEXABVC2DBoxI@@J@Z (protected). No callees.
// Retail CWorldMap (the engine header engine/CWorldMap.h is the PDB layout with 16-byte containers; retail STLport
// vectors / CArrays are 12 bytes and the map member is 12 bytes, so everything after +0x14 sits lower):
//   +0x20 Maps (CArray<CMapInfo>: start +0x20, finish +0x24; PDB +0x24), +0x2C Regions (vector<CRegion>, 0x58-byte
//   elements; PDB +0x34), +0x38 RegionTime, +0x44 PRegionGraph, +0x4C SoundThemesDisabled, +0x50 Granularity,
//   +0x54 GranularityShift (PDB +0x64), +0x58 MapPlacementPitch (PDB +0x68), +0x5C Area {TLX, TLY, BRX, BRY}
//   (PDB +0x6C), +0x6C MapPlacement (vector<unsigned short>, PDB +0x7C), +0x9C LoadedRegion (PDB +0xBC).
// CMapInfo (0x48 bytes, PDB = retail): +0x00 Area, +0x10 PMap, +0x18 LevelName, +0x1C LevelScriptName,
// +0x20 MapUID, +0x24 Used ... +0x40 IsSea.
// Shape: the box corners are divided by Granularity (+0x50) with signed idiv; every bound is re-divided after each
// 16-bit store (the store may alias the members), the row pointer is formed per row from the pitch and grid start.
class C2DBoxI { public: long TLX, TLY, BRX, BRY; };

class CWorldMap { public:
    unsigned char   _pad[0x50];
    long            Granularity;                // +0x50 (PDB +0x60)
    long            GranularityShift;           // +0x54
    long            MapPlacementPitch;          // +0x58
    C2DBoxI         Area;                       // +0x5C
    unsigned short* MapPlacement;               // +0x6C vector<unsigned short> start (PDB +0x7C)
    unsigned short* MapPlacementFinish;         // +0x70
    unsigned short* MapPlacementEnd;            // +0x74
    void SetMapPlacement(const C2DBoxI& box, long map);
};

void CWorldMap::SetMapPlacement(const C2DBoxI& box, long map)
{
    for (long y = box.TLY / Granularity; y < box.BRY / Granularity; ++y)
    {
        unsigned short* row = &MapPlacement[y * MapPlacementPitch];
        for (long x = box.TLX / Granularity; x < box.BRX / Granularity; ++x)
            row[x] = (unsigned short)map;
    }
}