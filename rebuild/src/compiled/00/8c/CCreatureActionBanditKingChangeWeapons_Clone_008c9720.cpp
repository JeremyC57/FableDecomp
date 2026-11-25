// CCreatureAction_BanditKingChangeWeapons::Clone() const -- `return new CCreatureAction_BanditKingChangeWeapons(*this);` with the implicit copy constructor inlined (CCreatureActionBase copy constructor, vftable store, own members)
// Leaf: ?Clone@CCreatureAction_BanditKingChangeWeapons@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C9720, 59 bytes: operator new(0xAC) 0x00BFEA1A, null test, CCreatureActionBase(const CCreatureActionBase&) 0x0062EBC0, C's vftable store, then 1 member copy;
//   a public const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCreatureAction_BanditKingChangeWeapons@@UBEPAVCCreatureActionBase@@XZ: RTTI placement +
//   Ego_r/FableWin; base copy constructor ??0CCreatureActionBase@@QAE@ABV0@@Z 0x0062EBC0 (Ego_r/FableWin; MELEE_COPYCTOR / COPYCTOR
//   opt-in).
// Respell (NF425): was the stand-in self ?Clone@CCreatureAction_BanditKingChangeWeapons@@QAEPAUCBase_0062ebc0@@XZ (non-virtual,
//   non-const, over a made-up base model); now the real virtual member, so the RTTI-placed name is defined.
// Header: engine/CCreatureAction_BanditKingChangeWeapons.h, its default branch: the polymorphic class there declares this Clone;
//   prelude: the CCombatActionBase MELEE block of the landed 0x008CAA70. The emitted ??_7CCreatureAction_BanditKingChangeWeapons@@6B@
//   names 9 of retail's 12 slots right.
#define FABLE_RETAIL_CCREATUREACTIONBASE_COPYCTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_BanditKingChangeWeapons.h"

CCreatureActionBase* CCreatureAction_BanditKingChangeWeapons::Clone() const
{
    return new CCreatureAction_BanditKingChangeWeapons(*this);
}
