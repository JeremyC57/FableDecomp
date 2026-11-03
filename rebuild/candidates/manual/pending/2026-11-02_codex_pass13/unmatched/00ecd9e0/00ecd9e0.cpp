// NScript::CV_SickChildScript::CSickChildsMother::Main() -- introduce the sick child quest and accept the cure from the hero.
// Leaf: ?Main@CSickChildsMother@CV_SickChildScript@NScript@@EAEXXZ
// Grade: DIFFER(2679v2679) -- two parent resource address calculations differ; generated Movie members remain implicit.
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_EXTERNAL_DTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRING_EXTERNAL_CONVERSION
#define FABLE_RETAIL_CDEFPOINTER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CENTITYSCRIPTBASE
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSCRIPTTHING
#define FABLE_RETAIL_CISTHINGALIVE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_DECLARED_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE
#include "engine/CV_SickChildScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
inline CScriptThing::~CScriptThing() {}
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
#include "engine/fable_script_def_global.h"
#include "engine/fable_script_distance.h"
void NScript::CV_SickChildScript::CSickChildsMother::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    {
        CScriptGameResourceObjectScriptedThingBase &seh_Mother=ParentClass.seh_Mother;
        if (seh_Mother.IsValid()) seh_Mother.Release();
    }
    while (!Game.StartScriptingEntity(Me,ParentClass.seh_Mother,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (Game.IsObjectInThingsPossession(pGlobalScriptDef->EndPotion,Game.GetHero()) || !ParentClass.MotherIntroDone) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.SetThingHasInformation(Me,false,true,false);
    }
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (!ParentClass.MotherIntroDone && IsDistanceBetweenThingsUnder(Me,Game.GetHero(),7.0f)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            ParentClass.PlayCutscene("CS_SICK_INTRO","NULL");
            Game.GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_SICK_CHILD",Game.GetActiveQuestName(),false);
            Game.SetQuestCardObjective(Game.GetActiveQuestName(),"TEXT_QUEST_SICK_CHILD_OBJECTIVE_01","","");
            ParentClass.MotherIntroDone=true;
            Game.ClearThingHasInformation(Me);
        }
        if (ParentClass.MotherIntroDone && Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (Game.IsObjectInThingsPossession(pGlobalScriptDef->EndPotion,Game.GetHero())) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.GiveHeroYesNoQuestion("TEXT_QST_B10_MOTHER_CURE_QUESTION","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                long YNQAnswer;
                while ((YNQAnswer=Game.MsgIsQuestionAnsweredYesOrNo())<0) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (YNQAnswer==1) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ParentClass.PlayCutscene("CS_SICK_OUTRO","NULL");
                    Game.TakeObjectFromHero(pGlobalScriptDef->EndPotion);
                    Game.ClearThingHasInformation(Me);
                    ParentClass.FinishedQuest=true;
                    break;
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(ParentClass.seh_Mother.GetScriptThing())>0.0f) {
    ParentClass.seh_Mother.Speak(Game.GetHero(),"TEXT_QST_B10_MOTHER_ASK_CURE",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
    while (ParentClass.seh_Mother.IsPerformingScriptTask()) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}
                }
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(ParentClass.seh_Mother.GetScriptThing())>0.0f) {
    ParentClass.seh_Mother.Speak(Game.GetHero(),"TEXT_QST_B10_MOTHER_CURE_REMIND_10",GROUP_SELECT_NONE,false,true,false);
    while (ParentClass.seh_Mother.IsPerformingScriptTask()) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (Game.GetHealth(ParentClass.seh_Mother.GetScriptThing())>0.0f) {
    ParentClass.seh_Mother.Speak(Game.GetHero(),"TEXT_QST_B10_MOTHER_HIT",GROUP_SELECT_NONE,false,true,false);
    while (ParentClass.seh_Mother.IsPerformingScriptTask()) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}
            Game.EntitySetThingAsAllyOfThing(Me,Game.GetHero());
            Game.EntitySetThingAsAllyOfThing(Game.GetHero(),Me);
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    {
        CScriptGameResourceObjectScriptedThingBase &seh_Mother=ParentClass.seh_Mother;
        if (seh_Mother.IsValid()) seh_Mother.Release();
    }
    for (;;) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
}
