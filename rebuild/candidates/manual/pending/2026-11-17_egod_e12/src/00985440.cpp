// CAIStateGroup_OpinionReactionRoot::HeroImitationPlayUpdate() -- imitate the hero at play: note whether a child is in sight, push the hero-imitation mode, face the thing reacted to, say the imitation line (about the hero when children are near, else to self), then walk around it for 2..5 seconds; pop the mode again
// Leaf: ?HeroImitationPlayUpdate@CAIStateGroup_OpinionReactionRoot@@AAE_NXZ
// Inline-budget: FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTTHING_BODY (existing opt-in: the GetPFirstThing body visible but __declspec(noinline))
//   -- retail CALLS the same-TU COMDAT 0x00986E30 (Ego_d object order: GetPFirstThing<CFilter_Bind3<IsNot, IsCreatureType, In2DRange>,
//   const CThingAICreature> is emitted in this object); with the body visible c2 knows the filter temporaries and &ret_pcreature are not
//   captured, which gives retail's frame (the declaration-only form leaves 14 frame-slot hunks, measured). That body, compiled here, grades
//   RELOCATION_MATCH at 0x00986E30 (148 bytes) with the IsCreatureType::operator() header inline and CTHING_SCANINCLUSION_ISALIVE.
// Retail: Fable.exe (TLC, VC7.1 RTM) @ 0x00985440, 625 bytes, plain `ret`. Ego_r.pdb / FableWin.pdb
//   ?HeroImitationPlayUpdate@CAIStateGroup_OpinionReactionRoot@@AAE_NXZ (private); Ego_d.pdb (ai_state_group_opinion_reaction_root.obj)
//   gives every local: kids_nearby, pushed_mode, ptc_perception (const CTCPerceiveThing*), frame_to_stop_walking, ret_pcreature, ret,
//   ret_bool, ptc_cmm.
// Callees: CVectorMap::LowerBound 0x0040F020 (expanded GetTC: 0x34 CTCPerceiveThing, 0x31 CTCCreatureModeManager x2),
//   CTCPerceiveThing::GetSightRadius 0x007448F0, CThing::GetPos 0x004C73D0, CAIStateGroupBase::PeekThingSearchTools 0x008FD000 /
//   GetPWorldSeed 0x008FCFD0 / AIAction_MoveToThingProximity 0x0097E380, CThingSearchTools::GetPFirstThing<...> 0x00986E30,
//   CTCCreatureModeManager::AddMode 0x0073AE10 / RemoveMode 0x0073ABA0 (NCreatureMode::MODE_HERO_IMITATION = 22, Ego_d enum NCreatureMode::EMode), CAIStateGroup_OpinionReactionRoot::TurnToFace
//   0x0095EDF0 / WaitForTextToLoadThenSpeak 0x0095ED80, CCharString(const char*) 0x0099EBF0 / dtor 0x0099EAE0, GFFloatRandom 0x004978A0,
//   CWorld::GetFrame 0x0049D870, __ftol2 0x00BFEA70, CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90
//   (the by-value CIntelligentPointer<const CThing> argument, vptr 0x01237F1C, CONVERT_MUTABLE inline body). Data:
//   ?ConstantFPS@CMainGameComponent@@1JA 0x01375550 (read before the random call: the left operand), 2.0f 0x01299898, 3.0f, the two
//   TEXT_AI_OPINION_ACT_HERO_IMITATION_PLAY_* strings 0x01299EC8 / 0x01299E98.
// Shape: kids_nearby is ret_pcreature != 0 (GetPFirstThing's result is ignored, 0x009854FF); the creature type is HUMAN_CHILD (1); the
//   final `return ret == FINISHED || ret == IN_PROGRESS;` is int-materialised (`xor eax,eax` / `mov eax,1`), so Ego_d's ret_bool is not that
//   expression; Ego_d lists no other bool local and no nested block, so ret_bool is the walk's bool& out argument (the frame byte pushed at
//   0x009855FE) and TurnToFace is tested directly (EVIDENCED by Ego_d's local set; audit e12: RELOCATION_MATCH). The re-lookup of the mode manager reuses
//   ptc_cmm (the miss path reads its frame word, 0x00985683).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_OPINIONREACTIONROOT_WIFEUPDATE
#define FABLE_RETAIL_CAISTATEGROUP_OPINIONREACTIONROOT_THINGREACTINGTO
#define FABLE_RETAIL_CAISTATEGROUP_OPINIONREACTIONROOT_HEROIMITATION
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTTHING
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTTHING_BODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTHINGAICREATURE_STATTYPEFLAGS
#define FABLE_RETAIL_CTHING_SCANINCLUSION_ISALIVE
#define FABLE_RETAIL_CTHINGCREATUREBASE_CREATURETYPE
#define FABLE_RETAIL_FILTER_BIND3_FUNCTION
#define FABLE_RETAIL_CTCPERCEIVETHING
#define FABLE_RETAIL_CTCPERCEIVETHING_GETSIGHTRADIUS
#define FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS
#define FABLE_RETAIL_CTCPERCEIVETHING_AWARE_CALLED
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CTCPerceiveThing.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/NCreatureMode.h"
#include "engine/CThingSearchTools.h"
#include "engine/CFilter_Bind3.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CThingFilter_IsCreatureType.h"
#include "engine/CThingFilter_In2DRange.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/GFFloatRandom.h"
#include "engine/EMovementType.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_OpinionReactionRoot.h"

bool CAIStateGroup_OpinionReactionRoot::HeroImitationPlayUpdate()
{
    const CThingAICreature* ret_pcreature = 0;
    const CTCPerceiveThing* ptc_perception = 0;
    GetCreature().GetTC(&ptc_perception);
    PeekThingSearchTools().GetPFirstThing(GFFilter_Bind3(CThingFilter_IsNot(GetCreature()), CThingFilter_IsCreatureType(HUMAN_CHILD),
        CThingFilter_In2DRange(GetCreature().GetPos(), ptc_perception->GetSightRadius())), &ret_pcreature);
    bool kids_nearby = ret_pcreature != 0;
    CTCCreatureModeManager* ptc_cmm = 0;
    GetCreature().GetTC(&ptc_cmm);
    bool pushed_mode = ptc_cmm->AddMode(NCreatureMode::MODE_HERO_IMITATION);
    if (!TurnToFace())
        return false;
    if (WaitForTextToLoadThenSpeak(kids_nearby ? "TEXT_AI_OPINION_ACT_HERO_IMITATION_PLAY_ABOUT_HERO" : "TEXT_AI_OPINION_ACT_HERO_IMITATION_PLAY_TO_SELF",
            (NSpeechGainManager::EDialogueLayer)1) != AI_ACTION_FINISHED)
        return false;
    long frame_to_stop_walking = (long)(CWorld::GetFrame() + CMainGameComponent::GetConstantFPS() * (GFFloatRandom(3.0f, GetPWorldSeed()) + 2.0f));
    EAIActionReturn ret;
    bool ret_bool;
    do {
        ret = AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK, ThingReactingTo, 1.0f, ret_bool, AI_MOVEMENT_WALKING, true, false, true, true, 0.0f);
    } while (ret == AI_ACTION_IN_PROGRESS && CWorld::GetFrame() < frame_to_stop_walking);
    if (pushed_mode) {
        GetCreature().GetTC(&ptc_cmm);
        ptc_cmm->RemoveMode(NCreatureMode::MODE_HERO_IMITATION);
    }
    return ret == AI_ACTION_FINISHED || ret == AI_ACTION_IN_PROGRESS;
}
