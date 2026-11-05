// CThingFilter_IsAlliedWithFaction::operator()(const CThing& thing) const -- whether the thing's enemy component (kind 0x49) says it is allied with the filter's faction; false without one
// Leaf: ??RCThingFilter_IsAlliedWithFaction
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's pointer members m_pFirst/m_pLast are the header CArray base's begin() / end() (STLport vector start +0 / finish +4).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C8C50, length 77 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (const instantiation),
// CTCEnemy::IsAlliedWithFaction @ 0x0076C7F0 (tail-jumped with the filter's faction reusing the argument slot).
// Identity: the catalog's "CThingCreatureBase::IsBlocking" is a propagated label; the two callers are
// GetPBestThing<CFilter_Bind2<CThingFilter_IsNot, CThingFilter_IsAlliedWithFaction>, ...> (0x00986DF5) and
// CAIStateGroup_OpinionReactionRoot::MarriageCommentIsRunnable, both with ecx = the filter object (`ret 4` __fastcall).
// Layout (retail, local model): the thing's component-presence bitset at +0x20 (kind k = word k>>5, bit k&31) and the
// sorted {kind, component} vector map at +0x44 {first, last} (no engine header names them).
// Shape: the out-parameter GetInterface template on the const thing (the component comes back in eax: `mov eax,[eax+4]
// ; mov ecx,eax`), key homed in a frame/argument slot and passed by address to the const LowerBound.
// The filter's faction is read into ecx before the component (`mov ecx,[edi]`), then stored over the argument slot for the
// tail jump: `return enemy->IsAlliedWithFaction(m_faction);` as the last statement; kind 0x49 = `test ch,2` on word 2.
class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef std::pair<ETCInterfaceType, CTCBase*> STCEntry;

typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMap;

class CTCEnemy { public:                               // component kind 0x49
    bool IsAlliedWithFaction(long faction) const;   // 0x0076C7F0
};

class CThingFilter_IsAlliedWithFaction { public:
    long m_faction;                             // +0x00
    bool operator()(const CThing& thing) const;
};

bool CThingFilter_IsAlliedWithFaction::operator()(const CThing& thing) const
{
    const CTCEnemy* enemy;
    if (thing.PeekTC(TCI_ENEMY, &enemy))
        return enemy->IsAlliedWithFaction(m_faction);
    return false;
}