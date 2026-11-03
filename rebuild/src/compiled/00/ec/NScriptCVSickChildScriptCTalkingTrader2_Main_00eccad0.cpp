// NScript::CV_SickChildScript::CTalkingTrader2::Main() -- manage trader dialogue and reactions to attacks.
// Leaf: ?Main@CTalkingTrader2@CV_SickChildScript@NScript@@EAEXXZ
// Original Ego_r TPI10a17; native 012f2348 slot1.
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
#include "engine/fable_script_text.h"
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
inline bool CScriptThing::IsAlive() const { if(PImp.Get()) return PImp->IsAlive(); return false; }

void NScript::CV_SickChildScript::CTalkingTrader2::Main()
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
    CScriptThing TalkToThisGuy=Game->GetThingWithScriptName("TalkingTrader1");
    long LineNumber=10;
    if(ParentClass.GotFishingSpotMushroom) {
        if(IsActiveThreadTerminating()) return;
        Game->RemoveThing(Me,false,true);
    }
    while(!IsActiveThreadTerminating()) {
        if(Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if(IsActiveThreadTerminating()) return;
            CCharString TalkString;
            switch(TimesTalkedTo) {
                case 0: TalkString="TEXT_QST_B10_TRADERB_INTRO"; break;
                case 1: TalkString="TEXT_QST_B10_TRADERB_REPEATA"; break;
                case 2: TalkString="TEXT_QST_B10_TRADERB_REPEATB_10"; TimesTalkedTo=-1; break;
            }
            ++TimesTalkedTo;
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
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_B10_TRADERB_OTHERTRADERDEAD_10",GROUP_SELECT_NONE,false,true,false);
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
            FullText="TEXT_QST_B10_TRADERB_ONHIT_"+GFIntToCharString(LineNumber);
            if(!Game->TextEntryExists(FullText)) {
                LineNumber=10;
                FullText="TEXT_QST_B10_TRADERB_ONHIT_"+GFIntToCharString(LineNumber);
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