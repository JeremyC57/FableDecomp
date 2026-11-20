// CAIStateGroup_DeedReactionRoot::Update() -- react to a hero deed: find the creature's attitude to the deed's doer (through its opinion source, else the blurred attitude to the player), pick the deed log's reaction animation and dialogue tag for that attitude (the greeting group for the too-frequent and indoors-greeting deeds, the generic-incomprehension tag for a flirt at an unmarried, not-courtable woman, the plain attitude's reaction for a wife attitude with none of its own), wait for an interruptible action, look at the doer, play the speech (retrying while the talk component asks) and the animation, then wait for the speech to end
// Inline-budget: no flag, no pragma. Devices (README ablations): FABLE_RETAIL_CCHARSTRING_FORCEINLINE_ISEMPTY (new add-only opt-in, the IsEmpty expansion 0x00960376..0x00960381; without: DIFFER(1596v1600), and the per-row /d2inlS21..27 that also expands it breaks the TU-mate 0x009609B0 IsRunnable, see README) and FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ (the `*anim_name == ""` expansion 0x0096069C..0x009606C8; without: DIFFER(1576v1600)). Header contracts that decide expanded vs called: FABLE_RETAIL_CINTELLIGENTPOINTER_COPY (the LookAt argument built in place, the PerformAction one copied by the out-of-line 0x00654D58; without: DIFFER(1584v1600), COPY_OUTOFLINE DIFFER(1580v1600)).
// Leaf: ?Update@CAIStateGroup_DeedReactionRoot@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1 RTM) @ 0x00960120, 1600 bytes (1580 code + `mov edi,edi` + the 5-entry wife-attitude jump table at 0x0096074C),
//   thiscall, esp frame (`sub esp,0D8h`: the 0xB4 CCreatureAction_PlayAnimation temporary), no EH frame. Vtable slot 12 of
//   .?AVCAIStateGroup_DeedReactionRoot@@ (TU-mate of the landed 0x009609B0 IsRunnable, same defines: see README).
// Identity: Ego_r.pdb / FableWin.pdb ?Update@CAIStateGroup_DeedReactionRoot@@UAEXXZ (public virtual).
// Ego_d.pdb (debug length 4480, lines 235..422, egod_lines/): locals this, anim_name, reaction_manager, ptc_deedlog, dialogue_tag, ptc_source,
//   ptc_talk, attitude; blocks { pplayer } { ptc_opinion } { ptc_wife { dummy_attitude } } { allow_on_non_ai_speaker, ret, ptc_talk }. Shapes
//   from the /Od sizes (`this` at bprel -492, so every this-load is 6 B): 236/238/239 the alive test, Terminate (11 B), return (5 B); 244 / 245
//   (7 B each) the ptc_source and anim_name initialisers; 248/251 the blur; 254 the ptc_deedlog initialiser; 270 `if (ptc_deedlog)`; 273 the
//   anim_name assignment; 276 the two-deed test; 330..332 the deed-log else arm (DeliveredReaction, Terminate, return).
// Callees: CAIStateGroupBase::GetWorld 0x008FCFC0, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const 0x0040F020 / non-const 0x004365B0
//   (the expanded PeekTC / GetTC), COpinionReactionManager::GetAttitudeOrReactionType 0x00701010 / GetThingAttitudeToPlayer 0x00701840 (the
//   inline GetAttitudeType, = the landed out-of-line copy 0x00681500) / GetBlurredAttitude 0x00700120 / LogDeedReactionPlayingSpeech 0x006FEAE0,
//   CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPControlledCreature 0x00487DC0, CTCHeroOpinionDeedLog::PeekPDeedEffectsDef 0x006C5630,
//   CThingCreatureBase::GetSex 0x00661FF0 / GetCreatureType 0x00662000, CTCWife::IsMarriedToThePlayer 0x006CEC80 / IsCourtableAndNotBlockingIt
//   0x006CF810, CTCCreatureOpinionOfHero::GetGreetDialogueGroupIndex 0x00683C10, CAIStateGroupBase::IsStopping 0x008FD010 / Terminate 0x008FCF40 /
//   AIAction_LookAtThing 0x0097D6F0 / AIAction_DoNothing 0x0097E200 / AIAction_PerformActionTargetFaceAndWait 0x0097DCB0, CTCTalk::PlayAISpeech
//   0x006E3B60 / IsTalking 0x006E28A0, CCharString::operator!=(const char*) 0x0099E960, CBasicString<char>::operator==(const char*) 0x004115A0,
//   IS_THING_DEAD<CIntelligentPointer<const CThing>> 0x00836BE0, CIntelligentPointer<const CThing> copy ctor 0x00654D58,
//   CCreatureAction_PlayAnimation(CThingCreatureBase&, const CCharString&, const CSoundPair&, bool, bool, bool, long, long) 0x008424F0, the temporary's
//   ~CCreatureActionBase 0x00693EF0 (the generated destructor: FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR), CBaseIntelligentPointer
//   0x00A01B10 / 0x00A01B50 / 0x00A01B90; brain vtable slot 2 SuspendSuspendableProcess, action vtable slot 27 IsActionReadyForInterruption.
// Globals: CMainGameComponent::PMainGameComponent 0x013B86A0, "" 0x0122D70E, CIntelligentPointer<const CThing> vtable 0x01237F1C; LookAt speed
//   0.0027777f (retail immediate 0x3B360A13, the landed 0x009357F0 / 0x0093A080 spelling; 0.0027777778f = 0x3B360B61 differs).
// Inferred: `const CCharString* anim_name = NULL;` at Ego_d line 245 (a 7 B dword zero store between the ptc_source initialiser and its PeekTC;
//   dialogue_tag = 0 there grades the same; anim_name chosen because the text tests `anim_name == NULL`). Retail-neutral either way.
// Overlay (add-only opt-ins): new engine/COpinionDeedReaction.h; engine/CTCHeroOpinionDeedLog.h FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_DEEDREACTIONS
//   (the typed DeedReactions + GetDeedReactionAnimName / GetDeedReactionDialogueTag); engine/COpinionReactionManager.h
//   FABLE_RETAIL_COPINIONREACTIONMANAGER_DEEDREACTIONS (inline GetAttitudeType and CanPlayDeedReactionSpeech, GetBlurredAttitude,
//   LogDeedReactionPlayingSpeech); fable_string.h FABLE_RETAIL_CCHARSTRING_FORCEINLINE_ISEMPTY.
#define FABLE_RETAIL_CAISTATEGROUP_DEEDREACTIONROOT
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAISTATEGROUPBASE_TARGETFACE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR
#define FABLE_RETAIL_CTHINGCREATUREDEF_TRANSFER_PASS24
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_SUSPENDABLE_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_GETSEX
#define FABLE_RETAIL_CTHINGCREATUREBASE_CREATURETYPE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_EQUAL_RAW
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_INTERRUPTION
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_DEAD_DECL_ONLY
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_ISEMPTY
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPLAYERMANAGER
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_GREET
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_DEEDS
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_COMPLETE
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_DEEDREACTIONS
#define FABLE_RETAIL_CTCOPINIONSOURCE
#define FABLE_RETAIL_CTCWIFE
#define FABLE_RETAIL_CTCTALK
#define FABLE_RETAIL_COPINIONREACTIONMANAGER_DEEDREACTIONS
#define FABLE_RETAIL_STLP_LIST
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/EOpinionAttitudeType.h"
#include "engine/EOpinionDeedType.h"
#include "engine/ECreatureType.h"
#include "engine/CThingCreatureDef.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CWorld.h"
#include "engine/COpinionReactionManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_PlayAnimation.h"
#include "engine/COpinionDeedEffectsDef.h"
#include "engine/CTCCreatureOpinionOfHero.h"
#include "stlport/fable_map.h"
#include "engine/COpinionDeedReaction.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/CTCOpinionSource.h"
#include "engine/CTCWife.h"
#include "engine/CTCTalk.h"
#include "engine/CAIStateGroup_DeedReactionRoot.h"
#include "engine/ThingAlive.h"

