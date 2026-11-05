// CThingFilter_NoiseCameFromThing::operator()(CThing* thing) -- true when the thing's interface-0x77 component's intelligent pointer (+0x28) holds the filter's Thing
// Leaf: ??RCThingFilter_NoiseCameFromThing
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: map pointer made const (the row grades the const LowerBound).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00809210, length 82 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50.
// Layout: PDB CThingFilter_NoiseCameFromThing { CThing* Thing; } (+0); interface 0x77 -> bitset word 3 (+0x2C) bit 23
// (`test dword ptr [eax+2Ch],800000h`); the component holds a CIntelligentPointer at +0x28 (`lea ecx,[eax+28h]`).
// A second instantiation of the same filter shape as 0x006A81F0 (different component type).
class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


struct CTCInterface77 {                             // interface 0x77 component (class not established)
    unsigned char _pad[0x28];
    CIntelligentPointer<CThing> m_source;               // +0x28
};

class CThingFilter_NoiseCameFromThing { public:
    CThing* Thing;                                  // +0x00
    bool operator()(CThing* thing) const;
};

bool CThingFilter_NoiseCameFromThing::operator()(CThing* thing) const
{
    const CTCInterface77* component;
    if (thing->PeekTC(TCI_SHOT_DEFINITION, &component) && component->m_source.Get() == Thing)
        return true;
    return false;
}