// CThingFilter_NoiseCameFromThing::operator()(CThing* thing) -- true when the thing's interface-0x37 component names the filter's source thing (component accessor == Thing)
// Leaf: ??RCThingFilter_NoiseCameFromThing
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: map pointer made const (the row grades the const LowerBound).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A81F0, length 79 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0, the component accessor @ 0x007498D0
// (manifest "_Cons_val<...>" is a propagated label; it returns the source thing).
// Layout: PDB CThingFilter_NoiseCameFromThing { CThing* Thing; } (+0); interface 0x37 -> bitset word 1 bit 23.
// Shape: `cmp eax,[edi]` -- the call result is the left operand; byte-constant exits.
class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CTCInterface37 {                             // interface 0x37 component (class not established)
    CThing* GetSourceThing() const;                       // 0x007498D0
};

class CThingFilter_NoiseCameFromThing { public:
    CThing* Thing;                                  // +0x00
    bool operator()(CThing* thing) const;
};

bool CThingFilter_NoiseCameFromThing::operator()(CThing* thing) const
{
    const CTCInterface37* component;
    if (thing->PeekTC(TCI_NOISE, &component) && component->GetSourceThing() == Thing)
        return true;
    return false;
}