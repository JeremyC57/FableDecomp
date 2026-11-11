// NScript::CQ_BanditCampBossBattleScript::CheckForSecondAreaMassacre() -- each frame add up the things the hero, his followers and his summoned creatures have killed; at the inner-area quota play the alarm cutscene, remove the gate-3 guard, open gate 3 and set the defensive bandits and a new level-2 bandit leader on the hero
// Grade: DIFFER(1945v2101) -- 229 masked lines at the true extent 2101 (SPLITS.tsv), unchanged in chunk H40d (no grade spent; 9 inline-trace probes, tools/iv.sh, 22 s each). The residue is the per-site call / expand split of ~vector<CScriptThing> and ~CScriptThing; this draft still declares the vector dtor only (all sites called). H40d solved the split as a constraint system on c2's static site frequencies (thresholds read from the trace: ~vector<CScriptThing> c1 44 is called below f 46.4, ~CScriptThing c1 60 below f 38.1; f<100: 7*score = 380 + 6400/f resp. 361.7 + 5960/f): retail's set (expand loop-1 / loop-2 thread exits, the two after-loop exits and the loop end; call the two nested exits, the massacre entry, all six massacre exits and the Gate3Guard temporary; expand the leader temporaries) has ONE solution over for / while loop forms and branch probabilities -- all three inner loops `for`, the massacre block reached with probability 0.25 (ours 0.5) and the flow in front of loop 1 between 19.5 and 23.8 (ours 105.5, i.e. the main loop header at 100 instead of 500)
// Next idea: find the source shape that gives those two numbers (probe with tools/iv.sh + tools/inlq.py on a copy WITHOUT the `template <> std::vector<CScriptThing>::~vector();` line; wanted: vector sites EXP CALL EXP EXP CALL EXP CALL CALLx6 EXP, ~CScriptThing CALL EXP EXP). Measured without effect on any frequency: for (;; NSF) { if (T) return; }, for (; !T; NSF), while (!T) { ...; NSF; }, the guarded do-while, `if (Gate3Open) { if (T) return; return; }` instead of break + trailing check, the massacre test as `else if` of the thread check. Measured with effect: while (true) + bottom check halves everything (header 250); no loop at all gives a tenth (too cold). The massacre 0.25 is what `a && b` gives when b folds away (GOTCHAS chunk 890 s6). The sibling 0x00D0EE70 CheckForFirstAreaMassacre (parked, same blocker) shows a different retail split for the same statements (loop-1 exits and the loop end CALLED, even the 8-cost ~vector<unsigned long> and the vector ctor 0x00450C72 called at the early sites), so the two functions' frequencies differ at the top of the loop body: solve both together. A device version (in-body inline_depth(0) at the nine called returns) cannot keep ~vector<unsigned long> expanded at the same returns (retail 0x00D0FE42), so it is not an option here
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
// Shape: the cast map's ctor expands (malloc'd header) while its dtor is the COMDAT (FABLE_RETAIL_STLP_MAP_OUTOFLINE_DTOR, overlay);
//   `!IsLevelLoaded` gives retail's neg/sbb/inc. Without a per-site inline lever the split above is out of reach.
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
inline CScriptThing::~CScriptThing() { }
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0, called inside the expanded movie dtor
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CRGBColour.h"
#include "engine/C3DVector.h"
#include "engine/CGameScriptInterfaceBase.h"
template <> std::vector<CScriptThing>::~vector();          // 0x008AC970 (called at six exits)
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "stlport/fable_map.h"

#include "engine/CQ_BanditCampBossBattleScript.h"
#include "engine/fable_script_cutscene.h"
#include "engine/fable_script_def_global.h"

void NScript::CQ_BanditCampBossBattleScript::CheckForSecondAreaMassacre()
{
    long killed = 0;
    for (;; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating())
            return;
        if (!Game->IsLevelLoaded("BanditCampResidential")) {
            if (IsActiveThreadTerminating())
                return;
            while (!Game->IsLevelLoaded("BanditCampResidential")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
        if (Gate3Open)
            break;
        std::vector<unsigned long> kills;
        if (Game->GetHero().MsgGetThingsKilled(kills)) {
            if (IsActiveThreadTerminating())
                return;
            killed += kills.size();
            kills.clear();
        }
        std::vector<CScriptThing> things;
        Game->GetFollowingEntityList(Game->GetHero(), things);
        for (unsigned long i = 0; i < things.size(); i++) {
            if (IsActiveThreadTerminating())
                return;
            if (things[i].MsgGetThingsKilled(kills)) {
                if (IsActiveThreadTerminating())
                    return;
                killed += kills.size();
                kills.clear();
            }
        }
        if (IsActiveThreadTerminating())
            return;
        Game->GetHeroSummonedCreaturesList(things);
        for (unsigned long i = 0; i < things.size(); i++) {
            if (IsActiveThreadTerminating())
                return;
            if (things[i].MsgGetThingsKilled(kills)) {
                if (IsActiveThreadTerminating())
                    return;
                killed += kills.size();
                kills.clear();
            }
        }
        if (IsActiveThreadTerminating())
            return;
        if ((float)killed >= pGlobalScriptDef->BAC_EnemiesToKillInSecondArea) {
            if (IsActiveThreadTerminating())
                return;
            {
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("", movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                CScriptGameResourceObjectScriptedThingBase seh_Hero;
                Game->StartScriptingEntity(Game->GetHero(), seh_Hero, (EScriptAIPriority)4);
                Game->FadeScreenOut(0.5f, 0.5f, CRGBColour(0, 0, 0, 255));
                Game->Pause(1.0f);
                std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast;
                RunCutsceneMacro("CS_BANDITCAMP_ALARM_INNER", cast, 0, 0, false, true);
                Game->RemoveThing(Game->GetThingWithScriptName("Gate3Guard"), false, true);
            }
            Gate3Open = true;
            std::vector<CScriptThing> bandits;
            Game->GetAllThingsWithScriptName("DefensiveGuardBandit", bandits);
            for (unsigned long i = 0; i < bandits.size(); i++) {
                if (IsActiveThreadTerminating())
                    return;
                Game->GiveThingBestEnemyTarget(bandits[i], Game->GetHero());
            }
            if (IsActiveThreadTerminating())
                return;
            CScriptThing leader = Game->CreateCreature("CREATURE_BANDIT_LEADER_LEVEL2", Game->GetThingWithScriptName("DefensiveGuardLeader").GetPos(), "", false);
            Game->GiveThingBestEnemyTarget(leader, Game->GetHero());
            return;
        }
    }
    if (IsActiveThreadTerminating())
        return;
}
