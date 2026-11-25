// NInventory::CTCInventoryBase::RemoveAllConfiscatedItems() -- forget every confiscated item stack without giving it back (ConfiscatedItems.clear())
// Leaf: ?RemoveAllConfiscatedItems@CTCInventoryBase@NInventory@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x005BF969, 20 bytes: `push [ecx+0B0h] ; add ecx,0ACh ; push [ecx] ; call 0x005C1ECC ; ret`.
//   Size TU (unaligned bodies, `#pragma optimize("s", on)` as the landed same-TU slot-73 ReturnAllConfiscatedItems 0x005BF91A).
// Identity (two sources): slot 74 of the CTCInventoryClothing vtable 0x0124356C (rtti-vtables.tsv; the catalog's "vfunc_74"),
//   whose CTCInventoryWeapons override 0x005C2E68 (landed) calls this base body directly; Ego_r.pdb
//   ?RemoveAllConfiscatedItems@CTCInventoryBase@NInventory@@UAEXXZ (0x00622A3D, right after ReturnAllConfiscatedItems 0x006229CE,
//   as retail 0x005BF969 follows 0x005BF91A).
// Shape: the inline STLport vector::clear() = erase(begin(), end()) with `this` = &ConfiscatedItems (both bounds pushed through
//   the vector pointer), erase out of line.
// Callees: std::vector<NInventory::CItem>::erase(CItem*, CItem*) 0x005C1ECC.
// Header: engine/NInventory_CTCInventoryBase.h (FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED); stlport/fable_containers.h (opt-ins below).
// Mode and copies (NF416, T3 nf409 R-NF409-1 b): there is no `#pragma optimize("y", off)`. Comparing the COMDATs with and
//   without it (no CompilerFlags) shows it changes only the end-of-TU template instances and the compiler-generated ??_GCItem
//   (__copy, ??_G, _Destroy, __destroy): they are framed (push ebp / mov ebp,esp) where retail's are not. The leaf and the erase
//   copy are byte-equal either way. The same-TU landed slot-73 row 0x005BF91A sets only optimize("s", on). This TU's pick-any
//   vector<NInventory::CItem> destroy chain is now EQ at retail: erase 0x005C1ECC, __copy 0x005C12FA, __destroy_aux 0x005C18C6,
//   ??_GCItem@NInventory@@QAEPAXI@Z 0x005C1390 and ~CCountedPointer<CPerItemData> 0x005C04BF. At HEAD __copy, ??_G and erase
//   were NEQ, and ??_G was reached from the leaf (retail chain 0x005BF977 -> 0x005C1ECC -> 0x005C18C6 -> 0x005C1390). With the
//   pragma kept and every opt-in below in place, __copy and ??_G are framed again (NEQ).
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
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_NINVENTORY_CITEM_TRANSFERIN
#define FABLE_RETAIL_STLP_DESTROY_FASTCALL
#define FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL
#define FABLE_RETAIL_STLP_DESTROY_DISPATCH_FORCEINLINE_FASTCALL
#include "engine/NInventory_CTCInventoryBase.h"
namespace NInventory {
void CTCInventoryBase::RemoveAllConfiscatedItems()
{
    ConfiscatedItems.clear();
}
}