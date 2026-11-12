// CAIStateGroup_SeekPerceivedThing::RunToPerceivedThingPosition() -- while the creature is aware of its perceived thing (and the state group is not stopping), run to the thing's last perceived position; on arriving there without finding the thing, play a random WHATS_THAT animation
// Leaf: ?RunToPerceivedThingPosition@CAIStateGroup_SeekPerceivedThing@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0091B3A0, 455 bytes, thiscall, no arguments, plain `ret`; 8-aligned frame, no EH frame.
// Identity: Ego_r.pdb / FableWin.pdb ?RunToPerceivedThingPosition@CAIStateGroup_SeekPerceivedThing@@AAEXXZ (private, non-virtual);
//   only caller 0x0091B749 in CAIStateGroup_SeekPerceivedThing::Update 0x0091B570.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound 0x0040F020 (the inline CThing::GetTC of TCI_PERCEIVE_THING, bit 20
//   of the +0x24 bitset dword), CAIStateGroupBase::IsStopping 0x008FD010, CAIStateGroupBase::AIAction_MoveToPosProximity 0x0097E890,
//   CThingPhysical::IsInRange 0x006A4DF0 (three calls, 3.5f), CTCPerceiveThing::GetPerceivedThing 0x007446A0, CThing::GetPos
//   0x004C73D0, CCharString(const char*, long) 0x0099EBF0 ("WHATS_THAT" 0x01273694) / ~CCharString 0x0099EAE0,
//   CThingCreatureBase::GetRandomAnimFromName 0x00662D80, CCreatureAction_PlayAnimation(CThingCreatureBase&, const CAnimationEntry&,
//   const CSoundPair&, bool, bool, bool, long, long) 0x00842570, CAIStateGroupBase::AIAction_PerformActionAndWait 0x0097DB70,
//   ~CCreatureActionBase 0x00693EF0. Data: 0.0f 0x0122DEDC (the inline IsAwareOfThing test).
// Shape: the component pointer is an uninitialised local filled by an untested GetTC (the not-present path reads the stack slot,
//   0x0091B3F1), and lives in ebp across both loops. The outer `while` tests awareness and IsStopping; the inner do-while repeats
//   the move while it returns AI_ACTION_IN_PROGRESS (4) and breaks to the OUTER test when awareness drops (0x0091B430 `jnp 0091B3F5`).
//   The last perceived position is copied to a local for the by-value move argument and the first range test; the last range test
//   reads the member again directly (edi = `lea [ebp+20h]`). No GetLastPerceivedPos existed: FableWin.pdb emits the trivial inline
//   accessors this function uses out of line (IsThingLineOfSight 0x01DA32C0, IsAwareOfThing 0x01F43D20, CAIStateGroupBase::GetCreature
//   0x0288D7B0, CAIBrain::GetCreature 0x0288D7D0) and has its own copy of this function (0x02BD3B90), but no such accessor. The creature is re-read from PBrain before every
//   call (inline GetCreature()). The animation entry is a separate statement (the name temporary dies right after
//   GetRandomAnimFromName, 0x0091B50E) and the action is a NAMED local: retail constructs it first and only then pushes `false`
//   and its address (0x0091B539); a temporary argument pushes `false` before the constructor runs. Its destruction is the implicit
//   destructor, i.e. ~CCreatureActionBase directly (0x0091B54A).
// Headers (overlay opt-ins, new for this row): FABLE_RETAIL_CAISTATEGROUP_SEEKPERCEIVEDTHING (the class on its CAIStateGroupBase
//   base), FABLE_RETAIL_CTCPERCEIVETHING_SEEKPERCEIVEDTHING_FRIEND (friend grant for the direct read of the private
//   LastPerceivedPos), FABLE_RETAIL_CTHINGPHYSICAL_ISINRANGE
//   (IsInRange declaration).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGPHYSICAL_ISINRANGE
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCPERCEIVETHING
#define FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS
#define FABLE_RETAIL_CTCPERCEIVETHING_SEEKPERCEIVEDTHING_FRIEND
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAISTATEGROUP_SEEKPERCEIVEDTHING
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCPerceiveThing.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_PlayAnimation.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_SeekPerceivedThing.h"

void CAIStateGroup_SeekPerceivedThing::RunToPerceivedThingPosition()
{
    CTCPerceiveThing* perceive;
    GetCreature().GetTC(TCI_PERCEIVE_THING, &perceive);
    while (perceive->IsAwareOfThing() && !IsStopping()) {
        EAIActionReturn ret;
        do {
            if (!perceive->IsAwareOfThing())
                break;
            C3DVector pos = perceive->LastPerceivedPos;
            bool nav_route_calculated;
            ret = AIAction_MoveToPosProximity(ACTION_REPEAT_CALLBACK, pos, 0.5f, nav_route_calculated, AI_MOVEMENT_RUNNING,
                true, false, true, 0, true, 0.0f);
            if (GetCreature().IsInRange(pos, 3.5f)
                && !GetCreature().IsInRange(perceive->GetPerceivedThing()->GetPos(), 3.5f)
                && !perceive->IsThingLineOfSight()
                && GetCreature().IsInRange(perceive->LastPerceivedPos, 3.5f)) {
                const CAnimationEntry& anim = GetCreature().GetRandomAnimFromName("WHATS_THAT", -1);
                CCreatureAction_PlayAnimation action(GetCreature(), anim, CSoundPair(), false, true, false, 0, 0);
                AIAction_PerformActionAndWait(action, false);
            }
        } while (ret == AI_ACTION_IN_PROGRESS);
    }
}