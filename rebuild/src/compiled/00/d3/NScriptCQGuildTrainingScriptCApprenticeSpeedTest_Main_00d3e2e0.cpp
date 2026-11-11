// NScript::CQ_GuildTrainingScript::CApprenticeSpeedTest::Main() -- the guild-training speed-test apprentice: argues with his friend while the hero is near, and runs the race state machine (0 boast and challenge, 1 race running against the race timer, 2 failed / offer a rematch, 3 won: post-race banter and the success question).
// Grade: RELOCATION_MATCH -- actual COFF operands checked against their native targets.
// Inline-budget: predicate nested-copy depth0; original implicit movie and handle destructors choose native hot/cold cleanup under the donor constant frame loop, no extra flags.
// Leaf: ?Main@CApprenticeSpeedTest@CQ_GuildTrainingScript@NScript@@EAEXXZ
// Retail: 0x00D3E2E0, 9440 bytes, thiscall.
// Identity (proven): slot 1 (Main) of the CApprenticeSpeedTest entity vtable 0x012CDA40 that Alloc 0x00D3E1F0 stores (slot 0 the
//   landed scalar deleting dtor 0x00D3E2C0, slot 2 Init 0x00D3E2A0, 3 GetParentScript 0x00D3E290); Ego_r.pdb / FableWin.pdb
//   ?Main@CApprenticeSpeedTest@CQ_GuildTrainingScript@NScript@@EAEXXZ.
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CISTHINGALIVE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CPREDICATEBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CTIMER_ORIGINAL_ACCESS
#define FABLE_RETAIL_CISTHINGCONSCIOUS_ORIGINAL_ACCESS
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_MAIN
#define FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_SPEEDTEST
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CQ_GuildTrainingScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CTimer.h"
#include "engine/TRetVoid.h"
#include "engine/CIsThingConscious.h"
#include "engine/fable_script_def_global.h"
#include "engine/fable_script_distance.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8
inline CScriptThing::~CScriptThing() { }
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& thing):Thing(thing) {}
#ifdef _MSC_VER
#pragma inline_depth()
#endif
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
inline NScript::CIsThingConscious::~CIsThingConscious() {}
#ifdef _MSC_VER
#pragma inline_depth()
#endif
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) {}
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline void CScriptThing::SetFriendsWithEverythingFlag(bool on) { if (PImp.Data) PImp.Data->SetFriendsWithEverythingFlag(on); }
#define T IsActiveThreadTerminating()
#define NSF Game.NewScriptFrame()
#define ST_START \
    if (seh_me.IsValid()) seh_me.Release(); \
    while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) { \
        NSF; \
        if (T) return _Fail_Val.Return(); \
    } \
    if (T) return _Fail_Val.Return();
#define ST_SAY(TEXT) \
    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) { \
        seh_me.Speak(Game.GetHero(),TEXT,GROUP_SELECT_NONE,false,true,false); \
        while (seh_me.IsPerformingScriptTask()) { \
            NSF; \
            if (T) return _Fail_Val.Return(); \
        } \
        if (T) return _Fail_Val.Return(); \
    }
#define ST_MOVIE \
    CScriptGameResourceObjectMovieBase speech_movie; \
    Game.StartMovieSequence("",speech_movie); \
    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
