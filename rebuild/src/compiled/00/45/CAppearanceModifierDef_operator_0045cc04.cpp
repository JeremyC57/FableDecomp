// CAppearanceModifierDef::operator=(const CAppearanceModifierDef&) -- def copy: CDefClassBase part, then the CAppearanceModifierDef fields memberwise in declaration order
// Leaf: ??4CAppearanceModifierDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x0045CC04, 117 bytes, size-optimised def TU.
// Identity: reached through the 5-byte `jmp` thunk at 0x0045CBFF in slot 19 of vtable 0x012330DC = CAppearanceModifierDef
//   (rtti-vtables.tsv); the manifest label "CThingBuildingDef::operator=" is propagated over every def assignment.
// Callees: CDefClassBase::operator= @ 0x00431F10, vector<CCharString>::operator= @ 0x00432EE9 (manifest "CCombatWheel::operator=", propagated), CAppearanceModifierGraphics::operator= @ 0x0045CC79.
// Layout (engine/CAppearanceModifierDef.h, PDB offsets minus the 4-byte container shrink): HideMaterials +0x28, MorphGraphics +0x34, CoversBodyAreaFlags +0x40, IsRemoveable +0x44, HeroSuit +0x48, SuitPart +0x4c, AppearanceType +0x50, SoundRadiusMultiplier +0x54, VisibilityMultiplier +0x58, ArmourDefIndex +0x5c, OveralProtectionWeighting +0x60, Scariness +0x64, Attractiveness +0x68, Goodstrength +0x6c.
// Shape: the implicit memberwise assignment written out (the def module out-lines it); the generated engine header
//   flattens the base (no base operator= to call), so the classes are modelled locally like the landed 0x00463BA2.
// SuitPart/AppearanceType are 4-byte enums (long). Retail vectors are 12 bytes, so offsets after HideMaterials sit 4/8 below the PDB.
#pragma optimize("s", on)

struct CDefClassBase {
    unsigned char _body[0x25];          // vptr, resource links, PDefManager +0x1c, GlobalIndex +0x20, flag bits +0x24
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};
struct CCharStringVector {                       // retail std::vector<CCharString> (retail 0xc bytes)
    unsigned long m_words[3];
    CCharStringVector& operator=(const CCharStringVector& other);   // 0x00432EE9
};
struct CAppearanceModifierGraphics {                       // CArray<CEntry>, PDB 16 bytes (retail 0xc bytes)
    unsigned long m_words[3];
    CAppearanceModifierGraphics& operator=(const CAppearanceModifierGraphics& other);   // 0x0045CC79
};

struct CAppearanceModifierDef : CDefClassBase {
    CCharStringVector        HideMaterials;               // +0x28
    CAppearanceModifierGraphics MorphGraphics;               // +0x34
    long                     CoversBodyAreaFlags;         // +0x40
    bool                     IsRemoveable;                // +0x44
    long                     HeroSuit;                    // +0x48
    long                     SuitPart;                    // +0x4c
    long                     AppearanceType;              // +0x50
    float                    SoundRadiusMultiplier;       // +0x54
    float                    VisibilityMultiplier;        // +0x58
    long                     ArmourDefIndex;              // +0x5c
    float                    OveralProtectionWeighting;   // +0x60
    float                    Scariness;                   // +0x64
    float                    Attractiveness;              // +0x68
    float                    Goodstrength;                // +0x6c
    CAppearanceModifierDef& operator=(const CAppearanceModifierDef& other);
};

CAppearanceModifierDef& CAppearanceModifierDef::operator=(const CAppearanceModifierDef& other)
{
    CDefClassBase::operator=(other);
    HideMaterials = other.HideMaterials;
    MorphGraphics = other.MorphGraphics;
    CoversBodyAreaFlags = other.CoversBodyAreaFlags;
    IsRemoveable = other.IsRemoveable;
    HeroSuit = other.HeroSuit;
    SuitPart = other.SuitPart;
    AppearanceType = other.AppearanceType;
    SoundRadiusMultiplier = other.SoundRadiusMultiplier;
    VisibilityMultiplier = other.VisibilityMultiplier;
    ArmourDefIndex = other.ArmourDefIndex;
    OveralProtectionWeighting = other.OveralProtectionWeighting;
    Scariness = other.Scariness;
    Attractiveness = other.Attractiveness;
    Goodstrength = other.Goodstrength;
    return *this;
}