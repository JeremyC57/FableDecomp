// NScript::CQ_GuildTrainingScript::CApprenticeSpeedTest::Main() -- the guild-training speed-test apprentice: argues with his friend while the hero is near, and runs the race state machine (0 boast and challenge, 1 race running against the race timer, 2 failed / offer a rematch, 3 won: post-race banter and the success question).
// Grade: DIFFER(9028v9440) -- blocker 1 + the function-wide inline split: retail expands the by-value SetIsPushableByHero Me copy but calls the CIsThingConscious copy 0x004ABE90, and expands the movie ctor / dtor and the GetHealth temp ~CScriptThing only at the state-0 sites and one exit path; VC7.1 calls every site here (a1) or expands every site (a2: IMPLICIT_COPY + inline ~CScriptThing + inline movie, 2098 lines); TRetVoid returns (a3) change nothing
// Next idea: blocker 1 first (a spelling that keeps the nested predicate copy called while depth-1 by-value copies expand); then a way to confine the movie / ~CScriptThing expansion to the state-0 block
// Leaf: ?Main@CApprenticeSpeedTest@CQ_GuildTrainingScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D3E2E0, 9440 bytes, thiscall, plain `ret`, no EH frame, then two 5-entry jump tables.
// Identity (proven): slot 1 (Main) of the CApprenticeSpeedTest entity vtable 0x012CDA40 that Alloc 0x00D3E1F0 stores (slot 0 the
//   landed scalar deleting dtor 0x00D3E2C0, slot 2 Init 0x00D3E2A0, 3 GetParentScript 0x00D3E290); Ego_r.pdb / FableWin.pdb
//   ?Main@CApprenticeSpeedTest@CQ_GuildTrainingScript@NScript@@EAEXXZ.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
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
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CTimer.h"
#include "engine/CIsThingConscious.h"
#include "engine/fable_script_def_global.h"
#include "engine/fable_script_distance.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8
inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& thing):Thing(thing) {}
inline NScript::CIsThingConscious::~CIsThingConscious() {}
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) {}
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline void CScriptThing::SetFriendsWithEverythingFlag(bool on) { if (PImp.Data) PImp.Data->SetFriendsWithEverythingFlag(on); }
#define T IsActiveThreadTerminating()
#define NSF Game->NewScriptFrame()
#define ST_START \
    if (seh_me.IsValid()) seh_me.Release(); \
    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) { \
        NSF; \
        if (T) return; \
    } \
    if (T) return;
#define ST_SAY(TEXT) \
    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) { \
        seh_me.Speak(Game->GetHero(),TEXT,GROUP_SELECT_NONE,false,true,false); \
        while (seh_me.IsPerformingScriptTask()) { \
            NSF; \
            if (T) return; \
        } \
        if (T) return; \
    }
#define ST_MOVIE \
    CScriptGameResourceObjectMovieBase movie; \
    Game->StartMovieSequence("",movie); \
    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
