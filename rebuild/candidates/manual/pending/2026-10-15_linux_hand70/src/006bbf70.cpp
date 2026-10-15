// CRegion::GetNameGraphicOffset() const -- the region def's name-graphic offset, or (0, 0) when the region has no def
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BBF70, length 47 bytes (`ret 4`: the C2DVector comes back through
// the hidden result pointer). No callees.
// Layout: the def field is retail CRegionDef +0xB4, the PDB's NameGraphicOffset (+0xC0; the retail CRegionDef sits
// 0x0C below the Ego_r.pdb layout there -- the neighbouring twin 0x006BBF40 reads WorldMapOffset at
// +0xAC). Retail CRegion (PDB names, retail offsets): the PDB vectors Maps/VisMaps are 16 bytes but retail's are 12,
// so CDefPointer<const CRegionDef> PDef sits at +0x24 (the PDB's +0x2c) and engine/CRegion.h cannot be used.
// Shape: C2DVector has a user constructor (an 8-byte POD would return in edx:eax) and the implicit copy (two integer
// moves); the null arm stores the two float zeros as immediates.
// Leaf: GetNameGraphicOffset
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};

struct CRegionDef {
    unsigned char _pad00[0xac];
    C2DVector     WorldMapOffset;                       // +0xac
    C2DVector     NameGraphicOffset;                    // +0xb4
};

struct CRegion {
    unsigned char     _pad00[0x24];                     // Maps, VisMaps (retail vectors), Name, DisplayName, RegionDefName
    const CRegionDef* PDef;                             // +0x24 CDefPointer<const CRegionDef>::Object
    C2DVector GetNameGraphicOffset() const;
};

C2DVector CRegion::GetNameGraphicOffset() const
{
    if (PDef)
        return PDef->NameGraphicOffset;
    return C2DVector(0.0f, 0.0f);
}