void CAIStateGroup_DeedReactionRoot::Update()
{
    if (!IS_THING_ALIVE(ThingReactingTo)) {
        Terminate();
        return;
    }
    COpinionReactionManager& reaction_manager = GetWorld().GetOpinionReactionManager();
    const CTCOpinionSource* ptc_source = NULL;
    const CCharString* anim_name = NULL;
    ThingReactingTo->PeekTC(&ptc_source);
    EOpinionAttitudeType attitude = reaction_manager.GetAttitudeType(GetCreature(), ptc_source);
    if (ptc_source == NULL)
        attitude = reaction_manager.GetBlurredAttitude(attitude);
    unsigned long dialogue_tag;
    const CTCHeroOpinionDeedLog* ptc_deedlog = NULL;
    if (DeedIsVirtual) {
        CThingPlayerCreature* pplayer = CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        if (pplayer)
            pplayer->PeekTC(&ptc_deedlog);
    } else
        ThingReactingTo->PeekTC(&ptc_deedlog);
    if (ptc_deedlog) {
        anim_name = &ptc_deedlog->GetDeedReactionAnimName(CurrentDeedType, attitude);
        if (CurrentDeedType == OPINION_DEED_TYPE_TOO_FREQUENT_OTHER_DEED || CurrentDeedType == OPINION_DEED_TYPE_INDOORS_GREETING) {
            CTCCreatureOpinionOfHero* ptc_opinion;
            GetCreature().GetTC(&ptc_opinion);
            dialogue_tag = ptc_opinion->GetGreetDialogueGroupIndex(attitude);
        } else {
            const CTCWife* ptc_wife;
            if (ptc_deedlog->PeekPDeedEffectsDef(CurrentDeedType)->IsFlirting && GetCreature().GetSex() == SEX_FEMALE &&
                GetCreature().GetCreatureType() == HUMAN_ADULT && GetCreature().PeekTC(&ptc_wife) &&
                !ptc_wife->IsMarriedToThePlayer() && !ptc_wife->IsCourtableAndNotBlockingIt())
                dialogue_tag = ptc_deedlog->GetDeedReactionDialogueTag(OPINION_DEED_TYPE_GENERIC_INCOMPREHENSION, attitude);
            else {
                dialogue_tag = ptc_deedlog->GetDeedReactionDialogueTag(CurrentDeedType, attitude);
                if ((anim_name == NULL || anim_name->IsEmpty()) && dialogue_tag == 0 && attitude >= OPINION_ATTITUDE_TYPE_WIFE_FIRST) {
                    EOpinionAttitudeType dummy_attitude = OPINION_ATTITUDE_TYPE_NONE;
                    switch (attitude) {
                    case OPINION_ATTITUDE_TYPE_WIFE_LOVE: dummy_attitude = OPINION_ATTITUDE_TYPE_LOVE; break;
                    case OPINION_ATTITUDE_TYPE_WIFE_LIKE: dummy_attitude = OPINION_ATTITUDE_TYPE_AGREEABLE; break;
                    case OPINION_ATTITUDE_TYPE_WIFE_NEUTRAL: dummy_attitude = OPINION_ATTITUDE_TYPE_NONE; break;
                    case OPINION_ATTITUDE_TYPE_WIFE_DISLIKE: dummy_attitude = OPINION_ATTITUDE_TYPE_OFFENSIVE; break;
                    case OPINION_ATTITUDE_TYPE_WIFE_HATE: dummy_attitude = OPINION_ATTITUDE_TYPE_DISDAIN; break;
                    }
                    dialogue_tag = ptc_deedlog->GetDeedReactionDialogueTag(CurrentDeedType, dummy_attitude);
                    anim_name = &ptc_deedlog->GetDeedReactionAnimName(CurrentDeedType, dummy_attitude);
                }
            }
        }
    } else {
        DeliveredReaction = true;
        Terminate();
        return;
    }
    while (GetCreature().PeekPCurrentAction() && GetCreature().PeekPCurrentAction()->GetInterruptionGroupID() >= 50 &&
           !GetCreature().PeekPCurrentAction()->IsActionReadyForInterruption() && !IsStopping())
        PBrain->SuspendSuspendableProcess();
    if (!IS_THING_ALIVE(ThingReactingTo)) {
        Terminate();
        return;
    }
    if (AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED, ThingReactingTo, 33, 0.0027777f) != AI_ACTION_FINISHED) {
        Terminate();
        return;
    }
    if (dialogue_tag != 0 && reaction_manager.CanPlayDeedReactionSpeech(CurrentDeedType)) {
        CTCTalk* ptc_talk;
        GetCreature().GetTC(&ptc_talk);
        bool allow_on_non_ai_speaker = ptc_deedlog->PeekPDeedEffectsDef(CurrentDeedType)->AllowSpeechOnNonPureAISpeaker;
        CTCTalk::ESpeechPlaybackResult ret;
        for (;;) {
            ret = ptc_talk->PlayAISpeech(dialogue_tag, allow_on_non_ai_speaker, ptc_deedlog->PeekPDeedEffectsDef(CurrentDeedType)->ReactionDialogueLayer, false, true);
            if (ret != CTCTalk::SPEECH_PLAYBACK_FAILED_RETRY)
                break;
            if (AIAction_DoNothing() != AI_ACTION_IN_PROGRESS) {
                Terminate();
                return;
            }
        }
        if (ret == CTCTalk::SPEECH_PLAYBACK_STARTED) {
            DeliveredSpeech = true;
            reaction_manager.LogDeedReactionPlayingSpeech(CurrentDeedType, true);
        }
        DeliveredReaction = true;
    }
    if (*anim_name != "") {
        if (IS_THING_DEAD(ThingReactingTo) ||
            AIAction_PerformActionTargetFaceAndWait(CCreatureAction_PlayAnimation(GetCreature(), *anim_name, CSoundPair(), false, true, false, 0, 0),
                                                    ThingReactingTo, false) != AI_ACTION_FINISHED) {
            Terminate();
            return;
        }
        DeliveredReaction = true;
    }
    if (dialogue_tag == 0 && *anim_name == "")
        DeliveredReaction = true;
    CTCTalk* ptc_talk;
    GetCreature().GetTC(&ptc_talk);
    while (ptc_talk->IsTalking() && !IsStopping())
        PBrain->SuspendSuspendableProcess();
    Terminate();
}