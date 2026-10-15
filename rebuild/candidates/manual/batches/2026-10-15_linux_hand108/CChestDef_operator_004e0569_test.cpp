#include <cstdio>
// CChestDef::operator=(const CChestDef&) -- def copy: CDefClassBase part, chest flags, open animation string, opener fields
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


CDefClassBase& CDefClassBase::operator=(const CDefClassBase& other) { for (int i = 4; i < 0x25; ++i) _body[i] = other._body[i]; return *this; }
CCharString& CCharString::operator=(const CCharString& other) { m_pStringData = other.m_pStringData; return *this; }

int main()
{
    static CChestDef a, b;
    for (int i = 4; i < 0x25; ++i) { b._body[i] = (unsigned char)(i * 3); a._body[i] = 0; }
    b.PersistOnOpening = true;
    b.OpenParticleEffect = 5;
    b.DisplayMessageOnEmpty = true;
    b.OpenAnimationForCreature.m_pStringData = (void*)&b;
    b.OpenerObject = 6;
    b.OpenersRequired = 2;
    CChestDef* r = &(a = b);
    if (r != &a) { std::printf("004e0569_TEST FAIL\n"); return 1; }
    for (int j = 4; j < 0x25; ++j) if (a._body[j] != (unsigned char)(j * 3)) { std::printf("%s_TEST FAIL\n", "004e0569"); return 1; }
    if (!(a.PersistOnOpening == b.PersistOnOpening)) { std::printf("004e0569_TEST FAIL\n"); return 1; }
    if (!(a.OpenParticleEffect == b.OpenParticleEffect)) { std::printf("004e0569_TEST FAIL\n"); return 1; }
    if (!(a.DisplayMessageOnEmpty == b.DisplayMessageOnEmpty)) { std::printf("004e0569_TEST FAIL\n"); return 1; }
    if (!(a.OpenAnimationForCreature.m_pStringData == b.OpenAnimationForCreature.m_pStringData)) { std::printf("004e0569_TEST FAIL\n"); return 1; }
    if (!(a.OpenerObject == b.OpenerObject)) { std::printf("004e0569_TEST FAIL\n"); return 1; }
    if (!(a.OpenersRequired == b.OpenersRequired)) { std::printf("004e0569_TEST FAIL\n"); return 1; }
    std::printf("004e0569_TEST PASS\n");
    return 0;
}
