// NScript::CQR_EscortTraderScript::CTraderToEscort::Main() -- trader hiring, follow/wait, arrival and reward.
// Grade: DIFFER(14526v14510) -- predicate prefix only: two out-of-line ~CPredicateBase calls; mechanism escalated (ESCALATE.md: Main inherits ~CIsThingAlive own-pipe body; the Codex depth-0 device fails pick-any vs 0x00E241B0)
// Source evidence: the original debug binary shows a loop-scope guard before the frame increment loop.
// The donor confirms the branch-local bonus index, one shared reward, and the two function-scope distances.
// Inline-budget: predicate destructor definitions and two retail-called door/position statements use scoped depth zero.
// Flags: (none)
// Leaf: ?Main@CTraderToEscort@CQR_EscortTraderScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E20600, 14510 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven, two sources): slot 1 of the CTraderToEscort entity vtable 0x012E151C that Alloc 0x00E1E7A0 stores (slot 0 scalar
//   deleting dtor 0x00E1E860, 2 Init 0x00E1F7E0, 3 GetParentScript 0x00E1E840, 4 CEntityScriptBase::OnPersist 0x00CDEBC0,
//   5 OnPredicateFail 0x00E1E850); Ego_r.pdb and FableWin.pdb both hold ?Main@CTraderToEscort@CQR_EscortTraderScript@NScript@@EAEXXZ.
// Callees: GetRandomSpeech 0x00E23F30, ResetTraderRandomSpeechTime 0x00E1FEB0 (landed as RestartRandomSpeechTimer: NAME_FIXES.tsv),
//   GetDistanceBetweenThings 0x00CBE512, IsDistanceBetweenThingsUnder, FadeOutAndKillEntity.
// Layout: engine/NScript_CQR_EscortTraderScript.h with the chunk 818 TRADERTOESCORT opt-in (hdr/README.md).
#define FABLE_RETAIL_PREDICATE_AND_PRIVATE_PASS25
#define FABLE_RETAIL_CPREDICATEBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CISTHINGALIVE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CCOUNTEDPOINTER_ORIGINAL_COMPLETE_MEMBERS_PASS22
#define FABLE_RETAIL_CSCRIPTTHING_ORIGINAL_ACCESS
#define FABLE_RETAIL_CTIMER_ESCORT_EQUAL_SECONDS_PASS25
#define FABLE_RETAIL_CTIMER_ORIGINAL_ACCESS
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CBASEOBJECT_ORIGINAL_ACCESS
#define FABLE_RETAIL_CSCRIPTRESOURCE_ORIGINAL_ACCESS
#define FABLE_RETAIL_NSCRIPT
// Header binding correction: NScript expands the conversion and calls GetString at 0x00415D70.
#define FABLE_RETAIL_CDEFSTRING_OUTOFLINE_GETSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_ESCORT_TRADER_MASTER_DATA_ACCESSOR
#define FABLE_RETAIL_CQR_ESCORTTRADERSCRIPT_TRADERTOESCORT
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "stlport/fable_containers.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/NScript_CQR_EscortTraderScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CPredicate_And.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer(); // 0x006E7AB0
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CTimer.h"
#include "engine/TRetVoid.h"
#include "engine/CScriptDef.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_fade.h"
#include "stlport/fable_map.h"
enum EScriptAIPriority {
    SCRIPT_AI_PRIORITY_LOWEST = 0,
    SCRIPT_AI_PRIORITY_LOW = 1,
    SCRIPT_AI_PRIORITY_MEDIUM = 2,
    SCRIPT_AI_PRIORITY_HIGH = 3,
    SCRIPT_AI_PRIORITY_HIGHEST = 4
};
enum ETextGroupSelectionMethod {
    GROUP_SELECT_NONE = 0,
    GROUP_SELECT_RANDOM = 1,
    GROUP_SELECT_RANDOM_NO_REPEAT = 2,
    GROUP_SELECT_SEQUENTIAL = 3,
    GROUP_SELECT_FIRST_ONLY = 4
};
enum EScriptEntityMoveType { ENTITY_MOVE_WALK = 0, ENTITY_MOVE_RUN = 1 };
#include <stdlib.h> // rand, MSVCR71 0x00BFEB16
float FABLE_FASTCALL GetDistanceBetweenThings(const CScriptThing&, const CScriptThing&); // 0x00CBE512
extern CGameScriptInterfaceBase* pDSTGame;                                               // 0x0143E8F8
#include "engine/fable_script_def_global.h"
#define FABLE_RETAIL_SCRIPT_INLINE_ESCORT_PASS25
#include "engine/FableScriptInlineEscortPass25.h"
#define T IsActiveThreadTerminating()
#define TT                                                                                                   \
    if (T)                                                                                                   \
    return _Fail_Val.Return()
