// CCreatureAction_TentacleRise::FinishAction() -- remove creature mode 0x54 through the owner's mode manager, set the flag byte +0x10 of the owner's interface-9 component, base FinishAction, then make a CCreatureAction_TentacleIdle the owner's current action
// Leaf: ?FinishAction@CCreatureAction_TentacleRise@@QAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CF610, length 231 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0x31 =
// CTCCreatureModeManager, then 9), CTCCreatureModeManager::RemoveMode(unsigned long) @ 0x0073ABA0, CCombatActionBase::FinishAction
// @ 0x00859A20, CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030 (action code 50),
// CThingCreatureBase::SetCurrentAction @ 0x006644F0, ~CCombatActionBase @ 0x0062E900 (manifest "~CTCLightningOrb" is propagated).
// Shape: the checked mode-manager lookup (component left in eax), then the UNCHECKED interface-9 lookup in an inline accessor
// (absent arm reads the shared key word); the idle action is a 0x114-byte temporary built in the SetCurrentAction call (the
// landed CCreatureAction_WaspQueenDie::FinishAction 0x008C5370 shape): its inline constructor stores the vtable 0x01284DAC
// after the base call, the CSoundPair() temporary is zeroed through ecx after the first two pushes, and the owner for the
// call's `this` is fetched after the temporary is built.
// Layout: owner CIntelligentPointer +8.
#include "fable_compiler.h"
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCCreatureModeManager { public:
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
};
struct CTCInterface9 {                          // the owner's interface-9 component (descriptive)
    unsigned char _pad[0x10];
    bool          m_flag10;                     // +0x10
};

class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};

class CCombatActionBase;
class CThingCreatureBase : public CThing { public:
    bool SetCurrentAction(const CCombatActionBase& action);   // 0x006644F0
};

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


class CCombatActionBase { public:
    virtual void v00();
    unsigned char _pad04[4];                    // +0x04
    CIntelligentPointer<CThingCreatureBase> PCreature;          // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];         // +0x10
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
    virtual ~CCombatActionBase();                       // 0x0062E900
    void FinishAction();                        // 0x00859A20
    FABLE_FORCEINLINE CTCInterface9* GetInterface9() const
    {
        CTCInterface9* component;
        PCreature.Get()->GetTC(TCI_TARGETED, &component);
        return component;
    }
};

class CCreatureAction_TentacleIdle : public CCombatActionBase { public:   // vtable 0x01284DAC
    virtual void v00();
    CCreatureAction_TentacleIdle(CThingCreatureBase* owner) : CCombatActionBase(owner, 50, CSoundPair()) { }
};

class CCreatureAction_TentacleRise : public CCombatActionBase { public:
    void FinishAction();
};

void CCreatureAction_TentacleRise::FinishAction()
{
    CTCCreatureModeManager* modeManager;
    if (PCreature.Get()->GetTC(TCI_ENTITY_MODE_MANAGER, &modeManager))
        modeManager->RemoveMode(0x54);
    GetInterface9()->m_flag10 = true;
    CCombatActionBase::FinishAction();
    PCreature.Get()->SetCurrentAction(CCreatureAction_TentacleIdle(PCreature.Get()));
}