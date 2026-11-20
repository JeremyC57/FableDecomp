// NScript::CQ_BanditCampBossBattleScript::CCROWDBANDITS::`scalar deleting destructor'(unsigned int flags) -- the implicit ~CCROWDBANDITS (no members with destructors), i.e. ~CEntityScriptBase, then free the entity when bit 0 is set
// Leaf: ??_GCCROWDBANDITS@CQ_BanditCampBossBattleScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D0B3C0, 30 bytes: push esi; mov esi,ecx; call ~CEntityScriptBase 0x00F35B40; test byte [esp+8],1; je;
//   push esi; call operator delete 0x00BFE9BC; add esp,4; mov eax,esi; pop esi; ret 4.
// Identity (two sources): slot 0 of the RTTI-less entity vtable 0x012C95F0 (engine/CQ_BanditCampBossBattleScript.h: slot 1 the landed
//   ?Main@CCROWDBANDITS@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ 0x00D0B3E0, slot 3 GetParentScript 0x00D0B370, slot 5 OnPredicateFail
//   0x00D0B380); the catalog name NScript::CQ_BanditCampBossBattleScript::CCROWDBANDITS::`scalar deleting destructor'; Ego_r.pdb / FableWin.pdb
//   name the class (??0 / ??1 / ??_E CCROWDBANDITS@CQ_BanditCampBossBattleScript@NScript@@; Ego_r.pdb layout 28 bytes, no member with a
//   destructor, so the implicit destructor is ~CEntityScriptBase alone, as the bytes show).
//   Name-fix NF186 (q4862): was a `struct T` VecDel stand-in (leaf ?VecDel@T@@QAEPAXI@Z; build catalog module CVtable_012c95f0).
// Class: engine/CQ_BanditCampBossBattleScript.h (shared header: FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_CROWDBANDITS; NF186 opt-in
//   FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_CROWDBANDITS_CTOR declares the PDB ctor). Only the ??_G is graded.
void operator delete(void* p);                          // 0x00BFE9BC
#include <new>
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_CROWDBANDITS
#define FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_CROWDBANDITS_CTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_BanditCampBossBattleScript.h"
// The PDB ctor (no retail body of its own: Alloc 0x00D0B2D0 expands it) is defined inline here, so this object emits it, if at all, as a
//   pick-any COMDAT.
inline NScript::CQ_BanditCampBossBattleScript::CCROWDBANDITS::CCROWDBANDITS(CQ_BanditCampBossBattleScript& parent, CScriptDataBase* data, const CScriptThing& me)
    : CEntityScriptBase(*parent.Game, me), ParentClass(parent)
{
    PMasterData = (CQ_SunnyvaleMasterData*)data;
}
namespace {
// Fidelity: scaffolding, not retail (nf86 / nf88 pattern, as 0x00E016F0): a placement construction through the inline ctor references the
// entity vtable, which makes this object emit the vtable with its ??_G (the graded leaf) as pick-any COMDATs. Internal to this TU; never called.
void ReferenceCrowdBanditsVtable(void* where, NScript::CQ_BanditCampBossBattleScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    new (where) NScript::CQ_BanditCampBossBattleScript::CCROWDBANDITS(parent, data, me);
}
}
