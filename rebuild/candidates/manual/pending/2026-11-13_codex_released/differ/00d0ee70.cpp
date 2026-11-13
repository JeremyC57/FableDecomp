// NScript::CQ_BanditCampBossBattleScript::CheckForFirstAreaMassacre() -- bandit-camp outer-area massacre watcher: waits for Gate1 to open, then every frame (after any BanditCampMain reload) tallies the kills reported by the hero, the hero's followers and the hero's summoned creatures; once BAC_EnemiesToKillInFirstArea is reached it plays CS_BANDITCAMP_ALARM_OUTER under a movie sequence with the hero scripted, removes Gate2Guard1, opens gate 2, sets the defensive guard bandits on the hero and spawns a level-2 bandit leader at DefensiveGuardLeader to attack the hero
// Grade: DIFFER(1991v1989) -- per-exit destructor expansion split (documented blocker family): retail calls ~CScriptThing 0x004AA840 for three of the four temporaries (Gate1's is expanded), calls ~vector<CScriptThing> 0x008AC970 at six exits and expands it at the second for-loop's two, and calls ~vector<unsigned long> 0x0045019A at two exits / expands two plus its COMDAT ctor 0x00450C72. Chunk 844 e1 (new, this source, md 466 from chunk 839's 640): declared-only `template<> std::vector<CScriptThing>::~vector(); template<> std::vector<unsigned long>::~vector();` make every vector dtor exit a call (2 bytes long); the remaining split is the two expanded ~vector<CScriptThing> exits of the second for-loop and the two expanded ~vector<unsigned long> exits (plus the allocator-argument ctor COMDAT).
// Next idea: VC7.1's inliner takes loop-nested sites first (chunk 844, 00e482f0); the two expanded ~vector<CScriptThing> exits are the second for-loop's, consistent with that order -- a plain-inline vector dtor whose cost puts exactly the loop exits under the budget would need the dtor body split by site (header opt-in), or an owner decision on the inline budget; the vector<unsigned long>(const allocator&) COMDAT 0x00450C72 needs the allocator-argument constructor declared only in fable_containers.h.
// Leaf: ?CheckForFirstAreaMassacre@CQ_BanditCampBossBattleScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D0EE70, 1989 bytes, thiscall, plain `ret`, no EH frame; frame 0x78, ebx/ebp/esi/edi at entry
//   (edi preloaded with the CScriptThing vtable 0x01238C8C for the expanded Gate1 temporary dtor, ebp the zero register).
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?CheckForFirstAreaMassacre@CQ_BanditCampBossBattleScript@NScript@@QAEXXZ
//   (engine/CQ_BanditCampBossBattleScript.h declares it); the landed Main 0x00D038E0 spawns it under the thread name
//   "CheckForFirstAreaMassacre", and the body reads pGlobalScriptDef->BAC_EnemiesToKillInFirstArea (+0xe78) and writes Gate2Open
//   (+0x4c) of the PDB layout (retail = PDB - 0x14).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0;
//   ~CScriptThing 0x004AA840 (called; the Gate1 temporary's dtor is expanded: vtable 0x01238C8C store, PImp release, ~CBaseClass
//   0x0099A2E0), free 0x00BFE9BC / 0x00BFEA14; vector<unsigned long>(const allocator&) 0x00450C72, vector<unsigned long>::erase(first,
//   last) 0x0046496F, ~vector<unsigned long> 0x0045019A (called twice, expanded twice); ~vector<CScriptThing> 0x008AC970 (called at six
//   exits, expanded at two); CBaseClass ctor 0x0099A380 (inside the expanded movie and scripted-thing ctors), ~CBaseObject 0x0099A430
//   (inside the expanded movie dtor), CCountedPointer<CScriptGameResourceObjectMovieBase> dtor 0x006E7AB0, ~CScriptGameResourceObject-
//   ScriptedThingBase 0x007E74D0, std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> ctor 0x00CDBF70 / dtor 0x00CDBFB0
//   (this TU's COMDAT copies), RunCutsceneMacro 0x00CBFB7D (__fastcall); Game (+0x40) slots 7 NewScriptFrame, 8 StartScriptingEntity,
//   11 IsLevelLoaded, 70 GetHero, 72 GetThingWithScriptName, 75 GetAllThingsWithScriptName, 91 CreateCreature, 108 RemoveThing, 231
//   GetFollowingEntityList, 232 GetHeroSummonedCreaturesList, 370 StartMovieSequence, 373 FadeScreenOut, 376 Pause, 379
//   PauseAllNonScriptedEntities (the inline CWideScreenMagicPauseEntities), 451 GiveThingBestEnemyTarget; CScriptThing slots 6 GetPos,
//   55 MsgGetThingsKilled, 64 IsOpenDoor (virtual through the temporaries / element references); pGlobalScriptDef 0x0143E90C.
// Literals: "Gate1" 0x012C86A4, "BanditCampMain" 0x012C8F78, "" 0x0122D70E, "CS_BANDITCAMP_ALARM_OUTER" 0x012C9AE8, "Gate2Guard1"
//   0x012C8A74, "DefensiveGuardBandit" 0x012C9AD0, "DefensiveGuardLeader" 0x012C9AB8, "CREATURE_BANDIT_LEADER_LEVEL2" 0x012C9A98.
// Layout (engine/CQ_BanditCampBossBattleScript.h, Ego_r.pdb - 0x14): Game +0x40, Gate2Open +0x4c; CScriptDef
//   BAC_EnemiesToKillInFirstArea +0xe78.
// Shape: the frame loop is `for (; !IsActiveThreadTerminating(); Game->NewScriptFrame())` (the loop-top thread check is the
//   condition, the frame step the increment, the kill vectors die before it); the massacre arm returns from inside it.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_STLP_MAP
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
template <> std::vector<unsigned long>::~vector();         // 0x0045019A (called at two exits)
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "stlport/fable_map.h"
template <> std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>::map();    // 0x00CDBF70: this TU calls the ctor COMDAT
template <> std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>::~map();   // 0x00CDBFB0: and the dtor COMDAT
#include "engine/CQ_BanditCampBossBattleScript.h"
#include "engine/fable_script_cutscene.h"
#include "engine/fable_script_def_global.h"

void NScript::CQ_BanditCampBossBattleScript::CheckForFirstAreaMassacre()
{
    while (!Game->GetThingWithScriptName("Gate1").IsOpenDoor()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    long killed = 0;
    for (; !IsActiveThreadTerminating(); Game->NewScriptFrame()) {
        if (Game->IsLevelLoaded("BanditCampMain")) {
            if (IsActiveThreadTerminating())
                return;
            while (Game->IsLevelLoaded("BanditCampMain")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
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
        if ((float)killed >= pGlobalScriptDef->BAC_EnemiesToKillInFirstArea) {
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
                RunCutsceneMacro("CS_BANDITCAMP_ALARM_OUTER", cast, 0, 0, false, true);
                Game->RemoveThing(Game->GetThingWithScriptName("Gate2Guard1"), false, true);
            }
            Gate2Open = true;
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
}
