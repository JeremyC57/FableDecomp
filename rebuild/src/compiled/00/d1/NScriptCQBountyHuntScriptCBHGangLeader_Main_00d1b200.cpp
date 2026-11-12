// NScript::CQ_BountyHuntScript::CBH_GangLeader::Main() -- the bounty hunt's bandit leader
// Grade: RELOCATION_MATCH -- actual COFF operands checked against their native targets.
// Inline-budget: minimum neighbour-proved /d2inlS15 expands all seven implicit movie destructor sites without introducing a derived vtable reset.
// Flags: /d2inlS15
// Leaf: ?Main@CBH_GangLeader@CQ_BountyHuntScript@NScript@@EAEXXZ
// Retail: 0x00D1B200, 6100 bytes, thiscall.
// Identity (proven, two sources): the Ego_r.pdb / FableWin.pdb name sets hold ?Main@CBH_GangLeader@CQ_BountyHuntScript@NScript@@EAEXXZ (private virtual);
//   the address has one .rdata reference, slot 1 of the entity vtable 0x012CA5C4 that the landed CBH_GangLeader::Alloc 0x00D16380
//   stores (slot 0 ??_G 0x00D16440, 2 Init 0x00D16DC0, 4 OnPersist 0x00D179B0).
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CISTHINGALIVE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CPREDICATEBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#define FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_GANGDEPUTY2
#define FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_GANGLEADER
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE_CTOR
#define FABLE_RETAIL_CDEFSTRING_FORCEINLINE_GETSTRING
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#include <string.h>
#include "fable_string.h"
#include "engine/CCountedPointer.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();                        // 0x006E7AB0
#include "engine/CScriptThing.h"
#include "engine/EHeroAbility.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "stlport/fable_containers.h"
#include "engine/CQ_BountyHuntScript.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CIsThingAlive.h"
#include "engine/C3DVector.h"
#include "engine/TRetVoid.h"
#include "engine/fable_script_cutscene.h"
#include "engine/fable_script_def_global.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_fade.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
#include "engine/EScriptEntityMoveType.h"
extern const float DELTA_NOTIONAL_ZERO;                                                                       // 0x0129BA3C (1e-4f)
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
template <> CCountedPointer<CScriptGameResourceObjectScriptedThingBase>& CCountedPointer<CScriptGameResourceObjectScriptedThingBase>::operator=(const CCountedPointer<CScriptGameResourceObjectScriptedThingBase>&);   // 0x008AB1E0
FABLE_FORCEINLINE CScriptGameResourceObjectScriptedThingBase& CScriptGameResourceObjectScriptedThingBase::operator=(const CScriptGameResourceObjectScriptedThingBase& other) { CScriptGameResourceObjectBase::operator=(other); PImp = other.PImp; return *this; }
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }
void NScript::CQ_BountyHuntScript::CBH_GangLeader::Main()
{
    TRetVoid _Fail_Val;
    float HealthDiff;
    if (!ParentClass.BountyHuntIntroComplete) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.RemoveThing(Me,false,false);
        return _Fail_Val.Return();
    }
    {
        CIsThingAlive alive(Me);
        SetPredicate(alive);
    }
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (!ParentClass.BanditCutsceneDone) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase sehMe;
    if (Game.IsRegionLoaded("GreatwoodEntrance")) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (sehMe.IsValid()) sehMe.Release();
        while (!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        CScriptThing fadeOutMarker=Game.GetThingWithScriptName("M_BH_FadeOut");
        sehMe.MoveToPosition(fadeOutMarker.GetPos(),4.0f,ENTITY_MOVE_RUN,false,false);
        while (!IsDistanceBetweenThingsUnder(Me,fadeOutMarker,5.0f)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        TeleportData tData;
        Game.EntityGetAppearanceSeed(Me,tData.appearanceSeed);
        tData.defName=Me.GetDefName();
        tData.health=Game.GetHealth(Me);
        tData.scriptName="BH_GangLeader";
        tData.teleportTo="GreatwoodLake";
        tData.teleportMkr="M_BH_LeaderPos";
        ParentClass.TeleportInfo.push_back(tData);
        FadeOutAndKillEntity(Me,1.0f,true,true);
        while (!(Game.IsRegionLoaded("GreatwoodLake") && ParentClass.FishComplete)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    else if (Game.IsRegionLoaded("GreatwoodLake")) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (sehMe.IsValid()) sehMe.Release();
        while (!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (!ParentClass.LeaderCutsceneComplete) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.FadeScreenOutUntilNextCallToFadeScreenIn(0.5f,0.0f);
            CScriptGameResourceObjectScriptedThingBase sehHero;
            CScriptGameResourceObjectScriptedThingBase sehVictim;
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> myMap;
            Game.StartScriptingEntity(Game.GetHero(),sehHero,SCRIPT_AI_PRIORITY_HIGHEST);
            Game.StartScriptingEntity(Game.GetThingWithScriptName("BH_Hostage2"),sehVictim,SCRIPT_AI_PRIORITY_HIGHEST);
            {
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                myMap["Hero"]=sehHero;
                myMap["Victim"]=sehVictim;
                myMap["Leader"]=sehMe;
                RunCutsceneMacro("CS_BOUNTYHUNT_GREATLAKEVICTIM",myMap,0,0,false,true);
            }
            if (sehHero.IsValid()) sehHero.Release();
            if (sehVictim.IsValid()) sehVictim.Release();
            ParentClass.LeaderCutsceneComplete=true;
        }
        if (!ParentClass.SpawnLeaderGrunts) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ParentClass.SpawnBanditGangGrunts("M_BH_GruntSpawnLake");
            ParentClass.AddSpawnedFunction(new CSpawnedFunc<CQ_BountyHuntScript>(CCharString("ParentClass.") + CCharString("WatchForLeaderGruntsKilled"), 0, &ParentClass, &CQ_BountyHuntScript::WatchForLeaderGruntsKilled), CCharString(""));
            ParentClass.SpawnLeaderGrunts=true;
        }
        Game.SetThingHasInformation(Me,false,true,false);
        HealthCurrent=HealthInitial=Game.GetHealth(Me);
        CScriptThing Hostage2=Game.GetThingWithScriptName("BH_Hostage2");
        Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),false);
        CScriptThing approachMkr1=Game.GetThingWithScriptName("M_BH_LeaderApproach1");
        CScriptThing approachMkr2=Game.GetThingWithScriptName("M_BH_LeaderApproach2");
        CScriptThing moneyMkr=Game.GetThingWithScriptName("M_BH_MoneyBag");
        for (; !false; Game.NewScriptFrame()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if ((Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO")))
                && !ParentClass.LeaderAttackingHero && !ParentClass.Hostage2Killed && !ParentClass.LeaderAttackingHostage) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(Me)>DELTA_NOTIONAL_ZERO) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (!ParentClass.LeaderChargeComplete) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),false);
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (sehMe.IsValid()) sehMe.Release();
                        while (!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        CScriptGameResourceObjectMovieBase speech_movie;
                        Game.StartMovieSequence("",speech_movie);
                        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                        if (Game.GetHealth(sehMe.GetScriptThing())>0.0f) {
                            sehMe.Speak(Game.GetHero(),"TEXT_QST_017_LEADER6",GROUP_SELECT_NONE,false,true,false);
                            while (sehMe.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (sehMe.IsValid()) sehMe.Release();
                        Game.GiveThingBestEnemyTarget(Me,Game.GetHero());
                        ParentClass.LeaderChargeComplete=true;
                    }
                }
                ParentClass.SpawnBanditGangGrunts("M_BH_GruntSpawnLeaderCharge");
                ParentClass.AddSpawnedFunction(new CSpawnedFunc<CQ_BountyHuntScript>(CCharString("ParentClass.") + CCharString("WatchForLeaderChargeGruntsKilled"), 0, &ParentClass, &CQ_BountyHuntScript::WatchForLeaderChargeGruntsKilled), CCharString(""));
                ParentClass.LeaderAttackingHero=true;
            }
            if (!ParentClass.LeaderAttackingHero && !ParentClass.LeaderIntro1Complete && IsDistanceBetweenThingsUnder(approachMkr1,Game.GetHero(),3.0f)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(Me)>DELTA_NOTIONAL_ZERO) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (!ParentClass.Hostage2Killed) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),false);
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (sehMe.IsValid()) sehMe.Release();
                        while (!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        CScriptGameResourceObjectMovieBase speech_movie;
                        Game.StartMovieSequence("",speech_movie);
                        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                        if (Game.GetHealth(sehMe.GetScriptThing())>0.0f) {
                            sehMe.Speak(Game.GetHero(),"TEXT_QST_017_LEADER_DEMAND_2",GROUP_SELECT_NONE,false,true,false);
                            while (sehMe.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                    ParentClass.LeaderIntro1Complete=true;
                }
            }
            else if (!ParentClass.LeaderAttackingHero && !ParentClass.LeaderIntro2Complete && IsDistanceBetweenThingsUnder(approachMkr2,Game.GetHero(),3.0f)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(Me)>DELTA_NOTIONAL_ZERO) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (!ParentClass.Hostage2Killed) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),false);
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (sehMe.IsValid()) sehMe.Release();
                        while (!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        CScriptGameResourceObjectMovieBase speech_movie;
                        Game.StartMovieSequence("",speech_movie);
                        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                        if (Game.GetHealth(sehMe.GetScriptThing())>0.0f) {
                            sehMe.Speak(Game.GetHero(),"TEXT_QST_017_LEADER_WARNING_3",GROUP_SELECT_NONE,false,true,false);
                            while (sehMe.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                    ParentClass.LeaderIntro2Complete=true;
                }
            }
            if (ParentClass.LeaderIntro2Complete && !ParentClass.Hostage2Killed && !ParentClass.LeaderAttackingHostage && !ParentClass.LeaderAttackingHero) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.EntitySetFacingAngleTowardsThing(Me,Hostage2,false);
                Game.ModifyThingHealth(Hostage2,1.0f-Game.GetHealth(Hostage2),false);
                if (!Game.IsObjectInThingsPossession(pGlobalScriptDef->BountyHuntRewardObject.GetString(),Game.GetHero())) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game.AddItemToContainer(Hostage2,pGlobalScriptDef->BountyHuntRewardObject.GetString());
                }
                Game.EntityGiveGold(Hostage2,pGlobalScriptDef->BountyHuntGoldReward);
                Game.GiveThingBestEnemyTarget(Me,Hostage2);
                long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_017_LEADER7",false,Me,Game.GetHero());
                ParentClass.SpawnBanditGangGrunts("M_BH_GruntSpawnLeaderCharge");
                ParentClass.AddSpawnedFunction(new CSpawnedFunc<CQ_BountyHuntScript>(CCharString("ParentClass.") + CCharString("WatchForLeaderChargeGruntsKilled"), 0, &ParentClass, &CQ_BountyHuntScript::WatchForLeaderChargeGruntsKilled), CCharString(""));
                Game.SetScriptingStateGroup(Me,(EScriptingStateGroups)0);
                ParentClass.LeaderAttackingHostage=true;
                if (sehMe.IsValid()) sehMe.Release();
            }
            if (ParentClass.Hostage2Killed || !Hostage2.IsAlive()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (sehMe.IsValid()) sehMe.Release();
                while (!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptThing Exit=Game.GetThingWithScriptName("M_BH_Greatwood3Exit");
                if (sehMe.IsValid() && Exit.IsAlive()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    sehMe.MoveToPosition(Exit.GetPos(),1.0f,ENTITY_MOVE_RUN,false,true);
                    while (!IsDistanceBetweenThingsUnder(Me,Exit,2.0f)) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ParentClass.LeaderEscaped=true;
                    FadeOutAndKillEntity(Me,2.0f,true,true);
                    for (;;) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
}
// Attempts: see STATUS.tsv / ATTEMPTS.log (best = probe/gl_a10.cpp)