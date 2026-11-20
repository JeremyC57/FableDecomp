// NScript::CQ_BountyHuntScript::CBH_Hostage2::Main() -- the bounty hunt's second hostage (at Greatwood Lake): removed unless the intro is done; live while alive; wait for the bandit cut-scene; in the Greatwood entrance make her unkillable, walk to M_BH_FadeOut, record a teleport entry for the lake and fade out until the lake is loaded after the fishing; at the lake wander round M_BH_Hostage2Pos facing BH_GangLeader while he lives, and once the Leader is dead or gone and his charge grunts are dead play CS_BOUNTYHUNT_GREATLAKEVICTIM_OUTRO, flag Hostage2Safe, pay the gold reward, hand the hero the reward object (taking it from her if she holds it), walk out through M_BH_Greatwood3Exit, give the renown, vanish and flag LakeComplete
// Leaf: ?Main@CBH_Hostage2@CQ_BountyHuntScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D1A740, 2743 bytes, thiscall, plain `ret`, no EH frame.
// Identity: slot 1 (Main) of the CBH_Hostage2 entity vtable 0x012CA5A8 stored by the landed Alloc 0x00D16290; Ego_r.pdb / Ego_d.pdb /
//   FableWin.pdb ?Main@CBH_Hostage2@CQ_BountyHuntScript@NScript@@EAEXXZ. Sibling of the landed CBH_Hostage1::Main 0x00D19BD0 (same TU).
// Source shape: Ego_d.pdb line table (lines 1578..1712) and locals (sehMe, _Fail_Val; fadeOutMarker, tData in the GreatwoodEntrance
//   branch; teleportMkr, Leader, Exit, sehHero, speech_movie, myMap, PauseMyEntitiesPlease in the GreatwoodLake branch), the generated
//   quest-script text of the landed Mains: every wait is `while (!cond) { NewScriptFrame; terminate test }` plus the post-loop test.
// Inline-budget: flag /d2inlS15 (as the sibling 0x00D19BD0 and 0x00D17F20: S15 expands the implicit MovieBase dtor at the cutscene
//   block) + the header opt-in FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_FORCEINLINE_ASSIGN for the two cast-map
//   assignments (retail expands CBaseObject::operator= 0x0099A3B0 + CCountedPointer::operator= 0x008AB1E0; the implicit member stays the
//   COMDAT call 0x008ABD10 under every flag value, as the sibling 0x00D19BD0 records).
// Inferred: the post-loop terminate test of the GreatwoodLake/FishComplete wait is spelled with an empty then-arm
//   (`if (IsActiveThreadTerminating()) {} else return _Fail_Val.Return();`). Both arms leave the branch, and nothing follows it, so the
//   behaviour is the plain test's. Retail keeps `call 0x00F35B30; test al,al; lea ecx,[esp+78h]; je 0x00D1AA91` (0x00D1AAA1) with two
//   copies of the tData / fadeOutMarker exit tail. The plain test, `!T()`, a do/while(0) wrapper, `return; ` after it and an else-return
//   all fold the test away (2743v2743, 153 instructions differ); only an empty then-arm (`{}` or `;`) keeps it. Ego_d line 1619 (353
//   bytes) holds the whole wait, so the line table cannot separate the spellings.
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
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_FORCEINLINE_ASSIGN
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#define FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_GANGDEPUTY2
#define FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_HOSTAGE2
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE_CTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();                        // 0x006E7AB0
class CScriptGameResourceObjectScriptedThingBase;
template <> CCountedPointer<CScriptGameResourceObjectScriptedThingBase>& CCountedPointer<CScriptGameResourceObjectScriptedThingBase>::operator=(const CCountedPointer<CScriptGameResourceObjectScriptedThingBase>&); // 0x008AB1E0
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
#include "engine/TRetVoid.h"
template <> std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>::map();                       // 0x00CDBF70
template <> std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>::~map();                      // 0x00CDBFB0
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
//enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
extern const float DELTA_NOTIONAL_ZERO;                                                                       // 0x0129BA3C (1e-4f)
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }

void NScript::CQ_BountyHuntScript::CBH_Hostage2::Main()
{
    TRetVoid _Fail_Val;
    if (!ParentClass.BountyHuntIntroComplete) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        Game->RemoveThing(Me, false, false);
        return;
    }
    {
        CIsThingAlive alive(Me);
        SetPredicate(alive);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    while (!ParentClass.BanditCutsceneDone) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase sehMe;
    if (Game->IsRegionLoaded("GreatwoodEntrance")) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        Game->EntitySetAsKillable(Me, false, true);
        if (sehMe.IsValid())
            sehMe.Release();
        while (!Game->StartScriptingEntity(Me, sehMe, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        CScriptThing fadeOutMarker = Game->GetThingWithScriptName("M_BH_FadeOut");
        sehMe.MoveToPosition(fadeOutMarker.GetPos(), 4.0f, (EScriptEntityMoveType)1, false, false);
        while (!IsDistanceBetweenThingsUnder(Me, fadeOutMarker, 5.0f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        TeleportData tData;
        Game->EntityGetAppearanceSeed(Me, tData.appearanceSeed);
        tData.defName = Me.GetDefName();
        tData.health = Game->GetHealth(Me);
        tData.scriptName = "BH_Hostage2";
        tData.teleportTo = "GreatwoodLake";
        tData.teleportMkr = "M_BH_Hostage2Pos";
        ParentClass.TeleportInfo.push_back(tData);
        FadeOutAndKillEntity(Me, 1.0f, true, true);
        while (!(Game->IsRegionLoaded("GreatwoodLake") && ParentClass.FishComplete)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) {} else
            return _Fail_Val.Return();
    } else if (Game->IsRegionLoaded("GreatwoodLake")) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        Game->EntitySetAsKillable(Me, true, true);
        Game->SetIsThingLightningable(Me, false);
        CScriptThing teleportMkr = Game->GetThingWithScriptName("M_BH_Hostage2Pos");
        Game->SetWanderCentrePoint(Me, teleportMkr.GetPos());
        Game->SetWanderMinDistance(Me, 0.0f);
        Game->SetWanderMaxDistance(Me, pGlobalScriptDef->BountyHuntHostage2WanderDistance);
        Game->SetScriptingStateGroup(Me, (EScriptingStateGroups)4);
        Game->SetThingHasInformation(Me, false, true, false);
        CScriptThing Leader = Game->GetThingWithScriptName("BH_GangLeader");
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        for (;;) {
            if (sehMe.IsValid())
                sehMe.Release();
            while (!Game->StartScriptingEntity(Me, sehMe, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            if (Leader.IsAlive() && !ParentClass.LeaderKilled) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                Game->EntitySetFacingAngleTowardsThing(Me, Leader, false);
            }
            if (ParentClass.AllLeaderChargeGruntsKilled && (ParentClass.LeaderKilled || ParentClass.LeaderEscaped)) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                Game->EntitySetAsScared(Me, false);
                if (sehMe.IsValid())
                    sehMe.Release();
                while (!Game->StartScriptingEntity(Me, sehMe, SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                {
                    CScriptGameResourceObjectScriptedThingBase sehHero;
                    std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> myMap;
                    Game->StartScriptingEntity(Game->GetHero(), sehHero, SCRIPT_AI_PRIORITY_HIGHEST);
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    myMap["Hero"] = sehHero;
                    myMap["Victim"] = sehMe;
                    RunCutsceneMacro("CS_BOUNTYHUNT_GREATLAKEVICTIM_OUTRO", myMap, 0, 0, false, true);
                }
                ParentClass.Hostage2Safe = true;
                Game->GiveHeroGold(pGlobalScriptDef->BountyHuntGoldReward);
                if (!Game->IsObjectInThingsPossession(pGlobalScriptDef->BountyHuntRewardObject, Game->GetHero())) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    if (Game->IsObjectInThingsPossession(pGlobalScriptDef->BountyHuntRewardObject, Me)) {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        Game->RemoveItemFromContainer(Me, pGlobalScriptDef->BountyHuntRewardObject);
                    }
                    Game->GiveHeroObject(pGlobalScriptDef->BountyHuntRewardObject, -1, false);
                }
                if (sehMe.IsValid())
                    sehMe.Release();
                while (!Game->StartScriptingEntity(Me, sehMe, SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                CScriptThing Exit = Game->GetThingWithScriptName("M_BH_Greatwood3Exit");
                if (Exit.IsAlive()) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    if (sehMe.IsValid()) {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        sehMe.MoveToPosition(Exit.GetPos(), 1.0f, (EScriptEntityMoveType)1, false, true);
                    }
                    while (!IsDistanceBetweenThingsUnder(Me, Exit, 2.0f)) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    Game->GiveHeroRenownPoints(pGlobalScriptDef->BountyHuntRenownHostage2);
                    FadeOutAndKillEntity(Me, 1.0f, true, true);
                }
                ParentClass.LakeComplete = true;
            }
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
        }
    }
}