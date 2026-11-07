// NScript::CQ_BanditCampBossBattleScript::CBanditForger::Main() -- the bandit-camp forger: gives himself the camp pass, then until the hero attacks him waves the hero over now and then while the hero carries a pass, and when talked to either says he is not needed (gate 2 open / hero has a pass) or offers to sell the pass for the forger's gold price (pays and grants experience on yes); after a sale he keeps offering it again; attacked, he turns hostile and fights the hero for good.
// Grade: DIFFER(5255v5373) -- chunk 828 re-grade md 765: frame 0xa0 v 0xa4 (retail spills the condition / talk bool to [esp+0x13]); ours rotates loop 1 (retail tests NotAttacked [esp+0x2f] at the top on every pass); writing the talk block as `if (!talked) continue; {...} break;` (probe f5 / f13) gives retail's loop head but VC7.1 then expands ~7 more return epilogues (+400 bytes) and materialises the bool with neg/sbb
// Next idea: f5's loop head with the return epilogues kept out of line (they are the per-exit movie / STB / CTimer dtor sites, chunk 813 _Fail_Val return tail); the bool spill to [esp+0x13] may follow once the epilogues match
// Leaf: ?Main@CBanditForger@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE_CTOR
#define FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_FORGER
#include "engine/C3DVector.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CQ_BanditCampBossBattleScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CIsThingAlive.h"
#include "engine/CTimer.h"
#include "engine/fable_script_def_global.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
namespace NGlobalConsole { extern bool GDefaultToUsingPhysicsInCutsceneAnimations; }   // 0x01375748
extern CGameScriptInterfaceBase* pDSTGame;
inline CTimer::CTimer() { TimerIndex=pDSTGame->RegisterTimer(); }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
inline NScript::CIsThingAlive::~CIsThingAlive() {}
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& thing_a, const CScriptThing& thing_b, float distance);   // 0x00CBE2FF
void NScript::CQ_BanditCampBossBattleScript::CBanditForger::Main()
{
    TRetVoid _Fail_Val;
    bool StillSellingPass=true;
    bool SaidFirstAside=false;
    bool NotAttacked=true;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CTimer WaveTimer;
    WaveTimer=0;
    if (!GivenPass) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->GiveThingHeroRewardItem(Me,"OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS","");
        GivenPass=true;
    }
    while (NotAttacked) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO")) || Game->AreEntitiesEnemies(Me,Game->GetHero())) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            NotAttacked=false;
        }
        if (WaveTimer.GetTime()<=0 && IsDistanceBetweenThingsUnder(Game->GetHero(),Me,10.0f) && !ParentClass.Gate2Open && Game->IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS",Game->GetHero())) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),false);
            seh_me.PlayAnimation("ST_WAVE_SPECIAL_02",false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            while (seh_me.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (!SaidFirstAside) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long conversation=Game->AddNewConversation(Me,false,false);
                Game->AddPersonToConversation(conversation,Game->GetHero());
                Game->AddLineToConversation(conversation,"TEXT_QST_009_FORGER_ASIDE_FIRST",false,Me,Game->GetHero());
                SaidFirstAside=true;
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long conversation=Game->AddNewConversation(Me,false,false);
                Game->AddPersonToConversation(conversation,Game->GetHero());
                Game->AddLineToConversation(conversation,"TEXT_QST_009_FORGER_ASIDE_SECOND",false,Me,Game->GetHero());
                SaidFirstAside=false;
            }
            WaveTimer=10;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (ParentClass.Gate2Open) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_009_FORGER_EARLY_NOT_NEED",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            else if (Game->IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS",Game->GetHero())) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_009_FORGER_EARLY_GOT_PASS",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_009_FORGER_CHAT",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                Game->GiveHeroYesNoQuestion("TEXT_QST_009_FORGER_CHAT_QUESTION","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                long answer=Game->MsgIsQuestionAnsweredYesOrNo();
                while (answer<0) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    answer=Game->MsgIsQuestionAnsweredYesOrNo();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (answer==1) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHeroGold()<pGlobalScriptDef->BAC_ForgerGoldAmount) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_009_FORGER_FAILED",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                    else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_009_FORGER_SUCCESS",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        Game->GiveHeroObject("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS",-1,false);
                        Game->GiveHeroGold(-pGlobalScriptDef->BAC_ForgerGoldAmount);
                        Game->EntityGiveGold(Me,pGlobalScriptDef->BAC_ForgerGoldAmount);
                        Game->RemoveItemFromContainer(Me,"OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS");
                        StillSellingPass=false;
                        Game->GiveHeroExperience(pGlobalScriptDef->EXPERIENCE_GRANT_SMALL);
                        Game->ClearThingHasInformation(Me);
                    }
                }
                else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_009_FORGER_TURNED_DOWN",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
            }
            break;
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (StillSellingPass && NotAttacked) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO")) || Game->AreEntitiesEnemies(Me,Game->GetHero())) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            NotAttacked=false;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (ParentClass.Gate2Open || Game->IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS",Game->GetHero())) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_009_FORGER_LATE_NOT_NEED",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->GiveHeroYesNoQuestion("TEXT_QST_009_FORGER_SECOND_QUESTION","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                long answer=Game->MsgIsQuestionAnsweredYesOrNo();
                while (answer<0) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    answer=Game->MsgIsQuestionAnsweredYesOrNo();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (answer==1) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHeroGold()<pGlobalScriptDef->BAC_ForgerGoldAmount) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_009_FORGER_SECOND_FAIL",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                    else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_009_FORGER_SUCCESS",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        Game->GiveHeroObject("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS",-1,false);
                        Game->GiveHeroGold(-pGlobalScriptDef->BAC_ForgerGoldAmount);
                        Game->EntityGiveGold(Me,pGlobalScriptDef->BAC_ForgerGoldAmount);
                        Game->RemoveItemFromContainer(Me,"OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS");
                        Game->GiveHeroExperience(pGlobalScriptDef->EXPERIENCE_GRANT_SMALL);
                        Game->ClearThingHasInformation(Me);
                        StillSellingPass=false;
                    }
                }
                else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_009_FORGER_TURNED_DOWN",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (!NotAttacked) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->ClearThingHasInformation(Me);
        Game->GiveThingBestEnemyTarget(Me,Game->GetHero());
    }
    if (seh_me.IsValid()) seh_me.Release();
    for (;;) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
}
