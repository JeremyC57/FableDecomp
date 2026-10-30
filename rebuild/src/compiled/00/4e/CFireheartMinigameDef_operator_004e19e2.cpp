// CFireheartMinigameDef::operator=(const CFireheartMinigameDef&) -- def copy: CDefClassBase part, two effect-name strings, three text ids
// Leaf: ??4CFireheartMinigameDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E19E2, 63 bytes, size-optimised TU (/Os def region, like the landed
//         0x004663AE / 0x004E05AD def assignments).
// Callees: CDefClassBase::operator= @ 0x00431F10 (the manifest label "CThingBuildingDef::operator=" is propagated
//          over every def base assignment), CCharString::operator= @ 0x0099EFB0.
// Identification: PDB CFireheartMinigameDef (0x3c) = CDefClassBase + MoonFX, SunFX (CCharString) +0x28/+0x2c,
//   QuitText, YesText, NoText +0x30..+0x38.
// Shape: memberwise; the strings go through their out-of-line operator=.
// The generated engine headers flatten the bases (no base operator= to call), so the classes are modelled locally.
#pragma optimize("s", on)

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)

class CCharString { public:
    void* m_pStringData;
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

// def vtable 0x0123E584: slot 0 0x004D9845 (implicit inline dtor); real overrides from the def vtable scan
class CFireheartMinigameDef : public CDefClassBase { public:
    CCharString MoonFX;                 // +0x28
    CCharString SunFX;                  // +0x2c
    unsigned long QuitText;             // +0x30
    unsigned long YesText;              // +0x34
    unsigned long NoText;               // +0x38
    CFireheartMinigameDef& operator=(const CFireheartMinigameDef& other);
    virtual void Transfer(CPersistContext& context);         // slot 18 0x004DF6F4
    virtual void Copy(const CDefClassBase* other);           // slot 19 0x004E19DD
    virtual unsigned long GetSizeofClass();                  // slot 20 0x004D665A
};

CFireheartMinigameDef& CFireheartMinigameDef::operator=(const CFireheartMinigameDef& other)
{
    CDefClassBase::operator=(other);
    MoonFX = other.MoonFX;
    SunFX = other.SunFX;
    QuitText = other.QuitText;
    YesText = other.YesText;
    NoText = other.NoText;
    return *this;
}