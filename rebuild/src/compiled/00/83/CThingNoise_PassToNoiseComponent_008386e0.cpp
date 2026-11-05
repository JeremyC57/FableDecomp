// CThingNoise::PassToNoiseComponent(CThing*) -- hand a thing to this thing's own noise component (interface 0x37)
// Leaf: PassToNoiseComponent
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008386E0, length 86 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (the inlined interface lookup,
// type 0x37 = bit 23 of the bitset dword at +0x24, tested with `test dword ptr [ecx+24h],800000h`), then
// the component's method @ 0x00749880 (it sits among CTCNoise's methods; the manifest's
// "CVillageTaskCarryCrate::AssignCreatureToTask" is a propagated name) taking the thing.
// Identity: the manifest label "CTCActionUseAcceptBoast::OnActionUse" is propagated over the byte-identical
// GetInterface shape. Here `this` IS the thing (the bitset is read straight off ecx); the body sits among
// CThingNoise's methods (0x00838550..0x008385B0) and is called on a live thing with another thing
// (CTCStealth::UpdateHearability 0x00747AF2, CTCShotDefinitionBase::OnCollisionProper 0x007C18C6), so it
// is modelled as a CThingNoise member; the method name is descriptive.
//
// Shape: the inlined GetInterface template (landed 0x00949320) with an UNINITIALISED component local: hit
// path -> the lookup result in eax; miss path -> the never-written local, homed in the argument's own
// stack slot (`mov eax,[esp+8]` beside the argument load). Frame: one `push ecx` (the key temporary).
class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCNoise { public:                               // interface type 0x37 component
    void TakeThing(CThing* thing);              // 0x00749880
};

class CThingNoise : public CThing { public:
    void PassToNoiseComponent(CThing* thing);
};

void CThingNoise::PassToNoiseComponent(CThing* thing)
{
    CTCNoise* noise;
    GetTC(TCI_NOISE, &noise);
    noise->TakeThing(thing);
}