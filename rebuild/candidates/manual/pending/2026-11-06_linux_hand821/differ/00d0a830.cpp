// NScript::CQ_BanditCampBossBattleScript::CBanditKing::Main() -- Twinblade's boss-fight script: set the bandit-camp objective and minimap marker, taunt the hero at three-quarter and half health until the fight is over, then lie in the defeated pose begging every ten seconds; when the hero hits him again play the "over" line, turn him into a killable enemy of the hero with a health bar and track his health until he dies
// Grade: DIFFER(2700v2720) -- the speech MovieBase dtor at the merged terminate exit of the "over" speech only (0x00D0B27A..0x00D0B28A): retail expands the compiler-generated dtor there (`call 0x006E7AB0` PImp ; `mov [esp+0x7c],0x0126008C` ; `call 0x0099A430`), ours calls the COMDAT 0x006E7B80; the scope-end site expands as in retail. With FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE (a2, 2732v2720, also 22 lines) both sites expand but the user-declared dtor adds a MovieBase vtable store at each
// Next idea: (chunk 821, 16 more attempts) the merged exit is too cold for the implicit dtor under c2's frequency rule; do-while(0) around the SAY block (the 0x00D2ED20 match), around Speak/wait, around each T, around the health tail, a for(;;) wait with the post check inside, and no TRetVoid all leave it called or expand only the in-loop exit (no merge); the c2 inlS diagnostic knob shifts other inlines (no diagnostic match): park
// Leaf: ?Main@CBanditKing@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ
// Inline-split: one do { } while (0) as the speech-movie scope (retail expands the movie dtor at both of its sites; construct unproven)
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_BANDITKING
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_BanditCampBossBattleScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CRGBColour.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CTimer.h"
#include "engine/TRetVoid.h"
#include "engine/fable_script_animation_defaults.h"
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& t):Thing(t) {}
inline NScript::CIsThingAlive::~CIsThingAlive() { }

extern CGameScriptInterfaceBase* pDSTGame;
inline CTimer::CTimer():TimerIndex(pDSTGame->RegisterTimer()) {}
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex,time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }

void NScript::CQ_BanditCampBossBattleScript::CBanditKing::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if(IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game->SetQuestCardObjective("Q_BanditCamp","TEXT_QUEST_BANDIT_CAMP_OBJECTIVE_07","","BanditCampMain");
    Game->MiniMapAddMarker(Me,"HUD_ORB_QUEST_CORE");
    Game->EntitySetAbleToBeEngagedInCombat(Me,false);
    long StartHealth=(long)Game->GetHealth(Me);
    bool FirstTaunt=false;
    bool SecondTaunt=false;
    CTimer TauntTimer;
    TauntTimer=0;
    while(!ParentClass.ItsAllOver) {
        Game->NewScriptFrame();
        if(IsActiveThreadTerminating()) return _Fail_Val.Return();
        ParentClass.KingHealth=(long)Game->GetHealth(Me);
        if(ParentClass.KingHealth<StartHealth*3/4 && !FirstTaunt && TauntTimer.GetTime()<=0) {
            if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            long HLStyleConversationIndex=Game->AddNewConversation(Me,false,false);
            Game->AddPersonToConversation(HLStyleConversationIndex,Game->GetHero());
            Game->AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_009_TWINBLADE_TAUNT_10",false,Me,Game->GetHero());
            FirstTaunt=true;
            TauntTimer=8;
        }
        if(ParentClass.KingHealth<StartHealth/2 && !SecondTaunt && TauntTimer.GetTime()<=0) {
            if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            long HLStyleConversationIndex=Game->AddNewConversation(Me,false,false);
            Game->AddPersonToConversation(HLStyleConversationIndex,Game->GetHero());
            Game->AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_009_TWINBLADE_TAUNT_20",false,Me,Game->GetHero());
            SecondTaunt=true;
            TauntTimer=8;
        }
    }
    if(IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if(seh_me.IsValid()) seh_me.Release();
    while(!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if(IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if(IsActiveThreadTerminating()) return _Fail_Val.Return();
    CTimer BegTimer;
    BegTimer=0;
    long BegCount=0;
    Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),false);
    Game->EntitySetFacingAngleTowardsThing(Game->GetHero(),Me,false);
    seh_me.PlayLoopingAnimation("DEFEATED_POSE",-1,false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
    for(;!false;Game->NewScriptFrame()) {
        if(IsActiveThreadTerminating()) return _Fail_Val.Return();
        if(BegTimer.GetTime()<=0) {
            if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            long HLStyleConversationIndex=Game->AddNewConversation(Me,false,false);
            Game->AddPersonToConversation(HLStyleConversationIndex,Game->GetHero());
            switch(BegCount) {
            case 0:
                Game->AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_009_TWINBLADE_BEGGING_10",false,Me,Game->GetHero());
                BegCount=1;
                break;
            case 1:
                Game->AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_009_TWINBLADE_BEGGING_20",false,Me,Game->GetHero());
                BegCount=2;
                break;
            case 2:
                Game->AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_009_TWINBLADE_BEGGING_30",false,Me,Game->GetHero());
                BegCount=3;
                break;
            case 3:
                Game->AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_009_TWINBLADE_BEGGING_40",false,Me,Game->GetHero());
                BegCount=4;
                break;
            case 4:
                Game->AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_009_TWINBLADE_BEGGING_50",false,Me,Game->GetHero());
                BegCount=3;
                break;
            }
            BegTimer=10;
        }
        if(Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            do {
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                seh_me.ClearAllActionsIncludingLoopingAnimations();
                if(Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_009_TWINBLADE_OVER",GROUP_SELECT_NONE,false,true,false);
                    while(seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if(IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if(IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                ParentClass.TwinBladeAttacked=true;
                ParentClass.AngryBanditNeeded=4;
                if(seh_me.IsValid()) seh_me.Release();
                Game->GiveThingBestEnemyTarget(Me,Game->GetHero());
                Game->EntitySetAsKillable(Me,true,true);
                Game->EntitySetAbleToBeEngagedInCombat(Me,true);
            } while (0);
            Game->DisplayQuestInfo(true);
            HealthBarIndex=Game->AddQuestInfoBar((float)StartHealth,0.0f,CRGBColour(255,0,0,255),CRGBColour(255,0,0,255),"HUD_QUEST_ICON_TWINBLADE","",1.0f);
            while(Me.IsAlive()) {
                Game->NewScriptFrame();
                if(IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->UpdateQuestInfoBar(HealthBarIndex,Game->GetHealth(Me),-1.0f,-1.0f);
            }
        }
    }
}
