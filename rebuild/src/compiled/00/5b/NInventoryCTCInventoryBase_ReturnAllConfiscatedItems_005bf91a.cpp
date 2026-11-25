// NInventory::CTCInventoryBase::ReturnAllConfiscatedItems() -- gives back every confiscated item stack, then empties ConfiscatedItems
// Leaf: ?ReturnAllConfiscatedItems@CTCInventoryBase@NInventory@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BF91A, length 79 bytes; size TU (`push 14h ; pop ecx` divisor, no frame:
// ebp is the stride register). Virtual slot 73 of the inventory screens (CTCInventoryClothing vtable 0x0124356C).
// Callees: CTCInventoryBase::AddItem(const CItem&) @ 0x005BF4A3, std::vector<CItem>::erase(first, last) @ 0x005C1ECC.
// Shape: the trailing erase reads both bounds through the vector pointer (`push [esi+4] ; mov ecx,esi ; push [esi]`), i.e. the
// erase(begin(), end()) sits in an inline member of the vector (`clear()`); written at the call site it re-addresses
// `[edi+0B0h]` from `this`.
// Name: the class is NInventory::CTCInventoryBase (RTTI vtable 0x01253DBC .?AVCTCInventoryBase@NInventory@@, slot 73);
// Ego_r.exe ?ReturnAllConfiscatedItems@CTCInventoryBase@NInventory@@UAEXXZ 0x006229CE.
// Names (NF95, 2026-11-19, queue q11240): moved off the row-local CTCInventoryBase / CItem_Retail / CItemVector models onto
//   engine/NInventory_CTCInventoryBase.h (PASS22, ConfiscatedItems +0xAC via FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED) and
//   stlport/fable_containers.h (range erase out of line: FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_NOINLINE), so the calls bind Ego_r.pdb /
//   FableWin.pdb ?AddInventoryItem@CTCInventoryBase@NInventory@@AAE_NABVCItem@2@@Z (0x005BF4A3) and the STLport
//   vector<NInventory::CItem>::erase(first, last) spelling the landed 0x005C2B8B binds (0x005C1ECC).
// Copies (NF416, T3 nf409 R-NF409-1 a): this TU's pick-any vector<NInventory::CItem> destroy chain is EQ at retail: erase
//   0x005C1ECC, __copy 0x005C12FA, __destroy_aux 0x005C18C6, ??_GCItem@NInventory@@QAEPAXI@Z 0x005C1390 and
//   ~CCountedPointer<CPerItemData> 0x005C04BF. At HEAD the erase and ??_G copies were NEQ, and an address-ordered link meets
//   this object before 0x005C1ECC / 0x005C1390. No CompilerFlags.
//   Each opt-in was measured on its own, with the others in place. Leaving one out changes the copies named below, and the
//   leaf is byte-equal in every case:
//   - FABLE_RETAIL_NINVENTORY_CITEM_TRANSFERIN (existing): makes ~CItem implicit, as in 0x005C1390 / 0x005C2118. Without it the
//     ??_GCItem copy calls the header's ??1CItem (28 bytes, NEQ v 0x005C1390).
//   - FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL (existing): __destroy_aux<CItem*> as the /Gr __fastcall COMDAT, EQ at 0x005C18C6.
//     Without it the erase copy calls a 27-byte cdecl __destroy_aux that retail has nowhere.
//   - FABLE_RETAIL_STLP_DESTROY_DISPATCH_FORCEINLINE_FASTCALL (new; stlport/fable_containers.h): _Destroy(first, last) and
//     __destroy are expanded into erase, which then calls __destroy_aux directly as retail 0x005C1ECC does (erase copy 55 bytes, EQ
//     there; HEAD's was 52 bytes, NEQ, calling an out-of-line cdecl _Destroy). Their unreferenced copies are __fastcall, 13 / 15 bytes,
//     and each has one retail home whose call resolves to 0x005C18C6: 0x005C1996 / 0x005C1937 (no references in retail).
//     Without it they are 21-byte cdecl copies with no retail home, and erase is NEQ.
//   - FABLE_RETAIL_STLP_DESTROY_FASTCALL (existing, NF398): _Destroy(CItem*) as __fastcall. Its unreferenced 8-byte copy has one
//     retail home whose call resolves to 0x005C1390: 0x005C149E (no references). Without it the copy is 12 bytes, cdecl, homeless.
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_ORIGINAL_COMPLETE_METHODS_PASS22
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_NINVENTORY_CITEM_TRANSFERIN
#define FABLE_RETAIL_STLP_DESTROY_FASTCALL
#define FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL
#define FABLE_RETAIL_STLP_DESTROY_DISPATCH_FORCEINLINE_FASTCALL
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_NOINLINE
#include "engine/NInventory_CTCInventoryBase.h"
namespace NInventory {
void CTCInventoryBase::ReturnAllConfiscatedItems()
{
    for (unsigned long i = 0; i < ConfiscatedItems.size(); ++i)
        AddInventoryItem(ConfiscatedItems[i]);
    ConfiscatedItems.clear();
}
}
