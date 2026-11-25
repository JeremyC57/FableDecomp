// CCreatureAction_SpiritReturnToHero::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~CCreatureAction_SpiritReturnToHero expanded (a
//   direct call to ~CCreatureActionBase, no own vtable store), then operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCCreatureAction_SpiritReturnToHero@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008550E0, 30 bytes, thiscall, `ret 4`; vftable 0x01275C54 slot 0 (RTTI .?AVCCreatureAction_SpiritReturnToHero@@, manifest
//   rtti-vtables.tsv), its only reference (raw scan: no call, jump or code operand).
// Name (NF424, 2026-11-25, deleting-destructor family, generator G1): (1) retail's own vftable placement and RTTI type descriptor name
//   the class, and the retail body is the scalar form (no `test 2` / eh vector destructor iterator), which VC7.1 emits as ??_G;
//   (2) FableWin.pdb name set: ??_GCCreatureAction_SpiritReturnToHero@@UAEPAXI@Z.
//   (3) FableWin.pdb name set: ??1CCreatureAction_SpiritReturnToHero@@UAE@XZ (its destructor).
//   order_match's object-order prediction (??_GCCreatureAction_PutDownTankard@@UAEPAXI@Z) disagrees with retail's own RTTI placement, which decides.
//   Was the free __fastcall stand-in ?CCreatureAction_SpiritReturnToHero_scalar_deleting_destructor@@YIPAXPAVCCreatureAction_SpiritReturnToHero@@HI@Z on a row-local model.
// Fidelity: scaffolding, not retail (nf86 / nf88 pattern, the 0x009568E0 / 0x009539C0 recipe): VC7.1 emits the deleting destructor
//   with the vtable. The shared-header model declares no constructor, so the anonymous-namespace helper below expands the implicit copy
//   constructor (its vtable store), and this object emits ??_7CCreatureAction_SpiritReturnToHero@@6B@ and ??_G pick-any. Internal to this TU; never called.
// Model: engine/CCreatureAction_SpiritReturnToHero.h, opt-in FABLE_RETAIL_CCREATUREACTION_SPIRITRETURNTOHERO_VIRTUAL_MODEL (declarations only; slot order probed against retail);
//   prelude: the #define / #include block of the landed 0x00855200 (a row that defines FABLE_RETAIL_CCREATUREACTION_SPIRITRETURNTOHERO_VIRTUAL_MODEL). This object's ??_7 has retail's slot owners (NF424 vtcheck).
// Callees: CCreatureActionBase::~CCreatureActionBase 0x00693EF0, operator delete 0x00BFE9BC.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_COPYCTOR
#define FABLE_RETAIL_CCREATUREACTION_SPIRITRETURNTOHERO_VIRTUAL_MODEL
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_SpiritReturnToHero.h"
#include <new>

namespace {
void ReferenceSpiritReturnToHeroVtable(void* where, const CCreatureAction_SpiritReturnToHero& other)
{
    new (where) CCreatureAction_SpiritReturnToHero(other);
}
}
