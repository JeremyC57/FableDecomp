// CRegion::SetMiniMapRegionExitTextOffsetY(const CCharString& exitName, float y) -- set the Y of the minimap exit-text offset stored for the named region exit (the entry is created on demand)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BC4F0, length 23 bytes (`ret 8`).
// Callees: map<CCharString, C2DVector>::operator[](const CCharString&) @ 0x006BD6A0 (returns C2DVector&).
// Layout: retail CRegion (PDB names, retail offsets): MiniMapRegionExitTextOffsets is the PDB's +0x40 map, at +0x38 in
// retail because the two leading vectors (Maps, VisMaps) are 12 bytes there, not 16 -- engine/CRegion.h cannot be
// used. Twin: SetMiniMapRegionExitTextOffsetX @ 0x006BC4D0 stores the X (+0) instead.
// Shape: the key reference is forwarded (`push [esp+4]` through eax), `this` is adjusted in place (`add ecx,38h`),
// and the float argument is copied with an integer move into the returned C2DVector's Y (+4).
// Leaf: SetMiniMapRegionExitTextOffsetY
struct CCharString;

struct C2DVector {
    float X, Y;
};

struct CRegionExitTextOffsetMap {                       // map<CCharString, C2DVector, less<CCharString> >
    unsigned char _body[0x0c];
    C2DVector& operator[](const CCharString& key);      // 0x006BD6A0
};

struct CRegion {
    unsigned char            _pad00[0x38];              // Maps, VisMaps, names, PDef, MiniMapGraphic, scale, offset
    CRegionExitTextOffsetMap MiniMapRegionExitTextOffsets;   // +0x38
    void SetMiniMapRegionExitTextOffsetY(const CCharString& exitName, float y);
};

void CRegion::SetMiniMapRegionExitTextOffsetY(const CCharString& exitName, float y)
{
    MiniMapRegionExitTextOffsets[exitName].Y = y;
}
