// CDrawMapInfo::CDrawMapInfo(const CDrawMapInfo&) -- memberwise copy constructor: the three flags, the map pointer, the static-mesh name (CCharString copy), the position, UID and extents
// Leaf: ??0CDrawMapInfo
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0050B490, length 79 bytes (`ret 4`, returns this).
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30.
// Label: the catalog name "CTurncoatDef::operator=" is propagated (no reference reaches this body); the
// copy into fresh storage through the string's COPY constructor and the PDB layout match CDrawMapInfo
// (32 bytes): Valid +0, Loadable +1, GameLoadable +2, PMap +4, StaticMeshName +8, Pos (C2DCoordI) +0x0c,
// UID +0x14, Extents (C2DExtentsI) +0x18.
// Shape: three byte copies through al/cl/dl, the map pointer stored around the string's address set-up,
// then one register per dword for the trailing scalars: Pos and Extents are written as separate long
// members (a struct-typed member copies through `lea`, GOTCHAS wave 24g).
struct CIDrawMap;

struct CCharString {
    void* m_pStringData;
    CCharString(const CCharString& other);         // 0x0099EC30
};

struct CDrawMapInfo {
    bool          Valid;                           // +0x00
    bool          Loadable;                        // +0x01
    bool          GameLoadable;                    // +0x02
    CIDrawMap*    PMap;                            // +0x04
    CCharString   StaticMeshName;                  // +0x08
    long          PosX;                            // +0x0c Pos (C2DCoordI)
    long          PosY;                            // +0x10
    unsigned long UID;                             // +0x14
    long          ExtentsX;                        // +0x18 Extents (C2DExtentsI)
    long          ExtentsY;                        // +0x1c
    CDrawMapInfo(const CDrawMapInfo& other);
};

CDrawMapInfo::CDrawMapInfo(const CDrawMapInfo& other)
    : Valid(other.Valid)
    , Loadable(other.Loadable)
    , GameLoadable(other.GameLoadable)
    , PMap(other.PMap)
    , StaticMeshName(other.StaticMeshName)
    , PosX(other.PosX)
    , PosY(other.PosY)
    , UID(other.UID)
    , ExtentsX(other.ExtentsX)
    , ExtentsY(other.ExtentsY)
{
}
