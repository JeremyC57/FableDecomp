// NScript::CQ_BountyHuntScript::CBH_Hostage2::Main() -- transfer and rescue the second bounty-hunt hostage.
// Leaf: ?Main@CBH_Hostage2@CQ_BountyHuntScript@NScript@@EAEXXZ
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CENTITYSCRIPTBASE
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSCRIPTTHING
#define FABLE_RETAIL_CISTHINGALIVE
#define FABLE_RETAIL_CTIMER
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE
#define FABLE_RETAIL_STLPORT_MAP
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER
#define FABLE_RETAIL_CDEFSTRING_EXTERNAL_CONVERSION
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
// Genuine original authored conversion; original keyword unproven, Owner conditional helper preference.
__forceinline CDefString::operator CCharString() const { return GetString(); }
void NScript::CQ_BountyHuntScript::CBH_Hostage2::Main()
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
        tData.scriptName="BH_Hostage2";
        tData.teleportTo="GreatwoodLake";
        tData.teleportMkr="M_BH_Hostage2Pos";
        ParentClass.TeleportInfo.push_back(tData);
        FadeOutAndKillEntity(Me,1.0f,true,true);
        while (!(Game.IsRegionLoaded("GreatwoodLake") && ParentClass.FishComplete)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    } else if (Game.IsRegionLoaded("GreatwoodLake")) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.EntitySetAsKillable(Me,true,true);
        Game.SetIsThingLightningable(Me,false);
        CScriptThing teleportMkr=Game.GetThingWithScriptName("M_BH_Hostage2Pos");
        Game.SetWanderCentrePoint(Me,teleportMkr.GetPos());
        Game.SetWanderMinDistance(Me,0.0f);
        Game.SetWanderMaxDistance(Me,pGlobalScriptDef->BountyHuntHostage2WanderDistance);
        Game.SetScriptingStateGroup(Me,ESSG_WANDER_NEAR);
        Game.SetThingHasInformation(Me,false,true,false);
        CScriptThing Leader=Game.GetThingWithScriptName("BH_GangLeader");
        for (; !false; Game.NewScriptFrame()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (sehMe.IsValid()) sehMe.Release();
            while (!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Leader.IsAlive() && !ParentClass.LeaderKilled) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.EntitySetFacingAngleTowardsThing(Me,Leader,false);
            }
            if (ParentClass.AllLeaderChargeGruntsKilled && (ParentClass.LeaderKilled || ParentClass.LeaderEscaped)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.EntitySetAsScared(Me,false);
                if (sehMe.IsValid()) sehMe.Release();
                while (!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                {
                    CScriptGameResourceObjectScriptedThingBase sehHero;
                    std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> myMap;
                    Game.StartScriptingEntity(Game.GetHero(),sehHero,SCRIPT_AI_PRIORITY_HIGHEST);
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    myMap["Hero"]=sehHero;
                    myMap["Victim"]=sehMe;
                    RunCutsceneMacro("CS_BOUNTYHUNT_GREATLAKEVICTIM_OUTRO",myMap,0,0,false,true);
                }
                ParentClass.Hostage2Safe=true;
                Game.GiveHeroGold(pGlobalScriptDef->BountyHuntGoldReward);
                if (!Game.IsObjectInThingsPossession(pGlobalScriptDef->BountyHuntRewardObject,Game.GetHero())) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.IsObjectInThingsPossession(pGlobalScriptDef->BountyHuntRewardObject,Me)) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game.RemoveItemFromContainer(Me,pGlobalScriptDef->BountyHuntRewardObject);
                    }
                    Game.GiveHeroObject(pGlobalScriptDef->BountyHuntRewardObject,-1,false);
                }
                if (sehMe.IsValid()) sehMe.Release();
                while (!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptThing Exit=Game.GetThingWithScriptName("M_BH_Greatwood3Exit");
                if (Exit.IsAlive()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (sehMe.IsValid()) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        sehMe.MoveToPosition(Exit.GetPos(),1.0f,ENTITY_MOVE_RUN,false,true);
                    }
                    while (!(IsDistanceBetweenThingsUnder(Me,Exit,2.0f))) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game.GiveHeroRenownPoints(pGlobalScriptDef->BountyHuntRenownHostage2);
                    FadeOutAndKillEntity(Me,1.0f,true,true);
                }
                ParentClass.LakeComplete=true;
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
}
