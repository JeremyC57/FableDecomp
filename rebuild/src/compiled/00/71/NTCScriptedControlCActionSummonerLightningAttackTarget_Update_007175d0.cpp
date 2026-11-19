// NTCScriptedControl::CActionSummonerLightningAttackTarget::Update() -- turn the summoner to face the target, then hand it a lightning-orb attack on the target (finishing at once if it is already in the orb mode 90 or the target is dead), and finish once the creature is idle and out of the orb mode.
// Leaf: ?Update@CActionSummonerLightningAttackTarget@NTCScriptedControl@@UAEXXZ
// Flags: none
// Retail: 0x007175D0, 318 bytes, thiscall, plain `ret`; slot 1 of vtable 0x0126586C.
// Shape: FableWin.pdb tc_scripted_control.cpp lines 2638..2690 (FableWin.exe 0x025675E0) -- two blocks, each with its own
//   `CThingCreatureBase& parent_creature = GetParentCreature();` and `const CTCCreatureModeManager* ptc_cmm;` (S_BLOCK32 records; Ego_r.pdb
//   keeps ptc_cmm). Block 1: PeekTC then IsModeActive(0x5A) (line 2658's debug-only check of the same call emits nothing here),
//   PTarget.IsAlive(), SetCurrentAction(CCreatureAction_SummonerLightningOrbInto(parent_creature, *PTarget.GetPThing())). Block 2:
//   PeekTC whose result is not tested (line 2683: an assert of the call, evaluated in release -- retail's absent path reads the
//   unwritten ptc_cmm word, 0x007176DD), then IsPerformingAnyAction / GetCurrentMode().GetType(). IsTerminated / Terminate /
//   GetParentCreature are the inline CActionBase accessors (retail reads +0x25 / writes +9 / reads PTCScript->ParentThing in place).
// Callees: CActionTurnToFacePosition::Update (virtual slot 1), CVectorMap LowerBound 0x004365B0 (PeekTC<const CTCCreatureModeManager>,
//   inline), CTCCreatureModeManager::IsModeActive 0x0073A8A0 / GetCurrentMode 0x0073A6E0, CModeBase::GetType (virtual slot 1),
//   CScriptThing::IsAlive (slot 75) / GetPThing (slot 11), CCreatureAction_SummonerLightningOrbInto ctor 0x008CBD90,
//   CThingCreatureBase::SetCurrentAction 0x006644F0 / IsPerformingAnyAction 0x006622A0, ~CBaseIntelligentPointer 0x00A01C10,
//   ~CCombatActionBase 0x0062E900 (the temporary's implicit destructor).
// New header opt-ins: FABLE_RETAIL_CACTIONSUMMONERLIGHTNINGATTACKTARGET_PTARGET (PTarget as CScriptThing),
//   FABLE_RETAIL_CCREATUREACTION_SUMMONERLIGHTNINGORBINTO_CLASS (the class over CCombatActionBase with POrb +0x114).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONBASE
#define FABLE_RETAIL_CTCSCRIPTEDCONTROL_CREATURE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CACTIONSUMMONERLIGHTNINGATTACKTARGET_PTARGET
#define FABLE_RETAIL_CCREATUREACTION_SUMMONERLIGHTNINGORBINTO_CLASS
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_SummonerLightningOrbInto.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/NTCScriptedControl_CActionSummonerLightningAttackTarget.h"
inline CThingCreatureBase& CTCScriptedControl::GetParentCreature() { CThingCreatureBase& t=static_cast<CThingCreatureBase&>(GetParentThing()); return t; }
inline CThingCreatureBase& CTCScriptedControl::CActionBase::GetParentCreature() { return PTCScript->GetParentCreature(); }
inline bool CTCScriptedControl::CActionBase::IsTerminated() const { return Terminated; }
inline void CTCScriptedControl::CActionBase::Terminate() { Terminated=true; }
namespace NTCScriptedControl {
void CActionSummonerLightningAttackTarget::Update()
{
    if (!TurnedToFace) {
        TurnToFaceAction.Update();
        if (!TurnToFaceAction.IsTerminated())
            return;
        TurnedToFace = true;
    }
    if (!StartedAttack) {
        CThingCreatureBase& parent_creature = GetParentCreature();
        const CTCCreatureModeManager* ptc_cmm;
        if (parent_creature.PeekTC(&ptc_cmm)) {
            if (ptc_cmm->IsModeActive(90)) {
                Terminate();
                return;
            }
        }
        if (!PTarget.IsAlive()) {
            Terminate();
            return;
        }
        if (parent_creature.SetCurrentAction(CCreatureAction_SummonerLightningOrbInto(parent_creature, *PTarget.GetPThing())))
            StartedAttack = true;
    }
    if (StartedAttack) {
        CThingCreatureBase& parent_creature = GetParentCreature();
        const CTCCreatureModeManager* ptc_cmm;
        parent_creature.PeekTC(&ptc_cmm);
        if (!parent_creature.IsPerformingAnyAction() && ptc_cmm->GetCurrentMode().GetType() != 90)
            Terminate();
    }
}
}