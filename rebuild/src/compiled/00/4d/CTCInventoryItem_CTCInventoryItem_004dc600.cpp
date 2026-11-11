// CTCInventoryItem::~CTCInventoryItem() -- inventory item component dtor (compiler-generated): release the item def pointer, then ~CTCBase
// Leaf: ??1CTCInventoryItem@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DC600, 32 bytes, thiscall; size-optimised TU (unaligned, `and [esi+0Ch],0`), ends `jmp ~CTCBase`.
// Identity (agentFAM06): the only caller is 0x004DC5E4, slot 0 (scalar deleting destructor) of the CTCInventoryItem RTTI vtable
//   0x01240004; Ego_r.pdb / FableWin.pdb ??1CTCInventoryItem@@UAE@XZ; Ego_r.pdb CTCInventoryItem (0x28 bytes) has PDef
//   (CDefPointer<const CInventoryItemDef>) at +0x0C, the member released here; the tail jump goes to ~CTCBase 0x00686830. The
//   catalog label CIVCountedPointer_Release and the landed leaf ?dtor_C3DAnimationBlendState@@... are placeholders.
// Body: the shared engine/CTCInventoryItem.h (retail CTCBase branch, no user-declared destructor) with the new nested opt-in
//   FABLE_RETAIL_CTCINVENTORYITEM_PDEF (the member typed CDefPointer<const CInventoryItemDef>). Retail stores no vtable: the
//   compiler-generated destructor. The helper below only makes VC7.1 emit the implicit ??1 COMDAT; it is not a retail function.
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_PDEF
#include "rebuild_abi.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCInventoryItem.h"

void EmitInventoryItemDestructor(CTCInventoryItem* p)                     // emits the implicit ??1; not a retail function
{
    p->CTCInventoryItem::~CTCInventoryItem();
}
