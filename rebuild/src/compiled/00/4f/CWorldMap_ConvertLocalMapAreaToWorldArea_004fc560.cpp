// CWorldMap::ConvertLocalMapAreaToWorldArea(long, const C2DBoxI&) const -- a map-local box shifted back into world space
// Leaf: ?ConvertLocalMapAreaToWorldArea@CWorldMap@@QBE?AVC2DBoxI@@JABV2@@Z
// Retail: Fable.exe @ 0x004FC560, 65 bytes, `ret 0Ch` (hidden C2DBoxI result). PDB ?ConvertLocalMapAreaToWorldArea@CWorldMap@@QBE?AVC2DBoxI@@JABV2@@Z
// Callees: none.
// Retail CWorldMap (engine/CWorldMap.h is the PDB layout with 16-byte containers; retail STLport vectors / CArrays
// and the map member are 12 bytes, so everything after +0x14 sits lower and a local model is used):
//   +0x20 Maps (CArray<CMapInfo>: start +0x20, PDB +0x24); CMapInfo is 0x48 bytes with its C2DBoxI Area at +0x00.
// Shape: the map corner is reached first, then the box is copied into a named local through the memberwise user copy
// constructor (all four words loaded up front), shifted in place (TLX/BRX by the corner X, TLY/BRY by Y) and returned
// by copy (the four-argument constructor form loads the corner first and orders the adds differently).
class C2DBoxI {
public:
    long TLX, TLY, BRX, BRY;
    C2DBoxI(long tlx, long tly, long brx, long bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) { }
    C2DBoxI(const C2DBoxI& o) : TLX(o.TLX), TLY(o.TLY), BRX(o.BRX), BRY(o.BRY) { }
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
    C2DBoxI ConvertLocalMapAreaToWorldArea(long map, const C2DBoxI& box) const;
};

C2DBoxI CWorldMap::ConvertLocalMapAreaToWorldArea(long map, const C2DBoxI& box) const
{
    const C2DBoxI& area = Maps[map].Area;
    C2DBoxI result(box);
    result.TLX += area.TLX;
    result.BRX += area.TLX;
    result.TLY += area.TLY;
    result.BRY += area.TLY;
    return result;
}