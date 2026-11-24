// CAIStateGroup_ScreamerPursueTarget::OnReplace() -- if the target is alive, flag it (CTCStealth::SetAsInDanger(creature, DANGER_STATUS_NULL));
//   then, if the target is still alive and the creature's mode manager has no MODE_SCREAMER_AGGRESSIVE mode, hand the brain a CBrainOutput
//   that performs a CCreatureAction_ScreamerIntoAggressive on it (CBrainOutput::SetAsPerformAction: Types |= 8, PAction.Reset(action.Clone()))
// Leaf: ?OnReplace@CAIStateGroup_ScreamerPursueTarget@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0096DDB0, length 401 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 / dtor @ 0x00A01C10, CVectorMap::LowerBound @ 0x0040F020 (inlined GetTC:
//   0x36 = bit 22 of +0x24, 0x31 = bit 17 of +0x24), CTCStealth::SetAsInDanger @ 0x00747B80, CTCCreatureModeManager::IsModeActive @ 0x0073A8A0,
//   CBrainOutput ctor @ 0x00835620, CCombatActionBase(creature, target, 150, CSoundPair) @ 0x00857F30 then the derived vtable 0x01296F14, its
//   Clone through the vtable slot [0x01296F40], CCountedPointer<CCreatureActionBase>::Reset @ 0x00668D90, ~CCombatActionBase @ 0x0062E900,
//   CAIBrain::SetOutput @ 0x0088B8F0, operator delete @ 0x00BFE9BC (count block, inlined counted-pointer dtor).
// Names (agentNF252, 2026-11-22, E3 0x01296F14 unit): ported off its 13 row-local models (CBaseIntelligentPointer / CIntelligentPointer, CTCStealth,
//   CTCCreatureModeManager, CSoundPair, CCreatureActionBase, CCombatActionBase, CCreatureAction_ScreamerIntoAggressive, CBrainOutput, CThingCreature,
//   CAIBrain, CAIStateGroupBase, CAIStateGroup_ScreamerPursueTarget) onto the shared headers of its TU-mate 0x0096DF50 Update, so:
//   - the action class is the anonymous-namespace one Ego_r.pdb / FableWin.pdb name (retail RTTI ?A0x6cb5dcbb, PDBs ?A0x4c815337; per-TU hash, EXTRA_RULES 2026-11-20): the
//     vtable this object emits is ??_7CCreatureAction_ScreamerIntoAggressive@?A0x...@@6B@ with the 0x0096DF50 copy's 81 slots (vtcheck.py)
//     instead of a global-namespace stand-in vtable with v00..v10 slots;
//   - 0x0073A8A0 binds ?IsModeActive@CTCCreatureModeManager@@QBE_NK@Z (both PDBs; 0x0096DF50 binds it) instead of the stand-in
//     ?HasMode@CTCCreatureModeManager@@QAE_NJ@Z;
//   - CBrainOutput is engine/campfire_types.h's (FABLE_RETAIL_CBRAINOUTPUT_ONLY; Ego_r layout: PAction +0x20, LookAtThing +0x40, size 0x48).
// Earlier name fixes kept: Reset binds the explicit instantiation ?Reset@?$CCountedPointer@VCCreatureActionBase@@@@QAEXPAVCCreatureActionBase@@@Z
//   (NF86, FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL); SetAsInDanger is ?SetAsInDanger@CTCStealth@@QAEXABVCThing@@W4EDangerStatus@NStealth@@@Z
//   (NF96); SetOutput is ?SetOutput@CAIBrain@@QAEXABVCBrainOutput@@@Z (both PDBs).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL
#define FABLE_RETAIL_CAIBRAIN_SETOUTPUT
#define FABLE_RETAIL_CBRAINOUTPUT_ONLY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_GETCOMBATTYPEDEF
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CTCSTEALTH_STATTYPE
#define FABLE_RETAIL_NSTEALTH_EDANGERSTATUS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_CMESSAGEEVENT
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS_PAYMENT
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_FINDRECENT_DECL
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_INITIALISE
#define FABLE_RETAIL_CCOMBATACTIONBASE_TARGETCTOR
#define FABLE_RETAIL_CCOMBATTYPEDEF
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D_ORIGINAL_SUM_PASS23
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_OWN_VIRTUALS
#define FABLE_RETAIL_CCOMBATACTIONBASE_FRAMEUPDATE
#define FABLE_RETAIL_CCOMBATACTIONBASE_ONANIMATIONLOADED
#define FABLE_RETAIL_CCOMBATACTIONBASE_FINISHACTION
#define FABLE_RETAIL_CCOMBATACTIONBASE_SCREAMER_VTABLE_PASS27
#include "fable_assert.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/CThing.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CAIStateGroup_ScreamerPursueTarget.h"
#include "engine/CAIBrain.h"
#include "engine/CThingCompare_Nearest.h"
#include "engine/CAdaptThingCompareToSTLComparitor.h"
#include "engine/CCreatureAction_ScreamerIntoAggressive.h"
#include "engine/CCombatTypeDef.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CTCTargetingAI.h"
#include "engine/CTCStealth.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CWorld.h"
#include "engine/CMessageEventManager.h"
#include "engine/CMessageEventFilters.h"
#include "engine/CFilter_Bind2.h"
#include "engine/NCreatureMode.h"
#include "engine/GFSqr.h"
#include "engine/GFRandom.h"
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
#include "engine/campfire_types.h"

void CAIStateGroup_ScreamerPursueTarget::OnReplace()
{
    if (IS_THING_ALIVE(PTarget)) {
        CTCStealth* ptc_stealth;
        if (PTarget->GetTC(&ptc_stealth))
            ptc_stealth->SetAsInDanger(GetCreature(), NStealth::DANGER_STATUS_NULL);
    }
    CTCCreatureModeManager* ptc_cmm;
    GetCreature().GetTC(&ptc_cmm);
    if (IS_THING_ALIVE(PTarget) && !ptc_cmm->IsModeActive(NCreatureMode::MODE_SCREAMER_AGGRESSIVE)) {
        CBrainOutput output;
        output.SetAsPerformAction(CCreatureAction_ScreamerIntoAggressive(GetCreature(), *PTarget));
        PBrain->SetOutput(output);
    }
}
