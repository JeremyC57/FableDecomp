// NScript::CQ_BreakSiegeScript::CBanditSiegeLeader::Main() -- introduce the siege, activate the route and monitor the bandit leader.
// Leaf: ?Main@CBanditSiegeLeader@CQ_BreakSiegeScript@NScript@@EAEXXZ
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CENTITYSCRIPTBASE
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_TEXT_COMPARE
#define FABLE_RETAIL_CCHARSTRING_TEXT_COMPARE_EXPANDED
#define FABLE_RETAIL_CSCRIPTTHING
#define FABLE_RETAIL_CISTHINGALIVE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_DECLARED_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE
#define FABLE_RETAIL_STLPORT_MAP
#include <string.h>
#include "engine/CCountedPointer.h"
class CScriptGameResourceObjectScriptedThingBase;
template<> CCountedPointer<CScriptGameResourceObjectScriptedThingBase>& CCountedPointer<CScriptGameResourceObjectScriptedThingBase>::operator=(const CCountedPointer&);
#include "engine/CQ_BreakSiegeScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_cutscene.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EHeroAbility { HERO_ABILITY_HEAL_LIFE_SPELL=14 };
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
inline NScript::CIsThingAlive::~CIsThingAlive() {}
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::map();
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map();
template<> std::vector<CScriptThing>::~vector();
void NScript::CQ_BreakSiegeScript::CBanditSiegeLeader::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (!ParentClass.BanditDemandsKnown) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.FadeScreenOutUntilNextCallToFadeScreenIn(0.5f,0.0f);
        CScriptThing Chief=Game.GetThingWithScriptName("KG_ChiefBreakSiege");
        CScriptThing Guard=Game.GetThingWithScriptName("KG_Sheriff");
        CScriptGameResourceObjectScriptedThingBase seh_hero;
        CScriptGameResourceObjectScriptedThingBase seh_chief;
        CScriptGameResourceObjectScriptedThingBase seh_guard;
        Game.StartScriptingEntity(Game.GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
        Game.StartScriptingEntity(Chief,seh_chief,SCRIPT_AI_PRIORITY_HIGHEST);
        Game.StartScriptingEntity(Guard,seh_guard,SCRIPT_AI_PRIORITY_HIGHEST);
        ParentClass.SendBodyguardsToTheRightPlace("BodyguardTeleportMarker1_BSGIVEN","BodyguardTeleportMarker2_BSGIVEN");
        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
        mymap["HERO"]=seh_hero;
        mymap["LEADER"]=seh_me;
        CScriptGameResourceObjectMovieBase speech_movie;
        Game.StartMovieSequence("",speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
        Game.FixMovieSequenceCamera(true);
        RunCutsceneMacro("CS_BREAKSIEGE_GIVEN",mymap,0,0,false,true);
        mymap["HERO"]=seh_hero;
        mymap["LEADER"]=seh_me;
        mymap["CHIEF"]=seh_chief;
        mymap["GUARD"]=seh_guard;
        ParentClass.SendBodyguardsToTheRightPlace("BodyguardTeleportMarker1_BSINTRO1","BodyguardTeleportMarker2_BSINTRO1");
        RunCutsceneMacro("CS_BREAKSIEGE_INTRO",mymap,0,0,false,true);
        Game.FixMovieSequenceCamera(false);
        ParentClass.SendBodyguardsToTheRightPlace("BodyguardTeleportMarker1_BSINTRO2","BodyguardTeleportMarker2_BSINTRO2");
        CScriptThing NoWayToKHG=Game.CreateObjectOnEntity("OBJECT_BANDIT_PALISADES_01",Game.GetThingWithScriptName("M_Blockage"),"Blockage");
        Game.EntityAttachToScript(NoWayToKHG,Game.GetActiveQuestName());
        std::vector<CScriptThing> Teleporters;
        Game.GetAllThingsWithScriptName("TeleportPoint",Teleporters);
        for (long i=0;i<Teleporters.size();++i) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Teleporters[i].GetDataString()=="KnotholeGlade") {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.SetTeleporterAsActive(Teleporters[i],true);
                break;
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (!ParentClass.QuestStartScreened) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.KickOffQuestStartScreen(Game.GetActiveQuestName(),true,false);
            ParentClass.QuestStartScreened=true;
        }
        ParentClass.HeroLetInTown=true;
        Game.SetQuestCardObjective(Game.GetActiveQuestName(),"TEXT_QUEST_BREAK_SIEGE_OBJECTIVE_02","KnotholeGlade","KnotholeGlade");
    }
    if (!ParentClass.QuestStartScreened) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.KickOffQuestStartScreen(Game.GetActiveQuestName(),true,false);
        ParentClass.QuestStartScreened=true;
    }
    ParentClass.BanditDemandsKnown=true;
    seh_me.Release();
    while (!ParentClass.BanditSiegeStart) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (!ParentClass.StartSecondWave) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (!ParentClass.AttackedBanditLeader && (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO")) || Me.MsgIsHitBy(""))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ParentClass.AttackedBanditLeader=true;
        }
        if (ParentClass.MissionSucceeded) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            while (!ParentClass.EndCutsceneDone) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.RemoveThing(Me,false,true);
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}
