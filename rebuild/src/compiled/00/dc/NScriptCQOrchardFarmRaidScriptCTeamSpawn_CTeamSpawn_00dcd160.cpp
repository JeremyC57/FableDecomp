// NScript::CQ_OrchardFarmRaidScript::CTeamSpawn::CTeamSpawn(CQ_OrchardFarmRaidScript& parent, CScriptDataBase* data, const CScriptThing& me) -- entity-script constructor: base CEntityScriptBase on the quest's game interface and the spawn point, remember the parent quest and its master data; members default-constructed
// Leaf: ??0CTeamSpawn@CQ_OrchardFarmRaidScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DCD160, 94 bytes, `ret 0Ch`; stores the entity vtable 0x012DB2D4 (its only store); caller the factory
//   0x00DCD0E0 (Alloc, not landed).
// Identity (NF179, PRIORITY q12011): Ego_r.pdb 0x00E015E0 + FableWin.pdb 0x03857060
//   ??0CTeamSpawn@CQ_OrchardFarmRaidScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z (engine/CQ_OrchardFarmRaidScript.h
//   declares it). Was the global stand-in struct CTeamSpawn (leaf ??0CTeamSpawn@@QAE@PAUCQ_OrchardFarmRaidScript@@PAUCQ_SunnyvaleMasterData@@ABVCScriptThing@@@Z)
//   on local CEntityScriptBase / CQ_OrchardFarmRaidScript models. Exe scan: no landed row calls 0x00DCD160; the vtable 0x012DB2D4 is
//   referenced only by this row (now ??_7CTeamSpawn@CQ_OrchardFarmRaidScript@NScript@@6B@).
// One definition (NF179, PRIORITY q12011 / q12014): 0x00DCD160 (ctor), 0x00DCF6E0 (??_G) and 0x00DCF700 (??1) share this exact
//   text below: the PDB ctor defined `inline` on engine/CQ_OrchardFarmRaidScript.h (FABLE_RETAIL_CQ_ORCHARDFARMRAIDSCRIPT_CTEAMSPAWN_MAIN)
//   and referenced from an anonymous-namespace placement-new helper, so each object emits ??0 / ??_7 / ??_G / ??1 as identical pick-any
//   COMDATs and no row defines any of them strongly (GOTCHAS nf86 / nf88; the landed 0x00E016F0 recipe). The `(CQ_SunnyvaleMasterData*)data`
//   conversion is the landed 0x00DCF700 / 0x00E016F0 text (PDB-declared types on both sides). The inline CScriptThing ctor / dtor /
//   operator= are the HEAD 0x00DCD160 / 0x00DCF700 text (retail expands both at 0x00DCD18F / 0x00DCF717).
void operator delete(void* p);                          // 0x00BFE9BC
#include <new>
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CQ_ORCHARDFARMRAIDSCRIPT_CTEAMSPAWN_MAIN
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
inline CScriptThing::CScriptThing() { }
inline CScriptThing::~CScriptThing() { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_OrchardFarmRaidScript.h"
// The PDB ctor (retail out of line at 0x00DCD160, owned by that landed row) is defined inline here, so this object emits it, if at all,
//   as a pick-any COMDAT.
inline NScript::CQ_OrchardFarmRaidScript::CTeamSpawn::CTeamSpawn(CQ_OrchardFarmRaidScript& parent, CScriptDataBase* data, const CScriptThing& me)
    : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
{
    PMasterData = (CQ_SunnyvaleMasterData*)data;
}
namespace {
// Fidelity: scaffolding, not retail (nf86 / nf88 pattern, as 0x009566C0): a placement construction through the inline ctor references the
// entity vtable, which makes this object emit the vtable with its ??_G (the graded leaf) and the implicit ~CTeamSpawn as pick-any COMDATs.
// Internal to this TU; never called.
void ReferenceTeamSpawnCtor(void* where, NScript::CQ_OrchardFarmRaidScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    new (where) NScript::CQ_OrchardFarmRaidScript::CTeamSpawn(parent, data, me);
}
}

