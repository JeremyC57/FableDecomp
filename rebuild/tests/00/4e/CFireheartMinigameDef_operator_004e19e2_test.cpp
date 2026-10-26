#include <cstdio>
// CFireheartMinigameDef::operator=(const CFireheartMinigameDef&) -- def copy: CDefClassBase part, two effect-name strings, three text ids
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E19E2, 63 bytes, size-optimised TU (/Os def region, like the landed
//         0x004663AE / 0x004E05AD def assignments).
// Callees: CDefClassBase::operator= @ 0x00431F10 (the manifest label "CThingBuildingDef::operator=" is propagated
//          over every def base assignment), CCharString::operator= @ 0x0099EFB0.
// Identification: PDB CFireheartMinigameDef (0x3c) = CDefClassBase + MoonFX, SunFX (CCharString) +0x28/+0x2c,
//   QuitText, YesText, NoText +0x30..+0x38.
// Shape: memberwise; the strings go through their out-of-line operator=.
// The generated engine headers flatten the bases (no base operator= to call), so the classes are modelled locally.

class CDefClassBase { public:
    unsigned char _body[0x25];          // vptr, resource links, PDefManager +0x1c, GlobalIndex +0x20, flag bits +0x24
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};

class CCharString { public:
    void* m_pStringData;
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

class CFireheartMinigameDef : public CDefClassBase { public:
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


CDefClassBase& CDefClassBase::operator=(const CDefClassBase& other) { for (int i = 4; i < 0x25; ++i) _body[i] = other._body[i]; return *this; }
CCharString& CCharString::operator=(const CCharString& other) { m_pStringData = other.m_pStringData; return *this; }

int main()
{
    static CFireheartMinigameDef a, b;
    for (int i = 4; i < 0x25; ++i) { b._body[i] = (unsigned char)(i * 3); a._body[i] = 0; }
    b.MoonFX.m_pStringData = (void*)&a;
    b.SunFX.m_pStringData = (void*)&b;
    b.QuitText = 1;
    b.YesText = 2;
    b.NoText = 3;
    CFireheartMinigameDef* r = &(a = b);
    if (r != &a) { std::printf("004e19e2_TEST FAIL\n"); return 1; }
    for (int j = 4; j < 0x25; ++j) if (a._body[j] != (unsigned char)(j * 3)) { std::printf("%s_TEST FAIL\n", "004e19e2"); return 1; }
    if (!(a.MoonFX.m_pStringData == b.MoonFX.m_pStringData)) { std::printf("004e19e2_TEST FAIL\n"); return 1; }
    if (!(a.SunFX.m_pStringData == b.SunFX.m_pStringData)) { std::printf("004e19e2_TEST FAIL\n"); return 1; }
    if (!(a.QuitText == b.QuitText)) { std::printf("004e19e2_TEST FAIL\n"); return 1; }
    if (!(a.YesText == b.YesText)) { std::printf("004e19e2_TEST FAIL\n"); return 1; }
    if (!(a.NoText == b.NoText)) { std::printf("004e19e2_TEST FAIL\n"); return 1; }
    std::printf("004e19e2_TEST PASS\n");
    return 0;
}