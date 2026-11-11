// NScript::CQ_BanditCampBossBattleScript::CBanditForger::Main() -- the bandit-camp forger: owns a pass, waves to a nearby hero who lacks one, offers the pass for gold, repeats the offer after an unsuccessful first conversation until sold, and turns hostile when attacked.
// Grade: DIFFER(5293v5373) -- Missing native 78-byte hot Pause/Movie/Timer/STB cleanup D0D3A1..D0D3EE and two-byte aligned-loop jump. Frame A4 and ordinary body/register choices agree; one native hot Timer expansion versus called common destructor remains unresolved.
// Inline-budget: S15 expands implicit Movie cleanup; visible called Timer destructor preserves the native frame. One native hot Timer cleanup copy remains missing.
// Flags: /d2inlS15
// Leaf: ?Main@CBanditForger@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ
// Retail: 0x00D0C410, 5373 bytes, thiscall.
#define FABLE_RETAIL_CPREDICATEBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CISTHINGALIVE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CSCRIPTRESOURCE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CBASEOBJECT_ORIGINAL_ACCESS
#define FABLE_RETAIL_CSCRIPTTHING_ORIGINAL_ACCESS
#define FABLE_RETAIL_CTIMER_ORIGINAL_ACCESS
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
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer() { ReleaseData(); }
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CIsThingAlive.h"
#include "engine/CTimer.h"
#include "engine/fable_script_def_global.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
#include "engine/EHeroAbility.h"
namespace NGlobalConsole { extern bool GDefaultToUsingPhysicsInCutsceneAnimations; }   // 0x01375748
extern CGameScriptInterfaceBase* pDSTGame;
inline CTimer::CTimer() { TimerIndex=pDSTGame->RegisterTimer(); }
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
inline bool CTimer::operator<=(long time) const { return GetTime()<=time; }
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
inline NScript::CIsThingAlive::~CIsThingAlive() {}
bool FABLE_FASTCALL IsDistanceBetweenThingsUnder(const CScriptThing& thing_a, const CScriptThing& thing_b, float distance);   // 0x00CBE2FF
void NScript::CQ_BanditCampBossBattleScript::CBanditForger::Main()
{
    TRetVoid _Fail_Val;
    bool TalkedTo=false;
    bool HasPass=true;
    bool PsstAmount=false;
    bool NotAttacked=true;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CTimer ForgeTimer;
    ForgeTimer=0;
    if (!GivenPass) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.GiveThingHeroRewardItem(Me,"OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS","");
        GivenPass=true;
    }
    while (!TalkedTo && NotAttacked) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO")) || Game.AreEntitiesEnemies(Me,Game.GetHero())) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            NotAttacked=false;
        }
        if (ForgeTimer<=0 && IsDistanceBetweenThingsUnder(Game.GetHero(),Me,10.0f) && !ParentClass.Gate2Open && !Game.IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS",Game.GetHero())) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),false);
            seh_me.PlayAnimation("ST_WAVE_SPECIAL_02",false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            while (seh_me.IsPerformingScriptTask()) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (!PsstAmount) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long Conversation=Game.AddNewConversation(Me,false,false);
                Game.AddPersonToConversation(Conversation,Game.GetHero());
                Game.AddLineToConversation(Conversation,"TEXT_QST_009_FORGER_ASIDE_FIRST",false,Me,Game.GetHero());
                PsstAmount=true;
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long Conversation=Game.AddNewConversation(Me,false,false);
                Game.AddPersonToConversation(Conversation,Game.GetHero());
                Game.AddLineToConversation(Conversation,"TEXT_QST_009_FORGER_ASIDE_SECOND",false,Me,Game.GetHero());
                PsstAmount=false;
            }
            ForgeTimer=10;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (ParentClass.Gate2Open) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_009_FORGER_EARLY_NOT_NEED",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            else if (Game.IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS",Game.GetHero())) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_009_FORGER_EARLY_GOT_PASS",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_009_FORGER_CHAT",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                Game.GiveHeroYesNoQuestion("TEXT_QST_009_FORGER_CHAT_QUESTION","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                long answer=Game.MsgIsQuestionAnsweredYesOrNo();
                while (answer<0) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    answer=Game.MsgIsQuestionAnsweredYesOrNo();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (answer==1) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHeroGold()<pGlobalScriptDef->BAC_ForgerGoldAmount) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_009_FORGER_FAILED",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                    else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_009_FORGER_SUCCESS",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        Game.GiveHeroObject("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS",-1,false);
                        Game.GiveHeroGold(-pGlobalScriptDef->BAC_ForgerGoldAmount);
                        Game.EntityGiveGold(Me,pGlobalScriptDef->BAC_ForgerGoldAmount);
                        Game.RemoveItemFromContainer(Me,"OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS");
                        HasPass=false;
                        Game.GiveHeroExperience(pGlobalScriptDef->EXPERIENCE_GRANT_SMALL);
                        Game.ClearThingHasInformation(Me);
                    }
                }
                else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_009_FORGER_TURNED_DOWN",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
            }
            TalkedTo=true;
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (HasPass && NotAttacked) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO")) || Game.AreEntitiesEnemies(Me,Game.GetHero())) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            NotAttacked=false;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (ParentClass.Gate2Open || Game.IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS",Game.GetHero())) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_009_FORGER_LATE_NOT_NEED",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.GiveHeroYesNoQuestion("TEXT_QST_009_FORGER_SECOND_QUESTION","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                long answer=Game.MsgIsQuestionAnsweredYesOrNo();
                while (answer<0) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    answer=Game.MsgIsQuestionAnsweredYesOrNo();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (answer==1) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHeroGold()<pGlobalScriptDef->BAC_ForgerGoldAmount) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_009_FORGER_SECOND_FAIL",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                    else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_009_FORGER_SUCCESS",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        Game.GiveHeroObject("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS",-1,false);
                        Game.GiveHeroGold(-pGlobalScriptDef->BAC_ForgerGoldAmount);
                        Game.EntityGiveGold(Me,pGlobalScriptDef->BAC_ForgerGoldAmount);
                        Game.RemoveItemFromContainer(Me,"OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS");
                        Game.GiveHeroExperience(pGlobalScriptDef->EXPERIENCE_GRANT_SMALL);
                        Game.ClearThingHasInformation(Me);
                        HasPass=false;
                    }
                }
                else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_009_FORGER_TURNED_DOWN",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
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
        Game.ClearThingHasInformation(Me);
        Game.GiveThingBestEnemyTarget(Me,Game.GetHero());
    }
    if (seh_me.IsValid()) seh_me.Release();
    for (;;) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}
