// NScript::CQ_BountyHuntScript::CBH_Hostage1::Main() -- transfer and rescue the first bounty-hunt hostage.
// Leaf: ?Main@CBH_Hostage1@CQ_BountyHuntScript@NScript@@EAEXXZ
// Grade: DIFFER(2785v2918) -- implicit generated resource expansion and associated stack/register allocation.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CENTITYSCRIPTBASE
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSCRIPTTHING
#define FABLE_RETAIL_CISTHINGALIVE
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES
#define FABLE_RETAIL_CTIMER
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE
#define FABLE_RETAIL_STLPORT_MAP
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "engine/C3DVector.h"
#include "engine/CQ_BountyHuntScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_def_global.h"
#include "engine/TRetVoid.h"
#include "stlport/fable_map.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EScriptingStateGroups { ESSG_NONE=0, ESSG_PERFORM_ACTION_PHYSICAL=1, ESSG_PERFORM_ACTION_VERBAL=2, ESSG_PERFORM_ACTION_AURAL=3, ESSG_WANDER_NEAR=4, ESSG_FOLLOW_PATH=5, ESSG_FOLLOW_RANDOM=6, ESSG_FOLLOW_NEAREST=7, ESSG_WALK_TO_RANDOM=8, ESSG_WALK_TO_NEAREST_DIFFERENT=9, ESSG_RUN_AT_HERO_AND_ATTACK_UNTIL_DEAD=10 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1, ENTITY_MOVE_SNEAK=2 };
bool FABLE_FASTCALL IsDistanceBetweenThingsUnder(const CScriptThing&,const CScriptThing&,float);
void FABLE_FASTCALL FadeOutAndKillEntity(const CScriptThing&,float,bool,bool);
void FABLE_FASTCALL RunCutsceneMacro(const CCharString&,std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>&,std::map<CCharString,bool>*,std::map<CCharString,CCharString>*,bool,bool);
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::map();
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map();
template<> void std::vector<NScript::CQ_BountyHuntScript::TeleportData>::push_back(const NScript::CQ_BountyHuntScript::TeleportData&);
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
template<> CCountedPointer<CScriptGameResourceObjectScriptedThingBase>& CCountedPointer<CScriptGameResourceObjectScriptedThingBase>::operator=(const CCountedPointer<CScriptGameResourceObjectScriptedThingBase>&);
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
inline NScript::CIsThingAlive::~CIsThingAlive() {}
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
void NScript::CQ_BountyHuntScript::CBH_Hostage1::Main()
{
    TRetVoid _Fail_Val;
    if (!ParentClass.BountyHuntIntroComplete) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.RemoveThing(Me,false,false);
        return;
    }
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (!(ParentClass.BanditCutsceneDone)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase sehMe;
    if (Game.IsRegionLoaded("GreatwoodEntrance")) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.EntitySetAsKillable(Me,false,true);
        if (sehMe.IsValid()) sehMe.Release();
        while (!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        CScriptThing fadeOutMarker=Game.GetThingWithScriptName("M_BH_FadeOut");
        sehMe.MoveToPosition(fadeOutMarker.GetPos(),4.0f,ENTITY_MOVE_RUN,false,false);
        while (!(IsDistanceBetweenThingsUnder(Me,fadeOutMarker,5.0f))) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        TeleportData tData;
        Game.EntityGetAppearanceSeed(Me,tData.appearanceSeed);
        tData.defName=Me.GetDefName();
        tData.health=Game.GetHealth(Me);
        tData.scriptName="BH_Hostage1";
        tData.teleportTo="Fisherman";
        tData.teleportMkr="M_BH_Hostage1Pos";
        ParentClass.TeleportInfo.push_back(tData);
        FadeOutAndKillEntity(Me,1.0f,true,true);
        while (!(Game.IsRegionLoaded("Fisherman"))) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    } else if (Game.IsRegionLoaded("Fisherman")) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.FadeScreenOutUntilNextCallToFadeScreenIn(0.5f,0.0f);
        Game.EntitySetAsKillable(Me,true,true);
        Game.SetIsThingLightningable(Me,false);
        CScriptThing Deputy1=Game.GetThingWithScriptName("BH_GangDeputy1");
        Game.SetThingHasInformation(Me,false,true,false);
        if (sehMe.IsValid()) sehMe.Release();
        while (!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        {
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> csMap;
            CScriptGameResourceObjectScriptedThingBase sehHero;
            CScriptGameResourceObjectScriptedThingBase sehBandit;
            Game.StartScriptingEntity(Deputy1,sehBandit,SCRIPT_AI_PRIORITY_HIGHEST);
            Game.StartScriptingEntity(Game.GetHero(),sehHero,SCRIPT_AI_PRIORITY_HIGHEST);
            csMap["Hero"]=sehHero;
            csMap["Victim"]=sehMe;
            csMap["Bandit"]=sehBandit;
            {
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                RunCutsceneMacro("BOUNTY_HUNT_FISHERMAN_VICTIM",csMap,0,0,false,true);
            }
            if (sehHero.IsValid()) sehHero.Release();
            if (sehMe.IsValid()) sehMe.Release();
            if (sehBandit.IsValid()) sehBandit.Release();
        }
        for (; !false; Game.NewScriptFrame()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Deputy1.IsAlive() && !ParentClass.Deputy1Killed) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.EntitySetFacingAngleTowardsThing(Me,Deputy1,false);
            }
            if (ParentClass.Deputy1Killed && ParentClass.AllDeputy1GruntsKilled) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.EntitySetAsScared(Me,false);
                ParentClass.FishComplete=true;
                if (sehMe.IsValid()) sehMe.Release();
                while (!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                {
                    CScriptGameResourceObjectScriptedThingBase sehHero;
                    Game.StartScriptingEntity(Game.GetHero(),sehHero,SCRIPT_AI_PRIORITY_HIGHEST);
                    std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> csMap;
                    csMap["Hero"]=sehHero;
                    csMap["Victim"]=sehMe;
                    {
                        CScriptGameResourceObjectMovieBase speech_movie;
                        Game.StartMovieSequence("",speech_movie);
                        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                        RunCutsceneMacro("CS_BOUNTYHUNT_FISHERMANVICTIM_OUTRO",csMap,0,0,false,true);
                    }
                    if (sehMe.IsValid()) sehMe.Release();
                    if (sehHero.IsValid()) sehHero.Release();
                }
                Game.SetQuestCardObjective(Game.GetActiveQuestName(),"TEXT_QUEST_BOUNTY_HUNT_OBJECTIVE_03","GreatwoodLake","Greatwood");
                ParentClass.Hostage1Safe=true;
                if (sehMe.IsValid()) sehMe.Release();
                while (!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptThing Exit=Game.GetThingWithScriptName("M_BH_FishermanExit");
                if (Exit.IsAlive()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (sehMe.IsValid()) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        sehMe.MoveToPosition(Exit.GetPos(),1.0f,ENTITY_MOVE_RUN,false,true);
                    }
                    while (!IsDistanceBetweenThingsUnder(Me,Exit,2.0f)) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    FadeOutAndKillEntity(Me,1.0f,true,true);
                    Game.GiveHeroRenownPoints(pGlobalScriptDef->BountyHuntRenownHostage1);
                }
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
}
