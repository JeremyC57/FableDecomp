// CCreatureAction_TrollGroundPound::FinishAction() -- if the creature has the haste component (interface type 0x8c), remove this action from its set of speed sources and let it recompute the speed; then the base FinishAction
// Leaf: ?FinishAction@CCreatureAction_TrollGroundPound@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C7480, length 99 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for
// type 0x8c = bit 12 of the bitset dword at +0x30: `mov ecx,[eax+30h] ; test ch,10h`), the set erase-by-key @ 0x0058119B (std::set<CCreatureActionBase*>
// at +0x18 of the component: equal_range + erase; the manifest "CBankFile::CompareDataOffset::operator()" label is propagated), the component's
// speed recomputation @ 0x00800020 (between the CTCHaste bodies 0x007FFFB0/0x00800000; manifest "CAnimation_RecalculateScale" is propagated),
// CCombatActionBase::FinishAction @ 0x00859A20 (direct base call, last).
// Shape: the out-parameter GetInterface as the if (component in esi, pushed only inside); `this` is passed to the erase by const reference, so it is
// copied into the frame word the lookup key used (`mov [esp+0Ch],edi` after the `lea ; push`).
// No engine header for this class; modelled locally (CCombatActionBase is 0x114 bytes in retail).
class CTCBase;
class CCreatureActionBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

struct CActionSet {                         // std::set<CCreatureActionBase*>
    unsigned long erase(CCreatureActionBase* const& key);   // 0x0058119B
};
class CTCHaste { public:                           // interface type 0x8c component
    unsigned char _pad0[0x18];
    CActionSet SpeedSources;                // +0x18
    void RecalculateSpeed();                // 0x00800020
};

class CCreatureActionBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();            // slot 4
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
};
class CCombatActionBase : public CCreatureActionBase { public:   // retail sizeof 0x114
    unsigned char _pad10[0x114 - 0x10];     protected: // +0x10
    virtual void FinishAction(); public:            // 0x00859A20
};
class CCreatureAction_TrollGroundPound : public CCombatActionBase { public:
    virtual void FinishAction();
};

void CCreatureAction_TrollGroundPound::FinishAction()
{
    CTCHaste* pHaste;
    if (PCreature.Get()->GetTC(TCI_HASTE, &pHaste)) {
        pHaste->SpeedSources.erase(this);
        pHaste->RecalculateSpeed();
    }
    CCombatActionBase::FinishAction();
}