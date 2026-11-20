// NScript::CV_SingingStonesScript::CManWithDoorName::`scalar deleting destructor'(unsigned int flags) -- the implicit ~CManWithDoorName (no members with destructors), i.e. ~CEntityScriptBase, then free the entity when bit 0 is set
// Leaf: ??_GCManWithDoorName@CV_SingingStonesScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ED17B0, 30 bytes: push esi; mov esi,ecx; call ~CEntityScriptBase 0x00F35B40; test byte [esp+8],1; je;
//   push esi; call operator delete 0x00BFE9BC; add esp,4; mov eax,esi; pop esi; ret 4.
// Identity (two sources): slot 0 of the RTTI-less entity vtable 0x012F2C98 (slot 1 the landed ?Main@CManWithDoorName@CV_SingingStonesScript@NScript@@EAEXXZ
//   0x00ED17D0, slot 2 Init 0x00ED1790, slot 3 GetParentScript 0x00ED1780); FableWin.pdb ??_ECManWithDoorName@CV_SingingStonesScript@NScript@@UAEPAXI@Z
//   (with ??0 / ??1 of the class); Ego_r.pdb layout 28 bytes, no member with a destructor, so the implicit destructor is ~CEntityScriptBase alone,
//   as the bytes show.
//   Name-fix NF186 (q4878): was a `struct T` VecDel stand-in (leaf ?VecDel@T@@QAEPAXI@Z; build catalog module CVtable_012f2c98).
// Class: engine/CV_SingingStonesScript.h (shared header; NF186 opt-in FABLE_RETAIL_CV_SINGINGSTONESSCRIPT_MANWITHDOORNAME_CTOR declares the PDB
//   ctor). Only the ??_G is graded.
void operator delete(void* p);                          // 0x00BFE9BC
#include <new>
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CV_SINGINGSTONESSCRIPT_MANWITHDOORNAME_CTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CV_SingingStonesScript.h"
// The PDB ctor (no retail body of its own: Alloc 0x00ED16E0 expands it) is defined inline here, so this object emits it, if at all, as a
//   pick-any COMDAT.
inline NScript::CV_SingingStonesScript::CManWithDoorName::CManWithDoorName(CV_SingingStonesScript& parent, CScriptDataBase* data, const CScriptThing& me)
    : CEntityScriptBase(*parent.Game, me), ParentClass(parent)
{
    PMasterData = (CQ_SunnyvaleMasterData*)data;
}
namespace {
// Fidelity: scaffolding, not retail (nf86 / nf88 pattern, as 0x00E016F0): a placement construction through the inline ctor references the
// entity vtable, which makes this object emit the vtable with its ??_G (the graded leaf) as pick-any COMDATs. Internal to this TU; never called.
void ReferenceManWithDoorNameVtable(void* where, NScript::CV_SingingStonesScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    new (where) NScript::CV_SingingStonesScript::CManWithDoorName(parent, data, me);
}
}
