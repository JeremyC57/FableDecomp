// CWorldMap::DrawGetSizeZAt(const C3DVector&, float) const -- draw interface: ask the loaded map under a world position for its Z size there (0 when no loaded map)
// Leaf: ?DrawGetSizeZAt@CWorldMap@@UBEMABUC3DVector@@M@Z
// Retail: Fable.exe @ 0x004FC6C0, 153 bytes, `ret 8`. PDB ?DrawGetSizeZAt@CWorldMap@@UBEMABVC3DVector@@M@Z
// Virtual slot 5 of the CIDrawWorldMap interface (vtable 0x01244AB4) at +0x04 of CWorldMap (primary vtable 0x01244AEC):
// `this` arrives at +4 and the primary calls go through `lea esi,[ebp-4]`.
// Callees: _ftol2 0x00BFEA70; primary slots 17 GetMapNumberAt(const C2DCoordI&) 0x004FC080 (+0x44), 10 IsMapLoaded(long)
// 0x004FC050 (+0x28), 19 PeekMap(long) 0x0051CE40 (+0x4C); CMap slot 4 DrawGetSizeZAt(const C3DVector*, float)
// 0x008F7630 (+0x10; PDB ?DrawGetSizeZAt@CMap@@UBEMPBVC3DVector@@M@Z). Float pool 0.0f 0x0122DEDC.
// Retail CWorldMap (the engine header engine/CWorldMap.h is the PDB layout with 16-byte containers; retail STLport
// vectors / CArrays are 12 bytes and the map member is 12 bytes, so everything after +0x14 sits lower):
//   +0x20 Maps (CArray<CMapInfo>: start +0x20, finish +0x24; PDB +0x24), +0x2C Regions (vector<CRegion>, 0x58-byte
//   elements; PDB +0x34), +0x38 RegionTime, +0x44 PRegionGraph, +0x4C SoundThemesDisabled, +0x50 Granularity,
//   +0x54 GranularityShift (PDB +0x64), +0x58 MapPlacementPitch (PDB +0x68), +0x5C Area {TLX, TLY, BRX, BRY}
//   (PDB +0x6C), +0x6C MapPlacement (vector<unsigned short>, PDB +0x7C), +0x9C LoadedRegion (PDB +0xBC).
// CMapInfo (0x48 bytes, PDB = retail): +0x00 Area, +0x10 PMap, +0x18 LevelName, +0x1C LevelScriptName,
// +0x20 MapUID, +0x24 Used ... +0x40 IsSea.
// Shape: the position is converted to map-local space (pos - map corner, Z kept) in a frame C3DVector that reuses the
// coordinate's slot; map 0 and unloaded maps give 0.
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct C2DBoxI { long TLX, TLY, BRX, BRY; };
struct C2DCoordI { long X, Y; C2DCoordI(long x, long y) : X(x), Y(y) { } };

struct CMap {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual float DrawGetSizeZAt(const C3DVector* pos, float f) const;   // slot 4 (+0x10) 0x008F7630
};

struct CMapInfo {
    C2DBoxI       Area;                         // +0x00
    unsigned char _rest[0x38];                  // +0x10 .. +0x48
};

template <class T> struct CArray {
    T* Start;                                   // +0x00
    T* Finish;                                  // +0x04
    T* EndOfStorage;                            // +0x08
    const T& operator[](unsigned long i) const { return Start[i]; }
};

struct CIWorldMap {                             // primary vtable 0x01244AEC
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual bool IsMapLoaded(long map) const;                      // slot 10 (+0x28)
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual long GetMapNumberAt(const C2DCoordI& coord) const;     // slot 17 (+0x44)
    virtual void v18();
    virtual const CMap& PeekMap(long map) const;                   // slot 19 (+0x4C)
};

struct CIDrawWorldMap {                         // vtable 0x01244AB4
    virtual void d00(); virtual void d01(); virtual void d02(); virtual void d03(); virtual void d04();
    virtual float DrawGetSizeZAt(const C3DVector& pos, float f) const = 0;   // slot 5
};

struct CWorldMap : CIWorldMap, CIDrawWorldMap {
    unsigned char    _pad08[0x18];
    CArray<CMapInfo> Maps;                      // +0x20
    float DrawGetSizeZAt(const C3DVector& pos, float f) const;
};

float CWorldMap::DrawGetSizeZAt(const C3DVector& pos, float f) const
{
    long map = GetMapNumberAt(C2DCoordI((long)pos.X, (long)pos.Y));
    if (map && IsMapLoaded(map))
    {
        const C2DBoxI& area = Maps[map].Area;
        C3DVector local(pos.X - area.TLX, pos.Y - area.TLY, pos.Z);
        return PeekMap(map).DrawGetSizeZAt(&local, f);
    }
    return 0.0f;
}