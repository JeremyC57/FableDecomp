// NScript::CV_SickChildScript::CTalkingTrader1::Main() -- the Bowerstone trader pair, trader A: take scripted control, find trader B, leave if the fishing-spot mushroom is already taken; then each frame: when the chat timer is out and the hero is within 20 and trader B within 10, play the next of the four A/B chats and rearm the timer; answer the hero (intro / repeat lines, or the other-trader-dead line when B is gone) and push the chat back; when the hero hits him while he lives, make him killable after the third hit and speak the next on-hit line
// Grade: RELOCATION_MATCH(2928v2928) -- chunk 868: the main loop written with the loop-head macro (`for(;;) { do { if(IsActiveThreadTerminating()) return; } while(0); ... Game->NewScriptFrame(); }`, docs/pipeline/INLINE_DEPTH_REVISIT.md lever 4) makes the loop exit hot (~CTimer expanded at 0x00ECC866 as retail) while the hit-block speech returns stay cold (their ~CTimer is the shared call 0x00CD4470 at 0x00ECC92C); the plain `while(!IsActiveThreadTerminating())` form expanded it at the hit returns too (2976 bytes, md 24)
// Inline-split: lever 4 at the main loop head plus the do-while(0) on the talk block's first terminate check (line "do { if(IsActiveThreadTerminating()) return; } while(0);"): both are the statement-macro shape; retail's construct at either site is unproven (without the talk-block one the loop exit is expanded but 0x00ECC8F9/0x00ECC907 still expand ~CTimer: 2972 bytes, md 25)
// Leaf: ?Main@CTalkingTrader1@CV_SickChildScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ECBDF0, 2928 bytes (the chat switch table 0x00ECC950 inside), thiscall, no EH frame.
// Identity (proven, two sources): the address appears once in .rdata, at slot 1 of the entity vtable the landed
//   CTalkingTrader1::Alloc 0x00ECBC80 stores (Init 0x00ECBD50, GetParentScript 0x00ECBD20, scalar deleting dtor 0x00ECBD30 landed);
//   Ego_r.pdb and FableWin.pdb both hold ?Main@CTalkingTrader1@CV_SickChildScript@NScript@@EAEXXZ. Twin: the landed
//   CTalkingTrader2::Main 0x00ECCAD0 (trader B, same shape without the chats).
// Callees: CIsThingAlive vtable 0x012C32F0 / dtor 0x00E241B0, CScriptThing copy 0x004ABE90 / dtor 0x004AA840 / IsAlive 0x004AB130,
//   SetPredicate 0x00F35B10, IsActiveThreadTerminating 0x00F35B30, scripted-thing IsValid / Release / dtor / GetScriptThing / Speak /
//   IsPerformingScriptTask, movie ctor / dtor 0x006E7B60 / 0x006E7B80, CCharString 0x0099E4B0 / 0x0099EBF0 / 0x0099EAE0 / operator=
//   0x0099EFB0 / 0x0099EFE0 / operator const char* 0x0099E4C0, GFIntToCharString 0x0099F830, operator+ 0x0099F690, IsDistanceBetween-
//   ThingsUnder 0x00CBE2FF, ~CTimer 0x00CD4470 (pDSTGame 0x0143E8F8 Register/Get/Set/DeregisterTimer inline).
// Data: engine/CV_SickChildScript.h GotFishingSpotMushroom +0x69 (ParentClass +0x14); TimesTalkedTo +0x1c, TimesHit +0x20.
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#include "engine/CV_SickChildScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CTimer.h"
#include "engine/fable_script_text.h"
#include "engine/fable_script_distance.h"
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
extern CGameScriptInterfaceBase* pDSTGame;
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}

