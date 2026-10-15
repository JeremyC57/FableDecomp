// CCreatureGeneratorDef::operator=(const CCreatureGeneratorDef&) -- def copy: CDefClassBase part, random-effects vector, sound criteria, two timers
// Leaf: ??4CCreatureGeneratorDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E2447, 51 bytes, size-optimised TU (/Os def region, like the landed
//         0x004663AE / 0x004E05AD def assignments).
// Callees: CDefClassBase::operator= @ 0x00431F10 (the manifest label "CThingBuildingDef::operator=" is propagated
//          over every def base assignment), std::vector<long>::operator= @ 0x00454886.
// Identification: PDB CCreatureGeneratorDef = CDefClassBase + RandomEffects (std::vector<long>) +0x28,
//   RandomSoundCriteria (CDefString) +0x34, SecondsBetweenEffects +0x38, SecondsBetweenSoundEffects +0x3c (retail offsets).
// Shape: memberwise; the vector goes through the out-of-line vector<long>::operator= 0x00454886.
// The generated engine headers flatten the bases (no base operator= to call), so the classes are modelled locally.
#pragma optimize("s", on)

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