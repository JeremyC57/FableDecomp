// CCombatAction_GenericBreakOffDrain::Initialise() -- picks the BREAK_OUT combat animation, removes mode 69 from the creature's mode manager, then runs the base Initialise
// Leaf: ?Initialise@CCombatAction_GenericBreakOffDrain@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x00867820, 144-byte slot (142 bytes of code). Initialise is vtable slot 12.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCombatActionBase::SetCombatAnimation(const CCharString&)
//          @ 0x008574D0 (manifest label "CTCInGameMenu::SetPDef" is propagated), CCharString::~CCharString @ 0x0099EAE0,
//          CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (inlined
//          GetInterface for type 0x31), CTCCreatureModeManager::RemoveMode @ 0x0073ABA0, CCombatActionBase::Initialise @ 0x00857520.
// Shape: the animation name is a named CCharString in its own block (`push -1 ; push lit` then `lea` re-addressing);
// its frame word is reused for the lookup key. The GetInterface result is NOT tested: RemoveMode runs on both
// paths (on the absent path with the uninitialised out pointer read from the frame -- the landed out-parameter
// template reproduces that garbage read), so the RemoveMode + base Initialise tail is duplicated per branch.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"

class CTCBase;

#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CTCCreatureModeManager { public:
    void RemoveMode(unsigned long mode);    // 0x0073ABA0
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    virtual void Initialise();              // slot 12: 0x00857520
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
    void SetCombatAnimation(const CCharString& name);   // 0x008574D0
};

class CCombatAction_GenericBreakOffDrain : public CCombatActionBase { public:
    virtual void Initialise();
};

void CCombatAction_GenericBreakOffDrain::Initialise()
{
    {
        CCharString name("BREAK_OUT", -1);
        SetCombatAnimation(name);
    }
    CTCCreatureModeManager* pModes;
    PCreature.Get()->GetTC(TCI_ENTITY_MODE_MANAGER, &pModes);
    pModes->RemoveMode(69);
    CCombatActionBase::Initialise();
}