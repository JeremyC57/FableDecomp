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

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)

struct CLongVector {                    // std::vector<long> (retail STLport, 12 bytes)
    long* m_pFirst;
    long* m_pLast;
    long* m_pEndOfStorage;
    CLongVector& operator=(const CLongVector& other);   // 0x00454886
};

// def vtable 0x01241384: slot 0 0x004E0529 (implicit inline dtor); real overrides from the def vtable scan
class CCreatureGeneratorDef : public CDefClassBase { public:
    CLongVector RandomEffects;          // +0x28
    long RandomSoundCriteria;           // +0x34
    long SecondsBetweenEffects;         // +0x38
    long SecondsBetweenSoundEffects;    // +0x3c
    CCreatureGeneratorDef& operator=(const CCreatureGeneratorDef& other);
    virtual void Transfer(CPersistContext& context);         // slot 18 0x0074C5F0
    virtual void Copy(const CDefClassBase* other);           // slot 19 0x004E2442
    virtual unsigned long GetSizeofClass();                  // slot 20 0x004DE200
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