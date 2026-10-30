// CChestDef::operator=(const CChestDef&) -- def copy: CDefClassBase part, chest flags, open animation string, opener fields
// Leaf: ??4CChestDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E0569, 63 bytes, size-optimised TU (/Os def region, like the landed
//         0x004663AE / 0x004E05AD def assignments).
// Callees: CDefClassBase::operator= @ 0x00431F10 (the manifest label "CThingBuildingDef::operator=" is propagated
//          over every def base assignment), CCharString::operator= @ 0x0099EFB0.
// Identification: PDB CChestDef (0x3c) = CDefClassBase + PersistOnOpening (bool) +0x25, OpenParticleEffect +0x28,
//   DisplayMessageOnEmpty (bool) +0x2c, OpenAnimationForCreature (CCharString) +0x30, OpenerObject +0x34,
//   OpenersRequired +0x38 (GOTCHAS: CChestDef::Transfer 0x004DE204, OpenerObject +0x34, OpenersRequired +0x38).
//   The first byte sits at +0x25 because CDefClassBase is a packed 0x25-byte record.
// Shape: memberwise, declaration order; the string goes through its out-of-line operator=.
// The generated engine headers flatten the bases (no base operator= to call), so the classes are modelled locally.
#pragma optimize("s", on)

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)

class CCharString { public:
    void* m_pStringData;
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

// def vtable 0x0123ACDC: slot 0 0x004D8072 (implicit inline dtor); real overrides from the def vtable scan
class CChestDef : public CDefClassBase { public:
    bool PersistOnOpening;              // +0x25
    long OpenParticleEffect;            // +0x28
    bool DisplayMessageOnEmpty;         // +0x2c
    CCharString OpenAnimationForCreature;// +0x30
    long OpenerObject;                  // +0x34
    long OpenersRequired;               // +0x38
    CChestDef& operator=(const CChestDef& other);
    virtual void Transfer(CPersistContext& context);         // slot 18 0x004DE204
    virtual void Copy(const CDefClassBase* other);           // slot 19 0x004E0564
    virtual unsigned long GetSizeofClass();                  // slot 20 0x004D3840
};

CChestDef& CChestDef::operator=(const CChestDef& other)
{
    CDefClassBase::operator=(other);
    PersistOnOpening = other.PersistOnOpening;
    OpenParticleEffect = other.OpenParticleEffect;
    DisplayMessageOnEmpty = other.DisplayMessageOnEmpty;
    OpenAnimationForCreature = other.OpenAnimationForCreature;
    OpenerObject = other.OpenerObject;
    OpenersRequired = other.OpenersRequired;
    return *this;
}