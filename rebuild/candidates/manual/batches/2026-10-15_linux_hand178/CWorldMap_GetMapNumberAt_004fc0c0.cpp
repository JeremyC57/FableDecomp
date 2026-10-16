// CWorldMap::GetMapNumberAt(const C3DVector&) const -- map number under a world position (0 outside the world area)
// Leaf: ?GetMapNumberAt@CWorldMap@@UBEJABUC3DVector@@@Z
// Retail: Fable.exe @ 0x004FC0C0, 107 bytes. PDB ?GetMapNumberAt@CWorldMap@@UBEJABVC3DVector@@@Z (virtual).
// Callee: _ftol2 0x00BFEA70 (compiler helper for the two float -> long conversions).
// Retail CWorldMap (the engine header engine/CWorldMap.h is the PDB layout with 16-byte containers; retail STLport
// vectors / CArrays are 12 bytes and the map member is 12 bytes, so everything after +0x14 sits lower):
//   +0x20 Maps (CArray<CMapInfo>: start +0x20, finish +0x24; PDB +0x24), +0x2C Regions (vector<CRegion>, 0x58-byte
//   elements; PDB +0x34), +0x38 RegionTime, +0x44 PRegionGraph, +0x4C SoundThemesDisabled, +0x50 Granularity,
//   +0x54 GranularityShift (PDB +0x64), +0x58 MapPlacementPitch (PDB +0x68), +0x5C Area {TLX, TLY, BRX, BRY}
//   (PDB +0x6C), +0x6C MapPlacement (vector<unsigned short>, PDB +0x7C), +0x9C LoadedRegion (PDB +0xBC).
// CMapInfo (0x48 bytes, PDB = retail): +0x00 Area, +0x10 PMap, +0x18 LevelName, +0x1C LevelScriptName,
// +0x20 MapUID, +0x24 Used ... +0x40 IsSea.
// Shape: a half-open box test TLX <= X < BRX, TLY <= Y < BRY written with the integer corner on the left (fild first);
// both coordinates truncated, scaled down by GranularityShift and used to index the 16-bit placement grid.
struct C3DVector { float X, Y, Z; };
struct C2DBoxI { long TLX, TLY, BRX, BRY; };
struct C2DCoordI { long X, Y; C2DCoordI(long x, long y) : X(x), Y(y) { } };

struct CWorldMap {
    virtual void v00();
    unsigned char   _pad04[0x50];
    long            GranularityShift;           // +0x54
    long            MapPlacementPitch;          // +0x58
    C2DBoxI         Area;                       // +0x5C
    unsigned short* MapPlacement;               // +0x6C vector<unsigned short> start
    unsigned short* MapPlacementFinish;         // +0x70
    unsigned short* MapPlacementEnd;            // +0x74
    virtual long GetMapNumberAt(const C3DVector& pos) const;
};

long CWorldMap::GetMapNumberAt(const C3DVector& pos) const
{
    if (Area.TLX <= pos.X && Area.BRX > pos.X && Area.TLY <= pos.Y && Area.BRY > pos.Y)
    {
        C2DCoordI coord((long)pos.X, (long)pos.Y);
        return MapPlacement[(coord.Y >> GranularityShift) * MapPlacementPitch + (coord.X >> GranularityShift)];
    }
    return 0;
}
