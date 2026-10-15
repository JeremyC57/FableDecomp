#include <cstdio>
// CCreatureGeneratorDef::operator=(const CCreatureGeneratorDef&) -- def copy: CDefClassBase part, random-effects vector, sound criteria, two timers
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E2447, 51 bytes, size-optimised TU (/Os def region, like the landed
//         0x004663AE / 0x004E05AD def assignments).
// Callees: CDefClassBase::operator= @ 0x00431F10 (the manifest label "CThingBuildingDef::operator=" is propagated
//          over every def base assignment), std::vector<long>::operator= @ 0x00454886.
// Identification: PDB CCreatureGeneratorDef = CDefClassBase + RandomEffects (std::vector<long>) +0x28,
//   RandomSoundCriteria (CDefString) +0x34, SecondsBetweenEffects +0x38, SecondsBetweenSoundEffects +0x3c (retail offsets).
// Shape: memberwise; the vector goes through the out-of-line vector<long>::operator= 0x00454886.
// The generated engine headers flatten the bases (no base operator= to call), so the classes are modelled locally.

struct CDefClassBase {
    unsigned char _body[0x25];          // vptr, resource links, PDefManager +0x1c, GlobalIndex +0x20, flag bits +0x24
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};

struct CLongVector {                    // std::vector<long> (retail STLport, 12 bytes)
    long* m_pFirst;
    long* m_pLast;
    long* m_pEndOfStorage;
    CLongVector& operator=(const CLongVector& other);   // 0x00454886
};

struct CCreatureGeneratorDef : CDefClassBase {
    CLongVector RandomEffects;          // +0x28
    long RandomSoundCriteria;           // +0x34
    long SecondsBetweenEffects;         // +0x38
    long SecondsBetweenSoundEffects;    // +0x3c
    CCreatureGeneratorDef& operator=(const CCreatureGeneratorDef& other);
};

CCreatureGeneratorDef& CCreatureGeneratorDef::operator=(const CCreatureGeneratorDef& other)
{
    CDefClassBase::operator=(other);
    RandomEffects = other.RandomEffects;
    RandomSoundCriteria = other.RandomSoundCriteria;
    SecondsBetweenEffects = other.SecondsBetweenEffects;
    SecondsBetweenSoundEffects = other.SecondsBetweenSoundEffects;
    return *this;
}


CDefClassBase& CDefClassBase::operator=(const CDefClassBase& other) { for (int i = 4; i < 0x25; ++i) _body[i] = other._body[i]; return *this; }
CLongVector& CLongVector::operator=(const CLongVector& other) { m_pFirst = other.m_pFirst; m_pLast = other.m_pLast; m_pEndOfStorage = other.m_pEndOfStorage; return *this; }

int main()
{
    static CCreatureGeneratorDef a, b;
    for (int i = 4; i < 0x25; ++i) { b._body[i] = (unsigned char)(i * 3); a._body[i] = 0; }
    b.RandomEffects.m_pFirst = (long*)&b;
    b.RandomEffects.m_pLast = (long*)&b + 1;
    b.RandomSoundCriteria = 9;
    b.SecondsBetweenEffects = 10;
    b.SecondsBetweenSoundEffects = 20;
    CCreatureGeneratorDef* r = &(a = b);
    if (r != &a) { std::printf("004e2447_TEST FAIL\n"); return 1; }
    for (int j = 4; j < 0x25; ++j) if (a._body[j] != (unsigned char)(j * 3)) { std::printf("%s_TEST FAIL\n", "004e2447"); return 1; }
    if (!(a.RandomEffects.m_pFirst == b.RandomEffects.m_pFirst)) { std::printf("004e2447_TEST FAIL\n"); return 1; }
    if (!(a.RandomEffects.m_pLast == b.RandomEffects.m_pLast)) { std::printf("004e2447_TEST FAIL\n"); return 1; }
    if (!(a.RandomSoundCriteria == b.RandomSoundCriteria)) { std::printf("004e2447_TEST FAIL\n"); return 1; }
    if (!(a.SecondsBetweenEffects == b.SecondsBetweenEffects)) { std::printf("004e2447_TEST FAIL\n"); return 1; }
    if (!(a.SecondsBetweenSoundEffects == b.SecondsBetweenSoundEffects)) { std::printf("004e2447_TEST FAIL\n"); return 1; }
    std::printf("004e2447_TEST PASS\n");
    return 0;
}