#define NSF Game.NewScriptFrame()
#define TE_RELEASE                                                                                           \
    if (seh_me.IsValid())                                                                                    \
        seh_me.Release();
#define TE_START(PRIORITY)                                                                                   \
    TE_RELEASE                                                                                               \
    while (!Game.StartScriptingEntity(Me, seh_me, PRIORITY)) {                                               \
        NSF;                                                                                                 \
        TT;                                                                                                  \
    }                                                                                                        \
    TT;
#define TE_SAY(TEXT)                                                                                         \
    if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {                                                    \
        seh_me.Speak(Game.GetHero(), TEXT, GROUP_SELECT_NONE, false, true, false);                           \
        while (seh_me.IsPerformingScriptTask()) {                                                            \
            NSF;                                                                                             \
            TT;                                                                                              \
        }                                                                                                    \
        TT;                                                                                                  \
    }
#define TE_CONV                                                                                              \
    long HLStyleConversationIndex = Game.AddNewConversation(Me, false, false);                               \
    Game.AddPersonToConversation(HLStyleConversationIndex, Game.GetHero());
#define TE_ADD(TEXT) Game.AddLineToConversation(HLStyleConversationIndex, TEXT, false, Me, Game.GetHero());
#define TE_LINE(TEXT) {TE_CONV TE_ADD(TEXT)}
#define TE_MOVIE                                                                                             \
    CScriptGameResourceObjectMovieBase speech_movie;                                                         \
    Game.StartMovieSequence("", speech_movie);                                                               \
    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);

