// CWorldMap::IsPosInMap(const C3DVector&) const -- is the world position inside the world area and on a map that is loaded?
// Leaf: ?IsPosInMap@CWorldMap@@QBE_NABVC3DVector@@@Z
// Retail: Fable.exe @ 0x004FC600, 128 bytes. PDB ?IsPosInMap@CWorldMap@@QBE_NABVC3DVector@@@Z
// Callees: _ftol2 0x00BFEA70, virtual slot 17 GetMapNumberAt(const C2DCoordI&) 0x004FC080 (+0x44; rtti-vtables.tsv).
// Retail CWorldMap (the engine header engine/CWorldMap.h is the PDB layout with 16-byte containers; retail STLport
// vectors / CArrays are 12 bytes and the map member is 12 bytes, so everything after +0x14 sits lower):
//   +0x20 Maps (CArray<CMapInfo>: start +0x20, finish +0x24; PDB +0x24), +0x2C Regions (vector<CRegion>, 0x58-byte
//   elements; PDB +0x34), +0x38 RegionTime, +0x44 PRegionGraph, +0x4C SoundThemesDisabled, +0x50 Granularity,
//   +0x54 GranularityShift (PDB +0x64), +0x58 MapPlacementPitch (PDB +0x68), +0x5C Area {TLX, TLY, BRX, BRY}
//   (PDB +0x6C), +0x6C MapPlacement (vector<unsigned short>, PDB +0x7C), +0x9C LoadedRegion (PDB +0xBC).
// CMapInfo (0x48 bytes, PDB = retail): +0x00 Area, +0x10 PMap, +0x18 LevelName, +0x1C LevelScriptName,
// +0x20 MapUID, +0x24 Used ... +0x40 IsSea.
// Shape: the same half-open area test as GetMapNumberAt(const C3DVector&) 0x004FC0C0; the truncated coordinate is a
// frame C2DCoordI passed by reference; a map whose PMap.Data (+0x10) is set counts as present.
class C3DVector { public: float X, Y, Z; };
class C2DBoxI { public: long TLX, TLY, BRX, BRY; };
class C2DCoordI { public: long X, Y; C2DCoordI(long x, long y) : X(x), Y(y) { } };
class CMap;

class CMapInfo { public:
    C2DBoxI       Area;                         // +0x00
    CMap*         PMap;                         // +0x10 CCountedPointer<CMap>::Data
    void*         PMapInfo;                     // +0x14 CCountedPointer<CMap>::Info
    unsigned char _rest[0x30];                  // +0x18 .. +0x48
};

template <class T> class CArray { public:
    T* Start;                                   // +0x00
    T* Finish;                                  // +0x04
    T* EndOfStorage;                            // +0x08
    const T& operator[](unsigned long i) const { return Start[i]; }
};

class CWorldMap { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual long GetMapNumberAt(const C2DCoordI& coord) const;   // slot 17 (+0x44) 0x004FC080
    unsigned char    _pad04[0x1c];
    CArray<CMapInfo> Maps;                      // +0x20
    unsigned char    _pad2c[0x30];
    C2DBoxI          Area;                      // +0x5C
    bool IsPosInMap(const C3DVector& pos) const;
};

bool CWorldMap::IsPosInMap(const C3DVector& pos) const
{
    if (Area.TLX <= pos.X && Area.BRX > pos.X && Area.TLY <= pos.Y && Area.BRY > pos.Y)
    {
        C2DCoordI coord((long)pos.X, (long)pos.Y);
        if (Maps[GetMapNumberAt(coord)].PMap)
            return true;
    }
    return false;
}