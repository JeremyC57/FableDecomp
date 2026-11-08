// NScript::CQ_BountyHuntScript::CBH_Hostage1::Main() -- the bounty hunt's first hostage (the Fisherman): removed unless the intro is done; live while alive; wait for the bandit cut-scene; in the Greatwood entrance make him unkillable, walk to M_BH_FadeOut, record a teleport entry for the Fisherman region and fade out; in the Fisherman region play BOUNTY_HUNT_FISHERMAN_VICTIM (Hero / Victim / Bandit cast), then face BH_GangDeputy1 while it lives and, once the deputy and his grunts are dead, play CS_BOUNTYHUNT_FISHERMANVICTIM_OUTRO, set objective 03, flag Hostage1Safe, walk out through M_BH_FishermanExit, vanish and give the hero his renown
// Inline-budget: flag /d2inlS15 + row-local __forceinline CScriptGameResourceObjectScriptedThingBase::operator= -- S15 expands the implicit MovieBase dtor sites (caller IL-size cap, as the sibling 0x00D17F20); the handle's implicit operator= at the five cast-map assignments (CBaseObject::operator= 0x0099A3B0 + CCountedPointer::operator= 0x008AB1E0, expanded by retail) stays the COMDAT call 0x008ABD10 under every flag value tried (S15..S30, T60..T150, S15+T60..T150), so its definition is forced inline under _MSC_VER
// Leaf: ?Main@CBH_Hostage1@CQ_BountyHuntScript@NScript@@EAEXXZ
// Flags: /d2inlS15
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
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#define FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_GANGDEPUTY2
#define FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_HOSTAGE1
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

#include "engine/fable_script_def_global.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_fade.h"
template <> std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>::map();                       // 0x00CDBF70
template <> std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>::~map();                      // 0x00CDBFB0
template <> CCountedPointer<CScriptGameResourceObjectScriptedThingBase>& CCountedPointer<CScriptGameResourceObjectScriptedThingBase>::operator=(const CCountedPointer<CScriptGameResourceObjectScriptedThingBase>&); // 0x008AB1E0
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
//enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
extern const float DELTA_NOTIONAL_ZERO;                                                                       // 0x0129BA3C (1e-4f)
// Inline control (owner decision 2026-11-08, device 3): retail expands the handle's implicit operator= (CBaseObject::operator=
// 0x0099A3B0 + CCountedPointer<...>::operator= 0x008AB1E0) at the five cast-map assignments; at this caller size VC7.1 calls the
// COMDAT 0x008ABD10 under every /d2inlS / /d2inlT value tried (S15..S30, T60..T150), so the row-local definition is forced inline.
#ifdef _MSC_VER
#define FABLE_INLINE_BUDGET_FORCEINLINE __forceinline
#else
#define FABLE_INLINE_BUDGET_FORCEINLINE inline
#endif
FABLE_INLINE_BUDGET_FORCEINLINE CScriptGameResourceObjectScriptedThingBase& CScriptGameResourceObjectScriptedThingBase::operator=(const CScriptGameResourceObjectScriptedThingBase& other)
{
    CScriptGameResourceObjectBase::operator=(other);
    PImp = other.PImp;
    return *this;
}
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }

void NScript::CQ_BountyHuntScript::CBH_Hostage1::Main()
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
        teleport.scriptName = "BH_Hostage1";
        teleport.teleportTo = "Fisherman";
        teleport.teleportMkr = "M_BH_Hostage1Pos";
        ParentClass.TeleportInfo.push_back(teleport);
        FadeOutAndKillEntity(Me, 1.0f, true, true);
        while (!Game->IsRegionLoaded("Fisherman")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
    } else if (Game->IsRegionLoaded("Fisherman")) {
        if (IsActiveThreadTerminating())
            return;
        Game->FadeScreenOutUntilNextCallToFadeScreenIn(0.5f, 0.0f);
        Game->EntitySetAsKillable(Me, true, true);
        Game->SetIsThingLightningable(Me, false);
        CScriptThing deputy = Game->GetThingWithScriptName("BH_GangDeputy1");
        Game->SetThingHasInformation(Me, false, true, false);
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
            std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> mymap;
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            CScriptGameResourceObjectScriptedThingBase seh_bandit;
            Game->StartScriptingEntity(deputy, seh_bandit, SCRIPT_AI_PRIORITY_HIGHEST);
            Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
            mymap["Hero"] = seh_hero;
            mymap["Victim"] = seh_me;
            mymap["Bandit"] = seh_bandit;
            {
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                RunCutsceneMacro("BOUNTY_HUNT_FISHERMAN_VICTIM", mymap, 0, 0, false, true);
            }
            if (seh_hero.IsValid())
                seh_hero.Release();
            if (seh_me.IsValid())
                seh_me.Release();
            if (seh_bandit.IsValid())
                seh_bandit.Release();
        }
        if (IsActiveThreadTerminating())
            return;
        for (;;) {
            if (deputy.IsAlive() && !ParentClass.Deputy1Killed) {
                if (IsActiveThreadTerminating())
                    return;
                Game->EntitySetFacingAngleTowardsThing(Me, deputy, false);
            }
            if (ParentClass.Deputy1Killed && ParentClass.AllDeputy1GruntsKilled) {
                if (IsActiveThreadTerminating())
                    return;
                Game->EntitySetAsScared(Me, false);
                ParentClass.FishComplete = true;
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
                    Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
                    std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> mymap;
                    mymap["Hero"] = seh_hero;
                    mymap["Victim"] = seh_me;
                    {
                        CScriptGameResourceObjectMovieBase speech_movie;
                        Game->StartMovieSequence("", speech_movie);
                        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                        RunCutsceneMacro("CS_BOUNTYHUNT_FISHERMANVICTIM_OUTRO", mymap, 0, 0, false, true);
                    }
                    if (seh_me.IsValid())
                        seh_me.Release();
                    if (seh_hero.IsValid())
                        seh_hero.Release();
                }
                Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_BOUNTY_HUNT_OBJECTIVE_03", "GreatwoodLake", "Greatwood");
                ParentClass.Hostage1Safe = true;
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
                if (fisherman_exit.IsAlive()) {
                    if (IsActiveThreadTerminating())
                        return;
                    if (seh_me.IsValid()) {
                        if (IsActiveThreadTerminating())
                            return;
                        seh_me.MoveToPosition(fisherman_exit.GetPos(), 1.0f, (EScriptEntityMoveType)1, false, true);
                    }
                    while (!IsDistanceBetweenThingsUnder(Me, fisherman_exit, 2.0f)) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    if (IsActiveThreadTerminating())
                        return;
                    FadeOutAndKillEntity(Me, 1.0f, true, true);
                    Game->GiveHeroRenownPoints(pGlobalScriptDef->BountyHuntRenownHostage1);
                }
            }
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
    }
}