void NScript::CQR_EscortTraderScript::CTraderToEscort::Main() {
    TRetVoid _Fail_Val;
    float distanceHeroToTrader;
    float distanceHeroToExit;
    SetPredicate(CIsThingAlive(Me) && CIsThingAlive(Game.GetHero()));
    NSF;
    TT;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    CTimer TraderProximityTimer;
    CScriptThing regionExit;
    TraderProximityTimer = 0;
    if (!ParentClass.AddedBonusObject) {
        TT;
        ParentClass.AddedBonusObject = true;
        if ((unsigned long)GetMasterData()->OrchardFarmTraderEscortCounter >=
            pGlobalScriptDef->EscortTraderRewardObjectPerfect.size()) {
            TT;
            long idx = rand() % pGlobalScriptDef->EscortTraderRewardObjectPerfect.size();
            // Debug script assertion at line 529: idx must be within the reward vector.
            ParentClass.BonusObject = pGlobalScriptDef->EscortTraderRewardObjectPerfect[idx];
            Game.AddItemToContainer(Me, ParentClass.BonusObject);
        } else {
            TT;
            // Debug script assertion at line 537: the master-data counter must be nonnegative.
            ParentClass.BonusObject =
                pGlobalScriptDef
                    ->EscortTraderRewardObjectPerfect[GetMasterData()->OrchardFarmTraderEscortCounter];
            Game.AddItemToContainer(Me, ParentClass.BonusObject);
        }
    }
    Game.EntityGiveGold(Me, pGlobalScriptDef->EscortTraderRewardDownpayment * 4);
    if (ParentClass.HiredByTrader) {
        TT;
        Game.EntityGiveGold(Me, -pGlobalScriptDef->EscortTraderRewardDownpayment);
        Game.EntitySetAsAllowedToFollowHero(Me, true);
    }
    if (ParentClass.QuestStage == QUEST_STAGE_HIRED_FOLLOWING) {
        TT;
        ParentClass.ExpressionEnableFollow(Me, seh_me);
    }
    if (false) {
    } else
        for (;; NSF) {
            TT;
            if (ParentClass.TotallyLost) {
                TT;
                ParentClass.ForceDisableFollow(Me);
                TE_MOVIE
                Game.EntitySetFacingAngleTowardsThing(Game.GetHero(), Me, false);
                NSF;
                TT;
                NSF;
                TT;
                Game.EntitySetFacingAngleTowardsThing(Me, Game.GetHero(), false);
                NSF;
                TT;
                NSF;
                TT;
                TE_START(SCRIPT_AI_PRIORITY_HIGHEST)
                TE_SAY("TEXT_QST_053_TOTALLY_LOST")
                Game.GiveHeroRenownPoints(pGlobalScriptDef->EscortTraderFailureRenown);
                CScriptThing Exit = Game.GetNearestWithDefName(Me, "REGION_EXIT_POINT");
                if (Exit.IsAlive()) {
                    TT;
                    if (seh_me.IsValid()) {
                        TT;
                        seh_me.MoveToPosition(Exit.GetPos(), 1.0f, ENTITY_MOVE_RUN, false, true);
                    }
                    while (!IsDistanceBetweenThingsUnder(Me, Exit, 2.0f)) {
                        NSF;
                        TT;
                    }
                    TT;
                    FadeOutAndKillEntity(Me, 1.0f, true, true);
                    ParentClass.TraderDepartedLost = true;
                }
            }
            if (ParentClass.TraderRandomSpeechTimer == 0 &&
                ParentClass.GetTraderRandomSpeechEnabled()) {
                TT;
                CCharString randomSpeech = GetRandomSpeech();
                TE_LINE(randomSpeech)
                ParentClass.ResetTraderRandomSpeechTime();
            }
            if (ParentClass.TraderHit && ParentClass.QuestStage != QUEST_STAGE_NONE &&
                ParentClass.TraderHitDetectTimer == 0) {
                TT;
                TE_LINE("TEXT_QST_053_HIT")
                ParentClass.TraderHit = false;
                ParentClass.TraderHitDetectTimer = pGlobalScriptDef->EscortTraderHitDetectTimer;
                ParentClass.ResetTraderRandomSpeechTime();
            }
            if (ParentClass.QuestStage == QUEST_STAGE_NONE) {
                TT;
                bool hero_spoken = false;
                while (!hero_spoken) {
                    NSF;
                    TT;
                    if (IsDistanceBetweenThingsUnder(Me, Game.GetHero(), 3.0f)) {
                        TT;
                        if (!ParentClass.GreetedHero) {
                            TE_LINE("TEXT_QST_053_GREETING")
                            ParentClass.GreetedHero = true;
                        }
                        Game.EntityForceToLookAtThing(Me, Game.GetHero());
                        while (!hero_spoken) {
                            NSF;
                            TT;
                            if (ParentClass.ExpressionDetectedFollow) {
                                TT;
                                if (ParentClass.TraderExpressionDetectTimer == 0) {
                                    TT;
                                    TE_LINE("TEXT_QST_053_HASTY1")
                                }
                                ParentClass.ForceDisableFollow(Me);
                            }
                            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                                TT;
                                hero_spoken = true;
                            } else if (IsDistanceBetweenThingsOver(Me, Game.GetHero(), 5.0f)) {
                                TT;
                                TE_RELEASE
                                break;
                            }
                        }
                        TT;
                        Game.EntityForceToLookAtNothing(Me);
                    }
                }
                TT;
                ParentClass.QuestStage = QUEST_STAGE_SETUP;
            } else if (ParentClass.QuestStage == QUEST_STAGE_SETUP) {
                TT;
                if (!ParentClass.TraderIntroDone) {
                    TT;
                    TE_START(SCRIPT_AI_PRIORITY_HIGHEST) {
                        TE_MOVIE
                        Game.EntitySetFacingAngleTowardsThing(Game.GetHero(), Me, false);
                        TE_SAY("TEXT_QST_053_INTRO_01")
                        TE_SAY("TEXT_QST_053_INTRO_02")
                        TE_SAY("TEXT_QST_053_INTRO_03")
                        TE_SAY("TEXT_QST_053_INTRO_04")
                        TE_START(SCRIPT_AI_PRIORITY_HIGH)
                        ParentClass.TraderIntroDone = true;
                    }
                }
                TE_START(SCRIPT_AI_PRIORITY_HIGH) {
                    CTimer timer_var;
                    timer_var = 5;
                    while (true) {
                        NSF;
                        TT;
                        if (ParentClass.ExpressionDetectedFollow) {
                            TT;
                            if (ParentClass.TraderExpressionDetectTimer == 0) {
                                TT;
                                TE_LINE("TEXT_QST_053_HASTY2")
                            }
                            ParentClass.ForceDisableFollow(Me);
                        }
                        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                            TT;
                            Game.GiveHeroYesNoQuestion("TEXT_QST_053_QUEST_ON_QUESTION",
                                                       "TEXT_OBJECT_HERO_ANSWER_YES",
                                                       "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
                            long YNQAnswer;
                            while (!((YNQAnswer = Game.MsgIsQuestionAnsweredYesOrNo()) >= 0)) {
                                NSF;
                                TT;
                            }
                            TT;
                            if (YNQAnswer == 1) {
                                TT;
                                break;
                            }
                        }
                        if (timer_var == 0) {
                            TT;
                            TE_RELEASE
                        }
                    }
                    TT;
                }
                if (!ParentClass.DoneExpressions) {
                    TT;
                    TE_START(SCRIPT_AI_PRIORITY_HIGHEST) {
                        TE_MOVIE
                        TE_SAY("TEXT_QST_053_INTRO_05")
                        TE_SAY("TEXT_QST_053_PLAN_01")
                        TE_SAY("TEXT_QST_053_PLAN_02")
                        TE_SAY("TEXT_QST_053_PLAN_03")
                        TE_SAY("TEXT_QST_053_PLAN_04")
                        TE_SAY("TEXT_QST_053_PLAN_05")
                        ParentClass.ResetTraderRandomSpeechTime();
                        Game.GiveHeroGold(pGlobalScriptDef->EscortTraderRewardDownpayment);
                        Game.EntityGiveGold(Me, -pGlobalScriptDef->EscortTraderRewardDownpayment);
                        Game.GiveHeroQuestCardDirectly("OBJECT_QUEST_ORCHARD_FARM_ESCORT",
                                                       Game.GetActiveQuestName(), true);
                        Game.SetQuestCardObjective(Game.GetActiveQuestName(),
                                                   "TEXT_QUEST_ORCHARD_FARM_ESCORT_OBJECTIVE_02",
                                                   "OrchardFarm", "Greatwood");
                        Game.EntitySetAsAllowedToFollowHero(Me, true);
                        Game.EntityStopFollowing(Me);
                        ParentClass.HiredByTrader = true;
                        ParentClass.RegionChangeDetected = false;
                        ParentClass.DoneExpressions = true;
                        TE_RELEASE
                    }
                }
                while (!ParentClass.TraderFollowing) {
                    NSF;
                    TT;
                    if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                        TT;
                        TE_START(SCRIPT_AI_PRIORITY_HIGHEST) {
                            TE_MOVIE
                            TE_SAY("TEXT_QST_053_WAITING_01")
                            TE_RELEASE
                            ParentClass.ResetTraderRandomSpeechTime();
                        }
                    }
                    if (ParentClass.ExpressionDetectedFollow) {
                        TT;
                        TE_LINE("TEXT_QST_053_FOLLOW")
                        if (!ParentClass.AddedQuestInfo) {
                            TT;
                            ParentClass.TraderHealthBarID = Game.AddQuestInfoBarHealth(
                                Me, CRGBColour(0, 255, 0, 255), "HUD_QUEST_ICON_TRADER", 1.0f);
                            ParentClass.TraderInitialHealth = Game.GetHealth(Me);
                            Game.DisplayQuestInfo(true);
                            ParentClass.AddedQuestInfo = true;
                        }
                        ParentClass.ExpressionEnableFollow(Me, seh_me);
                        ParentClass.ResetTraderRandomSpeechTime();
                    }
                }
                TT;
            } else if (ParentClass.QuestStage == QUEST_STAGE_HIRED_FOLLOWING) {
                TT;
                if (ParentClass.TraderFollowing) {
                    TT;
                    regionExit = Game.GetNearestWithDefName(Game.GetHero(), "REGION_EXIT_POINT");
                    distanceHeroToTrader = GetDistanceBetweenThings(Me, Game.GetHero());
                    distanceHeroToExit = GetDistanceBetweenThings(Game.GetHero(), regionExit);
                    TE_CONV
                    if (distanceHeroToExit < pGlobalScriptDef->EscortTraderRegionExitWait &&
                        distanceHeroToTrader > pGlobalScriptDef->EscortTraderRegionExitDist &&
                        TraderProximityTimer == 0) {
                        TT;
                        TE_ADD("TEXT_QST_053_EXIT_PROXIMITY_WARNING")
                        TraderProximityTimer = pGlobalScriptDef->EscortTraderProximityTimer;
                        ParentClass.ResetTraderRandomSpeechTime();
                    } else if (distanceHeroToExit < pGlobalScriptDef->EscortTraderRegionExitStop &&
                               distanceHeroToTrader > pGlobalScriptDef->EscortTraderRegionExitDist) {
                        TT;
                        TE_ADD("TEXT_QST_053_EXIT_PROXIMITY_DETACH")
                        ParentClass.ForceEnableWait(Me);
                        ParentClass.ResetTraderRandomSpeechTime();
                    } else if (distanceHeroToTrader > pGlobalScriptDef->EscortTraderFollowProximity1 &&
                               TraderProximityTimer == 0) {
                        TT;
                        TE_ADD("TEXT_QST_053_PROXIMITY_WARNING")
                        TraderProximityTimer = pGlobalScriptDef->EscortTraderProximityTimer;
                        ParentClass.ResetTraderRandomSpeechTime();
                    } else if (distanceHeroToTrader > pGlobalScriptDef->EscortTraderFollowProximity2) {
                        TT;
                        TE_ADD("TEXT_QST_053_PROXIMITY_DETACH")
                        ParentClass.ForceEnableWait(Me);
                        ParentClass.ResetTraderRandomSpeechTime();
                    } else {
                        TT;
                        if (ParentClass.ExpressionDetectedWait) {
                            TT;
                            TE_ADD("TEXT_QST_053_WAIT_01")
                            ParentClass.ExpressionEnableWait(Me, seh_me);
                            ParentClass.ResetTraderRandomSpeechTime();
                        }
                    }
                }
                if (ParentClass.RegionChangeDetected) {
                    TT;
                    TE_CONV
                    if (ParentClass.TraderFollowing) {
                        TT;
                        if (ParentClass.TheBlackGateIsClosed) {
                            TT;
                            TE_ADD("TEXT_QST_053_FOUND")
                            ParentClass.TheBlackGateIsClosed = false;
                            ParentClass.GoneWrongWay = false;
                            ParentClass.ResetTraderRandomSpeechTime();
                        } else if (ParentClass.TravellingWrongPath()) {
                            TT;
                            if (ParentClass.GoneWrongWay) {
                                TT;
                                ParentClass.TotallyLost = true;
                            } else {
                                TT;
                                TE_ADD("TEXT_QST_053_LOST")
                                TE_ADD("TEXT_QST_053_LOST_DETACH")
                                ParentClass.ForceEnableWait(Me);
                                ParentClass.GoneWrongWay = true;
                                ParentClass.ResetTraderRandomSpeechTime();
                            }
                        } else if (ParentClass.InFinalRegion()) {
                            TT;
                            NSF;
                            TT;
                            ParentClass.GoneWrongWay = false;
                            CScriptThing FarmDoors = Game.GetThingWithScriptName("FarmRearEntrance");
                            CScriptThing OR_GoodHSP = Game.GetThingWithScriptName("OrchardFarmGoodHSP");
                            CScriptThing OR_EvilHSP = Game.GetThingWithScriptName("OrchardFarmEvilHSP");
                            bool inThroughTheBackDoor = GetDistanceBetweenThings(OR_EvilHSP, Game.GetHero()) <
                                                        GetDistanceBetweenThings(OR_GoodHSP, Game.GetHero());
                            CScriptThing teleportHero;
                            CScriptThing teleportTrader;
                            if (inThroughTheBackDoor) {
                                TT;
                                teleportHero = Game.GetThingWithScriptName("M_ET_BlackGateHero");
                                teleportTrader = Game.GetThingWithScriptName("M_ET_BlackGateTrader");
                            } else {
                                TT;
                                teleportHero = Game.GetThingWithScriptName("M_ET_FrontDoorHero");
                                teleportTrader = Game.GetThingWithScriptName("M_ET_FrontDoorTrader");
                            }
                            Game.EntityTeleportToThing(Me, teleportTrader, false);
                            Game.EntityTeleportToThing(Game.GetHero(), teleportHero, false);
                            NSF;
                            TT;
                            NSF;
                            TT;
                            Game.EntitySetFacingAngleTowardsThing(Game.GetHero(), Me, false);
                            NSF;
                            TT;
                            NSF;
                            TT;
                            Game.EntitySetFacingAngleTowardsThing(Me, Game.GetHero(), false);
                            NSF;
                            TT;
                            NSF;
                            TT;
                            if (inThroughTheBackDoor && FarmDoors.IsAlive()) {
                                TT;
                                TE_START(SCRIPT_AI_PRIORITY_HIGHEST) {
                                    TE_MOVIE
                                    TE_SAY("TEXT_QST_053_THE_BLACK_GATE_IS_CLOSED")
                                    TE_RELEASE
                                    ParentClass.TheBlackGateIsClosed = true;
                                    Game.MiniMapAllowRouteBetweenRegions("GreatwoodEntrance", "OrchardFarm",
                                                                         false);
                                }
                            } else {
                                TT;
                                if (!ParentClass.TraderAtDestination) {
                                    TT;
                                    CScriptThing door = Game.GetThingWithScriptName("OFFarmhouseDoor");
                                    bool closeDoor = false;
                                    if (door.IsAlive() && !door.IsOpenDoor()) {
                                        TT;
                                        Game.OpenDoor(door);
                                        closeDoor = true;
                                    }
                                    Game.MiniMapRemoveMarker(Me);
                                    ParentClass.TraderFollowing = false;
                                    Game.RemoveQuestInfoElement(ParentClass.TraderHealthBarID);
                                    ParentClass.ForceDisableFollow(Me);
                                    if (!ParentClass.TraderDeparted) {
                                        TT;
                                        long reward;
                                        EscortTraderSuccessType success = ParentClass.GetSuccessType(Me);
                                        Game.EntitySetFacingAngleTowardsThing(Game.GetHero(), Me, false);
                                        TE_START(SCRIPT_AI_PRIORITY_HIGHEST)
                                        if (success == ET_SUCCESS_TYPE_PERFECT) {
                                            TT;
                                            TE_MOVIE
                                            TE_SAY("TEXT_QST_053_SUCCESS_PERFECT_01")
                                            TE_SAY("TEXT_QST_053_SUCCESS_PERFECT_02")
                                            reward = pGlobalScriptDef->EscortTraderRewardDownpayment * 3;
                                            Game.GiveHeroGold(reward);
                                            Game.EntityGiveGold(Me, -reward);
                                            Game.GiveHeroObject(ParentClass.BonusObject, -1, false);
                                            Game.RemoveItemFromContainer(Me, ParentClass.BonusObject);
                                        } else if (success == ET_SUCCESS_TYPE_HIGH) {
                                            TT;
                                            TE_MOVIE
                                            TE_SAY("TEXT_QST_053_SUCCESS_HIGH_01")
                                            TE_SAY("TEXT_QST_053_SUCCESS_HIGH_02")
                                            reward = pGlobalScriptDef->EscortTraderRewardDownpayment * 2;
                                            Game.GiveHeroGold(reward);
                                            Game.EntityGiveGold(Me, -reward);
                                        } else if (success == ET_SUCCESS_TYPE_MEDIUM) {
                                            TT;
                                            TE_MOVIE
                                            TE_SAY("TEXT_QST_053_SUCCESS_MEDIUM_01")
                                            TE_SAY("TEXT_QST_053_SUCCESS_MEDIUM_02")
                                            reward = pGlobalScriptDef->EscortTraderRewardDownpayment;
                                            Game.GiveHeroGold(reward);
                                            Game.EntityGiveGold(Me, -reward);
                                        } else if (success == ET_SUCCESS_TYPE_LOW) {
                                            TT;
                                            TE_MOVIE
                                            TE_SAY("TEXT_QST_053_SUCCESS_LOW_01")
                                            TE_SAY("TEXT_QST_053_SUCCESS_LOW_02")
                                        } else {
                                            TT;
                                            // The original checked build reports an invalid success category here.
                                        }
                                        TE_START(SCRIPT_AI_PRIORITY_HIGH)
                                        ParentClass.TraderDeparted = true;
                                    }
                                    ParentClass.MissionSucceeded = true;
                                    TE_START(SCRIPT_AI_PRIORITY_HIGHEST)
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                                    while (!door.IsOpenDoor()) {
                                        NSF;
                                        TT;
                                    }
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                                    TT;
                                    CScriptThing exit = Game.GetThingWithScriptName("M_TraderExit");
                                    // The original checked build asserts exit.IsAlive() here.
                                    Game.SetThingPersistent(Me, false);
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                                    seh_me.MoveToPosition(exit.GetPos(), 1.0f, ENTITY_MOVE_RUN, false, true);
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                                    while (!IsDistanceBetweenThingsUnder(Me, exit, 3.0f)) {
                                        NSF;
                                        TT;
                                    }
                                    TT;
                                    ParentClass.TraderAtDestination = true;
                                    FadeOutAndKillEntity(Me, 1.0f, true, true);
                                    if (closeDoor) {
                                        TT;
                                        Game.CloseDoor(door);
                                    }
                                    return;
                                }
                            }
                        } else {
                            TT;
                            TE_ADD("TEXT_QST_053_FOUND")
                            ParentClass.GoneWrongWay = false;
                            ParentClass.ResetTraderRandomSpeechTime();
                        }
                    }
                    ParentClass.RegionChangeDetected = false;
                }
                if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                    TT;
                    TE_LINE("TEXT_QST_053_REMINDER")
                    ParentClass.ResetTraderRandomSpeechTime();
                }
            } else if (ParentClass.QuestStage == QUEST_STAGE_HIRED_WAITING) {
                TT;
                if (ParentClass.ExpressionDetectedFollow) {
                    TT;
                    TE_LINE("TEXT_QST_053_FOLLOW")
                    ParentClass.ExpressionEnableFollow(Me, seh_me);
                    ParentClass.ResetTraderRandomSpeechTime();
                }
                if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                    TT;
                    TE_LINE("TEXT_QST_053_WAIT_02")
                    ParentClass.ResetTraderRandomSpeechTime();
                }
            }
        }
    TT;
    while (true) {
        NSF;
        TT;
    }
    TT;
}
