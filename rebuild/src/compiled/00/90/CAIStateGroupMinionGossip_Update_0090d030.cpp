// CAIStateGroup_MinionGossip::Update() -- pair up with a gossip partner (answer a nearby gossip request or post one and wait), look at it, greet, walk up, interact, then take turns listening and talking until a 1-in-5 roll ends it
// Leaf: ?Update@CAIStateGroup_MinionGossip@@MAEXXZ
// Flags: /d2inlT91
// Inline-budget: /d2inlT91 -- an inline decision only: retail expands GetPOriginator<CThingAICreature> at sites 1-2 (0x0090D132, 0x0090D172)
//   and calls it at site 3 (0x0090D1AD -> COMDAT 0x0090DF30), and calls FindMostRecentMessage<CFilter_Bind4<...>> (0x0090D120 -> 0x0090E4A0).
//   Measured on this source (length, call list by tools/cseq.py): no flag 3165 and /d2inlT90 3237 -- site 2 called (one GetPOriginator call
//   in place of retail's three GetPItem); /d2inlT91, T95, T99, T100 RELOCATION_MATCH; /d2inlT109, T110 3301 -- FindMostRecentMessage and
//   the CFilter_Bind4 operator() expanded in place. Smallest N = 91; no /d2inlS flag needed.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0090D030, 3269 bytes (plain `ret`).
// Exits (s12, FITTED -- chosen by grading): each failure before the conversation loop and inside its inner while is `Terminate(); return;`;
//   each failure directly in the do-while body `break`s to the closing Terminate() (the CROUCH_INTO test must reach the shared tail: retail
//   jne 0x0090D99A -> 0x0090DCE6). The earlier single `goto terminate;` label form compiled to a function-wide ebx/ebp register permutation
//   (42 hunks); walk loop as do { } while (result == AI_ACTION_IN_PROGRESS).
// Literal (EVIDENCED): LookAtThing speed 0.0027777f, retail `push 3B360A13h` at 0x0090D3FC / 0x0090D71E / 0x0090D8D8 (1.0f / 360.0f = 3B360B61h).
// Locals: this_sex / their_sex / nav_route_calculated / ptc_mode_manager as in the Anniversary PDB (weak source); `initiator` set at both arm
//   ends = retail `xor al,al` / `mov al,1` / join store [esp+2Ch] (0x0090D294..0x0090D3BD).
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGCREATUREBASE_CREATURETYPE
#define FABLE_RETAIL_CTHINGCREATUREBASE_GETSEX
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMQUERY
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_FORCEINLINE_CTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_INLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_OUTOFLINE
#define FABLE_RETAIL_CMESSAGEEVENT
#define FABLE_RETAIL_CMESSAGEEVENT_ORIGINATOR_ORIGINAL
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS_ISOFTYPE_CTOR
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS_IN2DRANGE_CTOR
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_FINDRECENT
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_REMOVE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAISTATEGROUPBASE_INTERACTWITH
#define FABLE_RETAIL_CAISTATEGROUP_MINIONGOSSIP
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/EMessageEventType.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBuilding.h"
#include "engine/CTCGossip.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_MinionGossip.h"
#include "engine/CMessageEvent.h"
#include "engine/CMessageEventFilters.h"
#include "engine/CMessageEventManager.h"
#include "engine/CFilter_Bind4.h"
#include "engine/CThingFilter_IsValidGossipee.h"
#include "engine/CThingFilter_IsNavLineOfSightToThing.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/GFRandom.h"
#include "engine/CSoundPair.h"
#include "engine/CAnimFilterAnyGroup.h"
#include "engine/CCreatureAction_PlayAnimation.h"
#include "engine/ThingAlive.h"
enum ESex { SEX_NONE = 0, SEX_MALE = 1, SEX_FEMALE = 2 };   // same completion of the forward-declared enum as landed 0x008330D0 (enumerator names descriptive)

inline CThingBuilding* CThingAICreature::GetHomeBuilding() const { return HomeBuilding.Get(); }

// File-scope animation / interaction tables of the gossip TU (retail .data; names descriptive -- no symbol in either PDB name set;
// declared extern like the landed sibling 0x00942920's g_storyGestures). Contents read from retail: Near = {ARMS_WIDE_GREET,
// HAND_UP_GREET}; Far = {ARMS_WIDE_GREET, WAVING_AT_DISTANCE, CALLING_OVER_FOR_FIGHT, SHOUTING_WITH_HANDS_CUPPED}; Rare = {"CONV_GENERAL"};
// TalkAnims = ten ST_OPINION_* reactions; Interactions[this_sex * 3 + their_sex] = {0, RUFFLE_HAIR, HUG_CHILD, RUFFLE_HAIR, SHAKE_HANDS,
// KISS, HUG_CHILD, HUG, HUG} (VILLAGER_INTERACTION_*).
extern const char* GossipGreetingsNear[];          // 0x0138C3C4 (2 entries)
extern const char* GossipGreetingsFar[];           // 0x0138C3CC (4)
extern const char* GossipTalkAnimsRare[];          // 0x0138C3DC (1)
extern const char* GossipTalkAnims[];              // 0x0138C3E0 (10)
extern const char* GossipInteractions[];           // 0x0138C408 (9)

