// C++03 fixture. The script below uses the selected production body and macros.
// Named engine operations are recorded or supplied deterministic results.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
typedef std::string CCharString;
enum { GROUP_SELECT_NONE, SCRIPT_AI_PRIORITY_HIGHEST, SCRIPT_AI_PRIORITY_HIGH, ENTITY_MOVE_RUN };
struct C3DVector { float x; };
struct CRGBColour { CRGBColour(unsigned char,unsigned char,unsigned char,unsigned char) {} };
struct TRetVoid { void Return() {} };
struct Run {
 int frame, stop, checks, interrupt, inventory, heroGold, traderGold, questBars, presents;
 int movies, pauses, fades, failStarts, answer, talkFrame, openCalls, closes;
 bool aliveHero, aliveMe, rear, backDoor, doorOpen;
 float traderDistance, exitDistance, health;
 std::vector<std::string> events;
 Run() : frame(0),stop(3),checks(0),interrupt(-1),inventory(0),heroGold(0),traderGold(0),
  questBars(0),presents(0),movies(0),pauses(0),fades(0),failStarts(0),answer(1),talkFrame(-1),
  openCalls(0),closes(0),aliveHero(true),aliveMe(true),rear(true),backDoor(false),doorOpen(false),
  traderDistance(2),exitDistance(50),health(10) {}
 bool Saw(const char* event) const {return std::find(events.begin(),events.end(),event)!=events.end();}
};
static Run* active;
struct CTimer {
 long deadline;
 CTimer():deadline(0) {}
 bool operator==(long seconds) const;
 long GetTime()const {return deadline>active->frame?deadline-active->frame:0;}
 CTimer& operator=(long n){ deadline=active->frame+n;return *this;}
};
#define FABLE_FORCEINLINE inline
// FableWin's checked build calls this operator at seven sites in CTraderToEscort::Main.
// Ego_d records the signed-long argument seconds. Both PDBs give the const member signature.
// Inline-budget: plain inline leaves five calls and /d2inlT100 changes the caller. Retail expands all seven.
FABLE_FORCEINLINE bool CTimer::operator==(long seconds) const {
    return GetTime() == seconds;
}
#undef FABLE_FORCEINLINE
struct CScriptThing {
 std::string name;
 CScriptThing(const std::string& n=""):name(n) {}
 C3DVector GetPos()const {C3DVector p={0};return p;}
 bool IsAlive()const {
  if(name=="hero")return active->aliveHero;
  if(name=="trader")return active->aliveMe;
  if(name=="FarmRearEntrance")return active->rear;
  return !name.empty();
 }
 bool IsOpenDoor()const {return active->doorOpen;}
 bool MsgIsTalkedToBy(const char*)const{return active->frame==active->talkFrame;}
};
struct CIsThingAlive {
 bool alive;
 CIsThingAlive(const CScriptThing&t):alive(t.IsAlive()){}
 bool operator&&(const CIsThingAlive&right)const{return alive&&right.alive;}
};
struct CScriptGameResourceObjectScriptedThingBase {
 bool valid; CScriptGameResourceObjectScriptedThingBase():valid(false){}
 bool IsValid()const{return valid;}
 void Release(){valid=false;}
 CScriptThing GetScriptThing()const{return CScriptThing("trader");}
 bool IsPerformingScriptTask()const{return false;}
 void Speak(const CScriptThing&,const char*line,int,bool,bool,bool){active->events.push_back(line);}
 void MoveToPosition(const C3DVector&,float,int,bool,bool){active->events.push_back("move");}
};
struct CScriptGameResourceObjectMovieBase {
 CScriptGameResourceObjectMovieBase(){++active->movies;}
 ~CScriptGameResourceObjectMovieBase(){--active->movies;}
};
struct CWideScreenMagicPauseEntities {
 template<class G> explicit CWideScreenMagicPauseEntities(G&){++active->pauses;}
 ~CWideScreenMagicPauseEntities(){--active->pauses;}
};
static float GetDistanceBetweenThings(const CScriptThing&a,const CScriptThing&b){
 if(a.name=="OrchardFarmEvilHSP")return active->backDoor?1.0f:4.0f;
 if(a.name=="OrchardFarmGoodHSP")return 2.0f;
 if(a.name=="hero"&&b.name=="REGION_EXIT_POINT")return active->exitDistance;
 if(b.name=="hero")return active->traderDistance;
 return 0;
}
static bool IsDistanceBetweenThingsUnder(const CScriptThing&a,const CScriptThing&b,float n){return GetDistanceBetweenThings(a,b)<n;}
static bool IsDistanceBetweenThingsOver(const CScriptThing&a,const CScriptThing&b,float n){return GetDistanceBetweenThings(a,b)>n;}
static void FadeOutAndKillEntity(const CScriptThing&,float,bool,bool){++active->fades;}
struct Engine {
 CScriptThing hero;
 Engine():hero("hero"){}
 CScriptThing& GetHero(){return hero;}
 void NewScriptFrame(){++active->frame;}
 bool StartScriptingEntity(const CScriptThing&,CScriptGameResourceObjectScriptedThingBase& handle,int){
  if(active->failStarts>0){--active->failStarts;return false;} handle.valid=true;return true;
 }
 CScriptThing GetThingWithScriptName(const CCharString& n){return CScriptThing(n);}
 CScriptThing GetNearestWithDefName(const CScriptThing&,const CCharString& n){return CScriptThing(n);}
 float GetHealth(const CScriptThing&){return active->health;}
 void GiveHeroGold(long n){active->heroGold+=n;}
 void EntityGiveGold(const CScriptThing&,long n){active->traderGold+=n;}
 void AddItemToContainer(const CScriptThing&,const CCharString&){++active->inventory;}
 void RemoveItemFromContainer(const CScriptThing&,const CCharString&){--active->inventory;}
 void GiveHeroObject(const CCharString&,long,bool){++active->presents;}
 void AddLineToConversation(long,const CCharString& line,bool,const CScriptThing&,const CScriptThing&){active->events.push_back(line);}
 long AddNewConversation(const CScriptThing&,bool,bool){return 1;}
 long MsgIsQuestionAnsweredYesOrNo(){return active->answer;}
 CCharString GetActiveQuestName(){return "escort";}
 long AddQuestInfoBarHealth(const CScriptThing&,const CRGBColour&,const CCharString&,float){return ++active->questBars;}
 void OpenDoor(const CScriptThing&){active->doorOpen=true;++active->openCalls;}
 void CloseDoor(const CScriptThing&){active->doorOpen=false;++active->closes;}
 void AddPersonToConversation(long, const CScriptThing&) { active->events.push_back("AddPersonToConversation"); }
 void DisplayQuestInfo(bool) { active->events.push_back("DisplayQuestInfo"); }
 void EntityForceToLookAtNothing(const CScriptThing&) { active->events.push_back("EntityForceToLookAtNothing"); }
 void EntityForceToLookAtThing(const CScriptThing&, const CScriptThing&) { active->events.push_back("EntityForceToLookAtThing"); }
 void EntitySetAsAllowedToFollowHero(const CScriptThing&, bool) { active->events.push_back("EntitySetAsAllowedToFollowHero"); }
 void EntitySetFacingAngleTowardsThing(const CScriptThing&, const CScriptThing&, bool) { active->events.push_back("EntitySetFacingAngleTowardsThing"); }
 void EntityStopFollowing(const CScriptThing&) { active->events.push_back("EntityStopFollowing"); }
 void EntityTeleportToThing(const CScriptThing&, const CScriptThing&, bool) { active->events.push_back("EntityTeleportToThing"); }
 void GiveHeroQuestCardDirectly(const CCharString&, const CCharString&, bool) { active->events.push_back("GiveHeroQuestCardDirectly"); }
 void GiveHeroRenownPoints(long) { active->events.push_back("GiveHeroRenownPoints"); }
 void GiveHeroYesNoQuestion(const CCharString&, const CCharString&, const CCharString&, const CCharString&, bool) { active->events.push_back("GiveHeroYesNoQuestion"); }
 void MiniMapAllowRouteBetweenRegions(const CCharString&, const CCharString&, bool) { active->events.push_back("MiniMapAllowRouteBetweenRegions"); }
 void MiniMapRemoveMarker(const CScriptThing&) { active->events.push_back("MiniMapRemoveMarker"); }
 void RemoveQuestInfoElement(long) { active->events.push_back("RemoveQuestInfoElement"); }
 void SetQuestCardObjective(const CCharString&, const CCharString&, const CCharString&, const CCharString&) { active->events.push_back("SetQuestCardObjective"); }
 void SetThingPersistent(const CScriptThing&, bool) { active->events.push_back("SetThingPersistent"); }
 void StartMovieSequence(const CCharString&, CScriptGameResourceObjectMovieBase&) { active->events.push_back("StartMovieSequence"); }
};
struct Master {long OrchardFarmTraderEscortCounter;};
struct Definition {
 std::vector<CCharString> EscortTraderRewardObjectPerfect;
 long EscortTraderRewardDownpayment,EscortTraderFailureRenown,EscortTraderHitDetectTimer,EscortTraderProximityTimer;
 float EscortTraderRegionExitWait,EscortTraderRegionExitStop,EscortTraderRegionExitDist,EscortTraderFollowProximity1,EscortTraderFollowProximity2;
 Definition():EscortTraderRewardDownpayment(10),EscortTraderFailureRenown(5),EscortTraderHitDetectTimer(4),EscortTraderProximityTimer(4),EscortTraderRegionExitWait(10),EscortTraderRegionExitStop(5),EscortTraderRegionExitDist(20),EscortTraderFollowProximity1(30),EscortTraderFollowProximity2(40){
  EscortTraderRewardObjectPerfect.push_back("a");EscortTraderRewardObjectPerfect.push_back("b");
 }
};
static Definition* pGlobalScriptDef;
namespace NScript {
struct CQR_EscortTraderScript {
 enum EscortTraderSuccessType {ET_SUCCESS_TYPE_LOW,ET_SUCCESS_TYPE_MEDIUM,ET_SUCCESS_TYPE_HIGH,ET_SUCCESS_TYPE_PERFECT};
 enum Stage {QUEST_STAGE_NONE,QUEST_STAGE_SETUP,QUEST_STAGE_HIRED_FOLLOWING,QUEST_STAGE_HIRED_WAITING};
 struct CTraderToEscort;
 Stage QuestStage; EscortTraderSuccessType success;
 CTimer TraderExpressionDetectTimer,TraderHitDetectTimer,TraderRandomSpeechTimer;
 long TraderHealthBarID;float TraderInitialHealth;
 CCharString BonusObject;
 bool AddedBonusObject, AddedQuestInfo, DoneExpressions, ExpressionDetectedFollow, ExpressionDetectedWait, GoneWrongWay, GreetedHero, HiredByTrader, MissionSucceeded, RegionChangeDetected, TheBlackGateIsClosed, TotallyLost, TraderAtDestination, TraderDeparted, TraderDepartedLost, TraderFollowing, TraderHit, TraderIntroDone, wrongPath, finalRegion, speech;
 CQR_EscortTraderScript():QuestStage(QUEST_STAGE_HIRED_WAITING),success(ET_SUCCESS_TYPE_LOW),TraderHealthBarID(0),TraderInitialHealth(0),AddedBonusObject(false), AddedQuestInfo(false), DoneExpressions(false), ExpressionDetectedFollow(false), ExpressionDetectedWait(false), GoneWrongWay(false), GreetedHero(false), HiredByTrader(false), MissionSucceeded(false), RegionChangeDetected(false), TheBlackGateIsClosed(false), TotallyLost(false), TraderAtDestination(false), TraderDeparted(false), TraderDepartedLost(false), TraderFollowing(false), TraderHit(false), TraderIntroDone(false), wrongPath(false), finalRegion(false), speech(false){}

