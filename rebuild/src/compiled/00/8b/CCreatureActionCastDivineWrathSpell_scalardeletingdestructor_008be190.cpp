// CCreatureAction_CastDivineWrathSpell::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~CCreatureAction_CastDivineWrathSpell expanded (a
//   direct call to ~CCombatActionBase, no own vtable store), then operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCCreatureAction_CastDivineWrathSpell@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008BE190, 30 bytes, thiscall, `ret 4`; vftable 0x0127E774 slot 0 (RTTI .?AVCCreatureAction_CastDivineWrathSpell@@, manifest
//   rtti-vtables.tsv), its only reference (raw scan: no call, jump or code operand).
// Name (NF424, 2026-11-25, deleting-destructor family, generator G1): (1) retail's own vftable placement and RTTI type descriptor name
//   the class, and the retail body is the scalar form (no `test 2` / eh vector destructor iterator), which VC7.1 emits as ??_G;
//   (2) FableWin.pdb name set: ??_GCCreatureAction_CastDivineWrathSpell@@UAEPAXI@Z.
//   (3) FableWin.pdb name set: ??1CCreatureAction_CastDivineWrathSpell@@UAE@XZ (its destructor).
//   Was the free __fastcall stand-in ?CCreatureAction_CastDivineWrathSpell_scalar_deleting_destructor@@YIPAXPAVCCreatureAction_CastDivineWrathSpell@@HI@Z on a row-local model.
// Fidelity: scaffolding, not retail (nf86 / nf88 pattern, the 0x009568E0 / 0x009539C0 recipe): VC7.1 emits the deleting destructor
//   with the vtable. The shared-header model declares no constructor, so the anonymous-namespace helper below expands the implicit copy
//   constructor (its vtable store), and this object emits ??_7CCreatureAction_CastDivineWrathSpell@@6B@ and ??_G pick-any. Internal to this TU; never called.
// Model: engine/CCreatureAction_CastDivineWrathSpell.h, opt-in FABLE_RETAIL_CCREATUREACTION_CASTDIVINEWRATHSPELL_VIRTUAL_MODEL (declarations only; slot order probed against retail);
//   prelude: the #define / #include block of the landed 0x008BDB40 (a row that defines FABLE_RETAIL_CCREATUREACTION_CASTDIVINEWRATHSPELL_VIRTUAL_MODEL). This object's ??_7 has retail's slot owners (NF424 vtcheck).
// Callees: CCombatActionBase::~CCombatActionBase 0x0062E900, operator delete 0x00BFE9BC.
#define FABLE_RETAIL_CCREATUREACTION_CASTDIVINEWRATHSPELL_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_CastDivineWrathSpell.h"
#include <new>

namespace {
void ReferenceCastDivineWrathSpellVtable(void* where, const CCreatureAction_CastDivineWrathSpell& other)
{
    new (where) CCreatureAction_CastDivineWrathSpell(other);
}
}
