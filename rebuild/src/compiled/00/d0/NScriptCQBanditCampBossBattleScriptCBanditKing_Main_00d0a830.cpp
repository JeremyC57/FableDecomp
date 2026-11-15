// NScript::CQ_BanditCampBossBattleScript::CBanditKing::Main() -- Twinblade's boss-fight script: set the bandit-camp objective and minimap marker, taunt the hero at three-quarter and half health until the fight is over, then lie in the defeated pose begging every ten seconds; when the hero hits him again play the "over" line, turn him into a killable enemy of the hero with a health bar and track his health until he dies
// Inline-budget: in-body #pragma inline_depth(0) ... inline_depth() round the one `Terminate: return` statement -- the speech-wait terminate exits destroy the movie EXPANDED (0x00D0B27A: call 0x006E7AB0 ; GROBase vtable store ; call 0x0099A430) and then CALL ~CTimer 0x00CD4470 / ~seh_me 0x007E74D0 / ~CTimer (shared tail 0x00D0B297), the only such site in the whole exe (scan of every expanded movie dtor). c2 scores a callee per site as c*g(f) and the implicit movie dtor costs more than ~CTimer (c 19 v 14), so no threshold splits them inside ONE return statement: the movie is destroyed at the BK_T jump and the timers at the Terminate return, which the gotos make hot (without the pragma ~CTimer expands there and the return cross-jumps into the expanded-timer exit 0x00D0B227: 2812v2720; /d2inlT40..100 and /d2inlS5..14 sweeps on that source: no match). Also FABLE_FORCEINLINE on the row-local CTimer::operator=(long): retail expands every timer store (the pDSTGame SetTimer vtable call, no CTimer call); plain inline DIFFER(2704v2720). GetTime and the pause / movie ctor opt-ins (FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_ / CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE_CTOR) are not forced: the leaf is byte-identical without them (agentR w22).
// Leaf: ?Main@CBanditKing@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D0A830, 2720 bytes incl. the 5-entry BegCount jump table after the last ret (0x00D0B2BC..), thiscall, no EH frame.
// Identity: Ego_r.pdb / FableWin.pdb ?Main@CBanditKing@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ (private virtual); its one .rdata reference is the CBanditKing entity-vtable slot at 0x012C9478.
// Shape (all measured by grading; behaviour identical to plain returns):
//   (1) BK_T, the speech-wait terminate check, is a statement macro (do { } while (0)) that leaves through `goto Terminate`: the jump
//       leaves the movie scope, so the pause object and the movie are destroyed at the jump (a hot block, movie dtor expanded at
//       0x00D0B27A) and the function's `Terminate: return` destroys BegTimer / seh_me / TauntTimer (the shared tail 0x00D0B297, all
//       three called). A plain `return` there destroys all five in one statement, where c2 cannot expand the dearer movie dtor and call
//       the cheaper ~CTimer (plain-return source: 2700v2720, movie dtor called).
//   (2) BK_SAY, the speak-and-wait block, is a statement macro (do { } while (0) round `if (GetHealth > 0) { Speak; wait; T; }`), the
//       idiom of the landed CEndGameScythe::Main 0x00D2ED20. It replaces the earlier hand-nested `do { do { T } while (0); } while (0)`
//       after the wait loop (same objects, cpp -P token streams differ only by that one do/brace pair moving).
//   (3) the movie scope is a do { } while (0) block (FITTED, retail's construct unproven): plain braces grade DIFFER(2704v2720).
//   Measured alternatives: unwrapped BK_T 2704; no pragma 2812 (also with an implicit end `Terminate: ;`); per-row /d2inlT40..100 and
//   /d2inlS5..14 on the pragma-free source: none matches (2688..2880), so the scoped pragma (Inline-budget line) is the least device.
// Callees: CTimer ctor 0x00CD4450 / dtor 0x00CD4470 (GetTime / operator= expanded through pDSTGame 0x0143E8F8), movie PImp dtor
//   CCountedPointer<CScriptGameResourceObjectMovieBase>::~ 0x006E7AB0 (out of line, as the landed movie rows), ~CBaseObject 0x0099A430,
//   ~CScriptGameResourceObjectScriptedThingBase 0x007E74D0, IsActiveThreadTerminating 0x00F35B30, SetPredicate (CIsThingAlive).
#include "fable_compiler.h"
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
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex,time); return *this; }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }

// BK_T: the terminate check of the speech wait. It leaves the movie scope through the function's Terminate exit (header item 1).
#define BK_T do { if(IsActiveThreadTerminating()) goto Terminate; } while (0)
// BK_SAY: say a line and wait for it (only while the speaker has health), as one statement (header item 2).
#define BK_SAY(text) \
    do { \
        if(Game->GetHealth(seh_me.GetScriptThing())>0.0f) { \
            seh_me.Speak(Game->GetHero(),text,GROUP_SELECT_NONE,false,true,false); \
            while(seh_me.IsPerformingScriptTask()) { \
                Game->NewScriptFrame(); \
                BK_T; \
            } \
            BK_T; \
        } \
    } while (0)
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
            do {   // the speech movie's scope (header item 3): movie and pause end before the health bar
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                seh_me.ClearAllActionsIncludingLoopingAnimations();
                BK_SAY("TEXT_QST_009_TWINBLADE_OVER");
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
Terminate:   // reached only from BK_T (the main loop never ends)
#pragma inline_depth(0)   // Inline-budget (line 2): ~CTimer called here, as retail's shared tail 0x00D0B297
    return _Fail_Val.Return();
#pragma inline_depth()
}