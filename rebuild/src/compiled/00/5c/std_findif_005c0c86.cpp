// std::find_if<const NInventory::CItem*, std::const_mem_fun_ref_t<bool, NInventory::CItem> >(first, last, pred) -- STLport 4.6 _algo.h: forwards
//   to the random-access __find_if 0x005C0965 with an iterator-category tag (the dead frame byte at [ebp+0Bh])
// Leaf: ??$find_if@PBVCItem@NInventory@@V?$const_mem_fun_ref_t@_NVCItem@NInventory@@@std@@@std@@YIPBVCItem@NInventory@@PBV12@0V?$const_mem_fun_ref_t@_NVCItem@NInventory@@@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C0C86, 19 bytes, `ret 4` (__fastcall: first in ecx, last in edx, the 4-byte predicate by value on
//   the stack): `push ebp ; mov ebp,esp ; lea eax,[ebp+0Bh] ; push eax ; push [ebp+8] ; call __find_if ; pop ebp ; ret 4`.
// Identity (NF92, q11212): the ported NInventory::CCategory::GetFirstEmptySlot 0x005BD96B (NF90) binds this address under this spelling
//   (std::find_if(ItemsInCategory.begin(), ItemsInCategory.end(), std::mem_fun_ref(&CItem::IsSlotEmpty))); FableWin.pdb names the
//   const_mem_fun_ref_t<bool, NInventory::CItem> find_if / mem_fun_ref instances of that TU (Dinkumware spelling). Byte twin of the NF88 forwarder
//   0x005C08C0 (CCategory / CIsInventoryCategory). Explicit instantiation of stlport/fable_algorithm.h find_if (NF88 opt-in
//   FABLE_RETAIL_STLP_FIND_IF_RANDOM_ACCESS, NF90 opt-in FABLE_RETAIL_STLP_CONST_MEM_FUN_REF). Was the stand-in
//   `std::vector<EWeaponClass>::_Umove<EWeaponClass*>` (extern "C" FableUmove_005c0c86 over a local element model). Descriptive-by-library
//   (STLport; both PDBs are Dinkumware builds).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#define FABLE_RETAIL_STLP_FIND_IF_RANDOM_ACCESS
#define FABLE_RETAIL_STLP_CONST_MEM_FUN_REF
#include "rebuild_abi.h"
#include "engine/NInventory_CTCInventoryBase.h"
#include "stlport/fable_algorithm.h"

template const NInventory::CItem* FABLE_FASTCALL std::find_if(const NInventory::CItem*, const NInventory::CItem*,
    std::const_mem_fun_ref_t<bool, NInventory::CItem>);
