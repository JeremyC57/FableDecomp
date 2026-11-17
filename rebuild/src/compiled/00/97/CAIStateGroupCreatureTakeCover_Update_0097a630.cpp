// CAIStateGroup_CreatureTakeCover::Update() -- take cover from a live target: make the creature directly aware of it, find a hidden cover
// position (none: mark the failure and terminate); a distant cover is reached by crouching (with a live previous cover) then moving into it;
// from low cover duck and attack up to 1-3 times while the position holds, otherwise step out to a firing position, attack, return and reload
// up to 1-3 times; finally drop the cover from the list and refresh an emptied list
// Leaf: ?Update@CAIStateGroup_CreatureTakeCover@@UAEXXZ
// Identity: Ego_r.pdb ?Update@CAIStateGroup_CreatureTakeCover@@UAEXXZ (public virtual; vtable 0x0128D54C). Retail 0x0097A630, 1195 bytes.
// Port (NF68): on the 0x009790F0 / landed MoveIntoCover 0x009711D0 macro block plus FABLE_RETAIL_CTCPERCEIVETHING, CCREATUREACTION_PLAYANIMATION,
//   CAISTATEGROUPBASE_TARGETFACE / _SPEECH (GetWorld), engine/GFRandom.h, engine/CSoundPair.h, and my overlay opt-ins
//   FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_ADVANCE / _TAKECOVER / _TYPED_COVERLIST, FABLE_RETAIL_CAISTATEGROUP_CREATURETAKECOVER_UPDATE,
//   FABLE_RETAIL_CCREATUREACTION_PROJECTILECROUCH_CLASS, FABLE_RETAIL_CTCPERCEIVETHING_SETASDIRECTLYAWARE. Every local model of the landed row
//   (CAIStateGroupBase / TakeCover flattened model, cover entries, CIntelligentPointerConstThing, interface map, PlayAnimation / Crouch stand-ins)
//   is gone; the local Random() / FlatDistanceTo() helpers are spelled with the shared GFRandom and C2DVector::GetMagnitude.
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
#define FABLE_RETAIL_CAISTATEGROUP_CREATURETAKECOVER_ISRUNNABLELEAF
#define FABLE_RETAIL_CAISTATEGROUP_CREATURETAKECOVER_UPDATE
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
#include "engine/CAIStateGroup_CreatureTakeCover.h"
#include "engine/CTCPerceiveThing.h"
#include "engine/GFRandom.h"
#include "fable_string.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_PlayAnimation.h"
#include "engine/CCreatureAction_ProjectileCrouch.h"

void CAIStateGroup_CreatureTakeCover::Update()
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
    CThing* previous_cover = 0;
    if (IS_THING_ALIVE(PCoverThing.Get()))
        previous_cover = PCoverThing.Get();
    C3DVector cover_pos;
    if (!GetHiddenCoverPosAndSetUpCoverThing(cover_pos)) {
        if (ValidPositionsFound <= 0)
            SetAtFailed();
        Terminate();
        return;
    }
    const C3DVector& pos = GetCreature().GetPos();
    bool far_away = C2DVector(cover_pos.X - pos.X, cover_pos.Y - pos.Y).GetMagnitude() > 1.8f;
    bool low_cover = IsLowCover(*PCoverThing.Get());
    if (far_away) {
        if (IS_THING_ALIVE(previous_cover)) {
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            AIAction_PerformActionAndWait(CCreatureAction_ProjectileCrouch(GetCreature(), *PTargetThing.Get(), cover_pos), false);
        }
        if (!MoveIntoCover(cover_pos)) {
            RemoveThingFromCoverList(PCoverThing.Get());
            ClearCoverThing();
            if (ValidPositionsFound > 0 && CoverList.size() == 0)
                RefreshCoverList();
            return;
        }
    }
    if (low_cover) {
        unsigned long attacks = GFRandom(3, GetWorld().GetWorldSeed()) + 1;
        bool drifted = false;
        for (unsigned long i = 0; i < attacks && !drifted; ++i) {
            DuckBehindCover(true);
            if (!IsStillRunnable()) { Terminate(); return; }
            if (!AttackTargetThing()) { Terminate(); return; }
            if (!IsStillRunnable()) { Terminate(); return; }
            GetHiddenCoverPos(cover_pos, *PCoverThing.Get());
            const C3DVector& here = GetCreature().GetPos();
            if (C2DVector(cover_pos.X - here.X, cover_pos.Y - here.Y).GetMagnitude() > 1.8f)
                drifted = true;
        }
        DuckBehindCover(true);
    } else {
        unsigned long attacks = GFRandom(3, GetWorld().GetWorldSeed()) + 1;
        if (!IS_THING_ALIVE(PTargetThing) || !IS_THING_ALIVE(PCoverThing.Get())) {
            Terminate();
            return;
        }
        for (unsigned long i = 0; i < attacks; ++i) {
            if (!IsStillRunnable()) { Terminate(); return; }
            GetHiddenCoverPos(cover_pos, *PCoverThing.Get());
            if (CanSeeTarget() || MoveFromCoverToFiringPosition(cover_pos, PCoverThing.Get())) {
                if (!IsStillRunnable()) { Terminate(); return; }
                if (!AttackTargetThing()) { Terminate(); return; }
                if (!IsStillRunnable()) { Terminate(); return; }
                GetHiddenCoverPos(cover_pos, *PCoverThing.Get());
                bool in_cover = MoveIntoCover(cover_pos);
                if (!IsStillRunnable()) { Terminate(); return; }
                if (in_cover) {
                    if (AIAction_PerformActionTargetFaceAndWait(CCreatureAction_PlayAnimation(GetCreature(), "PROJECTILE_WEAPON_LOAD",
                            CSoundPair(), false, true, false, 0, 0), PTargetThing, false) != 2) {
                        Terminate();
                        return;
                    }
                }
            }
        }
    }
    RemoveThingFromCoverList(PCoverThing.Get());
    if (ValidPositionsFound > 0 && CoverList.size() == 0)
        RefreshCoverList();
}
