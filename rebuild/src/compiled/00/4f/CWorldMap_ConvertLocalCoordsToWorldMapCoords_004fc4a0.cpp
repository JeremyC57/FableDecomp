// CWorldMap::ConvertLocalCoordsToWorldMapCoords(long, const C3DVector&) const -- a map-local position back in world space (Z kept)
// Leaf: ?ConvertLocalCoordsToWorldMapCoords@CWorldMap@@QBE?AVC3DVector@@JABV2@@Z
// Retail: Fable.exe @ 0x004FC4A0, 45 bytes, `ret 0Ch` (hidden C3DVector result). PDB ?ConvertLocalCoordsToWorldMapCoords@CWorldMap@@QBE?AVC3DVector@@JABV2@@Z
// Callees: none.
// Retail CWorldMap (engine/CWorldMap.h is the PDB layout with 16-byte containers; retail STLport vectors / CArrays
// and the map member are 12 bytes, so everything after +0x14 sits lower and a local model is used):
//   +0x20 Maps (CArray<CMapInfo>: start +0x20, PDB +0x24); CMapInfo is 0x48 bytes with its C2DBoxI Area at +0x00.
// Shape: right-to-left constructor arguments (Z loaded first, then Y + TLY, X + TLX), stored X, Y, Z.
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
    C3DVector ConvertLocalCoordsToWorldMapCoords(long map, const C3DVector& pos) const;
};

C3DVector CWorldMap::ConvertLocalCoordsToWorldMapCoords(long map, const C3DVector& pos) const
{
    const C2DBoxI& area = Maps[map].Area;
    return C3DVector(pos.X + area.TLX, pos.Y + area.TLY, pos.Z);
}