void NScript::CV_SickChildScript::CTalkingTrader1::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if(IsActiveThreadTerminating()) return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if(seh_me.IsValid()) seh_me.Release();
    while(!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if(IsActiveThreadTerminating()) return;
    }
    if(IsActiveThreadTerminating()) return;
    CScriptThing TalkToThisGuy=Game->GetThingWithScriptName("TalkingTrader2");
    CTimer ChatTimer;
    long ChatNumber=0;
    long LineNumber=10;
    if(ParentClass.GotFishingSpotMushroom) {
        if(IsActiveThreadTerminating()) return;
        Game->RemoveThing(Me,false,true);
    }
    for(;;) {
        do { if(IsActiveThreadTerminating()) return; } while(0);
        if(ChatTimer.GetTime()==0 && IsDistanceBetweenThingsUnder(Me,Game->GetHero(),20.0f) && IsDistanceBetweenThingsUnder(Me,TalkToThisGuy,10.0f)) {
            if(IsActiveThreadTerminating()) return;
            long Conversation=Game->AddNewConversation(Me,false,false);
            Game->AddPersonToConversation(Conversation,TalkToThisGuy);
            switch(ChatNumber) {
                case 0:
                    Game->AddLineToConversation(Conversation,"TEXT_QST_B10_TRADERA_CHAT1_10",false,Me,TalkToThisGuy);
                    Game->AddLineToConversation(Conversation,"TEXT_QST_B10_TRADERB_CHAT1_20",false,TalkToThisGuy,Me);
                    break;
                case 1:
                    Game->AddLineToConversation(Conversation,"TEXT_QST_B10_TRADERA_CHAT2_10",false,Me,TalkToThisGuy);
                    Game->AddLineToConversation(Conversation,"TEXT_QST_B10_TRADERB_CHAT2_20",false,TalkToThisGuy,Me);
                    break;
                case 2:
                    Game->AddLineToConversation(Conversation,"TEXT_QST_B10_TRADERA_CHAT3_10",false,Me,TalkToThisGuy);
                    Game->AddLineToConversation(Conversation,"TEXT_QST_B10_TRADERB_CHAT3_20",false,TalkToThisGuy,Me);
                    Game->AddLineToConversation(Conversation,"TEXT_QST_B10_TRADERA_CHAT3_30",false,TalkToThisGuy,Me);
                    break;
                case 3:
                    Game->AddLineToConversation(Conversation,"TEXT_QST_B10_TRADERA_CHAT4_10",false,Me,TalkToThisGuy);
                    Game->AddLineToConversation(Conversation,"TEXT_QST_B10_TRADERB_CHAT4_20",false,TalkToThisGuy,Me);
                    break;
            }
            if(ChatNumber<3) {
                if(IsActiveThreadTerminating()) return;
                ++ChatNumber;
            } else {
                if(IsActiveThreadTerminating()) return;
                ChatNumber=0;
            }
            ChatTimer=16;
        }
        if(Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            do { if(IsActiveThreadTerminating()) return; } while(0);
            CCharString TalkString;
            switch(TimesTalkedTo) {
                case 0: TalkString="TEXT_QST_B10_TRADERA_INTRO"; break;
                case 1: TalkString="TEXT_QST_B10_TRADERA_REPEATA"; break;
                case 2: TalkString="TEXT_QST_B10_TRADERA_REPEATB_10"; TimesTalkedTo=-1; break;
            }
            ++TimesTalkedTo;
            ChatTimer=ChatTimer.GetTime()+5;
            if(TalkToThisGuy.IsAlive()) {
                if(IsActiveThreadTerminating()) return;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if(Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),TalkString,GROUP_SELECT_NONE,false,true,false);
                    while(seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if(IsActiveThreadTerminating()) return;
                    }
                    if(IsActiveThreadTerminating()) return;
                }
            } else {
                if(IsActiveThreadTerminating()) return;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if(Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_B10_TRADERA_OTHERTRADERDEAD_10",GROUP_SELECT_NONE,false,true,false);
                    while(seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if(IsActiveThreadTerminating()) return;
                    }
                    if(IsActiveThreadTerminating()) return;
                }
            }
        }
        if((Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) && Game->GetHealth(Me)>0.0f) {
            if(IsActiveThreadTerminating()) return;
            if(TimesHit>2) {
                if(IsActiveThreadTerminating()) return;
                Game->EntitySetAsKillable(Me,true,true);
                Game->EntitySetAsToAddToComboMultiplierWhenHit(Me,true);
            }
            CCharString FullText;
            FullText="TEXT_QST_B10_TRADERA_ONHIT_"+GFIntToCharString(LineNumber);
            if(!Game->TextEntryExists(FullText)) {
                LineNumber=10;
                FullText="TEXT_QST_B10_TRADERA_ONHIT_"+GFIntToCharString(LineNumber);
            }
            LineNumber+=10;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if(Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                seh_me.Speak(Game->GetHero(),FullText,GROUP_SELECT_NONE,false,true,false);
                while(seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if(IsActiveThreadTerminating()) return;
                }
                if(IsActiveThreadTerminating()) return;
            }
            ++TimesHit;
        }
        Game->NewScriptFrame();
    }
}