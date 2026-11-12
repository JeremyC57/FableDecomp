// CGameScriptInterface::EntitySetAsSittingOnFloor(const CScriptThing& thing, bool sit) const -- for a live creature: when sit is set and it is not already in the sit-on-floor mode, finish its action and start a sit-down action with a random ST_SIT_ON_FLOOR animation that leaves it in a new floor-sitting mode of that animation's group; when sit is clear and it is sitting on the floor, finish its action and start the ST_STAND_UP_FROM_FLOOR get-up action
// Leaf: ?EntitySetAsSittingOnFloor@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z
// Inline-budget: FABLE_RETAIL_CDEFSTRING_FORCEINLINE_CONVERSION (engine/CDefString.h opt-in) -- retail expands the inline
//   CDefString -> CCharString forwarder here and calls GetString 0x00415DD0; plain inline makes VC7.1 call the forwarder's own
//   COMDAT (??B 0x00415D70, a different relocation), so the forwarder is forced inline and GetString kept a visible noinline body.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008A8C60, 470 bytes (+ int3 padding), thiscall `ret 8`. Identity: retail
//   CGameScriptInterface vtable 0x01260F0C slot 493 (engine/CGameScriptInterface.h); Ego_r.pdb / FableWin.pdb
//   ?EntitySetAsSittingOnFloor@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z.
// Callees (PDB spellings): CScriptThing slots 75 IsAlive / 11 GetPThing, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const
//   0x004365B0 (PeekTC, TCI_ENTITY_MODE_MANAGER 49), CTCCreatureModeManager::GetCurrentMode() const 0x0073A6E0 (CModeBase slot 1
//   GetType), CThingCreatureBase::FinishCurrentAction 0x00662650 / GetRandomAnimFromNameWithFilter 0x00662EF0 / SetCurrentAction
//   0x006644F0, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CDefString::GetString 0x00415DD0, operator new
//   0x00BFEA1A, NCreatureMode::CModeSitFloor ctor 0x008AC3C0, CCountedPointer<NCreatureMode::CModeSitFloor> ctor 0x008AB380 /
//   dtor 0x008AB3D0, CCreatureAction_SitDown ctor 0x008AB9D0 / dtor 0x008ABAA0, CCreatureAction_GetUpFromSitting ctor 0x00860150,
//   ~CCombatActionBase 0x0062E900; CAnimFilterAnyGroup vtable 0x0127261C (inline ctor / dtor).
// Model: shared headers (overlay: new CCreatureAction_GetUpFromSitting.h, CAnimFilterAnyGroup.h, NCreatureMode_CModeSitFloor.h;
//   opt-ins CCREATUREACTION_SITDOWN, CTHINGCREATUREBASE_ANIMFILTER, CCOUNTEDPOINTER_VISIBLE_CALLED_DTOR; hdr/README.md).
// Shape: the mode-manager pointer is uninitialised (the absent-component path reads the stale slot, as retail 0x008A8CD6); the
//   sit mode's group name is the implicit CDefString -> CCharString conversion, its forwarder expanded around the out-of-line
//   GetString (retail takes the temporary's address after the call, 0x008A8D5E, and sets the conditional-destroy flag after it;
//   an explicit GetString() pushes the call's eax and sets the flag first); the CCountedPointer<CModeSitFloor> ctor
//   and dtor are retail's same-TU COMDATs (0x008AB380 / 0x008AB3D0) called, not expanded, with visible bodies (VISIBLE_CALLED_CTOR /
//   _DTOR): only then does VC7.1 pack the sit mode into the dead name temporary's slot (frame 0x12C, not 0x130) and copy it into
//   the by-value argument without re-reading Info (local-model park: DIFFER(472v472) frame slots).
// Name fix (agentNF27, 2026-11-12, RELOC_CONFLICT land1011dn): the floor mode is held in a CCountedPointer<NCreatureMode::CModeSitFloor>
//   (0x008AB380 / 0x008AB3D0 store / free DeleteData<CModeSitFloor> 0x008AA810; CCountedPointer<CModeSitBase>'s ctor and dtor are
//   0x0094B3A0 / 0x008AB320); SitDown's by-value CCountedPointer<CModeSitBase> is built by the header's converting member template
//   (FableWin.pdb ??$?0VCModeSitFloor@NCreatureMode@@@?$CCountedPointer@VCModeSitBase@NCreatureMode@@@@QAE@ABV?$CCountedPointer@VCModeSitFloor@NCreatureMode@@@@@Z).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_FINISHACTION
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMFILTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_SITDOWN
#define FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_DTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_FORCEINLINE_CONVERSION
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/ETCInterfaceType.h"
#include "engine/NCreatureMode.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/NCreatureMode_CModeSitFloor.h"
#include "engine/CAnimationEntry.h"
#include "engine/CAnimFilterAnyGroup.h"
#include "engine/CCreatureAction_SitDown.h"
#include "engine/CCreatureAction_GetUpFromSitting.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CScriptThing.h"

void CGameScriptInterface::EntitySetAsSittingOnFloor(const CScriptThing& thing, bool sit) const
{
    if (!thing.IsAlive() || !(thing.GetPThing()->GetTypeFlags() & 8))
        return;
    CThingCreatureBase* creature = static_cast<CThingCreatureBase*>(thing.GetPThing());
    const CTCCreatureModeManager* modeManager;
    creature->PeekTC(&modeManager);
    if (sit) {
        if (modeManager->GetCurrentMode().GetType() == NCreatureMode::MODE_SIT_FLOOR)
            return;
        creature->FinishCurrentAction();
        const CAnimationEntry& anim = creature->GetRandomAnimFromNameWithFilter("ST_SIT_ON_FLOOR", CAnimFilterAnyGroup());
        CCountedPointer<NCreatureMode::CModeSitFloor> sitMode(new NCreatureMode::CModeSitFloor(*creature, anim.GroupName));
        creature->SetCurrentAction(CCreatureAction_SitDown(*creature, anim, sitMode));
    }
    else {
        if (modeManager->GetCurrentMode().GetType() != NCreatureMode::MODE_SIT_FLOOR)
            return;
        creature->FinishCurrentAction();
        creature->SetCurrentAction(CCreatureAction_GetUpFromSitting(*creature, "ST_STAND_UP_FROM_FLOOR", NCreatureMode::MODE_SIT_FLOOR, false));
    }
}