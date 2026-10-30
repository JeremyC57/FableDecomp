// CTattooDef::operator=(const CTattooDef&) -- def copy: CDefClassBase part, then the CTattooDef fields memberwise in declaration order
// Leaf: ??4CTattooDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x00453D46, 129 bytes, size-optimised def TU.
// Identity: reached through the 5-byte `jmp` thunk at 0x00453D41 in slot 19 of vtable 0x012323B4 = CTattooDef
//   (rtti-vtables.tsv); the manifest label "CThingBuildingDef::operator=" is propagated over every def assignment.
// Callees: CDefClassBase::operator= @ 0x00431F10, CCharString::operator= @ 0x0099EFB0.
// Layout (engine/CTattooDef.h, matches retail): TattooName +0x28, TextureLayer +0x2c, ReplacingTextureIndex +0x30, BankIndex +0x34, Blend +0x38, RemovesAllTattoos +0x3c, Permanent +0x3d, CoversBodyAreaFlags +0x40, VisibilityMultiplier +0x44, SpecificCoversBodyAreaFlags +0x48, CustomFileName +0x4c, CustomFileNameBase +0x50, Attractiveness +0x54, Scariness +0x58, Goodstrength +0x5c.
// Shape: the implicit memberwise assignment written out (the def module out-lines it); the generated engine header
//   flattens the base (no base operator= to call), so the classes are modelled locally like the landed 0x00463BA2.
// Blend is the 4-byte ECompositeBlendType enum, modelled as long.
#pragma optimize("s", on)

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)
class CCharString { public:
    void* m_pStringData;
    const CCharString& operator=(const CCharString& other);       // 0x0099EFB0
};

// def vtable 0x012323B4: slot 0 0x0044FA51 (implicit inline dtor); real overrides from the def vtable scan
class CTattooDef : public CDefClassBase { public:
    CCharString              TattooName;                  // +0x28
    long                     TextureLayer;                // +0x2c
    long                     ReplacingTextureIndex;       // +0x30
    long                     BankIndex;                   // +0x34
    long                     Blend;                       // +0x38
    bool                     RemovesAllTattoos;           // +0x3c
    bool                     Permanent;                   // +0x3d
    long                     CoversBodyAreaFlags;         // +0x40
    float                    VisibilityMultiplier;        // +0x44
    long                     SpecificCoversBodyAreaFlags; // +0x48
    CCharString              CustomFileName;              // +0x4c
    CCharString              CustomFileNameBase;          // +0x50
    float                    Attractiveness;              // +0x54
    float                    Scariness;                   // +0x58
    float                    Goodstrength;                // +0x5c
    CTattooDef& operator=(const CTattooDef& other);
    virtual void Transfer(CPersistContext& context);         // slot 18 0x004529AE
    virtual void Copy(const CDefClassBase* other);           // slot 19 0x00453D41
    virtual unsigned long GetSizeofClass();                  // slot 20 0x0044C3D4
};

CTattooDef& CTattooDef::operator=(const CTattooDef& other)
{
    CDefClassBase::operator=(other);
    TattooName = other.TattooName;
    TextureLayer = other.TextureLayer;
    ReplacingTextureIndex = other.ReplacingTextureIndex;
    BankIndex = other.BankIndex;
    Blend = other.Blend;
    RemovesAllTattoos = other.RemovesAllTattoos;
    Permanent = other.Permanent;
    CoversBodyAreaFlags = other.CoversBodyAreaFlags;
    VisibilityMultiplier = other.VisibilityMultiplier;
    SpecificCoversBodyAreaFlags = other.SpecificCoversBodyAreaFlags;
    CustomFileName = other.CustomFileName;
    CustomFileNameBase = other.CustomFileNameBase;
    Attractiveness = other.Attractiveness;
    Scariness = other.Scariness;
    Goodstrength = other.Goodstrength;
    return *this;
}