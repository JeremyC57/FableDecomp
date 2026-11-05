// CWorldMap::ConvertWorldMapCoordsToLocalCoords(const C2DCoordI&) const -- a world block relative to the corner of the map under it
// Leaf: ?ConvertWorldMapCoordsToLocalCoords@CWorldMap@@QBE?AVC2DCoordI@@ABV2@@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: the local C2DCoordI removed too (engine/C3DVector.h includes engine/C2DCoordI.h, Ego_r TPI 0xa59e: public long X/Y, size 8).
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe @ 0x004FC3F0, 77 bytes, `ret 8` (hidden C2DCoordI result). PDB ?ConvertWorldMapCoordsToLocalCoords@CWorldMap@@QBE?AVC2DCoordI@@ABV2@@Z
// Callees: virtual slot 17 GetMapNumberAt(const C2DCoordI&) 0x004FC080 (+0x44).
// Retail CWorldMap (engine/CWorldMap.h is the PDB layout with 16-byte containers; retail STLport vectors / CArrays
// and the map member are 12 bytes, so everything after +0x14 sits lower and a local model is used):
//   +0x20 Maps (CArray<CMapInfo>: start +0x20, PDB +0x24); CMapInfo is 0x48 bytes with its C2DBoxI Area at +0x00.
// Shape: the block is copied into a frame C2DCoordI for GetMapNumberAt (the overload family builds its lookup block as a value), then the (long, const C2DCoordI&) overload 0x004FC3C0 is inlined and re-reads the argument.
class C2DBoxI {
public:
    long TLX, TLY, BRX, BRY;
    C2DBoxI(long tlx, long tly, long brx, long bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) { }
};
#include "engine/C3DVector.h"
class C2DVector {
public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};

class CMapInfo {
public:
    C2DBoxI       Area;                         // +0x00
    unsigned char _rest[0x38];                  // +0x10 .. +0x48
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
    C2DCoordI ConvertWorldMapCoordsToLocalCoords(long map, const C2DCoordI& pos) const
    {
        const C2DBoxI& area = Maps[map].Area;
        return C2DCoordI(pos.X - area.TLX, pos.Y - area.TLY);
    }
    C2DCoordI ConvertWorldMapCoordsToLocalCoords(const C2DCoordI& pos) const;
};

C2DCoordI CWorldMap::ConvertWorldMapCoordsToLocalCoords(const C2DCoordI& pos) const
{
    return ConvertWorldMapCoordsToLocalCoords(GetMapNumberAt(C2DCoordI(pos.X, pos.Y)), pos);
}