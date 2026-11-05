// CCreatureAction_JackFloatUp::FinishAction() -- make sure Jack's mode manager holds mode 0x31 (floating), then the base FinishAction
// Leaf: ?FinishAction@CCreatureAction_JackFloatUp@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C1180, length 101 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (the inlined
// GetInterface lookup for type 0x31 = bit 17 of the presence word at +0x24), CTCCreatureModeManager::IsModeActive @ 0x0073A8A0,
// CTCCreatureModeManager::AddMode @ 0x0073AE10, CCombatActionBase::FinishAction @ 0x00859A20 (direct base call, last).
// Shape: the GetInterface result is NOT tested -- the absent path reads the uninitialised out pointer from the lookup-key
// frame word (`mov esi,[esp+8]`), the landed out-parameter GetInterface template (0x007504E0) reproduces that; the mode
// manager rides in esi for both calls. Retail CCombatActionBase is 0x114 bytes (the PDB header sits 0xc high), so `this`
// is modelled locally like the landed 0x008CB5C0.
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
    bool AddMode(unsigned long mode);       // 0x0073AE10
public:
    bool IsModeActive(unsigned long) const;   // 0x0073A8A0
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    protected: virtual void FinishAction(); public:            // slot 4: 0x00859A20
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
};

class CCreatureAction_JackFloatUp : public CCombatActionBase { public:
    virtual void FinishAction();
};

void CCreatureAction_JackFloatUp::FinishAction()
{
    CTCCreatureModeManager* modes;
    PCreature.Get()->GetTC(TCI_ENTITY_MODE_MANAGER, &modes);
    if (!modes->IsModeActive(0x31))
        modes->AddMode(0x31);
    CCombatActionBase::FinishAction();
}