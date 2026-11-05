// CCombatSequence_WillEnflame::OnStart() -- when the sequence's creature has a will-power indicator component, restart its recharge with Reset(-1.0f) (a negative time means the def's recharge time)
// Leaf: ?OnStart@CCombatSequence_WillEnflame@@UAEXXZ
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's CArray member Data is the header's CArray base (Ego_r: CVectorMap derives CArray<pair<K,V> >): Data.end() / begin() / size() read through the base; the local std::bitset operator[] / reference is the STLport model of stlport/fable_containers.h under FABLE_RETAIL_STLP_BITSET_INDEX (retail tests the word inline, e.g. 0x0062D032 test dword ptr [ebx+0x24],0x10000).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00909730, 71 bytes (+ int3 padding to 80), thiscall, plain `ret` (virtual).
// Identity (proven): CCombatSequence_WillEnflame::OnStart() (Ego_r.pdb ?OnStart@CCombatSequence_WillEnflame@@UAEXXZ); evidence: slot 17 of the CCombatSequence_WillEnflame vtable 0x0127A25C (RTTI; base CCombatSequenceBase)
// Label: the manifest's "CCombatSequence_WillEnflame::OnStart" (doubled class) names this row's own class; family J020: six byte-identical
//   will-power combat sequences (retail has no identical-code folding, so each address is one class's OnStart).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*, ...>::LowerBound 0x0040F020 (as J018), CTCAICreatureWillPowerIndicator::
//   Reset(float) 0x007E0E70 (?Reset@CTCAICreatureWillPowerIndicator@@QAEXM@Z, Ego_r.pdb / FableWin.pdb; unbound in the catalog,
//   the landed label agrees).
// Shape: CThing::GetTC (interface 0xCE = CTCAICreatureWillPowerIndicator, the landed GetInterfaceType 0x007E1140: bit 14 of
//   UsingComponents word 6, `test ch, 40h`) with its result tested. Layout: CCombatSequenceBase (Ego_r.pdb members; retail
//   0x28 bytes with the 12-byte STLport vector) Creature +0x1C. Local model: the generated engine/CCombatSequence_WillEnflame.h has the Ego_r
//   size (0x2C) and a data-member vptr.
#include <stddef.h>
#define FABLE_RETAIL_STLP_BITSET_INDEX
#include "engine/CVectorMap.h"



enum ETCInterfaceType { ETCInterfaceType_Force32 = 0x7fffffff };   // enumerators not needed (CTCAIScratchpad is 0xAA)
class CTCBase;
class CThing {
public:
    unsigned char      _pad00[0x20];
    std::bitset<274>   UsingComponents;                                   // +0x20
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > Components;   // +0x44
    template <class T> __forceinline bool GetTC(T** out)
    {
        if (UsingComponents[T::INTERFACE_TYPE]) {
            ETCInterfaceType type = (ETCInterfaceType)T::INTERFACE_TYPE;
            CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >* map = &Components;
            CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >::iterator it = map->LowerBound(type);
            if (it == map->end() || T::INTERFACE_TYPE < it->first)
                it = map->end();
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};


class CThingCreatureBase : public CThing { };
class CTCAICreatureWillPowerIndicator {
public:
    enum { INTERFACE_TYPE = 0xCE };
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
class CCombatSequence_WillEnflame : public CCombatSequenceBase {                // RTTI: CCombatSequence_WillEnflame <- CCombatSequenceBase
public:
    virtual void OnStart();
};

void CCombatSequence_WillEnflame::OnStart()
{
    CTCAICreatureWillPowerIndicator* indicator;
    if (Creature->GetTC(&indicator))
        indicator->Reset(-1.0f);
}