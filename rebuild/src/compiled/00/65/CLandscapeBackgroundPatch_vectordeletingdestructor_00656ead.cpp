// NPlayerGui::CDrawQuestInfo::CQuestInfoElementCounterList::`scalar deleting destructor'(unsigned int flags) -- ~CQuestInfoElementCounterList
//   (the base CQuestInfoElement's PrimitiveHandles, a vector<CEnginePrimitiveHandle> at +0x10, by its ~vector 0x00440C40), then operator
//   delete when bit 0 is set; returns this
// Leaf: ??_GCQuestInfoElementCounterList@CDrawQuestInfo@NPlayerGui@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00656EAD, 31 bytes, thiscall, `ret 4`; size-optimised TU. One reference: `push 1 ; call` in the
//   CCountedPointer<CQuestInfoElementCounterList>::DeleteData 0x00657016 (`if (p) delete p;`, at 0x0065701C), the deleter the
//   CCountedPointer ctor 0x00657138 stores in its count block. Non-virtual (QAE): the class's vftable 0x0125A504 has no destructor slot
//   (slot 0 DrawAt 0x00650C1D, slot 3 GetType 0x00656C38), and the implicit destructor stores no vptr.
// Name (NF409): (1) FableWin.pdb names exactly this spelling (??_GCQuestInfoElementCounterList@CDrawQuestInfo@NPlayerGui@@QAEPAXI@Z, with
//   ??1 / DeleteData / ReleaseData of CCountedPointer<CQuestInfoElementCounterList>); (2) retail: 0x0064FA77 allocates 0x28 bytes, runs
//   ??0CQuestInfoElementCounterList@CDrawQuestInfo@NPlayerGui@@QAE@JJM@Z 0x00656C0A (table-bound; it stores vftable 0x0125A504, RTTI
//   .?AVCQuestInfoElementCounterList@CDrawQuestInfo@NPlayerGui@@) and hands the object to the CCountedPointer ctor 0x00657138 whose
//   deleter 0x00657016 is this body's only caller; the +0x10 member is CQuestInfoElement::PrimitiveHandles (Ego_r.pdb), destroyed by
//   ??1?$vector@VCEnginePrimitiveHandle@@V?$allocator@VCEnginePrimitiveHandle@@@std@@@std@@QAE@XZ 0x00440C40 (table-bound).
// NF409: was the strong free stand-in ?vector_deleting_destructor@@YIPAXPAVCLandscapeBackgroundPatch@@HI@Z (catalog
//   ScalarDeletingDtor_00656ead / manifest CLandscapeBackgroundPatch::`vector_deleting_destructor', propagated) on a row-local model with
//   the callee stand-in ?Sub_destroy@@YIXPAUSub@@@Z -> 0x00440C40.
// Fidelity: scaffolding, not retail (nf86 / nf88 pattern): the deleting destructor is a compiler-emitted COMDAT; the anonymous-namespace
//   helper below deletes an element (the retail DeleteData's `delete p`) so this object emits it pick-any. Internal to this TU; never
//   called. `#pragma optimize("s",on)` is the row's own mode.
// Fidelity: scaffolding, not retail: the closing `#pragma optimize("", on)` restores the command-line mode at the end of the TU, where
//   VC7.1 takes the optimize mode of template instances (GOTCHAS 2026-11-24). The ??_G keeps its size-mode body (the `pop ecx` after the
//   delete), and the header's vector<CEnginePrimitiveHandle> chain is emitted in speed mode. Its ~vector copy is then byte-equal to
//   retail 0x00440C40 (the address the ??_G calls). In a whole-TU size compile it is a 30-byte size-mode body that is NEQ there and
//   reached from the leaf. Every other chain copy becomes byte- and reloc-identical to the landed 0x0072DB90 / 0x0072DBE0 / 0x004A3740
//   copies (rows with no optimize pragma and no CompilerFlags; this row has no CompilerFlags either, so "" restores /O2). Measured: with
//   and without the closing pragma, only the vector<CEnginePrimitiveHandle> template chain differs; the leaf, the helper and the two
//   implicit destructors are equal (ruling R-NF409-2, the row-end extension of the p25 clause).
#pragma optimize("s",on)
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_CDRAWQUESTINFO_ELEMENTS
#include "engine/NPlayerGui_CDrawQuestInfo.h"

namespace {
void EmitCounterListDeletingDestructor(NPlayerGui::CDrawQuestInfo::CQuestInfoElementCounterList* element)
{
    delete element;
}
}
#pragma optimize("", on)
