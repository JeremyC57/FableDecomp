// NScript::CQ_BountyHuntScript::CBH_GangDeputy2::Main() -- the bounty hunt's second bandit deputy: removed unless the intro is done; live while alive; wait for the bandit cut-scene; in the Greatwood entrance before the fishing is done walk to M_BH_FadeOut, record a teleport entry for the lake, fade out and wait for the lake; once the fishing is done stand at M_BH_Deputy2Pos, wander there, spawn his grunts once (fewer than four about) with the WatchForDeputy2GruntsKilled thread, wait until his grunts are dead / the hero hits him / comes near, follow the hero until in attack range or hit (and still healthy), then (first time) stop, face the hero, say TEXT_QST_017_DEPUTY2 under a paused cut-scene and set objective 04, and attack the hero
// Grade: DIFFER(3337v3321) -- the two MovieBase dtor sites only (0x00D1995D, 0x00D19A44): retail expands the compiler-generated dtor as `call 0x006E7AB0` (PImp) ; `mov [esp+0x50],0x0126008C` (GROBase vtable) ; `call 0x0099A430`; this source's user-declared __forceinline empty dtor (opt-in FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE) adds a MovieBase vtable store before the PImp release at each site (2 x 8 bytes); every other byte, the frame and all relocations match
// Next idea: documented caller-size gap (GOTCHAS chunks 804/805/821/822: the implicit dtor expands only under the diagnostic /d2inlS15). Chunk 832: the implicit-dtor model (FORCEINLINE_CTOR + declared-only PImp dtor) regrades DIFFER(3287v3321) 6 lines (both sites call the COMDAT 0x006E7B80); an explicit CCharString("") movie name moves the frame (96); the dtor spec after the header is C2908. Park until the lane has a caller-size answer
// Leaf: ?Main@CBH_GangDeputy2@CQ_BountyHuntScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D18DF0, 3321 bytes, thiscall, plain `ret` (many epilogue copies), no EH frame; ends at 0x00D19AE9.
// Identity (proven): Main slot of the CBH_GangDeputy2 entity vtable stored by the landed CBH_GangDeputy2::Alloc 0x00D161A0 (its Init
//   0x00D169E0 / OnPersist 0x00D16250 / OnPredicateFail 0x00D16260 / GetParentScript 0x00D16240 are landed); Ego_r.pdb / FableWin.pdb
//   name set ?Main@CBH_GangDeputy2@CQ_BountyHuntScript@NScript@@EAEXXZ; "BH_GangDeputy2" is its binding / script name.
// Callees: CIsThingAlive (vtable 0x012C32F0, inline) + CScriptThing copy 0x004ABE90 / ~ 0x004AA840, SetPredicate 0x00F35B10,
//   IsActiveThreadTerminating 0x00F35B30, CBaseObject 0x0099A380 / ~ 0x0099A430, ScriptedThingBase dtor 0x007E74D0 / IsValid 0x00CD23B9 /
//   Release 0x00CD2770 / MoveToPosition 0x007E72F0 / FollowThing 0x007E7320 / StopFollowingThing 0x007E7330 / GetScriptThing 0x007E7490 /
//   Speak 0x007E7390 / IsPerformingScriptTask 0x007E7450, movie PImp dtor 0x006E7AB0 (inline movie ctor / dtor), CCharString 0x0099EBF0 /
//   ~ 0x0099EAE0 / operator=(const CCharString&) 0x0099EFB0 / operator=(const char*) 0x0099EFE0, operator+ 0x0099F570, TeleportData ctor
//   0x00D15140 / dtor 0x00CD4550, vector<TeleportData>::push_back 0x00D18C00, ~vector<CScriptThing> 0x008AC970, SpawnBanditGangGrunts
//   0x00D17470, operator new 0x00BFEA1A, CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc vtable 0x012C9EDC, WatchForDeputy2GruntsKilled
//   0x00D19AF0), AddSpawnedFunction 0x00CB7E50, IsDistanceBetweenThingsUnder 0x00CBE2FF, FadeOutAndKillEntity 0x00CBE716; pGlobalScriptDef
//   0x0143E90C (BountyHuntBanditDeputy2WanderDistance +0xB34 / AttackProximity +0xB38), DELTA_NOTIONAL_ZERO 0x0129BA3C; interface slots
//   7, 8, 12 IsRegionLoaded, 70, 72, 75, 108 RemoveThing, 264 GetHealth, 296 SetQuestCardObjective, 360 SetThingHasInformation, 370, 379,
//   451 GiveThingBestEnemyTarget, 475, 483, 655, 763 SetWanderCentrePoint, 766 SetWanderMinDistance, 769 SetWanderMaxDistance, 784
//   SetScriptingStateGroup; CScriptThing slots 2 GetDefName, 6 GetPos, 21, 41, 42.
// Layout: engine/CQ_BountyHuntScript.h (opt-in FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_GANGDEPUTY2): ParentClass Deputy2IntroComplete +0x53,
//   Hostage2Killed +0x55, SpawnDeputy2Grunts +0x5b, AllDeputy2GruntsKilled +0x5e, FishComplete +0x62, BanditCutsceneDone +0x6f,
//   BountyHuntIntroComplete +0x70, TeleportInfo +0x88.
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
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE
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

