// NScript::CQ_OrchardFarmRaidScript::CTeamSpawn::`scalar deleting destructor'(unsigned int flags) -- the out-of-line implicit ~CTeamSpawn 0x00DCF700, then free the entity when bit 0 is set
// Leaf: ??_GCTeamSpawn@CQ_OrchardFarmRaidScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DCF6E0, 30 bytes: push esi; mov esi,ecx; call ~CTeamSpawn 0x00DCF700; test byte [esp+8],1; je;
//   push esi; call operator delete 0x00BFE9BC; add esp,4; mov eax,esi; pop esi; ret 4.
// Identity (two sources): slot 0 of the RTTI-less entity vtable 0x012DB2D4, which only the landed ctor 0x00DCD160 stores (slots 1-3 are
//   the landed Main 0x00DCD350 / Init 0x00DCD1D0 / GetParentScript 0x00DCD1C0); Ego_r.pdb
//   ??_GCTeamSpawn@CQ_OrchardFarmRaidScript@NScript@@UAEPAXI@Z (0x00E047D0; FableWin.pdb has the /Od ??_E 0x0385D1C0 and ??1). Its only
//   callee is the landed ??1CTeamSpawn@CQ_OrchardFarmRaidScript@NScript@@UAE@XZ 0x00DCF700.
//   Name-fix NF163 (q4134): was a free __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor stand-in calling
//   @CLandscapeBackgroundPatch_dtor_00dcf700@4.
// Class: engine/CQ_OrchardFarmRaidScript.h (shared header; opt-in FABLE_RETAIL_CQ_ORCHARDFARMRAIDSCRIPT_CTEAMSPAWN_MAIN declares the nested
//   CTeamSpawn with its PDB ctor). Only the ??_G is graded.
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
void ReferenceTeamSpawnVtable(void* where, NScript::CQ_OrchardFarmRaidScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    new (where) NScript::CQ_OrchardFarmRaidScript::CTeamSpawn(parent, data, me);
}
}

