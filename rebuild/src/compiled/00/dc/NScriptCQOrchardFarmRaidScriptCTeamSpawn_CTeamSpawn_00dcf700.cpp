// NScript::CQ_OrchardFarmRaidScript::CTeamSpawn::~CTeamSpawn() -- implicit entity-script destructor: destroy TeamMemberDefName and TeamMemberName, release OtherSpawnPoint (inline ~CScriptThing), then ~CEntityScriptBase
// Leaf: ??1CTeamSpawn@CQ_OrchardFarmRaidScript@NScript@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DCF700, 94 bytes, thiscall, tail jump to ~CEntityScriptBase 0x00F35B40; sole caller
//   the scalar deleting dtor 0x00DCF6E0 (slot 0 of the RTTI-less entity vtable 0x012DB2D4).
// Identity (proven): vtable 0x012DB2D4 is stored only by the landed ctor 0x00DCD160 (Ego_r.pdb / FableWin.pdb
//   ??0CTeamSpawn@CQ_OrchardFarmRaidScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z, `ret 0Ch`), whose only caller
//   is the factory 0x00DCD0E0 (`push 40h` = Ego_r sizeof CTeamSpawn 64; bound ?Alloc@CTeamSpawn@CQ_OrchardFarmRaidScript@NScript@@..)
//   referenced from CQ_OrchardFarmRaidScript::Main 0x00DCC770 next to the binding name "GuardTeamSpawn" (0x012DB2C4); the vtable's
//   slots 1-3 are the landed Main 0x00DCD350 / Init 0x00DCD1D0 / GetParentScript 0x00DCD1C0 (`mov eax,[ecx+14h]`). Ego_r.pdb
//   0x00E04400 and FableWin.pdb 0x0385D470 both spell ??1CTeamSpawn@CQ_OrchardFarmRaidScript@NScript@@UAE@XZ. No 0x012DB2D4 store
//   in the body = compiler-generated dtor. The catalog label CActionPerformExpression::~CActionPerformExpression is wrong
//   (NTCScriptedControl::CActionPerformExpression is an engine action class; this body chains to ~CEntityScriptBase).
// Layout: Ego_r.pdb CTeamSpawn (0x40; entity scripts keep their PDB offsets, the landed ctor writes the same): CEntityScriptBase 0x14,
//   ParentClass +0x14, PMasterData +0x18, OtherSpawnPoint (CScriptThing) +0x1c, TeamID +0x28, TeamMemberLimit +0x2c,
//   TeamRespawnTime +0x30, TeamMemberName +0x34, TeamMemberDefName +0x38, BanditsLeftID +0x3c.
// Callees: CCharString::~CCharString 0x0099EAE0 (x2), the inline ~CScriptThing (vtable 0x01238C8C store, inline
//   ~CCountedPointer<CScriptThing> release through operator delete 0x00BFE9BC, then ~CBaseClass 0x0099A2E0), ~CEntityScriptBase
//   0x00F35B40 (tail).
// TU: retail's own out-of-line ctor (0x00DCD160, landed) is defined here, which emits the vtable, ??_G and this implicit dtor
//   (the landed CV_FisticuffsClubScript::CFisticuffsMember dtor 0x00E839F0 model); ~CScriptThing inline as in that row.
//   FABLE_RETAIL_CLEVELSCRIPTBASE: retail's ~CLevelScriptBase<T> is compiler-generated (chunk 713 branch).
// NF179 (PRIORITY q12014): the strong out-of-line PDB ctor this row defined (it clashed, LNK2005, with every pick-any emitter of the
//   ctor) and the row-local CQ_OrchardFarmRaidScript / CTeamSpawn / CCharString models are gone: the row is on the shared header and emits
//   ??1 as a pick-any COMDAT through the same scaffolding as the ctor row 0x00DCD160 and the ??_G row 0x00DCF6E0.
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
void ReferenceTeamSpawnDtor(void* where, NScript::CQ_OrchardFarmRaidScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    new (where) NScript::CQ_OrchardFarmRaidScript::CTeamSpawn(parent, data, me);
}
}

