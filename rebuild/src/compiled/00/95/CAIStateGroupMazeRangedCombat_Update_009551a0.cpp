// CAIStateGroup_MazeRangedCombat::Update @ 0x009551A0, 280 retail bytes.
// Leaf: ?Update@CAIStateGroup_MazeRangedCombat@@UAEXXZ
// Attempt a shot, play SPECIAL_IDLE on success, retry while eligible, and
// terminate when the action sequence ends.
// Retail CCreatureActionBase is 0xa8 (the donor header says 0xb4); retail CCombatActionBase is 0x114 (the donor layout is 0x12c).
// Neighboring landed constructors 009566c0 and 008b7ff0 establish those offsets and lifetimes. Retail expands the ThrowShotObject
// constructor (0x009566C0) here: the base constructor 0x00857F30 with 150 and the SWORD_SWING sound pair, then the vftable 0x012944D4 store.
// NF137 (2026-11-20, q5021): moved onto the shared headers. AIAction_PerformActionAndWait 0x0097DB70 now binds the PDB
//   ?AIAction_PerformActionAndWait@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@ABVCCreatureActionBase@@_N@Z (Ego_r.pdb 0x00848940 + FableWin.pdb
//   0x02892570; engine/CAIStateGroupBase.h PERFORMACTION), was the MazeRangedMethods stand-in over the CCreatureActionBaseRetail chain.
//   The CCreatureActionBaseRetail / CCombatActionBaseRetail / _ThrowShotObjectRetail / _PlayCombatAnimationRetail stand-ins, the local
//   CBaseIntelligentPointer / CIntelligentPointer / CSoundPair models and the host-adapter branch are gone for engine/CCombatActionBase.h,
//   engine/CCreatureAction_PlayCombatAnimation.h (retail mode), engine/CCreatureAction_ThrowShotObject.h (overlay opt-in
//   FABLE_RETAIL_CCREATUREACTION_THROWSHOTOBJECT_CLASS: the retail class with its inline constructor) and engine/CIntelligentPointer.h.
//   Terminate 0x008FCF40 now binds ?Terminate@CAIStateGroupBase@@QAEXXZ. The MazeRangedMethods stand-in is gone for the retail class (overlay
//   opt-in FABLE_RETAIL_CAISTATEGROUP_MAZERANGEDCOMBAT on the quarantine CAIStateGroup_MazeBase retail mode, as landed 0x00954F40): own leaf
//   now the PDB ?Update@CAIStateGroup_MazeRangedCombat@@UAEXXZ (Ego_r.pdb 0x00979BB0 + FableWin.pdb 0x02BF6400) and the CanRun 0x00954220
//   stand-in now the PDB ?IsStillRunnable@CAIStateGroup_MazeRangedCombat@@IAE_NXZ (Ego_r.pdb 0x009789D0 + FableWin.pdb 0x02BF6110).
//   NF148 (2026-11-20, q11785, one vtable): also defines FABLE_RETAIL_CCREATUREACTION_THROWSHOTOBJECT_CLONE / _INITIALISE, so this object's
//   ??_7CCreatureAction_ThrowShotObject@@6B@ COMDAT holds the retail overrides at slots 11 / 12 (0x00956960 Clone, 0x008B66D0 Initialise; were
//   the inherited CCombatActionBase::Clone / CCreatureActionBase::Initialise), identical to the copies 0x009566C0 and 0x00956960 emit.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_TARGETCTOR
#define FABLE_RETAIL_CCREATUREACTION_PLAYCOMBATANIMATION
#define FABLE_RETAIL_CCREATUREACTION_THROWSHOTOBJECT_CLASS
#define FABLE_RETAIL_CCREATUREACTION_THROWSHOTOBJECT_CLONE
#define FABLE_RETAIL_CCREATUREACTION_THROWSHOTOBJECT_INITIALISE
#define FABLE_RETAIL_CAISTATEGROUP_MAZERANGEDCOMBAT
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CThingAICreature.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_ThrowShotObject.h"
#include "engine/CCreatureAction_PlayCombatAnimation.h"
#include "engine/CAIStateGroup_MazeRangedCombat.h"

void CAIStateGroup_MazeRangedCombat::Update() {
    EAIActionReturn result = AI_ACTION_FINISHED;
    do {
        if (IsStillRunnable()) {
            CThingAICreature& creature = GetCreature();
            result = AIAction_PerformActionAndWait(
                CCreatureAction_ThrowShotObject(creature, *PTargetThing.Get()), false);
        } else {
            Terminate();
        }
        if (IsStillRunnable() && result == AI_ACTION_FINISHED) {
            CCharString animation("SPECIAL_IDLE", -1);
            CThingAICreature& creature = GetCreature();
            result = AIAction_PerformActionAndWait(
                CCreatureAction_PlayCombatAnimation(creature, animation, true), false);
        } else {
            Terminate();
        }
    } while (IsStillRunnable() && result == AI_ACTION_FINISHED);
    Terminate();
}
