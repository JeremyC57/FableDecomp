// NScript::CQR_EscortTraderScript::CTraderToEscort::Main() -- the escorted trader: hire (bonus object, downpayment gold, quest card), wait to be talked to, the intro movie and the yes / no follow question, then follow the hero through the regions (random follow / wait speech, hit reactions, the TotallyLost give-up walk and fade-out, the black-gate warning) until the final region; at the farmhouse open the door, grade the escort (perfect / high / medium / low reward lines and gold), depart through M_TraderExit and fade out
// Grade: DIFFER(14478v14510) -- 355 masked lines (504 before): the predicate lever of this chunk is applied (inline_depth(0) round the row-local predicate ctor / dtor + FABLE_RETAIL_CPREDICATEBASE_NOVTABLE); left: the CPredicate_And members (retail calls ~CIsThingAlive 0x00E241B0 there and stores the CPredicateBase vtable 0x012C2FF4 at 0x00E20696, which NOVTABLE drops), the first return pad's home (0x00E208A4) and the GetPos split
// Next idea: this row needs the base vtable store: grade it without FABLE_RETAIL_CPREDICATEBASE_NOVTABLE once the ctor region has another way to keep CPredicateBase() expanded, or keep the CIsThingAlive members of the And predicate out of the region H40f (3 grades): baseline 355 lines / 14478; main loop `for (;!T && true;NSF)` 329 lines / 14430; `for (;!T || false;NSF)` 309 lines / 14622 v 14510 -- retail's size sits between the plain and the 7.5x body, so the split is per site, not one loop factor. H40j (2 grades + 2 inline probes): without NOVTABLE 370 lines; no ctor pragma + ~CPredicate_And under inline_depth(1) 360 (the And members still expand: a template member definition ignores the pragma, inline trace c22/c19 both under T74, and ~CPredicateBase is then called). Retail calls ~CIsThingAlive for the And members but expands it for the temporaries: look for what makes the member sites cost more (an implicit ~CPredicate_And?).
// Leaf: ?Main@CTraderToEscort@CQR_EscortTraderScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E20600, 14510 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven, two sources): slot 1 of the CTraderToEscort entity vtable 0x012E151C that Alloc 0x00E1E7A0 stores (slot 0 scalar
//   deleting dtor 0x00E1E860, 2 Init 0x00E1F7E0, 3 GetParentScript 0x00E1E840, 4 CEntityScriptBase::OnPersist 0x00CDEBC0,
//   5 OnPredicateFail 0x00E1E850); Ego_r.pdb and FableWin.pdb both hold ?Main@CTraderToEscort@CQR_EscortTraderScript@NScript@@EAEXXZ.
// Callees: GetRandomSpeech 0x00E23F30, ResetTraderRandomSpeechTime 0x00E1FEB0 (landed as RestartRandomSpeechTimer: NAME_FIXES.tsv),
//   GetDistanceBetweenThings 0x00CBE512, IsDistanceBetweenThingsUnder, FadeOutAndKillEntity.
// Layout: engine/NScript_CQR_EscortTraderScript.h with the chunk 818 TRADERTOESCORT opt-in (hdr/README.md).
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
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
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CTimer.h"
#include "engine/CScriptDef.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_fade.h"
#include "stlport/fable_map.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1 };
extern "C" int __cdecl rand();   // 0x00BFEB16 (MSVCR71)
float FABLE_FASTCALL GetDistanceBetweenThings(const CScriptThing&, const CScriptThing&);   // 0x00CBE512
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8
extern CScriptDef* pGlobalScriptDef;         // 0x0143E90C
#pragma inline_depth(0)
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
#pragma inline_depth()
#pragma inline_depth(0)
inline NScript::CIsThingAlive::~CIsThingAlive() {}
#pragma inline_depth()
template<class P1,class P2> inline NScript::CPredicate_And<P1,P2>::~CPredicate_And() {}
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline CScriptThing::CScriptThing() { }
inline CScriptThing::~CScriptThing() { }
inline bool CScriptThing::IsOpenDoor() const { if (!PImp.Data) return false; return PImp.Data->IsOpenDoor(); }
#define T IsActiveThreadTerminating()
#define TT if (T) return
#define NSF Game->NewScriptFrame()
#define TE_RELEASE if (seh_me.IsValid()) seh_me.Release();
#define TE_START(PRIORITY) \
    TE_RELEASE \
    while (!Game->StartScriptingEntity(Me,seh_me,PRIORITY)) { \
        NSF; \
        TT; \
    } \
    TT;
