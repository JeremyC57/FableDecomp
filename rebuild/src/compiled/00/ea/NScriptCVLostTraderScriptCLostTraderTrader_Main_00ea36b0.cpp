// NScript::CV_LostTraderScript::CLostTraderTrader::Main() -- the lost trader: while the hero is far away he yells for help, then turns to the hero and yells at him, and once the hero is close he introduces himself; he then follows the hero (with a health bar on the quest HUD), asking him to get back when talked to and making random comments, until he is close enough to his brother, when he stops being scared and stops following.
// Leaf: ?Main@CLostTraderTrader@CV_LostTraderScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EA36B0, length 4804 bytes including its 5-entry switch table at 0x00EA4960
//   (cases 1..5 -> 0x00EA37BA / 0x00EA395E / 0x00EA3E4E / 0x00EA422F / 0x00EA4824; default to the loop step 0x00EA47F9).
// Name: slot 1 of the entity vtable 0x012EDDA8 that the Alloc 0x00EA34F0 stores (0 scalar deleting dtor 0x00EA3690, 2 Init
//   0x00EA35A0, 3 GetParentScript 0x00EA3590, 4 OnPersist 0x00EA5960, 5 OnPredicateFail 0x00EA3640) and the only .rdata
//   reference to the body; Ego_r.pdb ?Main@CLostTraderTrader@CV_LostTraderScript@NScript@@EAEXXZ (FableWin.pdb has it too).
// Callees: SetPredicate 0x00F35B10, CIsThingAlive copy 0x004ABE90 / ~CIsThingAlive 0x00E241B0 (out of line),
//   IsActiveThreadTerminating 0x00F35B30, scripted-thing handle (CBaseObject 0x0099A380, IsValid 0x00CD23B9, Release
//   0x00CD2770, ~ 0x007E74D0, ClearCommands 0x007E7360, Speak 0x007E7390, IsPerformingScriptTask 0x007E7450,
//   GetScriptThing 0x007E7490), CTimer ctor 0x00CD4450 / dtor 0x00CD4470, MovieBase ctor 0x006E7B60 / dtor 0x006E7B80 /
//   PImp dtor 0x006E7AB0, ~CBaseObject 0x0099A430, ~CScriptThing 0x004AA840, CCharString 0x0099EBF0 / ~ 0x0099EAE0, rand
//   0x00BFEB16, IsDistanceBetweenThingsUnder 0x00CBE2FF / Over 0x00CBE3EA (__fastcall), the trader's own helpers
//   TraderYellForHelp 0x00EA4980, TraderYellAtHero 0x00EA4BB0, MakeRandomComment 0x00EA4DE0; interface slots through
//   Game and pDSTGame 0x0143E8F8 (SetTimer 89 / GetTimer 90 / DeregisterTimer 88).
// Data: pGlobalScriptDef 0x0143E90C (Outer/Middle/InnerTraderDistance +0x6D8/+0x6DC/+0x6E0, TraderYellForHelp/YellAtHero/
//   CommentRandomValue +0x6E4/+0x6E8/+0x6EC); ParentClass ReachedTrader +0x4C, g_bTraderHit +0x4B,
//   DistanceForBrotherToReunite +0x50; own stateBrain +0x1C.
// Shape: generated entity script. The CTimer ctor is a plain inline definition that VC7.1 still calls (0x00CD4450) at both
//   sites, but with its body visible the timer index stays in ebp across the TraderYellForHelp / rand / terminate calls
//   (GetTimer and SetTimer both push ebp); declared-only, every use reloads it (58 lines). Plain-inline ~CTimer gives
//   retail's mix (one exit expands DeregisterTimer, the rest call 0x00CD4470); the implicit MovieBase pair with the
//   declared-only CCountedPointer dtor gives the out-of-line ctor in case 3 and the expanded ctor + one expanded exit in
//   case 4. One function-wide flags word guards the hit-test CCharString temporaries (bits 1..0x4000).
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CV_LOSTTRADERSCRIPT_TRADER
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CV_LostTraderScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CTimer.h"
#include "engine/TRetVoid.h"
#include "engine/CIsThingAlive.h"
#include "engine/fable_script_def_global.h"
#include "engine/fable_script_distance.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
extern CGameScriptInterfaceBase* pDSTGame;
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex,time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
extern "C" int __cdecl rand();
void NScript::CV_LostTraderScript::CLostTraderTrader::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGH)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    for (; !false; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        switch (stateBrain) {
        case 1:
            while (IsDistanceBetweenThingsOver(Me,Game->GetHero(),pGlobalScriptDef->OuterTraderDistance)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->EntitySetThingAsAllyOfThing(Me,Game->GetHero());
                    Game->EntitySetThingAsAllyOfThing(Game->GetHero(),Me);
                }
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            stateBrain=2;
            break;
        case 2: {
            CTimer YellTimer;
            YellTimer=rand()%3+2;
            while (IsDistanceBetweenThingsOver(Me,Game->GetHero(),pGlobalScriptDef->MiddleTraderDistance)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (rand()%pGlobalScriptDef->TraderYellForHelpRandomValue==0 && YellTimer.GetTime()==0) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    TraderYellForHelp();
                    YellTimer=rand()%3+5;
                }
                if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->EntitySetThingAsAllyOfThing(Me,Game->GetHero());
                    Game->EntitySetThingAsAllyOfThing(Game->GetHero(),Me);
                }
                if (IsDistanceBetweenThingsOver(Me,Game->GetHero(),pGlobalScriptDef->OuterTraderDistance)) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    stateBrain=1;
                    break;
                }
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntitySetFacingAngleTowardsThing(Game->GetHero(),Me,false);
            while (IsDistanceBetweenThingsOver(Me,Game->GetHero(),pGlobalScriptDef->InnerTraderDistance)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (rand()%pGlobalScriptDef->TraderYellAtHeroRandomValue==0 && YellTimer.GetTime()==0) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    TraderYellAtHero();
                    YellTimer=rand()%2+5;
                }
                if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->EntitySetThingAsAllyOfThing(Me,Game->GetHero());
                    Game->EntitySetThingAsAllyOfThing(Game->GetHero(),Me);
                }
                if (IsDistanceBetweenThingsOver(Me,Game->GetHero(),pGlobalScriptDef->MiddleTraderDistance)) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    break;
                }
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (IsDistanceBetweenThingsUnder(Me,Game->GetHero(),pGlobalScriptDef->InnerTraderDistance)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                stateBrain=3;
            }
            break;
        }
        case 3:
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            while (IsDistanceBetweenThingsOver(Me,Game->GetHero(),3.0f)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->EntitySetThingAsAllyOfThing(Me,Game->GetHero());
                    Game->EntitySetThingAsAllyOfThing(Game->GetHero(),Me);
                }
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            seh_me.ClearCommands();
            {
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_042_LOSTTRADER_INTRO_10",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_042_LOSTTRADER_INTRO_20",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            if (seh_me.IsValid()) seh_me.Release();
            stateBrain=4;
            break;
        case 4: {
            Game->DisplayQuestInfo(true);
            Game->AddQuestInfoBarHealth(Me,CRGBColour(255,0,0,255),"HUD_QUEST_ICON_TRADER_HAT_01",1.0f);
            Game->EntityFollowThing(Me,Game->GetHero(),1.0f,true);
            Game->EntitySetOpinionReactionsEnabled(Me,false);
            if (seh_me.IsValid()) seh_me.Release();
            ParentClass.ReachedTrader=true;
            CTimer CommentTimer;
            CommentTimer=5;
            CScriptThing Brother=Game->GetThingWithScriptName("LostTraderBrother");
            while (!IsDistanceBetweenThingsUnder(Me,Brother,(float)ParentClass.DistanceForBrotherToReunite)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    {
                        CScriptGameResourceObjectMovieBase speech_movie;
                        Game->StartMovieSequence("",speech_movie);
                        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                        if (seh_me.IsValid()) seh_me.Release();
                        while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_042_LOSTTRADER_GET_BACK",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                    if (seh_me.IsValid()) seh_me.Release();
                }
                if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->EntitySetThingAsAllyOfThing(Me,Game->GetHero());
                    Game->EntitySetThingAsAllyOfThing(Game->GetHero(),Me);
                }
                if (Me.MsgIsHitBy("")) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    long HLStyleConversationIndex=Game->AddNewConversation(Me,false,false);
                    Game->AddPersonToConversation(HLStyleConversationIndex,Game->GetHero());
                    Game->AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_042_LOSTTRADER_QUICK",false,Me,Game->GetHero());
                }
                if (!ParentClass.g_bTraderHit && rand()%pGlobalScriptDef->TraderCommentRandomValue!=0 && CommentTimer.GetTime()==0) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    CommentTimer=rand()%5+10;
                    Game->EntitySetOpinionReactionsEnabled(Me,true);
                    MakeRandomComment();
                }
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntitySetAsScared(Me,false);
            Game->EntityStopFollowing(Me);
            if (seh_me.IsValid()) seh_me.Release();
            stateBrain=5;
            break;
        }
        case 5:
            while (!false) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            break;
        }
    }
}