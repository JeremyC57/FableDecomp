// NScript::CQ_BountyHuntScript::CBH_GangDeputy1::Main() -- the bounty hunt's first bandit deputy (the Fisherman's captor): removed unless the intro is done; live while alive; wait for the bandit cut-scene; in the Greatwood entrance walk to M_BH_FadeOut, record a teleport entry for the Fisherman region, fade out and idle; in the Fisherman region stand at M_BH_Deputy1Pos, ally with BH_Hostage1, then loop: if the hostage is dead or killed walk out through M_BH_FishermanExit and vanish (Deputy1Escaped); first time the hero comes near / is seen / hits him say TEXT_QST_017_DEPUTY1 under a paused cut-scene, spawn his grunts and the WatchForDeputy1GruntsKilled thread; attack the hero and wait for the hostage's death
// Grade: DIFFER(3035v3068) -- per-site MovieBase dtor: both speech-movie scope ends call the COMDAT 0x006E7B80 where retail expands it (0x006E7AB0 + GROBase vptr + ~CBaseObject 0x0099A430); 78 diff lines, all from that; the same source is RELOCATION_MATCH under the diagnostic /d2inlS15 (caller-size budget, GOTCHAS chunk 822)
// Next idea: none in source found after 19 grades (SAY do-while(0), heat on every speech site, arm swap, TRetVoid, dropping either forceinline ctor opt-in, removing code before or after the site all leave the call); a reduced body (x5) expands it, so it is the caller-size penalty: needs an owner decision on the inline budget, not a new spelling
// Leaf: ?Main@CBH_GangDeputy1@CQ_BountyHuntScript@NScript@@EAEXXZ
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
#define FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_GANGDEPUTY1
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

#include "engine/fable_script_def_global.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_fade.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
extern const float DELTA_NOTIONAL_ZERO;                                                                       // 0x0129BA3C (1e-4f)
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }

void NScript::CQ_BountyHuntScript::CBH_GangDeputy1::Main()
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
            teleport.scriptName = "BH_GangDeputy1";
            teleport.teleportTo = "Fisherman";
            teleport.teleportMkr = "M_BH_Deputy1Pos";
            ParentClass.TeleportInfo.push_back(teleport);
            FadeOutAndKillEntity(Me, 1.0f, true, true);
            while (!Game->IsRegionLoaded("Fisherman")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
    } else if (Game->IsRegionLoaded("Fisherman")) {
        if (IsActiveThreadTerminating())
            return;
        CScriptThing deputy_pos = Game->GetThingWithScriptName("M_BH_Deputy1Pos");
        Game->SetWanderCentrePoint(Me, deputy_pos.GetPos());
        Game->SetWanderMinDistance(Me, 0.0f);
        Game->SetWanderMaxDistance(Me, pGlobalScriptDef->BountyHuntBanditDeputy1WanderDistance);
        Game->SetScriptingStateGroup(Me, (EScriptingStateGroups)4);
        Game->SetThingHasInformation(Me, false, true, false);
        CScriptThing hostage = Game->GetThingWithScriptName("BH_Hostage1");
        Game->EntitySetThingAsAllyOfThing(Me, hostage);
        Game->EntitySetThingAsAllyOfThing(hostage, Me);
        if (IsActiveThreadTerminating())
            return;
        for (;;) {
            if (ParentClass.Hostage1Killed || !hostage.IsAlive()) {
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
                CScriptThing fisherman_exit = Game->GetThingWithScriptName("M_BH_FishermanExit");
                if (seh_me.IsValid() && fisherman_exit.IsAlive()) {
                    if (IsActiveThreadTerminating())
                        return;
                    seh_me.MoveToPosition(fisherman_exit.GetPos(), 1.0f, (EScriptEntityMoveType)1, false, false);
                    while (!IsDistanceBetweenThingsUnder(Me, fisherman_exit, 2.0f)) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    if (IsActiveThreadTerminating())
                        return;
                    ParentClass.Deputy1Escaped = true;
                    FadeOutAndKillEntity(Me, 1.0f, true, true);
                    for (;;) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return;
                    }
                }
            }
            if (!ParentClass.Deputy1IntroComplete) {
                if (IsActiveThreadTerminating())
                    return;
                while (!(IsDistanceBetweenThingsUnder(Me, Game->GetHero(), pGlobalScriptDef->BountyHuntBanditDeputy1AttackProximity) ||
                         Game->CanThingBe_Seen_ByOtherThing(Game->GetHero(), Me) || Me.MsgIsHitBy("SCRIPT_NAME_HERO") ||
                         (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO")) ||
                         ParentClass.Hostage1Killed)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
                if (Game->GetHealth(Me) > DELTA_NOTIONAL_ZERO) {
                    if (IsActiveThreadTerminating())
                        return;
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
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_017_DEPUTY1", (ETextGroupSelectionMethod)0, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return;
                        }
                        if (IsActiveThreadTerminating())
                            return;
                    }
                }
                ParentClass.Deputy1IntroComplete = true;
                if (!ParentClass.SpawnDeputy1Grunts) {
                    if (IsActiveThreadTerminating())
                        return;
                    ParentClass.SpawnBanditGangGrunts("M_BH_GruntSpawnFisher");
                    ParentClass.SpawnDeputy1Grunts = true;
                }
                ParentClass.AddSpawnedFunction(new CSpawnedFunc<CQ_BountyHuntScript>(CCharString("ParentClass.") + CCharString("WatchForDeputy1GruntsKilled"), 0, &ParentClass, &CQ_BountyHuntScript::WatchForDeputy1GruntsKilled), CCharString(""));
            }
            Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
            if (seh_me.IsValid())
                seh_me.Release();
            for (;;) {
                if (IsActiveThreadTerminating())
                    return;
                if (ParentClass.Hostage1Killed)
                    break;
                Game->NewScriptFrame();
            }
            if (IsActiveThreadTerminating())
                return;
            if (IsActiveThreadTerminating())
                return;
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
    }
    for (;;) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
}
