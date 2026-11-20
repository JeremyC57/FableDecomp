// NScript::CQ_OrchardFarmRaidScript::CTeamSpawn::Init() -- entity-script init: pick the other spawn point, set the team (bandits or guards) with its member and def names, then the team size and respawn time from the global script def (own team or enemy team)
// Leaf: ?Init@CTeamSpawn@CQ_OrchardFarmRaidScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DCD1D0, length 370 bytes (plain `ret`; entity-script slot "Init").
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0 / CCharString::operator=(const char*)
//   @ 0x0099EFE0, ~CBaseClass @ 0x0099A2E0, operator delete @ 0x00BFE9BC (the counted-pointer info block),
//   Compare(const char*, const char*) @ 0x00411570 (`__fastcall`, the inlined CBasicString<char>::operator==), game
//   interface slot 74 GetRandomThingWithScriptName (ghidra_out/cgsi_slots_aligned.tsv), CScriptThing slot 1 GetName
//   (0x004AA8B0, rebuild/manifest/rtti-vtables.tsv).
// Data: "EitherTeamSpawn" @ 0x012DB30C, "BanditTeamSpawn" @ 0x012DB2AC, "" @ 0x0122D70E, "BanditTeamMember" @ 0x012DB268,
//   "CREATURE_BANDIT_GRUNT" @ 0x012D23A0, "GuardTeamMember" @ 0x012DB27C, "CREATURE_ORCHARD_FARM_GUARD" @ 0x012DB2F0;
//   pGlobalScriptDef @ 0x0143E90C whose +0xD6C/+0xD70 longs are GuardReinforcementsYourTeam/EnemyTeam (PDB +0xE1C/+0xE20).
// Layout: Ego_r.pdb CTeamSpawn (0x40, unshifted): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14,
//   PMasterData +0x18, OtherSpawnPoint +0x1C, TeamID +0x28, TeamMemberLimit +0x2C, TeamRespawnTime +0x30,
//   TeamMemberName +0x34, TeamMemberDefName +0x38, BanditsLeftID +0x3C; the parent CQ_OrchardFarmRaidScript's HeroTeam
//   is retail +0x78 (PDB +0x90 less the narrower STLport vector and string members before it).
// Shape: CCountedPointer::operator= reads the source Info then Data into locals before the self test (retail loads both
//   up front); the inline CCharString ==(const char*) null arm is the strcmp intrinsic over the pooled "" (as landed
//   CV_ChapelOfEvilScript::WatchForExpressions 0x00E6A000).
// agentNF60: CCharString / CCharStringData / CBasicString<char> are fable_string.h's (were local models); CBasicString::operator==(const char*)
//   expands in place (FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE) over the protected static Compare 0x00411570.
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
// NF179 (PRIORITY q12012): Ego_r.pdb 0x00E018D0 + FableWin.pdb 0x03857140 ?Init@CTeamSpawn@CQ_OrchardFarmRaidScript@NScript@@EAEXXZ (private
//   virtual; slot 2 of the entity vtable 0x012DB2D4); engine/CQ_OrchardFarmRaidScript.h FABLE_RETAIL_CQ_ORCHARDFARMRAIDSCRIPT_CTEAMSPAWN_MAIN
//   declares it. Was the global stand-in class CTeamSpawn (leaf ?Init@CTeamSpawn@@QAEXXZ) with a local CQ_OrchardFarmRaidScript; now on the
//   shared header with the landed Main 0x00DCD350 defines (no landed row calls 0x00DCD1D0: exe scan).
#include "fable_compiler.h"
#include <string.h>
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CQ_ORCHARDFARMRAIDSCRIPT_CTEAMSPAWN_MAIN
#include "fable_string.h"
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"
#include "engine/CScriptThing.h"
inline CScriptThing::~CScriptThing() {}
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
#include "stlport/fable_containers.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_OrchardFarmRaidScript.h"
#include "engine/fable_script_def_global.h"          // pGlobalScriptDef 0x0143E90C (CDefPointer<const CScriptDef>)
void NScript::CQ_OrchardFarmRaidScript::CTeamSpawn::Init()
{
    OtherSpawnPoint = Game->GetRandomThingWithScriptName("EitherTeamSpawn");
    if (Me.GetName() == "BanditTeamSpawn") {
        TeamID = 1;
        TeamMemberName = "BanditTeamMember";
        TeamMemberDefName = "CREATURE_BANDIT_GRUNT";
    } else {
        TeamID = 0;
        TeamMemberName = "GuardTeamMember";
        TeamMemberDefName = "CREATURE_ORCHARD_FARM_GUARD";
    }
    if (TeamID == ParentClass->HeroTeam) {
        TeamMemberLimit = 2;
        TeamRespawnTime = pGlobalScriptDef->GuardReinforcementsYourTeam;
    } else {
        TeamMemberLimit = 3;
        TeamRespawnTime = pGlobalScriptDef->GuardReinforcementsEnemyTeam;
    }
    BanditsLeftID = 0;
}