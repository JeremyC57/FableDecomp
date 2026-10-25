// CWorldMap::DrawConvertWorldMapCoordsToLocalCoords(long, const C3DVector&) const -- draw interface: a world position relative to one map's corner (Z kept)
// Leaf: ?DrawConvertWorldMapCoordsToLocalCoords@CWorldMap@@UBE?AVC3DVector@@JABV2@@Z
// Retail: Fable.exe @ 0x0051CE60, 45 bytes, `ret 0Ch` (hidden C3DVector result). PDB
// ?DrawConvertWorldMapCoordsToLocalCoords@CWorldMap@@UBE?AVC3DVector@@JABV2@@Z
// Virtual slot 6 of the CIDrawWorldMap interface (vtable 0x01244AB4, RTTI: CWorldMap : CBaseClassNonCopyable,
// CIDrawWorldMap at +4). The interface declares the virtual first, so the override is compiled with `this` at the
// +4 subobject and Maps.Start (+0x20 of CWorldMap) is read as [ecx+1Ch].
// Callees: none (ConvertWorldMapCoordsToLocalCoords(long, const C3DVector&), 0x004FC2A0, is inlined).
// Retail CWorldMap (engine/CWorldMap.h is the PDB layout with 16-byte containers; retail STLport vectors / CArrays and
// the map member are 12 bytes, so a local model is used): +0x20 Maps (CArray<CMapInfo>, PDB +0x24).
// Shape: byte-identical to 0x004FC2A0 apart from the +0x1C displacement.
class C2DBoxI { public: long TLX, TLY, BRX, BRY; };
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

class CBaseClass {
public:
    virtual ~CBaseClass();
};
class CBaseClassNonCopyable : public CBaseClass {
public:
    virtual ~CBaseClassNonCopyable();
};

class CIDrawWorldMap {                          // vtable 0x01244AB4 in CWorldMap
public:
    virtual long DrawGetNoMaps() const = 0;                     // slot 0
    virtual void d01() const = 0; virtual void d02() const = 0; virtual void d03() const = 0;
    virtual void d04() const = 0; virtual void d05() const = 0;
    virtual C3DVector DrawConvertWorldMapCoordsToLocalCoords(long map, const C3DVector& pos) const = 0;   // slot 6
};

class CWorldMap : public CBaseClassNonCopyable, public CIDrawWorldMap {
public:
    void*            World;                     // +0x08
    void*            Component;                 // +0x0C
    void*            PThingMapManager;          // +0x10
    unsigned long    LoadedScriptedMapBrushes[3];   // +0x14 STLport map (12 bytes)
    CArray<CMapInfo> Maps;                      // +0x20 (PDB +0x24)
    C3DVector ConvertWorldMapCoordsToLocalCoords(long map, const C3DVector& pos) const
    {
        const C2DBoxI& area = Maps[map].Area;
        return C3DVector(pos.X - area.TLX, pos.Y - area.TLY, pos.Z);
    }
    virtual C3DVector DrawConvertWorldMapCoordsToLocalCoords(long map, const C3DVector& pos) const;
};

C3DVector CWorldMap::DrawConvertWorldMapCoordsToLocalCoords(long map, const C3DVector& pos) const
{
    return ConvertWorldMapCoordsToLocalCoords(map, pos);
}