void CAIStateGroup_MinionGossip::Update()
{
    bool nav_route_calculated;
    Critical = false;
    NowRelyingOnGossipee = false;
    GetCreature().GetTC(&PTCGossip);

    const CMessageEvent* message;
    {
        CThingFilter_IsNavLineOfSightToThing line_of_sight(GetCreature());
        message = GetWorld().GetMessageEventManager().FindMostRecentMessage(
        CFilter_Bind4<CMessageEventFilter_IsOfType, CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange>,
                      CMessageEventFilter_ThingSearchFilter<CThingFilter_IsValidGossipee>,
                      CMessageEventFilter_ThingSearchFilter<CThingFilter_IsNavLineOfSightToThing> >(
            CMessageEventFilter_IsOfType(MESSAGE_EVENT_REQUEST_GOSSIP),
            CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange>(CThingFilter_In2DRange(GetCreature().GetPos(), 16.0f)),
            CMessageEventFilter_ThingSearchFilter<CThingFilter_IsValidGossipee>(CThingFilter_IsValidGossipee(GetCreature())),
            CMessageEventFilter_ThingSearchFilter<CThingFilter_IsNavLineOfSightToThing>(line_of_sight)));
    }

    bool initiator;
    if (message) {
        {
            CThingAICreature* originator = message->GetPOriginator<CThingAICreature>();
            if (!originator || originator->IsDead()) {
                Terminate();
                return;
            }
        }
        if (!message->GetPOriginator<CThingAICreature>()->HasTC(TCI_GOSSIP)) {
            Terminate();
            return;
        }
        PTCGossip->Gossipee = CIntelligentPointer<CThingAICreature>(message->GetPOriginator<CThingAICreature>());
        PTCGossip->Gossipee->GetTC(&PTCGossipOther);
        GetWorld().GetMessageEventManager().RemoveMessage(message);
        PTCGossip->MyTurnToSpeak = true;
        PTCGossipOther->Gossipee = CIntelligentPointer<CThingAICreature>(&GetCreature());
        PTCGossipOther->MyTurnToSpeak = false;
        initiator = false;
    } else {
        GetWorld().GetMessageEventManager().AddMessage(MESSAGE_EVENT_REQUEST_GOSSIP, CMainGameComponent::GetConstantFPS() * 4,
                                                       GetCreature().PeekPos(), &GetCreature(), 0, 16.0f, 16.0f, 0);
        long timeout = CWorld::GetFrame() + CMainGameComponent::GetConstantFPS() * 4;
        while (IS_THING_DEAD(PTCGossip->Gossipee)) {
            if (AIAction_DoNothing() == AI_ACTION_TERMINATE_PROCESS) {
                Terminate();
                return;
            }
            if (CWorld::GetFrame() > timeout) {
                Terminate();
                return;
            }
        }
        GetCreature().GetTC(&PTCGossip);
        PTCGossip->Gossipee->GetTC(&PTCGossipOther);
        initiator = true;
    }
    NowRelyingOnGossipee = true;

    if (initiator || GFRandom(100, GetPWorldSeed()) < 20) {
        if (AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED, CIntelligentPointer<const CThing>(PTCGossip->Gossipee), 17, 0.0027777f) != AI_ACTION_FINISHED) {
            Terminate();
            return;
        }
        const char* greeting;
        if ((PTCGossip->Gossipee->GetPos() - GetCreature().GetPos()).GetMagnitude() < 5.0f)
            greeting = GossipGreetingsNear[GFRandom(2, GetPWorldSeed())];
        else
            greeting = GossipGreetingsFar[GFRandom(4, GetPWorldSeed())];
        if (AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), greeting, CSoundPair(), false, true, false, 0, 0), false) != AI_ACTION_FINISHED) {
            Terminate();
            return;
        }
    }

    {
        long this_sex = GetCreature().GetCreatureType() == HUMAN_CHILD ? 0 : GetCreature().GetSex();
        long their_sex = PTCGossip->Gossipee->GetCreatureType() == HUMAN_CHILD ? 0 : PTCGossip->Gossipee->GetSex();
        const char* interaction = GossipInteractions[this_sex * 3 + their_sex];
        if (this_sex != 0 && their_sex != 0 && GFRandom(7, GetPWorldSeed()) == 0)
            interaction = "VILLAGER_INTERACTION_EXCHANGE_MONEY";
        if (GetCreature().GetHomeBuilding() == PTCGossip->Gossipee->GetHomeBuilding()) {
            long roll = GFRandom(2, GetPWorldSeed());
            if (roll == 0)
                interaction = 0;
        }
        EAIActionReturn result;
        do
            result = AIAction_MoveToPosProximity(ACTION_REPEAT_CALLBACK, PTCGossip->Gossipee->GetPos(), 1.02f, nav_route_calculated,
                                                 AI_MOVEMENT_WALKING, true, false, true, 0, true, 0.0f);
        while (result == AI_ACTION_IN_PROGRESS);
        if (result != AI_ACTION_FINISHED) {
            Terminate();
            return;
        }
        if (interaction) {
            Critical = true;
            AIAction_InteractWith(ACTION_REPEAT_UNTIL_FINISHED, PTCGossip->Gossipee, interaction, 30, initiator);
            Critical = false;
        }
        CTCCreatureModeManager* ptc_mode_manager;
        GetCreature().GetTC(&ptc_mode_manager);
        do {
            if (this_sex != SEX_MALE) {
                if (AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED, CIntelligentPointer<const CThing>(PTCGossip->Gossipee), 17, 0.0027777f) == AI_ACTION_TERMINATE_PROCESS)
                    break;
                if (AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), "CONV_LISTENING_INTO", CSoundPair(), false, true, false, 0, 0), false) != AI_ACTION_FINISHED)
                    break;
                while (!PTCGossip->MyTurnToSpeak) {
                    if (AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), "CONV_LISTENING_LOOP", CSoundPair(), false, true, false, 0, 0), false) != AI_ACTION_FINISHED)
                        break;
                }
                if (AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), "CONV_LISTENING_OUTOF", CSoundPair(), false, true, false, 0, 0), false) != AI_ACTION_FINISHED)
                    break;
            } else {
                while (!PTCGossip->MyTurnToSpeak) {
                    if (AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED, CIntelligentPointer<const CThing>(PTCGossip->Gossipee), 17, 0.0027777f) == AI_ACTION_TERMINATE_PROCESS) {
                        Terminate();
                        return;
                    }
                }
            }
            if (their_sex == 0 && this_sex != 0) {
                if (AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), "ST_CROUCH_TALK_TO_CHILD_INTO", CSoundPair(), false, true, false, 0, 0), false) != AI_ACTION_FINISHED)
                    break;
                long count = GFRandom(4, GetPWorldSeed()) + 3;
                for (long i = 0; i < count; i++) {
                    if (AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), "ST_CROUCH_TALK_TO_CHILD_LOOP", CSoundPair(), false, true, false, 0, 0), false) != AI_ACTION_FINISHED)
                        break;
                }
                PTCGossip->MyTurnToSpeak = false;
                PTCGossipOther->MyTurnToSpeak = true;
                if (AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), "ST_CROUCH_TALK_TO_CHILD_OUTOF", CSoundPair(), false, true, false, 0, 0), false) != AI_ACTION_FINISHED)
                    break;
            } else {
                ptc_mode_manager->AddMode(0x1B);
                long count = GFRandom(4, GetPWorldSeed()) + 3;
                const char* last_anim = 0;
                for (long i = 0; i < count; i++) {
                    const char* anim;
                    if (GFRandom(11, GetPWorldSeed()) == 0) {
                        do
                            anim = GossipTalkAnims[GFRandom(10, GetPWorldSeed())];
                        while (anim == last_anim);
                    } else if (GFRandom(2, GetPWorldSeed()) == 0)
                        anim = GossipTalkAnimsRare[GFRandom(1, GetPWorldSeed())];
                    else
                        anim = "ST_IDLE";
                    last_anim = anim;
                    if (GetCreature().HasAnim(anim, CAnimFilterAnyGroup())) {
                        if (AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), anim, CSoundPair(), false, true, false, 0, 0), false) != AI_ACTION_FINISHED)
                            break;
                    } else if (AIAction_DoNothing() != AI_ACTION_FINISHED)
                        break;
                }
                PTCGossip->MyTurnToSpeak = false;
                PTCGossipOther->MyTurnToSpeak = true;
            }
            ptc_mode_manager->RemoveMode(0x1B);
        } while (GFRandom(100, GetPWorldSeed()) >= 20);
    }
    Terminate();
}