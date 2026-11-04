// CWorldMap::IsPosInMap(const C2DCoordI&) const -- is the world block inside the world area and on a map that is present?
// Leaf: ?IsPosInMap@CWorldMap@@QBE_NABVC2DCoordI@@@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe @ 0x004FC5B0, 66 bytes, `ret 4`. PDB ?IsPosInMap@CWorldMap@@QBE_NABVC2DCoordI@@@Z
// Callee: virtual slot 17 GetMapNumberAt(const C2DCoordI&) 0x004FC080 (+0x44; rtti-vtables.tsv 0x01244AEC).
// Retail CWorldMap (engine/CWorldMap.h is the PDB layout with 16-byte containers; retail STLport vectors / CArrays and
// the map member are 12 bytes, so a local model is used): +0x20 Maps (CArray<CMapInfo>, PDB +0x24), +0x5C Area
// {TLX, TLY, BRX, BRY} (PDB +0x6C). CMapInfo (0x48 bytes, PDB = retail): +0x00 Area, +0x10 PMap.
// Shape: the twin of the landed C3DVector overload 0x004FC600 without the truncation: the block itself is handed to
// GetMapNumberAt, the area test is written coordinate-first (`cmp eax,[esi+5Ch]`), the nested `if` returns true.
class C2DBoxI { public: long TLX, TLY, BRX, BRY; };
class C2DCoordI { public: long X, Y; };
class CMap;

class CMapInfo {
public:
    C2DBoxI       Area;                         // +0x00
    CMap*         PMap;                         // +0x10 CCountedPointer<CMap>::Data
    void*         PMapInfo;                     // +0x14 CCountedPointer<CMap>::Info
    unsigned char _rest[0x30];                  // +0x18 .. +0x48
};

#include "engine/CArray.h"

class CWorldMap {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual long GetMapNumberAt(const C2DCoordI& pos) const;     // slot 17 (+0x44) 0x004FC080
    unsigned char    _pad04[0x1c];
    CArray<CMapInfo> Maps;                      // +0x20 (PDB +0x24)
    unsigned char    _pad2c[0x30];
    C2DBoxI          Area;                      // +0x5C (PDB +0x6C)
    bool IsPosInMap(const C2DCoordI& pos) const;
};

bool CWorldMap::IsPosInMap(const C2DCoordI& pos) const
{
    if (pos.X >= Area.TLX && pos.X < Area.BRX && pos.Y >= Area.TLY && pos.Y < Area.BRY)
    {
        if (Maps[GetMapNumberAt(pos)].PMap)
            return true;
    }
    return false;
}