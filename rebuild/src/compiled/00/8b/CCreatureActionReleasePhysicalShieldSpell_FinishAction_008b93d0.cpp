// CCreatureAction_ReleasePhysicalShieldSpell::FinishAction() -- if the world hands out a scripted camera manager, release its scripted control, then run the combat-action base FinishAction
// Leaf: ?FinishAction@CCreatureAction_ReleasePhysicalShieldSpell@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B93D0, length 45 bytes.
// Callees: CCreatureActionBase::GetWorld @ 0x00692940 (thiscall), CWorld::GetGameCamera (inline, PCurrentGameCamera +0x34),
// CAIGameCameraBase::GetCameraManager (slot 9, `call [eax+24h]`, one out-pointer), CGameCameraManager::ReleaseScriptedControl @ 0x00698950,
// CCombatActionBase::FinishAction @ 0x00859A20 (non-virtual qualified call).
// agentNF164 (q5102): ported onto the shared headers, as the landed sibling CCreatureAction_ReleaseTimeSpell::FinishAction 0x008CDD40 (same
//   camera release): CCreatureActionBase::GetWorld now binds the PDB spelling ?GetWorld@CCreatureActionBase@@IAEAAVCWorld@@XZ (Ego_r.pdb +
//   FableWin.pdb protected; was ?GetWorld@CCombatActionBase@@QAEPAVCWorld@@XZ on a local CCombatActionBase), CWorld / CAIGameCameraBase /
//   CGameCameraManager / CCombatActionBase from their headers; the class itself is the NF164 retail opt-in of
//   engine/CCreatureAction_ReleasePhysicalShieldSpell.h (RTTI base CCombatActionBase, no own data). Was local ICameraOwner / CWorld /
//   CGameCameraManager / CCombatActionBase / CCreatureAction_ReleasePhysicalShieldSpell models.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CGAMECAMERAMANAGER_RELEASESCRIPTEDCONTROL
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTIONBASE_ACCESSORS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_FINISHACTION
#define FABLE_RETAIL_CCREATUREACTION_RELEASEPHYSICALSHIELDSPELL
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CWorld.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CGameCameraManager.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_ReleasePhysicalShieldSpell.h"

void CCreatureAction_ReleasePhysicalShieldSpell::FinishAction()
{
    CGameCameraManager* camera_manager;
    if (GetWorld().GetGameCamera()->GetCameraManager(&camera_manager))
        camera_manager->ReleaseScriptedControl();
    CCombatActionBase::FinishAction();
}
