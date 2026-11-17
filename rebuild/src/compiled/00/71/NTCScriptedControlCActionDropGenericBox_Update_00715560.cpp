// NTCScriptedControl::CActionDropGenericBox::Update() -- once the drop action is plugged in, finish when the creature stops performing it
// (no action, or another action ID); otherwise set a CCreatureAction_DropGenericBox as the current action: on failure drop the box from the
// primary slot, leave mode 0x22 and finish; on success remember the action's ID
// Leaf: ?Update@CActionDropGenericBox@NTCScriptedControl@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00715560, 321 bytes; vtable 0x0128D7BC slot 1.
// Callees: CThingCreatureBase::IsPerformingAnyAction 0x006622A0, CCreatureActionBase::GetActionID 0x00692620 (PCurrentAction +0x100),
//   CCreatureAction_DropGenericBox ctor 0x00842150, CThingCreatureBase::SetCurrentAction 0x006644F0, ~CCreatureActionBase 0x00693EF0,
//   CVectorMap::LowerBound 0x0040F020 (the expanded GetTC for TCI_CARRYING 0x46 and TCI_ENTITY_MODE_MANAGER 0x31),
//   CTCCarrying::RemoveThingFromPrimarySlot 0x006A1E00, CTCCreatureModeManager::RemoveMode 0x0073ABA0.
// Identity (agentNF77, q10884): Ego_r.pdb 0x00728FC0 ?Update@CActionDropGenericBox@NTCScriptedControl@@UAEXXZ (+ vtable slot 1 of
//   .?AVCActionDropGenericBox@NTCScriptedControl@@, the base's pure Update slot); the self leaf was the local non-virtual QAEXXZ (catalog
//   label vfunc_1). The action temporary's destruction is ~CCreatureActionBase (??1CCreatureActionBase@@UAE@XZ, both PDBs): retail's
//   CCreatureAction_DropGenericBox destructor is implicit (engine/CCreatureAction_DropGenericBox.h GENERICBOX branch), formerly bound as a
//   local ??1ActionBase@Retail@@QAE@XZ.
// Layout: the new overlay header engine/NTCScriptedControl_CActionDropGenericBox.h (Ego_r.pdb ActionID +0x0C, ActionPluggedIn +0x10) on
//   engine/CTCScriptedControl.h (CActionBase: PTCScript +0x04, Terminated +0x09), engine/CThingCreatureBase.h, CCreatureActionBase.h,
//   CCreatureAction_DropGenericBox.h, CTCCarrying.h and CTCCreatureModeManager.h. Was a local Retail::ActionBase / Retail::DropGenericBox /
//   CActionDropGenericBox / Controller / Creature / CVectorMap / component model. GetParentCreature / Terminate are given their inline bodies
//   as in the landed sibling CActionPlayAnimation::Update 0x00712900; the current action is read through the inline
//   ?PeekPCurrentAction@CThingCreatureBase@@QBEPBVCCreatureActionBase@@XZ (Ego_r.pdb).
// Shape: the parent creature is re-read through PTCScript at every use; the failure path's GetTC calls leave the out pointer unset on a miss.
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
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_GETACTIONID
#define FABLE_RETAIL_CCREATUREACTION_GENERICBOX
#define FABLE_RETAIL_CTCCARRYING
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_STLP_LIST
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_DropGenericBox.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/NTCScriptedControl_CActionDropGenericBox.h"
inline CThingCreatureBase& CTCScriptedControl::GetParentCreature() { CThingCreatureBase& t=static_cast<CThingCreatureBase&>(GetParentThing()); return t; }
inline CThingCreatureBase& CTCScriptedControl::CActionBase::GetParentCreature() { return PTCScript->GetParentCreature(); }
inline void CTCScriptedControl::CActionBase::Terminate() { Terminated=true; }
namespace NTCScriptedControl {
void CActionDropGenericBox::Update()
{
    if (ActionPluggedIn) {
        if (!GetParentCreature().IsPerformingAnyAction() || GetParentCreature().PeekPCurrentAction()->GetActionID() != ActionID)
            Terminate();
    } else {
        if (!GetParentCreature().SetCurrentAction(CCreatureAction_DropGenericBox(GetParentCreature()))) {
            CTCCarrying* ptc_carrying;
            GetParentCreature().GetTC(TCI_CARRYING, &ptc_carrying);
            ptc_carrying->RemoveThingFromPrimarySlot(false);
            CTCCreatureModeManager* ptc_modes;
            GetParentCreature().GetTC(TCI_ENTITY_MODE_MANAGER, &ptc_modes);
            ptc_modes->RemoveMode(0x22);
            Terminate();
        } else {
            ActionPluggedIn = true;
            ActionID = GetParentCreature().PeekPCurrentAction()->GetActionID();
        }
    }
}
}
