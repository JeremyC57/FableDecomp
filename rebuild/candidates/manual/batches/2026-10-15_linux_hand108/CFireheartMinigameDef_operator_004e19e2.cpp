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

struct CDefClassBase {
    unsigned char _body[0x25];          // vptr, resource links, PDefManager +0x1c, GlobalIndex +0x20, flag bits +0x24
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};

struct CCharString {
    void* m_pStringData;
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

struct CFireheartMinigameDef : CDefClassBase {
    CCharString MoonFX;                 // +0x28
    CCharString SunFX;                  // +0x2c
    unsigned long QuitText;             // +0x30
    unsigned long YesText;              // +0x34
    unsigned long NoText;               // +0x38
    CFireheartMinigameDef& operator=(const CFireheartMinigameDef& other);
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