#define TE_SAY(TEXT) \
    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) { \
        seh_me.Speak(Game->GetHero(),TEXT,GROUP_SELECT_NONE,false,true,false); \
        while (seh_me.IsPerformingScriptTask()) { \
            NSF; \
            TT; \
        } \
        TT; \
    }
#define TE_CONV \
    long conversation=Game->AddNewConversation(Me,false,false); \
    Game->AddPersonToConversation(conversation,Game->GetHero());
#define TE_ADD(TEXT) Game->AddLineToConversation(conversation,TEXT,false,Me,Game->GetHero());
#define TE_LINE(TEXT) { TE_CONV TE_ADD(TEXT) }
#define TE_MOVIE \
    CScriptGameResourceObjectMovieBase movie; \
    Game->StartMovieSequence("",movie); \
    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);

void NScript::CQR_EscortTraderScript::CTraderToEscort::Main()
{
    SetPredicate(CIsThingAlive(Me) && CIsThingAlive(Game->GetHero()));
    NSF;
    TT;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    CTimer timer;
    CScriptThing exit_point;
    timer=0;
    if (!ParentClass.AddedBonusObject) {
        TT;
        ParentClass.AddedBonusObject=true;
        if ((unsigned long)PMasterData->OrchardFarmTraderEscortCounter>=pGlobalScriptDef->EscortTraderRewardObjectPerfect.size()) {
            TT;
            ParentClass.BonusObject=pGlobalScriptDef->EscortTraderRewardObjectPerfect[rand()%pGlobalScriptDef->EscortTraderRewardObjectPerfect.size()];
        }
        else {
            TT;
            ParentClass.BonusObject=pGlobalScriptDef->EscortTraderRewardObjectPerfect[PMasterData->OrchardFarmTraderEscortCounter];
        }
        Game->AddItemToContainer(Me,ParentClass.BonusObject);
    }
    Game->EntityGiveGold(Me,pGlobalScriptDef->EscortTraderRewardDownpayment*4);
    if (ParentClass.HiredByTrader) {
        TT;
        Game->EntityGiveGold(Me,-pGlobalScriptDef->EscortTraderRewardDownpayment);
        Game->EntitySetAsAllowedToFollowHero(Me,true);
    }
    if (ParentClass.QuestStage==QUEST_STAGE_HIRED_FOLLOWING) {
        TT;
        ParentClass.ExpressionEnableFollow(Me,seh_me);
    }
    for (;!T;NSF) {
        if (ParentClass.TotallyLost) {
            TT;
            ParentClass.ForceDisableFollow(Me);
            TE_MOVIE
            Game->EntitySetFacingAngleTowardsThing(Game->GetHero(),Me,false);
            NSF;
            TT;
            NSF;
            TT;
            Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),false);
            NSF;
            TT;
            NSF;
            TT;
            TE_START(SCRIPT_AI_PRIORITY_HIGHEST)
            TE_SAY("TEXT_QST_053_TOTALLY_LOST")
            Game->GiveHeroRenownPoints(pGlobalScriptDef->EscortTraderFailureRenown);
            CScriptThing exit_marker=Game->GetNearestWithDefName(Me,"REGION_EXIT_POINT");
            if (exit_marker.IsAlive()) {
                TT;
                if (seh_me.IsValid()) {
                    TT;
                    seh_me.MoveToPosition(exit_marker.GetPos(),1.0f,ENTITY_MOVE_RUN,false,true);
                }
                while (!IsDistanceBetweenThingsUnder(Me,exit_marker,2.0f)) {
                    NSF;
                    TT;
                }
                TT;
                FadeOutAndKillEntity(Me,1.0f,true,true);
                ParentClass.TraderDepartedLost=true;
            }
        }
        if (ParentClass.TraderRandomSpeechTimer.GetTime()==0 && ParentClass.GetTraderRandomSpeechEnabled()) {
            TT;
            CCharString speech=GetRandomSpeech();
            TE_LINE(speech)
            ParentClass.ResetTraderRandomSpeechTime();
        }
        if (ParentClass.TraderHit && ParentClass.QuestStage!=QUEST_STAGE_NONE && ParentClass.TraderHitDetectTimer.GetTime()==0) {
            TT;
            TE_LINE("TEXT_QST_053_HIT")
            ParentClass.TraderHit=false;
            ParentClass.TraderHitDetectTimer=pGlobalScriptDef->EscortTraderHitDetectTimer;
            ParentClass.ResetTraderRandomSpeechTime();
        }
        if (ParentClass.QuestStage==QUEST_STAGE_NONE) {
            TT;
            bool talked=false;
            do {
                NSF;
                TT;
                if (IsDistanceBetweenThingsUnder(Me,Game->GetHero(),3.0f)) {
                    TT;
                    if (!ParentClass.GreetedHero) {
                        TE_LINE("TEXT_QST_053_GREETING")
                        ParentClass.GreetedHero=true;
                    }
                    Game->EntityForceToLookAtThing(Me,Game->GetHero());
                    while (!talked) {
                        NSF;
                        TT;
                        if (ParentClass.ExpressionDetectedFollow) {
                            TT;
                            if (ParentClass.TraderExpressionDetectTimer.GetTime()==0) {
                                TT;
                                TE_LINE("TEXT_QST_053_HASTY1")
                            }
                            ParentClass.ForceDisableFollow(Me);
                        }
                        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                            TT;
                            talked=true;
                        }
                        else if (IsDistanceBetweenThingsOver(Me,Game->GetHero(),5.0f)) {
                            TT;
                            TE_RELEASE
                            break;
                        }
                    }
                    TT;
                    Game->EntityForceToLookAtNothing(Me);
                }
            } while (!talked);
            TT;
            ParentClass.QuestStage=QUEST_STAGE_SETUP;
        }
        else if (ParentClass.QuestStage==QUEST_STAGE_SETUP) {
            TT;
            if (!ParentClass.TraderIntroDone) {
                TT;
                TE_START(SCRIPT_AI_PRIORITY_HIGHEST)
                {
                    TE_MOVIE
                    Game->EntitySetFacingAngleTowardsThing(Game->GetHero(),Me,false);
                    TE_SAY("TEXT_QST_053_INTRO_01")
                    TE_SAY("TEXT_QST_053_INTRO_02")
                    TE_SAY("TEXT_QST_053_INTRO_03")
                    TE_SAY("TEXT_QST_053_INTRO_04")
                    TE_START(SCRIPT_AI_PRIORITY_HIGH)
                    ParentClass.TraderIntroDone=true;
                }
            }
            TE_START(SCRIPT_AI_PRIORITY_HIGH)
            {
                CTimer wait_timer;
                wait_timer=5;
                for (;;) {
                    NSF;
                    TT;
                    if (ParentClass.ExpressionDetectedFollow) {
                        TT;
                        if (ParentClass.TraderExpressionDetectTimer.GetTime()==0) {
                            TT;
                            TE_LINE("TEXT_QST_053_HASTY2")
                        }
                        ParentClass.ForceDisableFollow(Me);
                    }
                    if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                        TT;
                        Game->GiveHeroYesNoQuestion("TEXT_QST_053_QUEST_ON_QUESTION","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                        long answer=Game->MsgIsQuestionAnsweredYesOrNo();
                        while (answer<0) {
                            NSF;
                            TT;
                            answer=Game->MsgIsQuestionAnsweredYesOrNo();
                        }
                        TT;
                        if (answer==1) {
                            TT;
                            break;
                        }
                    }
                    if (wait_timer.GetTime()==0) {
                        TT;
                        TE_RELEASE
                    }
                }
                TT;
            }
            if (!ParentClass.DoneExpressions) {
                TT;
                TE_START(SCRIPT_AI_PRIORITY_HIGHEST)
                {
                    TE_MOVIE
                    TE_SAY("TEXT_QST_053_INTRO_05")
                    TE_SAY("TEXT_QST_053_PLAN_01")
                    TE_SAY("TEXT_QST_053_PLAN_02")
                    TE_SAY("TEXT_QST_053_PLAN_03")
                    TE_SAY("TEXT_QST_053_PLAN_04")
                    TE_SAY("TEXT_QST_053_PLAN_05")
                    ParentClass.ResetTraderRandomSpeechTime();
                    Game->GiveHeroGold(pGlobalScriptDef->EscortTraderRewardDownpayment);
                    Game->EntityGiveGold(Me,-pGlobalScriptDef->EscortTraderRewardDownpayment);
                    Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_ORCHARD_FARM_ESCORT",Game->GetActiveQuestName(),true);
                    Game->SetQuestCardObjective(Game->GetActiveQuestName(),"TEXT_QUEST_ORCHARD_FARM_ESCORT_OBJECTIVE_02","OrchardFarm","Greatwood");
                    Game->EntitySetAsAllowedToFollowHero(Me,true);
                    Game->EntityStopFollowing(Me);
                    ParentClass.HiredByTrader=true;
                    ParentClass.RegionChangeDetected=false;
                    ParentClass.DoneExpressions=true;
                    TE_RELEASE
                }
            }
            while (!ParentClass.TraderFollowing) {
                NSF;
                TT;
                if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                    TT;
                    TE_START(SCRIPT_AI_PRIORITY_HIGHEST)
                    {
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
                        ParentClass.TraderHealthBarID=Game->AddQuestInfoBarHealth(Me,CRGBColour(0,255,0,255),"HUD_QUEST_ICON_TRADER",1.0f);
                        ParentClass.TraderInitialHealth=Game->GetHealth(Me);
                        Game->DisplayQuestInfo(true);
                        ParentClass.AddedQuestInfo=true;
                    }
                    ParentClass.ExpressionEnableFollow(Me,seh_me);
                    ParentClass.ResetTraderRandomSpeechTime();
                }
            }
            TT;
        }
        else if (ParentClass.QuestStage==QUEST_STAGE_HIRED_FOLLOWING) {
            TT;
            if (ParentClass.TraderFollowing) {
                TT;
                exit_point=Game->GetNearestWithDefName(Game->GetHero(),"REGION_EXIT_POINT");
                float trader_distance=GetDistanceBetweenThings(Me,Game->GetHero());
                float exit_distance=GetDistanceBetweenThings(Game->GetHero(),exit_point);
                TE_CONV
                if (exit_distance<pGlobalScriptDef->EscortTraderRegionExitWait && trader_distance>pGlobalScriptDef->EscortTraderRegionExitDist && timer.GetTime()==0) {
                    TT;
                    TE_ADD("TEXT_QST_053_EXIT_PROXIMITY_WARNING")
                    timer=pGlobalScriptDef->EscortTraderProximityTimer;
                    ParentClass.ResetTraderRandomSpeechTime();
                }
                else if (exit_distance<pGlobalScriptDef->EscortTraderRegionExitStop && trader_distance>pGlobalScriptDef->EscortTraderRegionExitDist) {
                    TT;
                    TE_ADD("TEXT_QST_053_EXIT_PROXIMITY_DETACH")
                    ParentClass.ForceEnableWait(Me);
                    ParentClass.ResetTraderRandomSpeechTime();
                }
                else if (trader_distance>pGlobalScriptDef->EscortTraderFollowProximity1 && timer.GetTime()==0) {
                    TT;
                    TE_ADD("TEXT_QST_053_PROXIMITY_WARNING")
                    timer=pGlobalScriptDef->EscortTraderProximityTimer;
                    ParentClass.ResetTraderRandomSpeechTime();
                }
                else if (trader_distance>pGlobalScriptDef->EscortTraderFollowProximity2) {
                    TT;
                    TE_ADD("TEXT_QST_053_PROXIMITY_DETACH")
                    ParentClass.ForceEnableWait(Me);
                    ParentClass.ResetTraderRandomSpeechTime();
                }
                else {
                    TT;
                    if (ParentClass.ExpressionDetectedWait) {
                        TT;
                        TE_ADD("TEXT_QST_053_WAIT_01")
                        ParentClass.ExpressionEnableWait(Me,seh_me);
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
                        ParentClass.TheBlackGateIsClosed=false;
                        ParentClass.GoneWrongWay=false;
                        ParentClass.ResetTraderRandomSpeechTime();
                    }
                    else if (ParentClass.TravellingWrongPath()) {
                        TT;
                        if (ParentClass.GoneWrongWay) {
                            TT;
                            ParentClass.TotallyLost=true;
                        }
                        else {
                            TT;
                            TE_ADD("TEXT_QST_053_LOST")
                            TE_ADD("TEXT_QST_053_LOST_DETACH")
                            ParentClass.ForceEnableWait(Me);
                            ParentClass.GoneWrongWay=true;
                            ParentClass.ResetTraderRandomSpeechTime();
                        }
                    }
                    else if (ParentClass.InFinalRegion()) {
                        TT;
                        NSF;
                        TT;
                        ParentClass.GoneWrongWay=false;
                        CScriptThing rear_entrance=Game->GetThingWithScriptName("FarmRearEntrance");
                        CScriptThing good_hsp=Game->GetThingWithScriptName("OrchardFarmGoodHSP");
                        CScriptThing evil_hsp=Game->GetThingWithScriptName("OrchardFarmEvilHSP");
                        bool black_gate=GetDistanceBetweenThings(evil_hsp,Game->GetHero())<GetDistanceBetweenThings(good_hsp,Game->GetHero());
                        CScriptThing hero_marker;
                        CScriptThing trader_marker;
                        if (black_gate) {
                            TT;
                            hero_marker=Game->GetThingWithScriptName("M_ET_BlackGateHero");
                            trader_marker=Game->GetThingWithScriptName("M_ET_BlackGateTrader");
                        }
                        else {
                            TT;
                            hero_marker=Game->GetThingWithScriptName("M_ET_FrontDoorHero");
                            trader_marker=Game->GetThingWithScriptName("M_ET_FrontDoorTrader");
                        }
                        Game->EntityTeleportToThing(Me,trader_marker,false);
                        Game->EntityTeleportToThing(Game->GetHero(),hero_marker,false);
                        NSF;
                        TT;
                        NSF;
                        TT;
                        Game->EntitySetFacingAngleTowardsThing(Game->GetHero(),Me,false);
                        NSF;
                        TT;
                        NSF;
                        TT;
                        Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),false);
                        NSF;
                        TT;
                        NSF;
                        TT;
                        if (black_gate && rear_entrance.IsAlive()) {
                            TT;
                            TE_START(SCRIPT_AI_PRIORITY_HIGHEST)
                            {
                                TE_MOVIE
                                TE_SAY("TEXT_QST_053_THE_BLACK_GATE_IS_CLOSED")
                                TE_RELEASE
                                ParentClass.TheBlackGateIsClosed=true;
                                Game->MiniMapAllowRouteBetweenRegions("GreatwoodEntrance","OrchardFarm",false);
                            }
                        }
                        else {
                            TT;
                            if (!ParentClass.TraderAtDestination) {
                                TT;
                                CScriptThing door=Game->GetThingWithScriptName("OFFarmhouseDoor");
                                bool opened_door=false;
                                if (door.IsAlive() && !door.IsOpenDoor()) {
                                    TT;
                                    Game->OpenDoor(door);
                                    opened_door=true;
                                }
                                Game->MiniMapRemoveMarker(Me);
                                ParentClass.TraderFollowing=false;
                                Game->RemoveQuestInfoElement(ParentClass.TraderHealthBarID);
                                ParentClass.ForceDisableFollow(Me);
                                if (!ParentClass.TraderDeparted) {
                                    TT;
                                    EscortTraderSuccessType success=ParentClass.GetSuccessType(Me);
                                    Game->EntitySetFacingAngleTowardsThing(Game->GetHero(),Me,false);
                                    TE_START(SCRIPT_AI_PRIORITY_HIGHEST)
                                    if (success==ET_SUCCESS_TYPE_PERFECT) {
                                        TT;
                                        TE_MOVIE
                                        TE_SAY("TEXT_QST_053_SUCCESS_PERFECT_01")
                                        TE_SAY("TEXT_QST_053_SUCCESS_PERFECT_02")
                                        long reward=pGlobalScriptDef->EscortTraderRewardDownpayment*3;
                                        Game->GiveHeroGold(reward);
                                        Game->EntityGiveGold(Me,-reward);
                                        Game->GiveHeroObject(ParentClass.BonusObject,-1,false);
                                        Game->RemoveItemFromContainer(Me,ParentClass.BonusObject);
                                    }
                                    else if (success==ET_SUCCESS_TYPE_HIGH) {
                                        TT;
                                        TE_MOVIE
                                        TE_SAY("TEXT_QST_053_SUCCESS_HIGH_01")
                                        TE_SAY("TEXT_QST_053_SUCCESS_HIGH_02")
                                        long reward=pGlobalScriptDef->EscortTraderRewardDownpayment*2;
                                        Game->GiveHeroGold(reward);
                                        Game->EntityGiveGold(Me,-reward);
                                    }
                                    else if (success==ET_SUCCESS_TYPE_MEDIUM) {
                                        TT;
                                        TE_MOVIE
                                        TE_SAY("TEXT_QST_053_SUCCESS_MEDIUM_01")
                                        TE_SAY("TEXT_QST_053_SUCCESS_MEDIUM_02")
                                        long reward=pGlobalScriptDef->EscortTraderRewardDownpayment;
                                        Game->GiveHeroGold(reward);
                                        Game->EntityGiveGold(Me,-reward);
                                    }
                                    else if (success==ET_SUCCESS_TYPE_LOW) {
                                        TT;
                                        TE_MOVIE
                                        TE_SAY("TEXT_QST_053_SUCCESS_LOW_01")
                                        TE_SAY("TEXT_QST_053_SUCCESS_LOW_02")
                                    }
                                    else {
                                        TT;
                                    }
                                    TE_START(SCRIPT_AI_PRIORITY_HIGH)
                                    ParentClass.TraderDeparted=true;
                                }
                                ParentClass.MissionSucceeded=true;
                                TE_START(SCRIPT_AI_PRIORITY_HIGHEST)
                                while (!door.IsOpenDoor()) {
                                    NSF;
                                    TT;
                                }
                                TT;
                                CScriptThing exit_marker=Game->GetThingWithScriptName("M_TraderExit");
                                Game->SetThingPersistent(Me,false);
                                seh_me.MoveToPosition(exit_marker.GetPos(),1.0f,ENTITY_MOVE_RUN,false,true);
                                while (!IsDistanceBetweenThingsUnder(Me,exit_marker,3.0f)) {
                                    NSF;
                                    TT;
                                }
                                TT;
                                ParentClass.TraderAtDestination=true;
                                FadeOutAndKillEntity(Me,1.0f,true,true);
                                if (opened_door) {
                                    TT;
                                    Game->CloseDoor(door);
                                }
                                return;
                            }
                        }
                    }
                    else {
                        TT;
                        TE_ADD("TEXT_QST_053_FOUND")
                        ParentClass.GoneWrongWay=false;
                        ParentClass.ResetTraderRandomSpeechTime();
                    }
                }
                ParentClass.RegionChangeDetected=false;
            }
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                TT;
                TE_LINE("TEXT_QST_053_REMINDER")
                ParentClass.ResetTraderRandomSpeechTime();
            }
        }
        else if (ParentClass.QuestStage==QUEST_STAGE_HIRED_WAITING) {
            TT;
            if (ParentClass.ExpressionDetectedFollow) {
                TT;
                TE_LINE("TEXT_QST_053_FOLLOW")
                ParentClass.ExpressionEnableFollow(Me,seh_me);
                ParentClass.ResetTraderRandomSpeechTime();
            }
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                TT;
                TE_LINE("TEXT_QST_053_WAIT_02")
                ParentClass.ResetTraderRandomSpeechTime();
            }
        }
    }
}
