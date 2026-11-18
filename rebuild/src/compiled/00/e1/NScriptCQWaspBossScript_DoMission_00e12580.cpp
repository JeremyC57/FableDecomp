// NScript::CQ_WaspBossScript::DoMission() -- register the post-wasp-boss gossip, spawn five hornet drones at the queen's deposit markers, gather
// the drones, chasers and attackers, run the wasp intro and wait until they are all dead; pause, create the queen hornet at the starting marker,
// start the guildmaster-help thread and, unless the mission failed, run the end of the mission
// Leaf: ?DoMission@CQ_WaspBossScript@NScript@@QAEXXZ
// Flags: /d2inlT60
// Inline-budget: /d2inlT60 -- the smallest budget that matches (agentNF71: T60 / T62 RELOCATION_MATCH; T59 / T58 DIFFER(1411v1446): the
//   ~vector<CScriptThing> that retail expands at the normal exit (0x00E12A43..0x00E12A6F, free at 0x00E12A67) is called out of line
//   there too; T56 DIFFER(1354v1446); retail calls 0x008AC970 at the early exits, e.g. 0x00E12B10). The default budget was not graded on
//   this text (agentNF68 measured DIFFER(1626v1446) on NF60's pragma-free local-model text). No #pragma inline_depth region.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E12580, 1446 bytes (int3 padded to 1456).
// Shared headers (agentNF71, round 7): engine/NScript_CQ_WaspBossScript.h (FABLE_RETAIL_CQ_WASPBOSSSCRIPT_DOMISSION), engine/CScriptThing.h
//   (FABLE_RETAIL_NSCRIPT, IMPLICIT_ASSIGNMENT; ctor / dtor / GetPos / SetToKillOnLevelUnload expanded inline in this TU, defined after the
//   include as the header comment asks), CGameScriptInterfaceBase.h, fable_script_def_global.h (pGlobalScriptDef 0x0143E90C, CScriptDef
//   WB_CreaturesSpawned +0xE3C / WB_WaspBossName +0xE40), CSpawnedFunc.h, STLport vector with its default inline ~vector (the earlier
//   drafts' VECTOR_DTOR_OUTOFLINE / ERASE_RANGE_OUTOFLINE defines sat after fable_string.h had already included fable_containers.h, so
//   they never applied: retail expands ~vector<CScriptThing> at the normal exit and calls 0x008AC970 at the early ones, by the budget).
// Callees: CCharString 0x0099EBF0 / ~ 0x0099EAE0, CDefString::operator CCharString 0x00415D70, GFIntToCharString 0x0099F830,
//   operator+(const char*, const CCharString&) 0x0099F690, IsActiveThreadTerminating 0x00CB7940, CBaseClass 0x0099A2D0 / ~ 0x0099A2E0,
//   ~CScriptThing 0x004AA840, AreAllThingsInVectorDead 0x00CBED00, CSpawnedFuncBase 0x00CDD450, AddSpawnedFunction 0x00CB7E50,
//   WaspIntro 0x00E12F20, EndMission 0x00E13310 (the self row's and the header's name), ~vector<CScriptThing> 0x008AC970, free 0x00BFEA14.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CQ_WASPBOSSSCRIPT_DOMISSION
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST,SCRIPT_AI_PRIORITY_LOW,SCRIPT_AI_PRIORITY_MEDIUM,SCRIPT_AI_PRIORITY_HIGH,SCRIPT_AI_PRIORITY_HIGHEST };
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/NScript_CQ_WaspBossScript.h"
#include "engine/fable_script_def_global.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
CCharString __fastcall GFIntToCharString(long);                                 // 0x0099F830
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
bool __fastcall AreAllThingsInVectorDead(const std::vector<CScriptThing>& things);   // 0x00CBED00
inline CScriptThing::CScriptThing() { }
inline CScriptThing::~CScriptThing() { }
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
inline void CScriptThing::SetToKillOnLevelUnload(bool kill) { if (!PImp.IsNull()) PImp->SetToKillOnLevelUnload(kill); }
void NScript::CQ_WaspBossScript::DoMission()
{
    Game->AddRumourCategory("Post waspboss killed");
    Game->AddNewRumourToCategory("Post waspboss killed", "TEXT_AI_GOSSIP_WASPBOSS_KILLED");
    Game->AddGossipFactionToCategory("Post waspboss killed", "FACTION_PICNIC_AREA");
    CScriptThing hornet;
    for (long i = 1; i < 6; ++i) {
        if (IsActiveThreadTerminating())
            return;
        hornet = Game->CreateCreature(pGlobalScriptDef->WB_CreaturesSpawned, Game->GetThingWithScriptName("QueenDepositPos" + GFIntToCharString(i)).GetPos(), "HornetDrone", false);
    }
    if (IsActiveThreadTerminating())
        return;
    std::vector<CScriptThing> wasps;
    Game->GetAllThingsWithScriptName("HornetDrone", wasps);
    Game->GetAllThingsWithScriptName("WaspChaser", wasps);
    Game->GetAllThingsWithScriptName("WaspAttacker", wasps);
    WaspIntro();
    while (!AreAllThingsInVectorDead(wasps)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->Pause(4.0f);
    Game->CreateCreature(pGlobalScriptDef->WB_WaspBossName, Game->GetThingWithScriptName("MK_WQ_STARTING").GetPos(), "QueenHornet", false);
    QueenHornetAttacks = true;
    AddSpawnedFunction(new CSpawnedFunc<CQ_WaspBossScript>("GuildmasterHelp", 0, this, &NScript::CQ_WaspBossScript::GuildmasterHelp), CCharString(""));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    if (!MissionFailed)
        EndMission();
}