#include "engine/fable_script_def_global.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_fade.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
extern const float DELTA_NOTIONAL_ZERO;                                                                       // 0x0129BA3C (1e-4f)
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }
template <> std::vector<CScriptThing, std::allocator<CScriptThing> >::~vector();                            // 0x008AC970

void NScript::CQ_BountyHuntScript::CBH_GangDeputy2::Main()
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
        if (!ParentClass.FishComplete) {
            if (IsActiveThreadTerminating())
                return;
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
            teleport.scriptName = "BH_GangDeputy2";
            teleport.teleportTo = "GreatwoodEntrance";
            teleport.teleportMkr = "M_BH_Deputy2Pos";
            ParentClass.TeleportInfo.push_back(teleport);
            FadeOutAndKillEntity(Me, 1.0f, true, true);
            while (!(Game->IsRegionLoaded("GreatwoodLake") && ParentClass.FishComplete)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        } else {
            if (IsActiveThreadTerminating())
                return;
            CScriptThing deputy_pos = Game->GetThingWithScriptName("M_BH_Deputy2Pos");
            Game->SetWanderCentrePoint(Me, deputy_pos.GetPos());
            Game->SetWanderMinDistance(Me, 0.0f);
            Game->SetWanderMaxDistance(Me, pGlobalScriptDef->BountyHuntBanditDeputy2WanderDistance);
            Game->SetScriptingStateGroup(Me, (EScriptingStateGroups)4);
            Game->SetThingHasInformation(Me, false, true, false);
            if (!ParentClass.SpawnDeputy2Grunts) {
                if (IsActiveThreadTerminating())
                    return;
                std::vector<CScriptThing> grunts;
                long grunt_count = Game->GetAllThingsWithScriptName("BH_GangGruntSpawn", grunts);
                grunt_count += Game->GetAllThingsWithScriptName("BH_GangGrunt", grunts);
                if (grunt_count < 4) {
                    if (IsActiveThreadTerminating())
                        return;
                    ParentClass.SpawnBanditGangGrunts("M_BH_GruntSpawnEntrance2");
                    ParentClass.AddSpawnedFunction(new CSpawnedFunc<CQ_BountyHuntScript>(CCharString("ParentClass.") + CCharString("WatchForDeputy2GruntsKilled"), 0, &ParentClass, &CQ_BountyHuntScript::WatchForDeputy2GruntsKilled), CCharString(""));
                }
                ParentClass.SpawnDeputy2Grunts = true;
            }
            while (!(ParentClass.AllDeputy2GruntsKilled || Me.MsgIsHitBy("SCRIPT_NAME_HERO") ||
                     (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO")) ||
                     IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 5.0f))) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            if (seh_me.IsValid())
                seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            seh_me.FollowThing(Game->GetHero(), 1.0f, true);
            if (IsActiveThreadTerminating())
                return;
            for (;;) {
                if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), pGlobalScriptDef->BountyHuntBanditDeputy2AttackProximity) || Me.MsgIsHitBy("SCRIPT_NAME_HERO") ||
                    (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO")) ||
                    ParentClass.Hostage2Killed) {
                    if (IsActiveThreadTerminating())
                        return;
                    if (Game->GetHealth(Me) > DELTA_NOTIONAL_ZERO)
                        break;
                }
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            if (!ParentClass.Deputy2IntroComplete) {
                if (IsActiveThreadTerminating())
                    return;
                seh_me.StopFollowingThing(Game->GetHero());
                Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
                if (seh_me.IsValid())
                    seh_me.Release();
                while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
                if (seh_me.IsValid())
                    seh_me.Release();
                while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_017_DEPUTY2", (ETextGroupSelectionMethod)0, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (seh_me.IsValid())
                    seh_me.Release();
                Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_BOUNTY_HUNT_OBJECTIVE_04", "GreatwoodLake", "Greatwood");
            }
            Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
            for (;;) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
        }
    } else if (Game->IsRegionLoaded("GreatwoodLake")) {
        if (IsActiveThreadTerminating())
            return;
    }
}
