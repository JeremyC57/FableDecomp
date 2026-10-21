// CWorldMap::DrawGetMapInfo(long) const -- draw interface: describe one map for the renderer (valid flag, loadable flags, map pointer, static mesh name, position, UID and extents)
// Leaf: ?DrawGetMapInfo@CWorldMap@@UBE?AUCDrawMapInfo@@J@Z
// Retail: Fable.exe @ 0x004FC7C0, 221 bytes, `ret 8`. PDB ?DrawGetMapInfo@CWorldMap@@UBE?AVCDrawMapInfo@@J@Z
// Virtual slot 1 of the CIDrawWorldMap interface (vtable 0x01244AB4) at +0x04 of CWorldMap: `this` arrives at +4, so
// the Maps array (+0x20) is read at +0x1C.
// Callees: CCharString::CCharString() 0x0099E4B0, CCharString::operator=(const CCharString&) 0x0099EFB0,
// CCharString::CCharString(const CCharString&) 0x0099EC30, ~CCharString 0x0099EAE0 (manifest "_Cons_val<...>").
// CMapInfo (0x48 bytes, PDB = retail): +0x00 Area, +0x10 PMap (CCountedPointer<CMap>::Data), +0x18 LevelName,
// +0x20 MapUID, +0x24 Used, +0x28 LoadedOnPlayerProximity. CDrawMapInfo (0x20 bytes, PDB): +0 Valid, +1 Loadable,
// +2 GameLoadable, +4 PMap, +8 StaticMeshName, +0x0C Pos, +0x14 UID, +0x18 Extents.
// Shape: a CDrawMapInfo local (only its string is constructed) filled member by member in the used arm, PMap assigned
// in both arms (merged store at the join), then returned through the implicit memberwise copy constructor.
// C2DExtentsI DECLARES its copy constructor (as the STLport pair / C3DVector rules): with the implicit one VC7.1
// sinks the UID store below the Extents copy.
struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    CCharString();                                           // 0x0099E4B0
    CCharString(const CCharString& other);                   // 0x0099EC30
    ~CCharString();                                          // 0x0099EAE0
    CCharString& operator=(const CCharString& other);        // 0x0099EFB0
};
struct C2DBoxI { long TLX, TLY, BRX, BRY; };
struct C2DCoordI { long X, Y; C2DCoordI() { } };
struct C2DExtentsI { long X, Y; C2DExtentsI() { } C2DExtentsI(long x, long y) : X(x), Y(y) { } C2DExtentsI(const C2DExtentsI& o) : X(o.X), Y(o.Y) { } };

struct CIDrawMap;
struct CMap;
struct CMapInfo {                                            // 0x48 bytes
    C2DBoxI       Area;                                      // +0x00
    CIDrawMap*    PMap;                                      // +0x10 CCountedPointer<CMap>::Data
    void*         PMapInfo;                                  // +0x14 CCountedPointer<CMap>::Info
    CCharString   LevelName;                                 // +0x18
    CCharString   LevelScriptName;                           // +0x1C
    unsigned long MapUID;                                    // +0x20
    bool          Used;                                      // +0x24
    bool          Placed;                                    // +0x25
    bool          Activated;                                 // +0x26
    bool          Loading;                                   // +0x27
    bool          LoadedOnPlayerProximity;                   // +0x28
    unsigned char _rest[0x1f];                               // +0x29 .. +0x48
};

struct CDrawMapInfo {                                        // 0x20 bytes
    bool          Valid;                                     // +0x00
    bool          Loadable;                                  // +0x01
    bool          GameLoadable;                              // +0x02
    CIDrawMap*    PMap;                                      // +0x04
    CCharString   StaticMeshName;                            // +0x08
    C2DCoordI     Pos;                                       // +0x0C
    unsigned long UID;                                       // +0x14
    C2DExtentsI   Extents;                                   // +0x18
};

template <class T> struct CArray {
    T* Start;                                                // +0x00
    T* Finish;                                               // +0x04
    T* EndOfStorage;                                         // +0x08
    const T& operator[](unsigned long i) const { return Start[i]; }
};

struct CIWorldMap {                                          // primary vtable 0x01244AEC
    virtual void v00();
};
struct CIDrawWorldMap {                                      // vtable 0x01244AB4
    virtual void d00();
    virtual CDrawMapInfo DrawGetMapInfo(long map) const = 0; // slot 1
};

struct CWorldMap : CIWorldMap, CIDrawWorldMap {
    unsigned char    _pad08[0x18];
    CArray<CMapInfo> Maps;                                   // +0x20
    CDrawMapInfo DrawGetMapInfo(long map) const;
};

CDrawMapInfo CWorldMap::DrawGetMapInfo(long map) const
{
    CDrawMapInfo info;
    const CMapInfo& mapInfo = Maps[map];
    if (mapInfo.Used)
    {
        info.Valid = true;
        info.StaticMeshName = mapInfo.LevelName;
        info.UID = mapInfo.MapUID;
        info.Pos.X = mapInfo.Area.TLX;
        info.Pos.Y = mapInfo.Area.TLY;
        info.Extents = C2DExtentsI(mapInfo.Area.BRX - mapInfo.Area.TLX, mapInfo.Area.BRY - mapInfo.Area.TLY);
        info.Loadable = mapInfo.LoadedOnPlayerProximity;
        info.GameLoadable = mapInfo.LoadedOnPlayerProximity;
        info.PMap = mapInfo.PMap;
    }
    else
    {
        info.PMap = 0;
        info.Valid = false;
    }
    return info;
}