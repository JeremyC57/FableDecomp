// CThingFilter_IsPerceptible::operator()(CThing* thing) -- true when the thing has no interface-0x36 component, else when any of that component's three flag bytes (+0x0C, +0x0D, +0x0E) is set
// Leaf: ??RCThingFilter_IsPerceptible
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: map pointer made const (the row grades the const LowerBound).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A8450, length 94 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0.
// Layout: interface 0x36 -> bitset word 1 (+0x24) bit 22 (`test dword ptr [eax+24h],400000h`).
// Shape: an absent component returns the constant (`mov al,1`), the `a || b || c` chain is materialised as an int
// (`mov eax,1` / `xor eax,eax`); the present case is the `if` body so the constant tail lands last.
class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CTCInterface36 {                             // interface 0x36 component (class not established)
    unsigned char _pad[0x0c];
    bool m_flagA;                                   // +0x0c
    bool m_flagB;                                   // +0x0d
    bool m_flagC;                                   // +0x0e
};

class CThingFilter_IsPerceptible { public:
    bool operator()(CThing* thing) const;
};

bool CThingFilter_IsPerceptible::operator()(CThing* thing) const
{
    const CTCInterface36* component;
    if (thing->PeekTC(TCI_STEALTH, &component))
        return component->m_flagA || component->m_flagB || component->m_flagC;
    return true;
}