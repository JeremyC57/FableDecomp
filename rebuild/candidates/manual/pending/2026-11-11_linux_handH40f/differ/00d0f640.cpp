// NScript::CQ_BanditCampBossBattleScript::CheckForSecondAreaMassacre() -- each frame add up the things the hero, his followers and his summoned creatures have killed; at the inner-area quota play the alarm cutscene, remove the gate-3 guard, open gate 3 and set the defensive bandits and a new level-2 bandit leader on the hero
// Grade: DIFFER(2131v2101) -- 108 masked lines at the true extent 2101 (229 before; chunk H40f, 2 grades here + 1 baseline). The call / expand split of ~vector<CScriptThing> and ~CScriptThing is now retail's at every site (inline trace: vector E C E E C E C Cx6 E, ~CScriptThing C E E): the uniform factor H40d asked for is three code-free folded operands -- main loop `for (; !T && true; NSF)` (0.5), `if (Gate3Open || false) break;` (fall-through 0.25 instead of 0.5), `if (Game->GetHero().MsgGetThingsKilled(kills) || false)` (join 0.625 instead of 0.75: x0.833) = 0.208 in front of loop 1, and `killed >= quota && true` for the massacre block (0.5 on top). Left, two causes: (1) in both follower loops `killed += kills.size(); kills.clear();` keeps killed in ebp (taken from the const 0 piece, `xor ebp,ebp` after the erase call) where retail adds through edx, stores [esp+10h] first and reloads kills._M_start (0x00D0F81C, 0x00D0F8EB); variant B without the `||` on the kills test has the same hunk, so it comes with the expanded vector dtors at the loop exits, not with the folded operand; (2) retail jumps from the four expanded loop exits to ONE kills-dtor + epilogue pad (`jmp 0x00D0FE42` at 0x00D0FA03 / FA37 / FA6B / FAA3, and `test eax,eax ; jmp 0x00D0FE48` at 0x00D0FE73), ours keeps a private copy of that pad at each (7 ret v 3)
// Next idea: c2prio --trace on this file (work/d0_E.prio: const 0 = `split: ebp #109, 1 piece immediate`, prio 58, allowed ebx only; killed = split, memory): find what keeps retail's const 0 piece whole over the two loops (then killed gets no register and the store / reload order follows); the unmerged pads are probably the same thing (the pads compare against ebp). The folded operands are a frequency model, not a claim about the source: a natural spelling with the same numbers (a real `&&` in the loop condition or the massacre test) would be better -- the sibling 0x00D0EE70 wants a different split from the same statements
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

#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "stlport/fable_map.h"

#include "engine/CQ_BanditCampBossBattleScript.h"
#include "engine/fable_script_cutscene.h"
#include "engine/fable_script_def_global.h"

void NScript::CQ_BanditCampBossBattleScript::CheckForSecondAreaMassacre()
{
    long killed = 0;
    for (; !IsActiveThreadTerminating() && true; Game->NewScriptFrame()) {
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
        if (Gate3Open || false)
            break;
        std::vector<unsigned long> kills;
        if (Game->GetHero().MsgGetThingsKilled(kills) || false) {
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
        if ((float)killed >= pGlobalScriptDef->BAC_EnemiesToKillInSecondArea && true) {
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
