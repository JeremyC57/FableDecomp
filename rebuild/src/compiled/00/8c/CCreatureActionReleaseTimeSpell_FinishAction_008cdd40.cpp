// CCreatureAction_ReleaseTimeSpell::FinishAction() -- end the time-spell release: the base combat FinishAction, hand the camera back
//   from scripted control, then either re-equip the item the action took out (a fast unsheathe carrying the haste key) when it still
//   lives, or register the creature's bullet-time multiplier as a haste speed multiplier under the haste key (adding a CTCHaste when
//   the creature has none)
// Leaf: ?FinishAction@CCreatureAction_ReleaseTimeSpell@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008CDD40, 430 bytes, thiscall, plain `ret` (virtual slot of the action vtable).
// Identity: Ego_r.pdb ?FinishAction@CCreatureAction_ReleaseTimeSpell@@UAEXXZ (Ego_r 0x00904B70); the catalog name.
// Callees: CCombatActionBase::FinishAction 0x00859A20, CCreatureActionBase::GetWorld 0x00692940, CWorld::GetGameCamera (inline,
//   PCurrentGameCamera +0x34), CAIGameCameraBase::GetCameraManager (slot 9, +0x24), CGameCameraManager::ReleaseScriptedControl
//   0x00698950, CBaseIntelligentPointer::GetPItem 0x00A01B50 (PItemRemoved / PCreature), CCharString(const char*, long) 0x0099EBF0 /
//   ~ 0x0099EAE0, the CCreatureAction_UnsheatheItemFromInventoryWithHasteAction ctor 0x0069F1F0 and its implicit dtor
//   (~CCreatureAction_UnsheatheItemFromInventory 0x005CC091), CThingCreatureBase::SetCurrentAction 0x006644F0, the CVectorMap
//   LowerBound const 0x004365B0 / non-const 0x0040F020 (PeekTC<CTCBulletTime> / GetTC<CTCHaste>), CThing::AddTC 0x004C9D60,
//   CTCBulletTime::GetTimeMultiplier 0x0079C740, map<void*,float>::insert 0x00580D21 and CTCHaste::UpdateActionSpeedMultiplier
//   0x00800020 (the inline CTCHaste::SetActionSpeedMultiplier).
// Shape: the haste tail is the landed sibling CCreatureAction_UnsheatheItemFromInventoryWithHasteAction::FinishAction 0x006A02B0's
//   (TC 146 = TCI_BULLET_TIME, not a physics TC: the earlier draft's CTCPhysicsBase::GetAirResistance spelling was wrong).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_IS_THING_ALIVE
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
#define FABLE_RETAIL_CCREATUREACTION_UNSHEATHEHASTE
#define FABLE_RETAIL_CCREATUREACTION_UNSHEATHEHASTE_CTOR
#define FABLE_RETAIL_CCREATUREACTION_RELEASETIMESPELL
#define FABLE_RETAIL_CTCHASTE_MAKE_PAIR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define FABLE_RETAIL_CTCBULLETTIME
#define FABLE_RETAIL_CTCHASTE
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/ThingAlive.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CWorld.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CGameCameraManager.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CTCBulletTime.h"
#include "engine/CTCHaste.h"
#include "engine/CCreatureAction_UnsheatheItemFromInventoryWithHasteAction.h"
#include "engine/CCreatureAction_ReleaseTimeSpell.h"

void CCreatureAction_ReleaseTimeSpell::FinishAction()
{
    CCombatActionBase::FinishAction();
    CGameCameraManager* camera_manager;
    if (GetWorld().GetGameCamera()->GetCameraManager(&camera_manager))
        camera_manager->ReleaseScriptedControl();
    if (IS_THING_ALIVE(PItemRemoved)) {
        CCharString animation("ST_ABILITY_FAST_UNSHEATHE_TO_COMBAT_POSE");
        PCreature->SetCurrentAction(CCreatureAction_UnsheatheItemFromInventoryWithHasteAction(HasteKey, *PCreature, animation,
                                                                                              *PItemRemoved, 100, 250, 0));
    } else {
        const CTCBulletTime* tctime = 0;
        if (PCreature->PeekTC(&tctime)) {
            CTCHaste* ptc_haste = 0;
            if (!PCreature->GetTC(&ptc_haste))
                ptc_haste = GFDebugPDynamicCast(PCreature->AddTC("CTCHaste", true, 0), (CTCHaste*)0);
            ptc_haste->SetActionSpeedMultiplier(HasteKey, tctime->GetTimeMultiplier());
        }
    }
}