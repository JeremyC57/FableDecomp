// NInventory::CItem::`scalar deleting destructor'(unsigned int flags) -- the element of the vector<NInventory::CItem> containers
//   (CTCInventoryBase::ConfiscatedItems, the CCategory item lists): ~CItem (its PPerItemData CCountedPointer<NInventory::CPerItemData>
//   at +8, by ~CCountedPointer 0x005C04BF), then operator delete when bit 0 is set; returns this
// Leaf: ??_GCItem@NInventory@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C1390, 31 bytes, thiscall, `ret 4`; size-optimised TU. Two references, both `push 0 ; call`:
//   the 0x14-byte-stride __destroy_aux<NInventory::CItem*> 0x005C18C6 (table-bound, at 0x005C18D2) and the unreferenced __fastcall
//   _Destroy copy 0x005C149E (at 0x005C14A0). Compiler-generated: STLport _construct.h `_Destroy(p) { p->~T(); }` is emitted by VC7.1 as
//   a call of this ??_G with 0. Non-virtual (no vptr; CItem is not polymorphic).
// Name (NF409): (1) FableWin.pdb names exactly this spelling (??_GCItem@NInventory@@QAEPAXI@Z, beside ??1CItem@NInventory@@QAE@XZ);
//   (2) retail: Ego_r.pdb's NInventory::CItem is 0x14 bytes with PPerItemData (CCountedPointer<NInventory::CPerItemData>) at +8, the
//   stride of its __destroy_aux 0x005C18C6 and the member whose destructor 0x005C04BF (??1?$CCountedPointer@VCPerItemData@NInventory@@@@
//   QAE@XZ, landed) this body calls on +8.
// Destructor: retail's body calls ~CCountedPointer directly, so CItem's destructor in this TU is the compiler-generated one (as in the
//   CPersistTraits<NInventory::CItem>::TransferIn TU 0x005C2118): engine/NInventory_CTCInventoryBase.h under
//   FABLE_RETAIL_NINVENTORY_CITEM_TRANSFERIN leaves the out-of-line ~CItem() declaration out.
// NF409: was the strong free stand-in ?vector_deleting_destructor@@YIPAXPAVCLandscapeBackgroundPatch@@HI@Z (catalog
//   ScalarDeletingDtor_005c1390 / manifest CLandscapeBackgroundPatch::`vector_deleting_destructor', propagated) on a row-local model.
// Fidelity: scaffolding, not retail (nf86 / nf88 pattern): the deleting destructor is a compiler-emitted COMDAT; the anonymous-namespace
//   helper below destroys an item so this object emits it pick-any. Internal to this TU; never called. `#pragma optimize("s",on)` is
//   the row's own mode.
#pragma optimize("s",on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_NINVENTORY_CITEM_TRANSFERIN
#include "engine/NInventory_CTCInventoryBase.h"

namespace {
void EmitItemDeletingDestructor(NInventory::CItem* item)
{
    item->~CItem();
}
}
