// NScript::CQ_BanditCampBossBattleScript::CheckForSecondAreaMassacre() -- each frame add up the things the hero, his followers and his summoned creatures have killed; at the inner-area quota play the alarm cutscene, remove the gate-3 guard, open gate 3 and set the defensive bandits and a new level-2 bandit leader on the hero
// Inline-budget: /d2inlT59 (per-row flag) + FABLE_FORCEINLINE on the row-local ~CScriptThing + `#pragma inline_depth(0)` round the Gate3Guard RemoveThing statement -- pure inline-decision residue (inline trace, tools/inl.sh): at the default threshold the two T exits that share retail's chain 0x00D0FE39 (summoned-loop inner T 0x00D0F8DA, cut-scene entry T 0x00D0FAAA, both f=0.824, ~vector<CScriptThing> sc 65) and the cast-map dtor (sc 61) expand where retail calls them; T59 is the smallest N that matches (T58 calls the followers-loop exits, sc 58, which retail expands; T62 expands the map dtor). ~CScriptThing is expanded at both leader sites (retail 0x00D0FDxx / 0x00D0FE1F) where f=0.309 scores it above any usable N, and called once in the cut scene (0x00D0FC0B), hence forceinline + the one inline_depth(0) statement.
// FITTED: the bandit loop as `unsigned long i = 0; while (i < AllBandits.size()) { ...; i++; }` (same rotated code as the `for`; the `for` spelling raises the leader-block site frequency 2.5x and expands its vector dtors). EVIDENCED: main loop `for (; !false; Game->NewScriptFrame()) { if (T) return; ...}` (sibling CBanditKing::Main 0x00D0A830; retail T before the loop 0x00D0F64F and after the NSF 0x00D0F9AA, condition exit returns without a second T), the post-loop T whose result is unused (0x00D0F9C1). Locals carry the Anniversary PDB names (followers, KilledCreatures, AllBandits, NewLeader, speech_movie, seh_hero, mymap).
// Flags: /d2inlT59
// Leaf: ?CheckForSecondAreaMassacre@CQ_BanditCampBossBattleScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D0F640, 2101 bytes, thiscall, no EH frame; referenced once (0x00D04165, the spawned
//   thread set up in CQ_BanditCampBossBattleScript::Main); an unlisted function starts at 0x00D0FE80 after 11 filler bytes.
// Identity (proven): Ego_r.pdb 0x00D13E10 / FableWin.pdb 0x0368B050 ?CheckForSecondAreaMassacre@CQ_BanditCampBossBattleScript@NScript@@QAEXXZ;
//   the body tests Gate3Open (+0x4D, set before the leader spawns), reads pGlobalScriptDef->BAC_EnemiesToKillInSecondArea (+0xE7C)
//   and plays CS_BANDITCAMP_ALARM_INNER (0x012C9B04); the older scan name Quest009_BanditCamp_Residential_OnLoad is wrong.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString 0x0099EBF0 / ~ 0x0099EAE0, vector<unsigned long>::erase
//   0x0046496F, ~vector<CScriptThing> 0x008AC970, ~CScriptThing 0x004AA840, CBaseObject 0x0099A380 / ~ 0x0099A430, ~CBaseClass
//   0x0099A2E0, RunCutsceneMacro 0x00CBFB7D, cast map dtor 0x00CDBFB0, ~CScriptGameResourceObjectScriptedThingBase 0x007E74D0,
//   movie PImp ~CCountedPointer 0x006E7AB0, malloc 0x00BFEA0E / free 0x00BFEA14 / operator delete 0x00BFE9BC; CGameScriptInterface
//   slots 7 NewScriptFrame, 8 StartScriptingEntity, 11 IsLevelLoaded, 70 GetHero, 72 GetThingWithScriptName, 75
//   GetAllThingsWithScriptName, 91 CreateCreature, 108 RemoveThing, 231 GetFollowingEntityList, 232 GetHeroSummonedCreaturesList,
//   370 StartMovieSequence, 373 FadeScreenOut, 376 Pause, 379 (the pause-entities dtor), 451 GiveThingBestEnemyTarget; CScriptThing
//   slots 6 GetPos, 55 MsgGetThingsKilled.
// Data: "BanditCampResidential" 0x012C8B5C, "Gate3Guard" 0x012C8A1C, "DefensiveGuardBandit" 0x012C9AD0, "DefensiveGuardLeader"
//   0x012C9AB8, "CREATURE_BANDIT_LEADER_LEVEL2" 0x012C9A98, "" 0x0122D70E.
// Shape: the cast map's ctor expands (malloc'd header) and its dtor is called (/d2inlT59; FABLE_RETAIL_STLP_MAP_OUTOFLINE_DTOR keeps the tree erase out of line);
//   `!IsLevelLoaded` gives retail's neg/sbb/inc.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_OUTOFLINE_DTOR
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
FABLE_FORCEINLINE CScriptThing::~CScriptThing() { }
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0, called inside the expanded movie dtor
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CRGBColour.h"
#include "engine/C3DVector.h"
#include "engine/CGameScriptInterfaceBase.h"

