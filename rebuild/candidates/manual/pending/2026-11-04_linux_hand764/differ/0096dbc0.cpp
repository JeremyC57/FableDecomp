// CCreatureAction_ScreamerIntoAggressive::Initialise() -- push a new NCreatureMode::CModeScreamerAggressive onto the creature's mode manager (built with the target when the target is alive, else creature-only), post message 0xC (flags 0xF) at the creature's position, set the INTO_COMBAT animation and run the combat base Initialise
// Grade: DIFFER(331v331) -- three-register rotation (retail this=ebp, &PTarget=ebx, temporary=esi; ours esi/ebp/ebx) and the mode-manager / new-result frame words swapped (+0x10 / +0x14); the length is retail's with the called CCountedPointer(T*) ctor overlay.
// Leaf: ?Initialise@CCreatureAction_ScreamerIntoAggressive@?A0x
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0096DBC0, 331 bytes (+ int3 padding). Ego_r.pdb
//   ?Initialise@CCreatureAction_ScreamerIntoAggressive@?A0x4c815337@@UAEXXZ (an anonymous-namespace class).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PCreature +8, PTarget +0xa8), CVectorMap::LowerBound 0x0040F020 (CThing::GetTC
//   <CTCCreatureModeManager>, TCI_ENTITY_MODE_MANAGER 0x31), operator new 0x00BFEA1A (0x84-byte mode), NCreatureMode::
//   CModeScreamerAggressive(CThing&, CThing&) 0x0096DD10 and (CThing&) 0x007412B0, CCountedPointer<CTCCreatureModeManager::CModeBase>
//   (CModeBase*) 0x00718A00, CTCCreatureModeManager::AddMode 0x0073ACC0, CCreatureActionBase::GetWorld 0x00692940 (then the inline
//   CWorld::GetMessageEventManager), CThing::GetPos 0x004C73D0, CMessageEventManager::AddMessage 0x00687540, CCharString ctor/dtor
//   0x0099EBF0 / 0x0099EAE0, CCombatActionBase::SetCombatAnimation 0x008574D0 and Initialise 0x00857520.
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE_ACCESSORS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_INITIALISE
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER_ADDMODEPOINTER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CWorld.h"
#include "engine/CTCCreatureModeManager.h"

bool __fastcall IS_THING_ALIVE(const CIntelligentPointer<CThing>& thing);   // 0x004CC340 (the inline form below is retail's: two GetPItem calls)

enum EMessageEventType { };
class CMessageEvent;
class CMessageEventManager { public:                       // no shared header
    CMessageEvent* AddMessage(EMessageEventType type, unsigned long flags, const C3DVector& pos, CThing* thing1, CThing* thing2,
                              float a, float b, long priority);   // 0x00687540 Ego_r ?AddMessage@CMessageEventManager@@QAEPAVCMessageEvent@@W4EMessageEventType@@KABVC3DVector@@PAVCThing@@2MMJ@Z
};

namespace NCreatureMode {
class CModeScreamerAggressive : public CTCCreatureModeManager::CModeBase { public:   // 0x84 bytes, vtable 0x012681EC
    CModeScreamerAggressive(CThing& creature, CThing& target);    // 0x0096DD10
    CModeScreamerAggressive(CThing& creature);                    // 0x007412B0
    virtual unsigned long GetType() const;
private:
    unsigned char _body[0x84 - 0x1c];
};
}

namespace {
class CCreatureAction_ScreamerIntoAggressive : public CCombatActionBase { public:
    virtual void Initialise();
};

void CCreatureAction_ScreamerIntoAggressive::Initialise()
{
    CTCCreatureModeManager* modeManager;
    PCreature->GetTC(&modeManager);
    modeManager->AddMode(CCountedPointer<CTCCreatureModeManager::CModeBase>((PTarget.GetPItem() && !PTarget->IsDead())
        ? new NCreatureMode::CModeScreamerAggressive(*PCreature.Get(), *PTarget.Get())
        : new NCreatureMode::CModeScreamerAggressive(*PCreature.Get())));
    CThing* pCreatureThing = PCreature.Get();
    CThing* pCreature = PCreature.Get();
    CMessageEventManager& manager = GetWorld().GetMessageEventManager();
    manager.AddMessage((EMessageEventType)0xc, 0xf, pCreature->GetPos(), pCreatureThing, PTarget.Get(), 0.0f, 0.0f, 0);
    SetCombatAnimation("INTO_COMBAT");
    CCombatActionBase::Initialise();
}
}
