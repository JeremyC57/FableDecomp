// CCombatAction_RecoverFromDazed::Initialise() -- resumes a player creature's suspended animation, removes mode 67 from its mode manager, then runs the base Initialise
// Leaf: ?Initialise@CCombatAction_RecoverFromDazed@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x008ED8C0, 144-byte slot (139 bytes of code). Initialise is vtable slot 12.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, three times through one `lea esi,[edi+8]`),
//          CThingAICreature::SuspendAI(bool, long) @ 0x00833A10, CVectorMap::LowerBound @ 0x0040F020 (inlined
//          GetInterface for type 0x31), CTCCreatureModeManager::RemoveMode @ 0x0073ABA0, CCombatActionBase::Initialise @ 0x00857520.
// Shape: the thing-type byte at +0x10 (1 = the player creature, as in the landed CModeKnockedDown::OnDeactivate)
// gates SuspendAI(false, 0); the GetInterface result is not tested, so RemoveMode + the base call are
// duplicated on both paths (the absent path reads the uninitialised out pointer from the `push ecx` frame word).
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThingAICreature { public:
    void SuspendAI(bool suspend, void* suspender);   // 0x00833A10 (Ego_r.pdb CThingAICreature::SuspendAI)
};

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
};

class CCombatAction_RecoverFromDazed : public CCombatActionBase { public:
    virtual void Initialise();
};

void CCombatAction_RecoverFromDazed::Initialise()
{
    if (PCreature.Get()->GetType() == 1) {
        CThing* pCreature = PCreature.Get();   // named: retail fetches it before the argument pushes
        ((CThingAICreature*)pCreature)->SuspendAI(false, 0);
    }
    CTCCreatureModeManager* pModes;
    PCreature.Get()->GetTC(TCI_ENTITY_MODE_MANAGER, &pModes);
    pModes->RemoveMode(67);
    CCombatActionBase::Initialise();
}