 void ExpressionEnableFollow(const CScriptThing&,CScriptGameResourceObjectScriptedThingBase&){TraderFollowing=true;}
 void ExpressionEnableWait(const CScriptThing&,CScriptGameResourceObjectScriptedThingBase&){TraderFollowing=false;}
 void ForceEnableWait(const CScriptThing&){TraderFollowing=false;}
 void ForceDisableFollow(const CScriptThing&){ExpressionDetectedFollow=false;}
 void ResetTraderRandomSpeechTime(){TraderRandomSpeechTimer=10;}
 bool GetTraderRandomSpeechEnabled()const{return speech;}
 bool TravellingWrongPath()const{return wrongPath;}
 bool InFinalRegion()const{return finalRegion;}
 EscortTraderSuccessType GetSuccessType(const CScriptThing&)const{return success;}
};
struct CQR_EscortTraderScript::CTraderToEscort {
 Engine& Game; CQR_EscortTraderScript& ParentClass; Master master; CScriptThing Me; bool predicate;
 CTraderToEscort(Engine& game,CQR_EscortTraderScript& p):Game(game),ParentClass(p),Me("trader"),predicate(false){master.OrchardFarmTraderEscortCounter=0;}
 Master* GetMasterData(){return &master;}
 void SetPredicate(bool p){predicate=p;}
 bool IsActiveThreadTerminating(){++active->checks;return !predicate||active->checks==active->interrupt||active->frame>=active->stop;}
 CCharString GetRandomSpeech(){return "random";}
 void Main();
}; }

