// CAIStateGroup_KidReactToBlockage::Update() -- look at the blocking creature, then (while it is still alive) throw a three-part tantrum (into, loop, out of), stopping at the first animation that does not complete; always terminate
// Leaf: ?Update@CAIStateGroup_KidReactToBlockage@@UAEXXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00941E00, length 448 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PThingCreature +0x2c), CThing::GetPosition @ 0x004C73D0,
// CAIStateGroupBase::AIAction_LookAtPos(EAIActionRepeat, C3DVector, long, float) @ 0x0097D580 (repeat 1, flags 1, turn
// speed 1/72), CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0 ("ST_TANTRUM_INTO" @ 0x012928A4,
// "ST_TANTRUM_LOOP" @ 0x01292894, "ST_TANTRUM_OUTOF" @ 0x01292880), CCreatureAction_PlayAnimation ctor @ 0x008424F0,
// AIAction_PerformActionAndWait @ 0x0097DB70, the action base dtor @ 0x00693EF0, Terminate @ 0x008FCF40.
// Shape: the creature is read through an inline brain getter (as the landed ChickenPanic 0x00911D90): PBrain is loaded before the
// argument pushes and the sound-pair zeros land before its address is pushed; a trailing `if (failed) { Terminate(); return; }`
// keeps the third result's setne and the dead `mov al` reload (both arms tail-merge into the one Terminate call).
// Layout (agentNF77, q10884): engine/CAIStateGroup_KidReactToBlockage.h (PThingCreature +0x2c, CIntelligentPointer<CThingCreatureBase>) on
//   engine/CAIStateGroupBase.h / CAIBrain.h, engine/CCreatureAction_PlayAnimation.h (FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR:
//   retail's derived action destructor is the compiler-generated one, so each temporary's destruction calls ~CCreatureActionBase 0x00693EF0
//   straight, ??1CCreatureActionBase@@UAE@XZ in both PDBs) and engine/CSoundPair.h. Was a local CAIStateGroupBase / CAIBrain /
//   CCreatureActionBase / CCreatureAction_PlayAnimation / CSoundPair / state-group model (the action destructor bound as a local
//   ??1CCreatureAction_PlayAnimation@@QAE@XZ). Self leaf respelled QAE -> UAE: Update is virtual in the shared header, Ego_r.pdb 0x00965C20
//   ?Update@CAIStateGroup_KidReactToBlockage@@UAEXXZ; the PlayAnimation constructor now binds its PDB reference spelling (AAVCThingCreatureBase@@).
// Name fix (agentNF5 2026-11-10): CAIStateGroupBase::AIAction_LookAtPos 0x0097D580 is protected, EAIActionReturn(EAIActionRepeat, C3DVector, long, float) (?AIAction_LookAtPos@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@JM@Z, Ego_r.pdb / FableWin.pdb).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCWEAPONTRAIL_UPDATE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR
#define FABLE_RETAIL_IS_THING_ALIVE
#include "fable_string.h"
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
#include "engine/C3DVector.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIStateGroup_KidReactToBlockage.h"
#include "engine/CAIBrain.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_PlayAnimation.h"
#include "engine/ThingAlive.h"

void CAIStateGroup_KidReactToBlockage::Update()
{
    if (IS_THING_ALIVE(PThingCreature)) {
        CThingCreatureBase* thing = PThingCreature.Get();
        if (AIAction_LookAtPos((EAIActionRepeat)(1), thing->GetPos(), 1, 0.013888889f) == 1) {
            Terminate();
            return;
        }
    }
    if (IS_THING_ALIVE(PThingCreature)) {
        bool failed;
        {
            CCharString anim("ST_TANTRUM_INTO");
            failed = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), anim, CSoundPair(),
                                                                                 false, true, false, 0, 0), false) != 2;
        }
        if (!failed) {
            {
                CCharString anim("ST_TANTRUM_LOOP");
                failed = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), anim, CSoundPair(),
                                                                                     false, true, false, 0, 0), false) != 2;
            }
            if (!failed) {
                CCharString anim("ST_TANTRUM_OUTOF");
                failed = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), anim, CSoundPair(),
                                                                                     false, true, false, 0, 0), false) != 2;
            }
        }
        if (failed) {
            Terminate();
            return;
        }
    }
    Terminate();
}