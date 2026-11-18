// std::__find_if(const NInventory::CItem* first, const NInventory::CItem* last, std::const_mem_fun_ref_t<bool, NInventory::CItem> pred, const
//   random_access_iterator_tag&) -- STLport random-access find_if over a category's items: the first item for which the bound const member
//   predicate returns true, else last
// Leaf: ??$__find_if@PBVCItem@NInventory@@V?$const_mem_fun_ref_t@_NVCItem@NInventory@@@std@@@std@@YIPBVCItem@NInventory@@PBV12@0V?$const_mem_fun_ref_t@_NVCItem@NInventory@@@0@ABUrandom_access_iterator_tag@0@@Z
// Retail: Fable.exe @ 0x005C0965, 145 bytes, `ret 8`: __fastcall -- first in ecx, last in edx (spilled to the `push ecx` frame word), the
//   4-byte predicate (a pointer to a const member function) BY VALUE and the unused tag on the stack.
// Body: STLport 4.x _algo.c __find_if: trip_count = (last - first) >> 2 (`idiv` by the 0x14 CItem size under optimize("s"), the stride
//   kept in ebp: `push 14h ; pop ebp`), four inlined `(x.*f)()` calls per trip (`mov ecx,esi ; call ebx`), then the 3/2/1 remainder switch.
// Identity (NF92, q11212 follow-on): the only caller is the find_if forwarder 0x005C0C86 (re-authored by NF92 under the spelling the ported
//   0x005BD96B binds), which calls it under exactly this spelling; Ego_r.pdb names the instantiation `_Find_if<const NInventory::CItem*,
//   const_mem_fun_ref_t<bool, CItem>>` (Dinkumware). The NF88 recipe of 0x005C0653 (the CCategory / CIsInventoryCategory twin): explicit
//   instantiation of stlport/fable_algorithm.h __find_if (FABLE_RETAIL_STLP_FIND_IF_RANDOM_ACCESS_BODY). Was a local std model over the global
//   generated CItem (engine/CItem.h; leaf ??$__find_if@PBUCItem@@..). Descriptive-by-library (STLport).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#define FABLE_RETAIL_STLP_FIND_IF_RANDOM_ACCESS
#define FABLE_RETAIL_STLP_FIND_IF_RANDOM_ACCESS_BODY
#define FABLE_RETAIL_STLP_CONST_MEM_FUN_REF
#include "rebuild_abi.h"
#include "engine/NInventory_CTCInventoryBase.h"
#include "stlport/fable_algorithm.h"

template const NInventory::CItem* FABLE_FASTCALL std::__find_if(const NInventory::CItem*, const NInventory::CItem*,
    std::const_mem_fun_ref_t<bool, NInventory::CItem>, const std::random_access_iterator_tag&);
