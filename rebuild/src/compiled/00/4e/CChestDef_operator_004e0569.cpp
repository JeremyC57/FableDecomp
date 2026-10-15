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

struct CDefClassBase {
    unsigned char _body[0x25];          // vptr, resource links, PDefManager +0x1c, GlobalIndex +0x20, flag bits +0x24
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};

struct CCharString {
    void* m_pStringData;
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

struct CChestDef : CDefClassBase {
    bool PersistOnOpening;              // +0x25
    long OpenParticleEffect;            // +0x28
    bool DisplayMessageOnEmpty;         // +0x2c
    CCharString OpenAnimationForCreature;// +0x30
    long OpenerObject;                  // +0x34
    long OpenersRequired;               // +0x38
    CChestDef& operator=(const CChestDef& other);
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