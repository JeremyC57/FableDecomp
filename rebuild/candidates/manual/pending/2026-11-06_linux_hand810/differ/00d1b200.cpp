// NScript::CQ_BountyHuntScript::CBH_GangLeader::Main() -- the bounty hunt's bandit leader
// Grade: DIFFER(6180v6100) -- SDIFF 49: the VC7.1 function-wide inline budget: retail expands the implicit movie dtor (call 0x006E7AB0, GROBase vptr store, call 0x0099A430) at all 7 sites and never calls 0x006E7B80; the compiler-generated dtor runs out of budget (5 sites call the COMDAT, a4 225 lines), MOVIEBASE_FORCEINLINE expands every site but its user-declared dtor adds the MovieBase vptr store retail lacks (7 x 11 B, this source); a raised inline budget (diagnostic only, NOTES.md) gives 129 lines; also the movie-exit block layout, 2 loop pads and the GreatwoodEntrance tail (retail keeps two identical ~TeleportData exit copies)
// Next idea: a movie-dtor spelling that expands at every site without the vptr reset, or a way to free budget elsewhere in the body; then the exit-block layout
// Leaf: ?Main@CBH_GangLeader@CQ_BountyHuntScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D1B200, 6100 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven, two sources): the Ego_r.pdb / FableWin.pdb name sets hold ?Main@CBH_GangLeader@CQ_BountyHuntScript@NScript@@EAEXXZ (private virtual);
//   the address has one .rdata reference, slot 1 of the entity vtable 0x012CA5C4 that the landed CBH_GangLeader::Alloc 0x00D16380
//   stores (slot 0 ??_G 0x00D16440, 2 Init 0x00D16DC0, 4 OnPersist 0x00D179B0).
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
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE
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
#include "engine/fable_script_cutscene.h"
#include "engine/fable_script_def_global.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_fade.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1, ENTITY_MOVE_SNEAK=2 };
extern const float DELTA_NOTIONAL_ZERO;                                                                       // 0x0129BA3C (1e-4f)
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
template <> CCountedPointer<CScriptGameResourceObjectScriptedThingBase>& CCountedPointer<CScriptGameResourceObjectScriptedThingBase>::operator=(const CCountedPointer<CScriptGameResourceObjectScriptedThingBase>&);   // 0x008AB1E0
__forceinline CScriptGameResourceObjectScriptedThingBase& CScriptGameResourceObjectScriptedThingBase::operator=(const CScriptGameResourceObjectScriptedThingBase& other) { CScriptGameResourceObjectBase::operator=(other); PImp = other.PImp; return *this; }
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }
void NScript::CQ_BountyHuntScript::CBH_GangLeader::Main()
{
    if (!ParentClass.BountyHuntIntroComplete) {
        if (IsActiveThreadTerminating()) return;
        Game->RemoveThing(Me,false,false);
        return;
    }
    {
        CIsThingAlive alive(Me);
        SetPredicate(alive);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    while (!ParentClass.BanditCutsceneDone) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (Game->IsRegionLoaded("GreatwoodEntrance")) {
        if (IsActiveThreadTerminating()) return;
        if (seh_me.IsValid()) seh_me.Release();
        while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        CScriptThing fade_out=Game->GetThingWithScriptName("M_BH_FadeOut");
        seh_me.MoveToPosition(fade_out.GetPos(),4.0f,ENTITY_MOVE_RUN,false,false);
        while (!IsDistanceBetweenThingsUnder(Me,fade_out,5.0f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        TeleportData teleport;
        Game->EntityGetAppearanceSeed(Me,teleport.appearanceSeed);
        teleport.defName=Me.GetDefName();
        teleport.health=Game->GetHealth(Me);
        teleport.scriptName="BH_GangLeader";
        teleport.teleportTo="GreatwoodLake";
        teleport.teleportMkr="M_BH_LeaderPos";
        ParentClass.TeleportInfo.push_back(teleport);
        FadeOutAndKillEntity(Me,1.0f,true,true);
        while (!(Game->IsRegionLoaded("GreatwoodLake") && ParentClass.FishComplete)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
    }
    else if (Game->IsRegionLoaded("GreatwoodLake")) {
        if (IsActiveThreadTerminating()) return;
        if (seh_me.IsValid()) seh_me.Release();
        while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        if (!ParentClass.LeaderCutsceneComplete) {
            if (IsActiveThreadTerminating()) return;
            Game->FadeScreenOutUntilNextCallToFadeScreenIn(0.5f,0.0f);
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            CScriptGameResourceObjectScriptedThingBase seh_victim;
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> cast;
            Game->StartScriptingEntity(Game->GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
            Game->StartScriptingEntity(Game->GetThingWithScriptName("BH_Hostage2"),seh_victim,SCRIPT_AI_PRIORITY_HIGHEST);
            {
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                cast["Hero"]=seh_hero;
                cast["Victim"]=seh_victim;
                cast["Leader"]=seh_me;
                RunCutsceneMacro("CS_BOUNTYHUNT_GREATLAKEVICTIM",cast,0,0,false,true);
            }
            if (seh_hero.IsValid()) seh_hero.Release();
            if (seh_victim.IsValid()) seh_victim.Release();
            ParentClass.LeaderCutsceneComplete=true;
        }
        if (!ParentClass.SpawnLeaderGrunts) {
            if (IsActiveThreadTerminating()) return;
            ParentClass.SpawnBanditGangGrunts("M_BH_GruntSpawnLake");
            ParentClass.AddSpawnedFunction(new CSpawnedFunc<CQ_BountyHuntScript>(CCharString("ParentClass.") + CCharString("WatchForLeaderGruntsKilled"), 0, &ParentClass, &CQ_BountyHuntScript::WatchForLeaderGruntsKilled), CCharString(""));
            ParentClass.SpawnLeaderGrunts=true;
        }
        Game->SetThingHasInformation(Me,false,true,false);
        HealthCurrent=HealthInitial=Game->GetHealth(Me);
        CScriptThing hostage=Game->GetThingWithScriptName("BH_Hostage2");
        Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),false);
        CScriptThing approach1=Game->GetThingWithScriptName("M_BH_LeaderApproach1");
        CScriptThing approach2=Game->GetThingWithScriptName("M_BH_LeaderApproach2");
        CScriptThing money_bag=Game->GetThingWithScriptName("M_BH_MoneyBag");
        for (; !IsActiveThreadTerminating(); Game->NewScriptFrame()) {
            if ((Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO")))
                && !ParentClass.LeaderAttackingHero && !ParentClass.Hostage2Killed && !ParentClass.LeaderAttackingHostage) {
                if (IsActiveThreadTerminating()) return;
                if (Game->GetHealth(Me)>DELTA_NOTIONAL_ZERO) {
                    if (IsActiveThreadTerminating()) return;
                    if (!ParentClass.LeaderChargeComplete) {
                        if (IsActiveThreadTerminating()) return;
                        Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),false);
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                        if (seh_me.IsValid()) seh_me.Release();
                        while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return;
                        }
                        if (IsActiveThreadTerminating()) return;
                        CScriptGameResourceObjectMovieBase speech_movie;
                        Game->StartMovieSequence("",speech_movie);
                        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_017_LEADER6",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return;
                            }
                            if (IsActiveThreadTerminating()) return;
                        }
                        if (seh_me.IsValid()) seh_me.Release();
                        Game->GiveThingBestEnemyTarget(Me,Game->GetHero());
                        ParentClass.LeaderChargeComplete=true;
                    }
                }
                ParentClass.SpawnBanditGangGrunts("M_BH_GruntSpawnLeaderCharge");
                ParentClass.AddSpawnedFunction(new CSpawnedFunc<CQ_BountyHuntScript>(CCharString("ParentClass.") + CCharString("WatchForLeaderChargeGruntsKilled"), 0, &ParentClass, &CQ_BountyHuntScript::WatchForLeaderChargeGruntsKilled), CCharString(""));
                ParentClass.LeaderAttackingHero=true;
            }
            if (!ParentClass.LeaderAttackingHero && !ParentClass.LeaderIntro1Complete && IsDistanceBetweenThingsUnder(approach1,Game->GetHero(),3.0f)) {
                if (IsActiveThreadTerminating()) return;
                if (Game->GetHealth(Me)>DELTA_NOTIONAL_ZERO) {
                    if (IsActiveThreadTerminating()) return;
                    if (!ParentClass.Hostage2Killed) {
                        if (IsActiveThreadTerminating()) return;
                        Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),false);
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                        if (seh_me.IsValid()) seh_me.Release();
                        while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return;
                        }
                        if (IsActiveThreadTerminating()) return;
                        CScriptGameResourceObjectMovieBase speech_movie;
                        Game->StartMovieSequence("",speech_movie);
                        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_017_LEADER_DEMAND_2",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return;
                            }
                            if (IsActiveThreadTerminating()) return;
                        }
                    }
                    ParentClass.LeaderIntro1Complete=true;
                }
            }
            else if (!ParentClass.LeaderAttackingHero && !ParentClass.LeaderIntro2Complete && IsDistanceBetweenThingsUnder(approach2,Game->GetHero(),3.0f)) {
                if (IsActiveThreadTerminating()) return;
                if (Game->GetHealth(Me)>DELTA_NOTIONAL_ZERO) {
                    if (IsActiveThreadTerminating()) return;
                    if (!ParentClass.Hostage2Killed) {
                        if (IsActiveThreadTerminating()) return;
                        Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),false);
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                        if (seh_me.IsValid()) seh_me.Release();
                        while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return;
                        }
                        if (IsActiveThreadTerminating()) return;
                        CScriptGameResourceObjectMovieBase speech_movie;
                        Game->StartMovieSequence("",speech_movie);
                        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_017_LEADER_WARNING_3",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return;
                            }
                            if (IsActiveThreadTerminating()) return;
                        }
                    }
                    ParentClass.LeaderIntro2Complete=true;
                }
            }
            if (ParentClass.LeaderIntro2Complete && !ParentClass.Hostage2Killed && !ParentClass.LeaderAttackingHostage && !ParentClass.LeaderAttackingHero) {
                if (IsActiveThreadTerminating()) return;
                Game->EntitySetFacingAngleTowardsThing(Me,hostage,false);
                Game->ModifyThingHealth(hostage,1.0f-Game->GetHealth(hostage),false);
                if (!Game->IsObjectInThingsPossession(pGlobalScriptDef->BountyHuntRewardObject.GetString(),hostage)) {
                    if (IsActiveThreadTerminating()) return;
                    Game->AddItemToContainer(hostage,pGlobalScriptDef->BountyHuntRewardObject.GetString());
                }
                Game->EntityGiveGold(hostage,pGlobalScriptDef->BountyHuntGoldReward);
                Game->GiveThingBestEnemyTarget(Me,hostage);
                long conversation=Game->AddNewConversation(Me,false,false);
                Game->AddPersonToConversation(conversation,Game->GetHero());
                Game->AddLineToConversation(conversation,"TEXT_QST_017_LEADER7",false,Me,Game->GetHero());
                ParentClass.SpawnBanditGangGrunts("M_BH_GruntSpawnLeaderCharge");
                ParentClass.AddSpawnedFunction(new CSpawnedFunc<CQ_BountyHuntScript>(CCharString("ParentClass.") + CCharString("WatchForLeaderChargeGruntsKilled"), 0, &ParentClass, &CQ_BountyHuntScript::WatchForLeaderChargeGruntsKilled), CCharString(""));
                Game->SetScriptingStateGroup(Me,(EScriptingStateGroups)0);
                ParentClass.LeaderAttackingHostage=true;
                if (seh_me.IsValid()) seh_me.Release();
            }
            if (ParentClass.Hostage2Killed || !hostage.IsAlive()) {
                if (IsActiveThreadTerminating()) return;
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
                CScriptThing exit_marker=Game->GetThingWithScriptName("M_BH_Greatwood3Exit");
                if (seh_me.IsValid() && exit_marker.IsAlive()) {
                    if (IsActiveThreadTerminating()) return;
                    seh_me.MoveToPosition(exit_marker.GetPos(),1.0f,ENTITY_MOVE_RUN,false,true);
                    while (!IsDistanceBetweenThingsUnder(Me,exit_marker,2.0f)) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                    ParentClass.LeaderEscaped=true;
                    FadeOutAndKillEntity(Me,2.0f,true,true);
                    for (;;) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                }
            }
        }
    }
}
// Attempts: see STATUS.tsv / ATTEMPTS.log (best = probe/gl_a10.cpp)
