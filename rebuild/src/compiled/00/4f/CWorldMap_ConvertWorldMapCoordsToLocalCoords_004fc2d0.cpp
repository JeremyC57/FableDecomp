// CWorldMap::ConvertWorldMapCoordsToLocalCoords(const C3DVector&) const -- a world position relative to the corner of the map under it (Z kept)
// Leaf: ?ConvertWorldMapCoordsToLocalCoords@CWorldMap@@QBE?AVC3DVector@@ABV2@@Z
// Retail: Fable.exe @ 0x004FC2D0, 87 bytes, `ret 8` (hidden C3DVector result). PDB ?ConvertWorldMapCoordsToLocalCoords@CWorldMap@@QBE?AVC3DVector@@ABV2@@Z
// Callees: _ftol2 0x00BFEA70 (x2), virtual slot 17 GetMapNumberAt(const C2DCoordI&) 0x004FC080 (+0x44).
// Retail CWorldMap (engine/CWorldMap.h is the PDB layout with 16-byte containers; retail STLport vectors / CArrays
// and the map member are 12 bytes, so everything after +0x14 sits lower and a local model is used):
//   +0x20 Maps (CArray<CMapInfo>: start +0x20, PDB +0x24); CMapInfo is 0x48 bytes with its C2DBoxI Area at +0x00.
// Shape: the truncated block is a frame C2DCoordI handed to GetMapNumberAt; the (long, const C3DVector&) overload 0x004FC2A0 is inlined (same TU) with the map number from the call.
class C2DBoxI {
public:
    long TLX, TLY, BRX, BRY;
    C2DBoxI(long tlx, long tly, long brx, long bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) { }
};
class C2DCoordI {
public:
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) { }
};
class C2DVector {
public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};
class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

class CMapInfo {
public:
    C2DBoxI       Area;                         // +0x00
    unsigned char _rest[0x38];                  // +0x10 .. +0x48
};

template <class T> class CArray {
public:
    T* Start;                                   // +0x00
    T* Finish;                                  // +0x04
    T* EndOfStorage;                            // +0x08
    const T& operator[](unsigned long i) const { return Start[i]; }
};

class CWorldMap {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual long GetMapNumberAt(const C2DCoordI& pos) const;     // slot 17 (+0x44) 0x004FC080
    unsigned char    _pad04[0x1c];
    CArray<CMapInfo> Maps;                      // +0x20 (PDB +0x24)
    C3DVector ConvertWorldMapCoordsToLocalCoords(long map, const C3DVector& pos) const
    {
        const C2DBoxI& area = Maps[map].Area;
        return C3DVector(pos.X - area.TLX, pos.Y - area.TLY, pos.Z);
    }
    C3DVector ConvertWorldMapCoordsToLocalCoords(const C3DVector& pos) const;
};

C3DVector CWorldMap::ConvertWorldMapCoordsToLocalCoords(const C3DVector& pos) const
{
    return ConvertWorldMapCoordsToLocalCoords(GetMapNumberAt(C2DCoordI((long)pos.X, (long)pos.Y)), pos);
}