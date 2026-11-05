// CCombatSequence_WillForcePush::OnStart() -- when the sequence's creature has a will-power indicator component, restart its recharge with Reset(-1.0f) (a negative time means the def's recharge time)
// Leaf: ?OnStart@CCombatSequence_WillForcePush@@UAEXXZ
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's CArray member Data is the header's CArray base (Ego_r: CVectorMap derives CArray<pair<K,V> >): Data.end() / begin() / size() read through the base; the local std::bitset operator[] / reference is the STLport model of stlport/fable_containers.h under FABLE_RETAIL_STLP_BITSET_INDEX (retail tests the word inline, e.g. 0x0062D032 test dword ptr [ebx+0x24],0x10000).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009099B0, 71 bytes (+ int3 padding to 80), thiscall, plain `ret` (virtual).
// Identity (proven): CCombatSequence_WillForcePush::OnStart() (Ego_r.pdb ?OnStart@CCombatSequence_WillForcePush@@UAEXXZ); evidence: slot 17 of the CCombatSequence_WillForcePush vtable 0x0127A2B4 (RTTI; base CCombatSequenceBase)
// Label: the manifest's "CCombatSequence_WillForcePush::OnStart" (doubled class) names this row's own class; family J020: six byte-identical
//   will-power combat sequences (retail has no identical-code folding, so each address is one class's OnStart).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*, ...>::LowerBound 0x0040F020 (as J018), CTCAICreatureWillPowerIndicator::
//   Reset(float) 0x007E0E70 (?Reset@CTCAICreatureWillPowerIndicator@@QAEXM@Z, Ego_r.pdb / FableWin.pdb; unbound in the catalog,
//   the landed label agrees).
// Shape: CThing::GetTC (interface 0xCE = CTCAICreatureWillPowerIndicator, the landed GetInterfaceType 0x007E1140: bit 14 of
//   UsingComponents word 6, `test ch, 40h`) with its result tested. Layout: CCombatSequenceBase (Ego_r.pdb members; retail
//   0x28 bytes with the 12-byte STLport vector) Creature +0x1C. Local model: the generated engine/CCombatSequence_WillForcePush.h has the Ego_r
//   size (0x2C) and a data-member vptr.
#include <stddef.h>
#define FABLE_RETAIL_STLP_BITSET_INDEX
#include "engine/CVectorMap.h"

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
class CTCBase;

class CThingCreatureBase : public CThing { };
class CTCAICreatureWillPowerIndicator {
public:
    void Reset(float time);                                  // 0x007E0E70
};
class CCombatAction;
class CCombatSequenceBase {                                  // retail 0x28 bytes
public:
    virtual ~CCombatSequenceBase();
    virtual void OnStart();                                  // retail slot 17
    bool                Running;                             // +0x04
    long                NoAvailableActions;                  // +0x08
    long                CurrentActionIndex;                  // +0x0C
    std::vector<CCombatAction, std::allocator<CCombatAction> > CombatActions;   // +0x10
    CThingCreatureBase* Creature;                            // +0x1C
    CThingCreatureBase* Target;                              // +0x20
    bool                CreateCalled;                        // +0x24
};
class CCombatSequence_WillForcePush : public CCombatSequenceBase {                // RTTI: CCombatSequence_WillForcePush <- CCombatSequenceBase
public:
    virtual void OnStart();
};

void CCombatSequence_WillForcePush::OnStart()
{
    CTCAICreatureWillPowerIndicator* indicator;
    if (Creature->GetTC(TCI_AICREATURE_WILL_POWER_INDICATOR, &indicator))
        indicator->Reset(-1.0f);
}