// CWorldMap::GetMapNumberFromMapUID(unsigned long) const -- index of the map with this UID, 0 when none
// Retail: Fable.exe @ 0x004FB920, 99 bytes. PDB ?GetMapNumberFromMapUID@CWorldMap@@QBEJK@Z. No callees.
// Retail CWorldMap (the engine header engine/CWorldMap.h is the PDB layout with 16-byte containers; retail STLport
// vectors / CArrays are 12 bytes and the map member is 12 bytes, so everything after +0x14 sits lower):
//   +0x20 Maps (CArray<CMapInfo>: start +0x20, finish +0x24; PDB +0x24), +0x2C Regions (vector<CRegion>, 0x58-byte
//   elements; PDB +0x34), +0x38 RegionTime, +0x44 PRegionGraph, +0x4C SoundThemesDisabled, +0x50 Granularity,
//   +0x54 GranularityShift (PDB +0x64), +0x58 MapPlacementPitch (PDB +0x68), +0x5C Area {TLX, TLY, BRX, BRY}
//   (PDB +0x6C), +0x6C MapPlacement (vector<unsigned short>, PDB +0x7C), +0x9C LoadedRegion (PDB +0xBC).
// CMapInfo (0x48 bytes, PDB = retail): +0x00 Area, +0x10 PMap, +0x18 LevelName, +0x1C LevelScriptName,
// +0x20 MapUID, +0x24 Used ... +0x40 IsSea.
// Shape: map 0 is skipped; the UID field pointer is strength-reduced (start + 0x68, step 0x48) while size() is
// recomputed from the members every pass.
class CMapInfo { public:
    unsigned char _pad[0x20];
    unsigned long MapUID;                       // +0x20
    unsigned char _rest[0x24];                  // +0x24 .. +0x48
};

template <class T> class CArray { public:
    T* Start;                                   // +0x00
    T* Finish;                                  // +0x04
    T* EndOfStorage;                            // +0x08
    unsigned long size() const { return Finish - Start; }
    const T& operator[](unsigned long i) const { return Start[i]; }
};

class CWorldMap { public:
    unsigned char    _pad[0x20];
    CArray<CMapInfo> Maps;                      // +0x20
    long GetMapNumberFromMapUID(unsigned long uid) const;
};

long CWorldMap::GetMapNumberFromMapUID(unsigned long uid) const
{
    for (unsigned long i = 1; i < Maps.size(); ++i)
        if (Maps[i].MapUID == uid)
            return i;
    return 0;
}