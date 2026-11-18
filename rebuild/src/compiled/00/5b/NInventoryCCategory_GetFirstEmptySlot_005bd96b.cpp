// NInventory::CCategory::GetFirstEmptySlot() const -- the index of the first slot holding no items, -1 when every slot is in use
// Leaf: ?GetFirstEmptySlot@CCategory@NInventory@@ABEJXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BD96B, 40 bytes, `ret`; size-optimised TU.
// Name (NF90, q11183): Ego_r.pdb 0x0062086D + FableWin.pdb ?GetFirstEmptySlot@CCategory@NInventory@@ABEJXZ (private const; the catalog already
//   names it); the landed / ported callers 0x005BF1D7 (NF88), 0x005BF19C and 0x005BF060 (NF90) bind this spelling.
// Body: std::find_if(begin, end, std::mem_fun_ref(&CItem::IsSlotEmpty)) (FableWin.pdb names the const_mem_fun_ref_t<bool, NInventory::CItem>
//   find_if / mem_fun_ref instances of this TU); retail pushes the functor (the address of the out-of-line CItem::IsSlotEmpty 0x005BC46C, Ego_r.pdb
//   ?IsSlotEmpty@CItem@NInventory@@QBE_NXZ: `xor eax,eax ; cmp [ecx+4],eax ; sete al`) and calls the STLport find_if 0x005C0C86 (__fastcall,
//   range in ecx / edx), then turns the iterator into an index (`idiv 14h`) or -1.
// Port (NF90): was a free __fastcall stand-in over a local CCategory model (leaf ?CCategory_GetFirstEmptySlot@@YIJPAUCCategory@@@Z, binding
//   ?find_slot@@YIPADPAD0PAH@Z and a made-up data global ?g_predicate@@3HA for the function address); now on engine/NInventory_CTCInventoryBase.h
//   (CONFISCATED CItem::IsSlotEmpty) + engine/CCategory.h (PASS22, NF88 _ITEMQUERIES) + stlport/fable_algorithm.h (find_if declared,
//   FIND_IF_CALLED_PASS23; NF90 opt-in FABLE_RETAIL_STLP_CONST_MEM_FUN_REF).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCATEGORY_ITEMQUERIES
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#define FABLE_RETAIL_STLP_FIND_IF_CALLED_PASS23
#define FABLE_RETAIL_STLP_CONST_MEM_FUN_REF
#include "engine/NInventory_CTCInventoryBase.h"
#include "stlport/fable_algorithm.h"

namespace NInventory {
long CCategory::GetFirstEmptySlot() const
{
    std::vector<CItem>::const_iterator it = std::find_if(ItemsInCategory.begin(), ItemsInCategory.end(), std::mem_fun_ref(&CItem::IsSlotEmpty));
    if (it != ItemsInCategory.end())
        return it - ItemsInCategory.begin();
    return -1;
}
}
