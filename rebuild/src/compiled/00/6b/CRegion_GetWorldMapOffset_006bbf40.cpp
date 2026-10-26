// CRegion::GetWorldMapOffset() const -- the region def's world-map offset, or (0, 0) when the region has no def
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BBF40, length 47 bytes (`ret 4`: the C2DVector comes back through
// the hidden result pointer). No callees.
// Label correction: the manifest names both 0x006BBF40 and 0x006BBF70 "GetNameGraphicOffset" (propagated). The def
// field read here is retail CRegionDef +0xAC, the PDB's WorldMapOffset (+0xB8; the retail CRegionDef sits 0x0C below
// the Ego_r.pdb layout there, which puts NameGraphicOffset at +0xB4 -- the field 0x006BBF70 reads), so this one is
// GetWorldMapOffset.
// Layout: retail CRegion (PDB names, retail offsets): the PDB vectors Maps/VisMaps are 16 bytes but retail's are
// 12, so CDefPointer<const CRegionDef> PDef sits at +0x24 (the PDB's +0x2c) and engine/CRegion.h cannot be used.
// Shape: C2DVector has a user constructor (an 8-byte POD would return in edx:eax) and the implicit copy (two integer
// moves); the null arm stores the two float zeros as immediates.
// Leaf: GetWorldMapOffset
class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};

class CRegionDef { public:
    unsigned char _pad00[0xac];
    C2DVector     WorldMapOffset;                       // +0xac
    C2DVector     NameGraphicOffset;                    // +0xb4
};

class CRegion { public:
    unsigned char     _pad00[0x24];                     // Maps, VisMaps (retail vectors), Name, DisplayName, RegionDefName
    const CRegionDef* PDef;                             // +0x24 CDefPointer<const CRegionDef>::Object
    C2DVector GetWorldMapOffset() const;
};

C2DVector CRegion::GetWorldMapOffset() const
{
    if (PDef)
        return PDef->WorldMapOffset;
    return C2DVector(0.0f, 0.0f);
}