#define T IsActiveThreadTerminating()
#define TT if (T) return _Fail_Val.Return()
#define NSF Game.NewScriptFrame()
#define TE_RELEASE if (seh_me.IsValid()) seh_me.Release();
#define TE_START(PRIORITY) \
    TE_RELEASE \
    while (!Game.StartScriptingEntity(Me,seh_me,PRIORITY)) { \
        NSF; \
        TT; \
    } \
    TT;
#define TE_SAY(TEXT) \
    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) { \
        seh_me.Speak(Game.GetHero(),TEXT,GROUP_SELECT_NONE,false,true,false); \
        while (seh_me.IsPerformingScriptTask()) { \
            NSF; \
            TT; \
        } \
        TT; \
    }
#define TE_CONV \
    long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false); \
    Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
#define TE_ADD(TEXT) Game.AddLineToConversation(HLStyleConversationIndex,TEXT,false,Me,Game.GetHero());
#define TE_LINE(TEXT) { TE_CONV TE_ADD(TEXT) }
#define TE_MOVIE \
    CScriptGameResourceObjectMovieBase speech_movie; \
    Game.StartMovieSequence("",speech_movie); \
    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);

void NScript::CQR_EscortTraderScript::CTraderToEscort::Main()
{
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


#undef T
#undef TT
#undef NSF
typedef NScript::CQR_EscortTraderScript Quest;
struct Fixture {
 Run events; Engine game; Definition def; Quest quest; Quest::CTraderToEscort trader;
 Fixture():trader(game,quest){active=&events;pGlobalScriptDef=&def;}
};
static int failures=0;
#define EXPECT(x) do {if(!(x)){++failures;std::printf("direct escort check %d failed\n",__LINE__);}}while(0)
static void Arrival(int success, int interruption, int &checks) {
 Fixture f; f.quest.QuestStage=Quest::QUEST_STAGE_HIRED_FOLLOWING;f.quest.TraderFollowing=true;
 f.quest.RegionChangeDetected=true;f.quest.finalRegion=true;f.quest.success=(Quest::EscortTraderSuccessType)success;
 f.events.stop=40;f.events.interrupt=interruption; f.trader.Main();checks=f.events.checks;
 EXPECT(f.events.pauses==0&&f.events.movies==0);
 if(interruption<0){EXPECT(f.quest.TraderAtDestination&&f.quest.MissionSucceeded&&f.quest.TraderDeparted);
 EXPECT(f.events.heroGold==success*10&&f.events.traderGold==40-success*10);
 EXPECT(f.events.presents==(success==3)&&f.events.openCalls==1&&f.events.closes==1&&f.events.fades==1);}
}
int main(){
 {Fixture f; CTimer timer;
  EXPECT(timer == 0 && !(timer == 1) && !(timer == -1));
  timer = 5; EXPECT(timer == 5 && !(timer == 0));
  f.events.frame = 4; EXPECT(timer == 1 && !(timer == 5));
  f.events.frame = 5; EXPECT(timer == 0);
  f.events.frame = 9; EXPECT(timer == 0 && !(timer == -1));}

 for(int h=0;h<2;++h)for(int m=0;m<2;++m){
  Fixture f; f.events.aliveHero=h!=0;f.events.aliveMe=m!=0;f.trader.Main();
  EXPECT(f.trader.predicate==(h&&m));EXPECT(f.events.inventory==(h&&m));
 }
 for(int index=0;index<4;++index)for(int added=0;added<2;++added){
  Fixture f;f.quest.AddedBonusObject=added!=0;f.trader.master.OrchardFarmTraderEscortCounter=index;f.quest.HiredByTrader=true;
  f.trader.Main();EXPECT(f.events.inventory==1-added&&f.events.traderGold==30);
  if(!added)EXPECT(index==0?f.quest.BonusObject=="a":index==1?f.quest.BonusObject=="b":!f.quest.BonusObject.empty());
 }
 {Fixture f;f.quest.speech=true;f.quest.TraderHit=true;f.trader.Main();
  EXPECT(f.events.Saw("random")&&f.events.Saw("TEXT_QST_053_HIT")&&!f.quest.TraderHit);}
 for(int success=0;success<4;++success){int checks=0;Arrival(success,-1,checks);
  for(int i=1;i<=checks;++i){int observed=0;Arrival(success,i,observed);EXPECT(observed==i);}}
 {Fixture f;f.quest.QuestStage=Quest::QUEST_STAGE_HIRED_FOLLOWING;f.quest.TraderFollowing=true;f.quest.RegionChangeDetected=true;
  f.quest.finalRegion=true;f.events.backDoor=true;f.events.stop=12;f.trader.Main();EXPECT(f.quest.TheBlackGateIsClosed&&!f.quest.MissionSucceeded);}
 {Fixture f;f.quest.QuestStage=Quest::QUEST_STAGE_HIRED_FOLLOWING;f.quest.TraderFollowing=true;f.quest.RegionChangeDetected=true;
  f.quest.wrongPath=true;f.trader.Main();EXPECT(f.quest.GoneWrongWay&&f.events.Saw("TEXT_QST_053_LOST"));}
 {Fixture f;f.quest.TotallyLost=true;f.events.stop=7;f.trader.Main();EXPECT(f.events.fades>0&&f.quest.TraderDepartedLost);}
 {Fixture f;f.quest.QuestStage=Quest::QUEST_STAGE_NONE;f.events.stop=7;f.events.talkFrame=3;f.trader.Main();
  EXPECT(f.quest.GreetedHero&&f.quest.QuestStage==Quest::QUEST_STAGE_SETUP&&f.events.Saw("TEXT_QST_053_GREETING"));}
 {Fixture f;f.quest.QuestStage=Quest::QUEST_STAGE_SETUP;f.events.stop=10;f.events.talkFrame=2;f.trader.Main();
  EXPECT(f.quest.TraderIntroDone&&f.quest.DoneExpressions&&f.quest.HiredByTrader&&f.events.heroGold==10);}
 {Fixture f;f.quest.QuestStage=Quest::QUEST_STAGE_HIRED_FOLLOWING;f.quest.TraderFollowing=true;
  f.events.traderDistance=45;f.events.exitDistance=1;f.trader.Main();
  EXPECT(f.events.Saw("TEXT_QST_053_EXIT_PROXIMITY_WARNING")&&f.events.Saw("TEXT_QST_053_EXIT_PROXIMITY_DETACH"));}
 std::puts(failures?"00e20600_TEST FAIL":"00e20600_TEST PASS");return failures?1:0;
}
