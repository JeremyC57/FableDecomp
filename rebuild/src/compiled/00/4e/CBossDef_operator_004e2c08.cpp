// CBossDef::operator=(const CBossDef&) -- def copy: CDefClassBase part, three phase/shot vectors, two twin-blade scalars
// Leaf: ??4CBossDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E2C08, 69 bytes, size-optimised TU (/Os def region, like the landed
//         0x004663AE / 0x004E05AD def assignments).
// Callees: CDefClassBase::operator= @ 0x00431F10 (the manifest label "CThingBuildingDef::operator=" is propagated
//          over every def base assignment), std::vector<long>::operator= @ 0x00454886.
// Identification: PDB CBossDef = CDefClassBase + PhaseHealthPercent, Shots, TwinBladeMissesBeforeLunge (std::vector<long>)
//   +0x28/+0x34/+0x40, TwinBladeStuckLoops +0x4c, TwinBladeTimeBeforeLunge (float) +0x50 (retail offsets).
// Shape: memberwise; each vector goes through the out-of-line vector<long>::operator= 0x00454886.
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

// def vtable 0x0124185C: slot 0 0x004E0D62 (implicit inline dtor); real overrides from the def vtable scan
class CBossDef : public CDefClassBase { public:
    CLongVector PhaseHealthPercent;     // +0x28
    CLongVector Shots;                  // +0x34
    CLongVector TwinBladeMissesBeforeLunge;// +0x40
    long TwinBladeStuckLoops;           // +0x4c
    float TwinBladeTimeBeforeLunge;     // +0x50
    CBossDef& operator=(const CBossDef& other);
    virtual void Transfer(CPersistContext& context);         // slot 18 0x004E7EE0
    virtual void Copy(const CDefClassBase* other);           // slot 19 0x004E2C03
    virtual unsigned long GetSizeofClass();                  // slot 20 0x004DE8F1
};

CBossDef& CBossDef::operator=(const CBossDef& other)
{
    CDefClassBase::operator=(other);
    PhaseHealthPercent = other.PhaseHealthPercent;
    Shots = other.Shots;
    TwinBladeMissesBeforeLunge = other.TwinBladeMissesBeforeLunge;
    TwinBladeStuckLoops = other.TwinBladeStuckLoops;
    TwinBladeTimeBeforeLunge = other.TwinBladeTimeBeforeLunge;
    return *this;
}