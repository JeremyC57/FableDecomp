// NScript::CV_DemonDoorsScript::CDF_SummonsCreatures::`scalar deleting destructor'(unsigned int flags) -- the implicit ~CDF_SummonsCreatures (no members with destructors), i.e. ~CEntityScriptBase, then free the entity when bit 0 is set
// Leaf: ??_GCDF_SummonsCreatures@CV_DemonDoorsScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E70370, 30 bytes: push esi; mov esi,ecx; call ~CEntityScriptBase 0x00F35B40; test byte [esp+8],1; je;
//   push esi; call operator delete 0x00BFE9BC; add esp,4; mov eax,esi; pop esi; ret 4.
// Identity (two sources): slot 0 of the RTTI-less entity vtable 0x012E8E80 (slot 1 the landed ?Main@CDF_SummonsCreatures@CV_DemonDoorsScript@NScript@@EAEXXZ
//   0x00E74D70, slot 3 GetParentScript 0x00E70360); FableWin.pdb ??_ECDF_SummonsCreatures@CV_DemonDoorsScript@NScript@@UAEPAXI@Z (with ??0 / ??1 of the
//   class); Ego_r.pdb layout 28 bytes, no member with a destructor, so the implicit destructor is ~CEntityScriptBase alone, as the bytes show.
//   Name-fix NF186 (q4877): was a free __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor stand-in (build catalog module
//   CVtable_012e8e80).
// Class: engine/CV_DemonDoorsScript.h (shared header; NF186 opt-in FABLE_RETAIL_CV_DEMONDOORSSCRIPT_SUMMONSCREATURES_CTOR declares the PDB ctor).
//   Only the ??_G is graded.
void operator delete(void* p);                          // 0x00BFE9BC
#include <new>
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CV_DEMONDOORSSCRIPT_SUMMONSCREATURES_CTOR
#include "engine/CV_DemonDoorsScript.h"
// The PDB ctor (no retail body of its own: Alloc 0x00E702C0 expands it) is defined inline here, so this object emits it, if at all, as a
//   pick-any COMDAT.
inline NScript::CV_DemonDoorsScript::CDF_SummonsCreatures::CDF_SummonsCreatures(CV_DemonDoorsScript& parent, CScriptDataBase* data, const CScriptThing& me)
    : CEntityScriptBase(*parent.Game, me), ParentClass(parent)
{
    PMasterData = (CQ_SunnyvaleMasterData*)data;
}
namespace {
// Fidelity: scaffolding, not retail (nf86 / nf88 pattern, as 0x00E016F0): a placement construction through the inline ctor references the
// entity vtable, which makes this object emit the vtable with its ??_G (the graded leaf) as pick-any COMDATs. Internal to this TU; never called.
void ReferenceSummonsCreaturesVtable(void* where, NScript::CV_DemonDoorsScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    new (where) NScript::CV_DemonDoorsScript::CDF_SummonsCreatures(parent, data, me);
}
}
