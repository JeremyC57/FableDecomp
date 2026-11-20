// NScript::CQ_GuildTrainingScript::CSpeedFriend::`scalar deleting destructor'(unsigned int flags) -- the implicit ~CSpeedFriend (no members with destructors), i.e. ~CEntityScriptBase, then free the entity when bit 0 is set
// Leaf: ??_GCSpeedFriend@CQ_GuildTrainingScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D40890, 30 bytes: push esi; mov esi,ecx; call ~CEntityScriptBase 0x00F35B40; test byte [esp+8],1; je;
//   push esi; call operator delete 0x00BFE9BC; add esp,4; mov eax,esi; pop esi; ret 4.
// Identity (two sources): slot 0 of the RTTI-less entity vtable 0x012CDF28 (slot 1 the landed ?Main@CSpeedFriend@CQ_GuildTrainingScript@NScript@@EAEXXZ
//   0x00D408B0); FableWin.pdb ??_GCSpeedFriend@CQ_GuildTrainingScript@NScript@@UAEPAXI@Z (with ??0 / ??1 of the class); Ego_r.pdb layout 28 bytes,
//   no member with a destructor, so the implicit destructor is ~CEntityScriptBase alone, as the bytes show.
//   Name-fix NF186 (q4863): was a free __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor stand-in (build catalog module
//   CVtable_012cdf28).
// Class: engine/CQ_GuildTrainingScript.h (shared header; NF186 opt-in FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_SPEEDFRIEND declares the entity class
//   with its PDB ctor). Only the ??_G is graded.
void operator delete(void* p);                          // 0x00BFE9BC
#include <new>
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_MAIN
#define FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_SPEEDFRIEND
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_GuildTrainingScript.h"
// The PDB ctor (no retail body of its own: Alloc 0x00D407C0 expands it) is defined inline here, so this object emits it, if at all, as a
//   pick-any COMDAT.
inline NScript::CQ_GuildTrainingScript::CSpeedFriend::CSpeedFriend(CQ_GuildTrainingScript& parent, CScriptDataBase* data, const CScriptThing& me)
    : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
{
    PMasterData = (CQ_SunnyvaleMasterData*)data;
}
namespace {
// Fidelity: scaffolding, not retail (nf86 / nf88 pattern, as 0x00E016F0): a placement construction through the inline ctor references the
// entity vtable, which makes this object emit the vtable with its ??_G (the graded leaf) as pick-any COMDATs. Internal to this TU; never called.
void ReferenceSpeedFriendVtable(void* where, NScript::CQ_GuildTrainingScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    new (where) NScript::CQ_GuildTrainingScript::CSpeedFriend(parent, data, me);
}
}
