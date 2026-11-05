// CCreatureAction_JackFall::FinishAction() -- drop mode 0x33 from Jack's mode manager if set, base FinishAction, then make "go underground" the creature's current action
// Leaf: ?FinishAction@CCreatureAction_JackFall@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C2490, length 160 bytes.
// Model: rebuild/include/engine/CCreatureAction_JackFall.h is the flattened PDB layout and sits high against the bytes (CCombatActionBase is 0x12c bytes in the PDB, 0x114 in retail: retail containers are 12 bytes), so `this` is a local model with the retail offsets.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, three times), CVectorMap::LowerBound @ 0x0040F020 (inlined
// GetInterface for type 0x31), CTCCreatureModeManager::HasMode @ 0x0073A8A0, CTCCreatureModeManager::RemoveMode @ 0x0073ABA0,
// CCombatActionBase::FinishAction @ 0x00859A20 (direct base call), CCreatureAction_GoUnderground::CCreatureAction_GoUnderground(CThingCreatureBase*)
// @ 0x008C3810, CThingCreatureBase::SetCurrentAction(CCreatureActionBase*) @ 0x006644F0, CCombatActionBase::~CCombatActionBase @ 0x0062E900.
// Shape: frame 0x118 = the lookup key word + the 0x114-byte action temporary built inside the call (`&CAction(creature)`, the landed
// 0x0094DBC0 PerformActionAndWait shape); the creature handed to the constructor is fetched BEFORE the base FinishAction (named local in
// esi), the creature whose action is set after it (chained, in edi). GetInterface's result is ignored (absent arm reads the key's word).
// The temporary is destroyed through the base destructor: GoUnderground declares none of its own.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CTCCreatureModeManager { public:             // interface type 0x31
    bool HasMode(unsigned long mode);       // 0x0073A8A0
    void RemoveMode(unsigned long mode);    // 0x0073ABA0
};

class CThingCreatureBase;
class CCreatureActionBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();            // slot 4
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
};
class CThingCreatureBase { public:
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};
class CCombatActionBase : public CCreatureActionBase { public:          // retail sizeof 0x114
    unsigned char _combat[0x114 - 0xa8];    // +0xa8
    virtual ~CCombatActionBase();                   protected: // 0x0062E900
    virtual void FinishAction(); public:            // 0x00859A20
    CThingCreatureBase* GetCreature() const { return (CThingCreatureBase*)PCreature.Get(); }
};
namespace N_008c3810 { struct CCreatureAction_GoUnderground : CCombatActionBase {
    CCreatureAction_GoUnderground(CThingCreatureBase* creature);   // 0x008C3810
}; } using N_008c3810::CCreatureAction_GoUnderground;
class CCreatureAction_JackFall : public CCombatActionBase { public:
    virtual void FinishAction();
};

void CCreatureAction_JackFall::FinishAction()
{
    CTCCreatureModeManager* modes;
    PCreature.Get()->GetTC(TCI_ENTITY_MODE_MANAGER, &modes);
    if (modes->HasMode(0x33))
        modes->RemoveMode(0x33);
    CThingCreatureBase* creature = GetCreature();
    CCombatActionBase::FinishAction();
    GetCreature()->SetCurrentAction(CCreatureAction_GoUnderground(creature));
}