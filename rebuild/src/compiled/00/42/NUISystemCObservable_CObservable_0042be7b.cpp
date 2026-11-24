// NUISystem::CObservable::CObservable() -- an observable UI node: two empty observer lists around
// a cleared exclusive-observer pointer
// Leaf: ??0CObservable@NUISystem@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042BE7B, length 46 bytes.
// Callee: the STLport _List_base constructor @ 0x0042AC0A (takes the allocator by const reference,
// nulls the head and allocates the sentinel node: `[this]=0 ; malloc(0Ch) ; next=prev=self`); the
// manifest's "DoublyLinkedList_InitializeEmpty" label describes it. Vtable 0x01230044 =
// NUISystem::CObservable (rtti-vtables.tsv); the manifest label
// "CConsoleCommandParameters::CConsoleCommandParameters" is propagated and rejected.
// Layout (PDB CObservable 32 bytes with 12-byte debug lists; retail lists are the 4-byte STLport
// head pointer): list<CObserver*> Observers +0x04, CObserver* m_pExclusiveObserver +0x08,
// list<CObserver*> ConcurrentExclusiveObservers +0x0c (size 0x10).
// Shape: a frame-pointer, size-optimised translation unit (`push ebp ; mov ebp,esp ; push ecx`,
// `and [..],0`, `leave`): the default allocator argument of each list is a 1-byte empty temporary
// homed at [ebp-1] (both lists reuse the slot), passed by address to the out-of-line base.
// Port (NF227, 2026-11-22, q12574): onto engine/NUISystem_CObservable.h with its FABLE_RETAIL_COBSERVABLE_CTOR_DECL_ONLY declaration
//   (Ego_r.pdb 0x0043A1C1 / FableWin.pdb 0x01AE4FA4 ??0CObservable@NUISystem@@QAE@XZ, the leaf unchanged). Was a row-local CObservable
//   (one `v00` virtual) on a CRetailList / CRetailListBase / SRetailAllocator stand-in, whose 1-slot vtable COMDAT bound slot 0
//   (SetEvent 0x0055CB10) as ?v00@CObservable@NUISystem@@UAEXXZ and whose base call bound 0x0042AC0A as a stand-in. The header's
//   STLport lists with FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE call the _List_base<CObserver*> constructor 0x0042AC0A out of line; the
//   vtable COMDAT is now the header's 11-slot ??_7CObservable@NUISystem@@6B@ (retail 0x01230044).
// Landed (NF228, 2026-11-22, q12597 / q12604): rechecked on the nf227 tree: RELOCATION_MATCH; the vtable COMDAT's 11 slots bind each slot's
//   self-row leaf at retail 0x01230044 (vtcheck), so 0x0055CB10 SetEvent loses the ?v00@CObservable@NUISystem@@UAEXXZ stand-in; the two
//   calls bind ??0?$_List_base@PAVCObserver@NUISystem@@...@Z at 0x0042AC0A (its self row is still a placeholder: NAME_FIXES). The object
//   also defines the unreferenced 5-B ~list<CObserver*> copy that 0x0041E641 / 0x0041E3F6 already emit (the q12585 folded alias of the
//   23-B ~_List_base 0x0042AC25: debt, queued) and an owner-less list<CObserver*>(const allocator&) copy.
#define FABLE_RETAIL_COBSERVABLE_CTOR_DECL_ONLY
#define FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE
#include "engine/NUISystem_CObservable.h"
// The TU is frame-pointer and size-optimised; the pragmas follow the includes (EXTRA_RULES 2026-11-20, nf189 placement).
#pragma optimize("y", off)
#pragma optimize("s", on)
namespace NUISystem {
CObservable::CObservable()
    : m_pExclusiveObserver(0)
{
}
}
