// CAIStateGroup_JackOfBladesStage1::MeleeCombat() -- if Jack is in melee as an attacker, mark him ready to attack in the world's
// combat manager; then each tick, while still runnable (target and arena centre alive, not stopping), post a pump-combat-actions
// brain output looking at the target (look flags 0x11) and yield; leave when the 2D distance to the target reaches the combat
// type's FrontMiddleLimit, when the hits have caused a rush, or when no longer runnable after the yield. Terminates only when the
// preconditions fail at the top of a tick. Always returns AI_ACTION_FINISHED (2).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00972CC0, length 612 bytes (+ int3 padding).
// Callees: CThingCreatureBase::IsInMeleeAsAttacker @ 0x00662410, CAIStateGroupBase::GetWorld @ 0x008FCFC0,
// CCombatManager::SetAttackerAsReadyToAttack @ 0x006EA450, CBaseIntelligentPointer ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 /
// SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10, CAIStateGroupBase::IsStopping @ 0x008FD010, CAIBrain::SetOutput @ 0x0088B8F0, the
// brain's virtual slot 2 SuspendSuspendableProcess (0x00A44840), CThing::GetPos @ 0x004C73D0, operator delete @ 0x00BFE9BC,
// CThingCreatureBase::GetCombatTypeDef @ 0x00666D10, CAIStateGroup_JackOfBladesStage1::HasHitsCausedRush @ 0x00972340 (same TU,
// out of line), CAIStateGroupBase::Terminate @ 0x008FCF40.
// Shape: CAIStateGroup_JackOfBladesBase::IsStillRunnable (0x00971B00, this TU) is expanded inline twice (loop top + latch); its
// body below is the landed row rebuild/src/compiled/00/97/CAIStateGroupJackOfBladesBase_IsStillRunnable_00971b00.cpp verbatim
// (nested ifs on PTargetThing.Get()->IsDead() / IsStopping() / PArenaCentre.Get()->IsDead()). The earlier one-expression spelling
// `IS_THING_ALIVE(PTargetThing) && !IsStopping() && IS_THING_ALIVE(PArenaCentre)`, taken from the parked 0x00975BB0 Update draft,
// grades the same bytes; the landed spelling is kept so one retail address has one body.
// CBrainOutput / CNavigationInput: the shared engine/campfire_types.h definitions (Ego_r.pdb layouts, struct_layouts_egor.tsv: Types +0,
// LookFlags +4, NavigationInput +8 with MovementType +0x10 typed EMovementType, PAction +0x20 = CCountedPointer<CCreatureActionBase>,
// MoveToPos +0x28, LookAtPos +0x34, LookAtThing +0x40), through the opt-ins FABLE_RETAIL_CBRAINOUTPUT_ONLY (just that slice of the bundle)
// and FABLE_RETAIL_CBRAINOUTPUT_INLINE (the inline bodies retail expanded: ??0CNavigationInput@@QAE@XZ with MovementType =
// AI_MOVEMENT_NULL, ??0CBrainOutput@@QAE@XZ, ??1CBrainOutput@@QAE@XZ, ?SetAsPumpCombatActions@CBrainOutput@@QAEXXZ = Types |= 0x20
// (FableWin.pdb only: thunk 0x0179B3F0 / body 0x028923D0; not in Ego_r.pdb, like ??0CNavigationInput), ?SetAsLookAtThing@CBrainOutput@@QAEXABVCThing@@J@Z). IsStillRunnable is protected
// (?IsStillRunnable@CAIStateGroup_JackOfBladesBase@@IAE_NXZ, both PDBs) as the shared Base header declares it. The target pointer is
// read before the creature's GetPos call. The ready-to-attack call needs no locals: VC7.1 evaluates the arguments right to left, so
// the creature ([[edi+4]+0x18] into esi at 0x00972CDE..0x00972CE1) is loaded before `call 0x8fcfc0` (GetWorld) at 0x00972CE6.
// Leaf: ?MeleeCombat@CAIStateGroup_JackOfBladesStage1@@IAE?AW4EAIActionReturn@@XZ
#define FABLE_RETAIL_JACK_STAGE1_UPDATE
#define FABLE_RETAIL_JACK_STAGE1_HASHITSCAUSEDRUSH
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_SETOUTPUT
#define FABLE_RETAIL_CAIBRAIN_SUSPENDABLE_BASE
#define FABLE_RETAIL_CAISTATEGROUP_JACKOFBLADESBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_COMBATTYPE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CCOMBATTYPEDEF
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_COMBATMANAGER
#define FABLE_RETAIL_CCOMBATMANAGER
#define FABLE_RETAIL_CCOMBATMANAGER_SETATTACKERREADY
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CBRAINOUTPUT_INLINE
#define FABLE_RETAIL_CBRAINOUTPUT_ONLY
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include <math.h>
#pragma intrinsic(sqrt)
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroup_JackOfBladesStage1.h"
#include "engine/CAIBrain.h"
#include "engine/CWorld.h"
#include "engine/CCombatTypeDef.h"
#include "engine/CCombatManager.h"
#include "engine/ThingAlive.h"
#include "engine/C3DVector.h"
#include "engine/EAIAction.h"
#include "engine/CCountedPointer.h"
#include "engine/CCreatureActionBase.h"
#include "engine/campfire_types.h"

inline bool CAIStateGroup_JackOfBladesBase::IsStillRunnable()
{
    if (PTargetThing.Get() != 0) {
        if (!PTargetThing.Get()->IsDead()) {
            if (!IsStopping()) {
                if (PArenaCentre.Get() != 0) {
                    if (!PArenaCentre.Get()->IsDead())
                        return true;
                }
            }
        }
    }
    return false;
}

EAIActionReturn CAIStateGroup_JackOfBladesStage1::MeleeCombat()
{
    if (GetCreature().IsInMeleeAsAttacker()) {
        GetWorld().GetCombatManager().SetAttackerAsReadyToAttack(GetCreature(), true);
    }
    for (;;) {
        if (!IsStillRunnable()) {
            Terminate();
            return AI_ACTION_FINISHED;
        }
        float distance;
        {
            CBrainOutput output;
            output.SetAsPumpCombatActions();
            output.SetAsLookAtThing(*PTargetThing.Get(), 0x11);
            PBrain->SetOutput(output);
            PBrain->SuspendSuspendableProcess();
            CThingCreatureBase& me = GetCreature();
            CThingCreatureBase* target = PTargetThing.Get();
            const C3DVector& a = me.GetPos();
            const C3DVector& b = target->GetPos();
            float dx = b.X - a.X;
            float dy = b.Y - a.Y;
            distance = sqrt(dy * dy + dx * dx);
        }
        if (!IsStillRunnable())
            return AI_ACTION_FINISHED;
        if (!(distance < GetCreature().GetCombatTypeDef().FrontMiddleLimit))
            return AI_ACTION_FINISHED;
        if (HasHitsCausedRush())
            return AI_ACTION_FINISHED;
    }
}