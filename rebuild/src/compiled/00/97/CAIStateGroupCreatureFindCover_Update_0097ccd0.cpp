// CAIStateGroup_CreatureFindCover::Update() -- while the enemy lives: become directly aware of it when it is the perceived thing, find hidden
// cover (or record the failure), move into it and duck behind it until the cover or the enemy dies, the creature drifts more than 1.0 from the
// hidden spot, or the state stops being runnable; then terminate
// Leaf: ?Update@CAIStateGroup_CreatureFindCover@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0097CCD0 (virtual, plain `ret`).
// Callees: CTCPerceiveThing::GetPerceivedThing 0x007446A0 / SetAsDirectlyAwareOfPerceivedThing 0x007449B0, GetHiddenCoverPosAndSetUpCoverThing
//   0x00970CD0, SetAtFailed 0x00970590, MoveIntoCover 0x009711D0, DuckBehindCover 0x009713E0, GetHiddenCoverPos 0x00970930, IsStillRunnable
//   0x0097CB10, Terminate 0x008FCF40.
// NF72 (round 7, q10885): the row bound 0x00970590 as the descriptive SetFailedToFindCover on the stand-in CAIStateGroupCreatureFindCoverMethods
//   (and its own leaf on that stand-in), over a renaming-macro model of CAIStateGroup_CreatureUseCoverBase. Ported onto the shared headers with the
//   macro block of the sibling CAIStateGroup_CreatureTakeCover::Update 0x0097A630 (NF68) and the overlay opt-in
//   FABLE_RETAIL_CAISTATEGROUP_CREATUREFINDCOVER_UPDATE (engine/CAIStateGroup_CreatureFindCover.h: the retail class, Ego_r.pdb public virtual Update,
//   protected IsStillRunnable).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VSLOTS
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_COVERLIST
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_ADVANCE
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_TYPED_COVERLIST
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_TAKECOVER
#define FABLE_RETAIL_CTCPERCEIVETHING
#define FABLE_RETAIL_CTCPERCEIVETHING_SETASDIRECTLYAWARE
#define FABLE_RETAIL_CAISTATEGROUPBASE_TARGETFACE
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR
#define FABLE_RETAIL_CCREATUREACTION_PROJECTILECROUCH_CLASS
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#include "rebuild_abi.h"
#include "fable_compiler.h"
#include "stlport/fable_containers.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/NCreatureMode.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CWorld.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_CreatureUseCoverBase.h"
#include "engine/CTCPerceiveThing.h"
#include "engine/GFRandom.h"
#include "fable_string.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_PlayAnimation.h"
#include "engine/CCreatureAction_ProjectileCrouch.h"
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREFINDCOVER_UPDATE
#include "engine/CAIStateGroup_CreatureFindCover.h"

void CAIStateGroup_CreatureFindCover::Update()
{
    if (!IS_THING_ALIVE(PTargetThing)) {
        Terminate();
        return;
    }
    CThingAICreature& me = GetCreature();
    if (me.HasTC(TCI_PERCEIVE_THING)) {
        CTCPerceiveThing& perceive = static_cast<CTCPerceiveThing&>(me.GetTC(TCI_PERCEIVE_THING));
        if (perceive.GetPerceivedThing() == PTargetThing.Get())
            perceive.SetAsDirectlyAwareOfPerceivedThing();
    }
    C3DVector cover_pos;
    if (!GetHiddenCoverPosAndSetUpCoverThing(cover_pos)) {
        SetAtFailed();
        Terminate();
        return;
    }
    if (MoveIntoCover(cover_pos)) {
        do {
            DuckBehindCover(true);
            if (!IS_THING_ALIVE(PCoverThing.Get()))
                break;
            if (!IS_THING_ALIVE(PTargetThing))
                break;
            C3DVector hidden_pos;
            GetHiddenCoverPos(hidden_pos, *PCoverThing.Get());
            const C3DVector& pos = GetCreature().GetPos();
            if (C2DVector(hidden_pos.X - pos.X, hidden_pos.Y - pos.Y).GetMagnitude() > 1.0f)
                break;
        } while (IsStillRunnable());
    }
    Terminate();
}
