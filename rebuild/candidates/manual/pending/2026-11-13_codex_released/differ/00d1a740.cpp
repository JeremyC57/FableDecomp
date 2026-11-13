// NScript::CQ_BountyHuntScript::CBH_Hostage2::Main() -- the bounty hunt's second hostage (at Greatwood Lake): removed unless the intro is done; live while alive; wait for the bandit cut-scene; in the Greatwood entrance make her unkillable, walk to M_BH_FadeOut, record a teleport entry for the lake and fade out until the lake is loaded after the fishing; at the lake wander round M_BH_Hostage2Pos facing BH_GangLeader while he lives, and once the leader is dead or gone and his charge grunts are dead play CS_BOUNTYHUNT_GREATLAKEVICTIM_OUTRO, flag Hostage2Safe, pay the gold reward, hand the hero the reward object (taking it from her if she holds it), walk out through M_BH_Greatwood3Exit, give the renown, vanish and flag LakeComplete
// Grade: DIFFER(2662v2743) -- inline budget: the two cast-map assignments call the implicit STB operator= COMDAT 0x008ABD10 (retail expands it), the movie dtor is the COMDAT call, and the Greatwood-entrance wait keeps no post-loop terminate test; 167 lines
// Next idea: with a row-local __forceinline operator= (ruled out) and /d2inlS15 only 4 lines remain: retail keeps `call IsActiveThreadTerminating; test al,al; je` after the GreatwoodLake/FishComplete wait (0x00D1AA9F) with both targets the same teleport-dtor tail; for/break, !A||!B, TRetVoid and block scopes all fold it (d6-d11)
// Leaf: ?Main@CBH_Hostage2@CQ_BountyHuntScript@NScript@@EAEXXZ
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#define FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_GANGDEPUTY2
#define FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_HOSTAGE2
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE_CTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();                        // 0x006E7AB0
#include "engine/CScriptThing.h"
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
#include "stlport/fable_map.h"

#include "engine/CDefString.h"
#include "engine/fable_script_def_global.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_fade.h"
template <> std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>::map();                       // 0x00CDBF70
template <> std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>::~map();                      // 0x00CDBFB0
template <> CCountedPointer<CScriptGameResourceObjectScriptedThingBase>& CCountedPointer<CScriptGameResourceObjectScriptedThingBase>::operator=(const CCountedPointer<CScriptGameResourceObjectScriptedThingBase>&); // 0x008AB1E0
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
//enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
extern const float DELTA_NOTIONAL_ZERO;                                                                       // 0x0129BA3C (1e-4f)
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }

void NScript::CQ_BountyHuntScript::CBH_Hostage2::Main()
{
    if (!ParentClass.BountyHuntIntroComplete) {
        if (IsActiveThreadTerminating())
            return;
        Game->RemoveThing(Me, false, false);
        return;
    }
    {
        CIsThingAlive alive(Me);
        SetPredicate(alive);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!ParentClass.BanditCutsceneDone) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (Game->IsRegionLoaded("GreatwoodEntrance")) {
        if (IsActiveThreadTerminating())
            return;
        Game->EntitySetAsKillable(Me, false, true);
        if (seh_me.IsValid())
            seh_me.Release();
        while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        CScriptThing fade_out = Game->GetThingWithScriptName("M_BH_FadeOut");
        seh_me.MoveToPosition(fade_out.GetPos(), 4.0f, (EScriptEntityMoveType)1, false, false);
        while (!IsDistanceBetweenThingsUnder(Me, fade_out, 5.0f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        TeleportData teleport;
        Game->EntityGetAppearanceSeed(Me, teleport.appearanceSeed);
        teleport.defName = Me.GetDefName();
        teleport.health = Game->GetHealth(Me);
        teleport.scriptName = "BH_Hostage2";
        teleport.teleportTo = "GreatwoodLake";
        teleport.teleportMkr = "M_BH_Hostage2Pos";
        ParentClass.TeleportInfo.push_back(teleport);
        FadeOutAndKillEntity(Me, 1.0f, true, true);
        while (!(Game->IsRegionLoaded("GreatwoodLake") && ParentClass.FishComplete)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
    } else if (Game->IsRegionLoaded("GreatwoodLake")) {
        if (IsActiveThreadTerminating())
            return;
        Game->EntitySetAsKillable(Me, true, true);
        Game->SetIsThingLightningable(Me, false);
        CScriptThing hostage_pos = Game->GetThingWithScriptName("M_BH_Hostage2Pos");
        Game->SetWanderCentrePoint(Me, hostage_pos.GetPos());
        Game->SetWanderMinDistance(Me, 0.0f);
        Game->SetWanderMaxDistance(Me, pGlobalScriptDef->BountyHuntHostage2WanderDistance);
        Game->SetScriptingStateGroup(Me, (EScriptingStateGroups)4);
        Game->SetThingHasInformation(Me, false, true, false);
        CScriptThing leader = Game->GetThingWithScriptName("BH_GangLeader");
        if (IsActiveThreadTerminating())
            return;
        for (;;) {
            if (seh_me.IsValid())
                seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            if (leader.IsAlive() && !ParentClass.LeaderKilled) {
                if (IsActiveThreadTerminating())
                    return;
                Game->EntitySetFacingAngleTowardsThing(Me, leader, false);
            }
            if (ParentClass.AllLeaderChargeGruntsKilled && (ParentClass.LeaderKilled || ParentClass.LeaderEscaped)) {
                if (IsActiveThreadTerminating())
                    return;
                Game->EntitySetAsScared(Me, false);
                if (seh_me.IsValid())
                    seh_me.Release();
                while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
                {
                    CScriptGameResourceObjectScriptedThingBase seh_hero;
                    std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> mymap;
                    Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    mymap["Hero"] = seh_hero;
                    mymap["Victim"] = seh_me;
                    RunCutsceneMacro("CS_BOUNTYHUNT_GREATLAKEVICTIM_OUTRO", mymap, 0, 0, false, true);
                }
                ParentClass.Hostage2Safe = true;
                Game->GiveHeroGold(pGlobalScriptDef->BountyHuntGoldReward);
                if (!Game->IsObjectInThingsPossession(pGlobalScriptDef->BountyHuntRewardObject, Game->GetHero())) {
                    if (IsActiveThreadTerminating())
                        return;
                    if (Game->IsObjectInThingsPossession(pGlobalScriptDef->BountyHuntRewardObject, Me)) {
                        if (IsActiveThreadTerminating())
                            return;
                        Game->RemoveItemFromContainer(Me, pGlobalScriptDef->BountyHuntRewardObject);
                    }
                    Game->GiveHeroObject(pGlobalScriptDef->BountyHuntRewardObject, -1, false);
                }
                if (seh_me.IsValid())
                    seh_me.Release();
                while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
                CScriptThing greatwood_exit = Game->GetThingWithScriptName("M_BH_Greatwood3Exit");
                if (greatwood_exit.IsAlive()) {
                    if (IsActiveThreadTerminating())
                        return;
                    if (seh_me.IsValid()) {
                        if (IsActiveThreadTerminating())
                            return;
                        seh_me.MoveToPosition(greatwood_exit.GetPos(), 1.0f, (EScriptEntityMoveType)1, false, true);
                    }
                    while (!IsDistanceBetweenThingsUnder(Me, greatwood_exit, 2.0f)) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    if (IsActiveThreadTerminating())
                        return;
                    Game->GiveHeroRenownPoints(pGlobalScriptDef->BountyHuntRenownHostage2);
                    FadeOutAndKillEntity(Me, 1.0f, true, true);
                }
                ParentClass.LakeComplete = true;
            }
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
    }
}
