// CWorldMap::GetRegionNumberMapIsIn(long) const -- index of the region containing a map, 0 when none
// Retail: Fable.exe @ 0x004FC190, 114 bytes. PDB ?GetRegionNumberMapIsIn@CWorldMap@@QBEJJ@Z
// Callee: CRegion::ContainsMap(long) const 0x006BBFA0 (manifest label "CHitParameters::IsDivineWrathStrike" is a
// byte-twin propagation).
// Retail CWorldMap (the engine header engine/CWorldMap.h is the PDB layout with 16-byte containers; retail STLport
// vectors / CArrays are 12 bytes and the map member is 12 bytes, so everything after +0x14 sits lower):
//   +0x20 Maps (CArray<CMapInfo>: start +0x20, finish +0x24; PDB +0x24), +0x2C Regions (vector<CRegion>, 0x58-byte
//   elements; PDB +0x34), +0x38 RegionTime, +0x44 PRegionGraph, +0x4C SoundThemesDisabled, +0x50 Granularity,
//   +0x54 GranularityShift (PDB +0x64), +0x58 MapPlacementPitch (PDB +0x68), +0x5C Area {TLX, TLY, BRX, BRY}
//   (PDB +0x6C), +0x6C MapPlacement (vector<unsigned short>, PDB +0x7C), +0x9C LoadedRegion (PDB +0xBC).
// CMapInfo (0x48 bytes, PDB = retail): +0x00 Area, +0x10 PMap, +0x18 LevelName, +0x1C LevelScriptName,
// +0x20 MapUID, +0x24 Used ... +0x40 IsSea.
// CRegion (retail 0x58 bytes; PDB 100 with 16-byte vectors): +0x00 Maps (vector<long>), +0x0C VisMaps, +0x18 Name
// (PDB +0x20), +0x1C DisplayName, +0x20 RegionDefName, ...
template <class T> struct CRetailVector {
    T* Start;                                   // +0x00
    T* Finish;                                  // +0x04
    T* EndOfStorage;                            // +0x08
    unsigned long size() const { return Finish - Start; }
    const T* begin() const { return Start; }
    const T& operator[](unsigned long i) const { return *(begin() + i); }
};
// Shape: region 0 is skipped; the element address is start (re-read each pass) + a strength-reduced 0x58 offset.
class CRegion { public:
    bool ContainsMap(long map) const;           // 0x006BBFA0
    unsigned char _body[0x58];
};

class CWorldMap { public:
    unsigned char          _pad[0x2c];
    CRetailVector<CRegion> Regions;             // +0x2C
    long GetRegionNumberMapIsIn(long map) const;
};

long CWorldMap::GetRegionNumberMapIsIn(long map) const
{
    for (unsigned long i = 1; i < Regions.size(); ++i)
        if (Regions[i].ContainsMap(map))
            return i;
    return 0;
}