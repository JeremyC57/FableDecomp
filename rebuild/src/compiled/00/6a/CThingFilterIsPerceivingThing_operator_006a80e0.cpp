// CThingFilter_IsPerceivingThing::operator()(const CThing& thing) const -- whether the thing's perception component (kind 0x34) is perceiving the filter's thing
// Leaf: ??RCThingFilter_IsPerceivingThing
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's pointer members m_pFirst/m_pLast are the header CArray base's begin() / end() (STLport vector start +0 / finish +4).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A80E0, length 82 bytes.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (const instantiation), the component's
// perceived-thing getter @ 0x007446A0 (`add ecx,0Ch ; jmp CIntelligentPointer::Get`; manifest "_Cons_val<...>" propagated).
// Identity: the catalog's "CThingDeadCreature::WorldUpdateBulletTime" is a propagated label; two of the three callers sit in
// GetAllTCsInRoughArea<CTCPerceiveThing, CFilter_Bind2<CThingFilter_IsPerceivingThing, CThingFilter_IsNot>, ...>
// (0x0084BC0D, 0x0091BBF5) with ecx = the filter object; Ego_r.pdb has `??RCThingFilter_IsPerceivingThing@@QBE_NABVCThing@@@Z`.
// Layout (retail, local model): the thing's component-presence bitset at +0x20 (kind k = word k>>5, bit k&31) and the
// sorted {kind, component} vector map at +0x44 {first, last} (no engine header names them).
// Shape: the out-parameter GetInterface template on the const thing (the component comes back in eax: `mov eax,[eax+4]
// ; mov ecx,eax`), key homed in a frame/argument slot and passed by address to the const LowerBound.
// The component local is NOT initialised: when the bit is clear retail calls the getter on the stale argument slot
// (`mov eax,[esp+0Ch]`), exactly the landed CThingNoise::PassToNoiseComponent 0x008386E0 shape; the comparison with the
// filter's thing is `getter() == m_thing` as a full-register result (`sub eax,[edi] ; neg ; sbb eax,eax ; inc eax`).
class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef std::pair<ETCInterfaceType, CTCBase*> STCEntry;

typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMap;

class CTCPerceiveThing { public:                       // component kind 0x34
    const CThing* GetPerceivedThing() const;    // 0x007446A0
};

class CThingFilter_IsPerceivingThing { public:
    const CThing* m_thing;                      // +0x00
    bool operator()(const CThing& thing) const;
};

bool CThingFilter_IsPerceivingThing::operator()(const CThing& thing) const
{
    const CTCPerceiveThing* perceive;
    thing.PeekTC(TCI_PERCEIVE_THING, &perceive);
    return perceive->GetPerceivedThing() == m_thing;
}