#define ST_ASK(TEXT) \
    Game->GiveHeroYesNoQuestion(TEXT,"TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true); \
    long answer=Game->MsgIsQuestionAnsweredYesOrNo(); \
    while (answer<0) { \
        NSF; \
        if (T) return; \
        answer=Game->MsgIsQuestionAnsweredYesOrNo(); \
    } \
    if (T) return;
#define ST_ME_LINE(TEXT) \
    if (T) return; \
    Game->AddLineToConversation(conversation,TEXT,false,Me,speed_friend);
#define ST_FRIEND_LINE(TEXT) \
    if (T) return; \
    Game->AddLineToConversation(conversation,TEXT,false,speed_friend,Me);
#define ST_PRE_ARGUE \
    if (IsDistanceBetweenThingsUnder(Game->GetHero(),Me,10.0f) && argue_timer.GetTime()<=0) { \
        if (T) return; \
        long conversation=Game->AddNewConversation(Me,false,false); \
        Game->AddPersonToConversation(conversation,speed_friend); \
        if (line<6) { \
            if (T) return; \
            switch (line) { \
            case 1: Game->AddLineToConversation(conversation,"TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIRST_LINE",false,Me,speed_friend); break; \
            case 2: Game->AddLineToConversation(conversation,"TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SECOND_LINE",false,speed_friend,Me); break; \
            case 3: Game->AddLineToConversation(conversation,"TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_THIRD_LINE",false,Me,speed_friend); break; \
            case 4: Game->AddLineToConversation(conversation,"TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_FOURTH_LINE",false,speed_friend,Me); break; \
            case 5: Game->AddLineToConversation(conversation,"TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIFTH_LINE",false,Me,speed_friend); break; \
            } \
        } \
        else if (line%2==0) { \
            ST_FRIEND_LINE("TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SIXTH_LINE") \
        } \
        else { \
            ST_ME_LINE("TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_SEVENTH_LINE") \
        } \
        argue_timer=3; \
        line++; \
    }
#define ST_RACE_START \
    race_timer=(long)pGlobalScriptDef->GUI_RaceTime; \
    total_timer=(long)(pGlobalScriptDef->GUI_RaceTime+20.0f); \
    quest_info=Game->AddQuestInfoTimer(race_timer,"HUD_CLOCK_ICON",1.0f);
#define ST_REMOVE_MARKER \
    if (!ParentClass.ReachedPlatform) { \
        if (T) return; \
        Game->MiniMapRemoveMarker(Game->GetThingWithScriptName("RaceMarker")); \
    }
void NScript::CQ_GuildTrainingScript::CApprenticeSpeedTest::Main()
{
    SetPredicate(CIsThingConscious(Me));
    NSF;
    if (T) return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    ST_START
    Game->SetIsPushableByHero(Me,false);
    CScriptThing speed_friend=Game->GetThingWithScriptName("SpeedFriend");
    CScriptThing village=Game->GetNearestWithDefName(Me,"VILLAGE_GUILD_COMPLEX_INSIDE");
    Game->EntityAttachToVillage(Me,village);
    Game->EntityAttachToVillage(speed_friend,village);
    Game->EntitySetAsKillable(Me,false,true);
    Game->EntitySetAsKillable(speed_friend,false,true);
    Game->SetThingHasInformation(Me,false,true,false);
    Me.SetFriendsWithEverythingFlag(true);
    speed_friend.SetFriendsWithEverythingFlag(true);
    RaceMode=0;
    long line=0;
    CTimer race_timer;
    CTimer argue_timer;
    CTimer total_timer;
    argue_timer=1;
    long quest_info=-1;
    bool info_set=false;
    while (!T) {
        while (RaceMode==0) {
            NSF;
            if (T) return;
            if (ParentClass.GameState==3 && !info_set) {
                if (T) return;
                Game->SetThingHasInformation(Me,false,true,false);
                info_set=true;
            }
            ST_PRE_ARGUE
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (T) return;
                ST_MOVIE
                ST_SAY("TEXT_QST_028_FAST_APPRENTICE_BOAST")
                ST_ASK("TEXT_QST_028_FAST_APPRENTICE_BOAST_QUESTION")
                if (answer==1) {
                    if (T) return;
                    ST_SAY("TEXT_QST_028_FAST_APPRENTICE_RUN")
                    RaceMode=1;
                    ParentClass.ReachedPlatform=false;
                    Game->MiniMapAddMarker(Game->GetThingWithScriptName("RaceMarker"),"HUD_ORB_QUEST_VIGNETTE");
                    ST_RACE_START
                    Game->DisplayQuestInfo(true);
                    if (!info_set) {
                        if (T) return;
                        Game->SetThingHasInformation(Me,false,true,false);
                        info_set=true;
                    }
                }
                else {
                    if (T) return;
                    ST_SAY("TEXT_QST_028_FAST_APPRENTICE_REFUSE")
                }
            }
        }
        if (T) return;
        while (RaceMode==1) {
            NSF;
            if (T) return;
            ST_PRE_ARGUE
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (T) return;
                Game->RemoveQuestInfoElement(quest_info);
                Game->DisplayQuestInfo(false);
                if (!ParentClass.ReachedPlatform) {
                    if (T) return;
                    ST_MOVIE
                    ST_SAY("TEXT_QST_028_FAST_APPRENTICE_NO_PLATFORM")
                    RaceMode=2;
                }
                else if (race_timer.GetTime()<=0) {
                    if (T) return;
                    ST_MOVIE
                    ST_SAY("TEXT_QST_028_FAST_APPRENTICE_TOO_SLOW")
                    RaceMode=2;
                    ST_REMOVE_MARKER
                }
                else {
                    if (T) return;
                    ST_MOVIE
                    ST_SAY("TEXT_QST_028_FAST_APPRENTICE_SUCCEED")
                    RaceMode=3;
                    Game->GiveHeroGold((long)pGlobalScriptDef->GUI_RaceGold);
                    Game->ClearThingHasInformation(Me);
                    line=0;
                }
            }
            if (total_timer.GetTime()<=0) {
                if (T) return;
                Game->RemoveQuestInfoElement(quest_info);
                Game->DisplayQuestInfo(false);
                ST_REMOVE_MARKER
                RaceMode=2;
            }
        }
        if (T) return;
        while (RaceMode==2) {
            NSF;
            if (T) return;
            ST_PRE_ARGUE
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (T) return;
                ST_MOVIE
                ST_SAY("TEXT_QST_028_FAST_APPRENTICE_RETURN_FAIL")
                ST_ASK("TEXT_QST_028_FAST_APPRENTICE_RETURN_FAIL_QUESTION")
                if (answer==1) {
                    if (T) return;
                    ST_SAY("TEXT_QST_028_FAST_APPRENTICE_RUN")
                    RaceMode=1;
                    ParentClass.ReachedPlatform=false;
                    ST_RACE_START
                    Game->MiniMapAddMarker(Game->GetThingWithScriptName("RaceMarker"),"HUD_ORB_QUEST_VIGNETTE");
                }
                else {
                    if (T) return;
                    ST_SAY("TEXT_QST_028_FAST_APPRENTICE_REFUSE")
                }
            }
        }
        if (T) return;
        while (RaceMode==3) {
            NSF;
            if (T) return;
            if (IsDistanceBetweenThingsUnder(Game->GetHero(),Me,10.0f) && argue_timer.GetTime()<=0) {
                if (T) return;
                long conversation=Game->AddNewConversation(Me,false,false);
                Game->AddPersonToConversation(conversation,speed_friend);
                if (line<6) {
                    if (T) return;
                    switch (line) {
                    case 1: Game->AddLineToConversation(conversation,"TEXT_QST_028_FAST_FRIEND_POST_ARGUE_FIRST_LINE",false,speed_friend,Me); break;
                    case 2: Game->AddLineToConversation(conversation,"TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_SECOND_LINE",false,Me,speed_friend); break;
                    case 3: Game->AddLineToConversation(conversation,"TEXT_QST_028_FAST_FRIEND_POST_ARGUE_THIRD_LINE",false,speed_friend,Me); break;
                    case 4: Game->AddLineToConversation(conversation,"TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_FOURTH_LINE",false,Me,speed_friend); break;
                    case 5: Game->AddLineToConversation(conversation,"TEXT_QST_028_FAST_FRIEND_POST_ARGUE_FIFTH_LINE",false,speed_friend,Me); break;
                    }
                }
                else if (line%2==0) {
                    ST_ME_LINE("TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_SIXTH_LINE")
                }
                else {
                    ST_FRIEND_LINE("TEXT_QST_028_FAST_FRIEND_POST_ARGUE_SEVENTH_LINE")
                }
                argue_timer=7;
                line++;
            }
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (T) return;
                ST_MOVIE
                ST_ASK("TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION")
                if (answer==1) {
                    if (T) return;
                    ST_SAY("TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION_YES")
                }
                else {
                    if (T) return;
                    ST_SAY("TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION_NO")
                }
                Game->MiniMapRemoveMarker(Game->GetThingWithScriptName("RaceMarker"));
            }
        }
        if (T) return;
        NSF;
    }
}