#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "stlport/fable_map.h"

#include "engine/CQ_BanditCampBossBattleScript.h"
#include "engine/fable_script_cutscene.h"
#include "engine/fable_script_def_global.h"
#include "engine/TRetVoid.h"

void NScript::CQ_BanditCampBossBattleScript::CheckForSecondAreaMassacre()
{
    TRetVoid _Fail_Val;
    long Killed = 0;
    for (; !false; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (!Game->IsLevelLoaded("BanditCampResidential")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            while (!Game->IsLevelLoaded("BanditCampResidential")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (Gate3Open) break;
        std::vector<unsigned long> KilledCreatures;
        if (Game->GetHero().MsgGetThingsKilled(KilledCreatures)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Killed += KilledCreatures.size();
            KilledCreatures.clear();
        }
        std::vector<CScriptThing> followers;
        Game->GetFollowingEntityList(Game->GetHero(), followers);
        for (unsigned long i = 0; i < followers.size(); i++) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (followers[i].MsgGetThingsKilled(KilledCreatures)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Killed += KilledCreatures.size();
                KilledCreatures.clear();
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->GetHeroSummonedCreaturesList(followers);
        for (unsigned long i = 0; i < followers.size(); i++) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (followers[i].MsgGetThingsKilled(KilledCreatures)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Killed += KilledCreatures.size();
                KilledCreatures.clear();
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if ((float)Killed >= pGlobalScriptDef->BAC_EnemiesToKillInSecondArea) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            {
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                CScriptGameResourceObjectScriptedThingBase seh_hero;
                Game->StartScriptingEntity(Game->GetHero(), seh_hero, (EScriptAIPriority)4);
                Game->FadeScreenOut(0.5f, 0.5f, CRGBColour(0, 0, 0, 255));
                Game->Pause(1.0f);
                std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> mymap;
                RunCutsceneMacro("CS_BANDITCAMP_ALARM_INNER", mymap, 0, 0, false, true);
#pragma inline_depth(0)
                Game->RemoveThing(Game->GetThingWithScriptName("Gate3Guard"), false, true);
#pragma inline_depth()
            }
            Gate3Open = true;
            std::vector<CScriptThing> AllBandits;
            Game->GetAllThingsWithScriptName("DefensiveGuardBandit", AllBandits);
            unsigned long i = 0;
            while (i < AllBandits.size()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->GiveThingBestEnemyTarget(AllBandits[i], Game->GetHero());
                i++;
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptThing NewLeader = Game->CreateCreature("CREATURE_BANDIT_LEADER_LEVEL2", Game->GetThingWithScriptName("DefensiveGuardLeader").GetPos(), "", false);
            Game->GiveThingBestEnemyTarget(NewLeader, Game->GetHero());
            return _Fail_Val.Return();
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}