#define ST_ASK(TEXT) \
    Game.GiveHeroYesNoQuestion(TEXT,"TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true); \
    long answer=Game.MsgIsQuestionAnsweredYesOrNo(); \
    while (answer<0) { \
        NSF; \
        if (T) return _Fail_Val.Return(); \
        answer=Game.MsgIsQuestionAnsweredYesOrNo(); \
    } \
    if (T) return _Fail_Val.Return();
#define ST_ME_LINE(TEXT) \
    if (T) return _Fail_Val.Return(); \
    Game.AddLineToConversation(Conversation,TEXT,false,Me,SpeedFriend);
#define ST_FRIEND_LINE(TEXT) \
    if (T) return _Fail_Val.Return(); \
    Game.AddLineToConversation(Conversation,TEXT,false,SpeedFriend,Me);
#define ST_PRE_ARGUE \
    if (IsDistanceBetweenThingsUnder(Game.GetHero(),Me,10.0f) && SpeedTimer.GetTime()<=0) { \
        if (T) return _Fail_Val.Return(); \
        long Conversation=Game.AddNewConversation(Me,false,false); \
        Game.AddPersonToConversation(Conversation,SpeedFriend); \
        if (LineOfSpeech<=5) { \
            if (T) return _Fail_Val.Return(); \
            switch (LineOfSpeech) { \
            case 1: Game.AddLineToConversation(Conversation,"TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIRST_LINE",false,Me,SpeedFriend); break; \
            case 2: Game.AddLineToConversation(Conversation,"TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SECOND_LINE",false,SpeedFriend,Me); break; \
            case 3: Game.AddLineToConversation(Conversation,"TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_THIRD_LINE",false,Me,SpeedFriend); break; \
            case 4: Game.AddLineToConversation(Conversation,"TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_FOURTH_LINE",false,SpeedFriend,Me); break; \
            case 5: Game.AddLineToConversation(Conversation,"TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIFTH_LINE",false,Me,SpeedFriend); break; \
            } \
        } \
        else if (LineOfSpeech%2==0) { \
            ST_FRIEND_LINE("TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SIXTH_LINE") \
        } \
        else { \
            ST_ME_LINE("TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_SEVENTH_LINE") \
        } \
        SpeedTimer=3; \
        LineOfSpeech++; \
    }
#define ST_RACE_START \
    RaceTimer=(long)pGlobalScriptDef->GUI_RaceTime; \
    KillRaceTimer=(long)(pGlobalScriptDef->GUI_RaceTime+20.0f); \
    RunningGameTimerID=Game.AddQuestInfoTimer(RaceTimer,"HUD_CLOCK_ICON",1.0f);
#define ST_REMOVE_MARKER \
    if (!ParentClass.ReachedPlatform) { \
        if (T) return _Fail_Val.Return(); \
        Game.MiniMapRemoveMarker(Game.GetThingWithScriptName("RaceMarker")); \
    }
void NScript::CQ_GuildTrainingScript::CApprenticeSpeedTest::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingConscious(Me));
    NSF;
    if (T) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    ST_START
    Game.SetIsPushableByHero(Me,false);
    CScriptThing SpeedFriend=Game.GetThingWithScriptName("SpeedFriend");
    CScriptThing village=Game.GetNearestWithDefName(Me,"VILLAGE_GUILD_COMPLEX_INSIDE");
    Game.EntityAttachToVillage(Me,village);
    Game.EntityAttachToVillage(SpeedFriend,village);
    Game.EntitySetAsKillable(Me,false,true);
    Game.EntitySetAsKillable(SpeedFriend,false,true);
    Game.SetThingHasInformation(Me,false,true,false);
    Me.SetFriendsWithEverythingFlag(true);
    SpeedFriend.SetFriendsWithEverythingFlag(true);
    RaceMode=0;
    long LineOfSpeech=0;
    CTimer RaceTimer;
    CTimer SpeedTimer;
    CTimer KillRaceTimer;
    SpeedTimer=1;
    long RunningGameTimerID=-1;
    bool Green=false;
    for (; !false; NSF) {
        if (T) return _Fail_Val.Return();
        while (RaceMode==0) {
            NSF;
            if (T) return _Fail_Val.Return();
            if (ParentClass.GameState==3 && !Green) {
                if (T) return _Fail_Val.Return();
                Game.SetThingHasInformation(Me,false,true,false);
                Green=true;
            }
            ST_PRE_ARGUE
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (T) return _Fail_Val.Return();
                ST_MOVIE
                ST_SAY("TEXT_QST_028_FAST_APPRENTICE_BOAST")
                ST_ASK("TEXT_QST_028_FAST_APPRENTICE_BOAST_QUESTION")
                if (answer==1) {
                    if (T) return _Fail_Val.Return();
                    ST_SAY("TEXT_QST_028_FAST_APPRENTICE_RUN")
                    RaceMode=1;
                    ParentClass.ReachedPlatform=false;
                    Game.MiniMapAddMarker(Game.GetThingWithScriptName("RaceMarker"),"HUD_ORB_QUEST_VIGNETTE");
                    ST_RACE_START
                    Game.DisplayQuestInfo(true);
                    if (!Green) {
                        if (T) return _Fail_Val.Return();
                        Game.SetThingHasInformation(Me,false,true,false);
                        Green=true;
                    }
                }
                else {
                    if (T) return _Fail_Val.Return();
                    ST_SAY("TEXT_QST_028_FAST_APPRENTICE_REFUSE")
                }
            }
        }
        if (T) return _Fail_Val.Return();
        while (RaceMode==1) {
            NSF;
            if (T) return _Fail_Val.Return();
            ST_PRE_ARGUE
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (T) return _Fail_Val.Return();
                Game.RemoveQuestInfoElement(RunningGameTimerID);
                Game.DisplayQuestInfo(false);
                if (!ParentClass.ReachedPlatform) {
                    if (T) return _Fail_Val.Return();
                    ST_MOVIE
                    ST_SAY("TEXT_QST_028_FAST_APPRENTICE_NO_PLATFORM")
                    RaceMode=2;
                }
                else if (RaceTimer.GetTime()<=0) {
                    if (T) return _Fail_Val.Return();
                    ST_MOVIE
                    ST_SAY("TEXT_QST_028_FAST_APPRENTICE_TOO_SLOW")
                    RaceMode=2;
                    ST_REMOVE_MARKER
                }
                else {
                    if (T) return _Fail_Val.Return();
                    ST_MOVIE
                    ST_SAY("TEXT_QST_028_FAST_APPRENTICE_SUCCEED")
                    RaceMode=3;
                    Game.GiveHeroGold((long)pGlobalScriptDef->GUI_RaceGold);
                    Game.ClearThingHasInformation(Me);
                    LineOfSpeech=0;
                }
            }
            if (KillRaceTimer.GetTime()<=0) {
                if (T) return _Fail_Val.Return();
                Game.RemoveQuestInfoElement(RunningGameTimerID);
                Game.DisplayQuestInfo(false);
                ST_REMOVE_MARKER
                RaceMode=2;
            }
        }
        if (T) return _Fail_Val.Return();
        while (RaceMode==2) {
            NSF;
            if (T) return _Fail_Val.Return();
            ST_PRE_ARGUE
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (T) return _Fail_Val.Return();
                ST_MOVIE
                ST_SAY("TEXT_QST_028_FAST_APPRENTICE_RETURN_FAIL")
                ST_ASK("TEXT_QST_028_FAST_APPRENTICE_RETURN_FAIL_QUESTION")
                if (answer==1) {
                    if (T) return _Fail_Val.Return();
                    ST_SAY("TEXT_QST_028_FAST_APPRENTICE_RUN")
                    RaceMode=1;
                    ParentClass.ReachedPlatform=false;
                    ST_RACE_START
                    Game.MiniMapAddMarker(Game.GetThingWithScriptName("RaceMarker"),"HUD_ORB_QUEST_VIGNETTE");
                }
                else {
                    if (T) return _Fail_Val.Return();
                    ST_SAY("TEXT_QST_028_FAST_APPRENTICE_REFUSE")
                }
            }
        }
        if (T) return _Fail_Val.Return();
        while (RaceMode==3) {
            NSF;
            if (T) return _Fail_Val.Return();
            if (IsDistanceBetweenThingsUnder(Game.GetHero(),Me,10.0f) && SpeedTimer.GetTime()<=0) {
                if (T) return _Fail_Val.Return();
                long Conversation=Game.AddNewConversation(Me,false,false);
                Game.AddPersonToConversation(Conversation,SpeedFriend);
                if (LineOfSpeech<=5) {
                    if (T) return _Fail_Val.Return();
                    switch (LineOfSpeech) {
                    case 1: Game.AddLineToConversation(Conversation,"TEXT_QST_028_FAST_FRIEND_POST_ARGUE_FIRST_LINE",false,SpeedFriend,Me); break;
                    case 2: Game.AddLineToConversation(Conversation,"TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_SECOND_LINE",false,Me,SpeedFriend); break;
                    case 3: Game.AddLineToConversation(Conversation,"TEXT_QST_028_FAST_FRIEND_POST_ARGUE_THIRD_LINE",false,SpeedFriend,Me); break;
                    case 4: Game.AddLineToConversation(Conversation,"TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_FOURTH_LINE",false,Me,SpeedFriend); break;
                    case 5: Game.AddLineToConversation(Conversation,"TEXT_QST_028_FAST_FRIEND_POST_ARGUE_FIFTH_LINE",false,SpeedFriend,Me); break;
                    }
                }
                else if (LineOfSpeech%2==0) {
                    ST_ME_LINE("TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_SIXTH_LINE")
                }
                else {
                    ST_FRIEND_LINE("TEXT_QST_028_FAST_FRIEND_POST_ARGUE_SEVENTH_LINE")
                }
                SpeedTimer=7;
                LineOfSpeech++;
            }
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (T) return _Fail_Val.Return();
                ST_MOVIE
                ST_ASK("TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION")
                if (answer==1) {
                    if (T) return _Fail_Val.Return();
                    ST_SAY("TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION_YES")
                }
                else {
                    if (T) return _Fail_Val.Return();
                    ST_SAY("TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION_NO")
                }
                Game.MiniMapRemoveMarker(Game.GetThingWithScriptName("RaceMarker"));
            }
        }
        if (T) return _Fail_Val.Return();
    }
    if (T) return _Fail_Val.Return();
}