// CThingFilter_IsInSameVolumeContextAsThing::CThingFilter_IsInSameVolumeContextAsThing(CThing* thing, bool override) -- constructor: Override from the argument, PThingInside = the thing's interface-0x75 component's intelligent-pointer item (+0x0C), else null
// Leaf: ??0CThingFilter_IsInSameVolumeContextAsThing
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: map pointer made const (the row grades the const LowerBound).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A8C00, length 87 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50.
// Layout: PDB CThingFilter_IsInSameVolumeContextAsThing { CThing* PThingInside; bool Override; } (8 bytes);
// interface 0x75 -> bitset word 3 (+0x2C) bit 21 (`test dword ptr [eax+2Ch],200000h`); the component holds an
// intelligent pointer at +0x0C (`lea ecx,[eax+0Ch]`).
// Shape: Override (+4) is stored before PThingInside (+0) is cleared -- the bool is an init-list member and the pointer
// is assigned in the body (zero first, then the component's item when present).
class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


struct CTCInterface75 {                             // interface 0x75 component (class not established)
    unsigned char _pad[0x0c];
    CIntelligentPointer<CThing> m_inside;               // +0x0c
};

class CThingFilter_IsInSameVolumeContextAsThing { public:
    CThing* PThingInside;                           // +0x00
    bool    Override;                               // +0x04
    CThingFilter_IsInSameVolumeContextAsThing(CThing* thing, bool override);
};

CThingFilter_IsInSameVolumeContextAsThing::CThingFilter_IsInSameVolumeContextAsThing(CThing* thing, bool override)
    : Override(override)
{
    PThingInside = 0;
    const CTCInterface75* component;
    if (thing->PeekTC(TCI_VOLUME_CONTAINMENT_TRACKER, &component))
        PThingInside = component->m_inside.Get();
}