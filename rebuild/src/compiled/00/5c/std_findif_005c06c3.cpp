// std::__find_if(NInventory::CCategory* first, NInventory::CCategory* last, NInventory::CIsInventoryCategory pred,
//   const random_access_iterator_tag&) -- STLport 4.6 _algo.c random-access find_if over the inventory categories: the first category whose
//   GlobalDefIndex equals the predicate's Index, else last
// Leaf: ??$__find_if@PAVCCategory@NInventory@@VCIsInventoryCategory@2@@std@@YIPAVCCategory@NInventory@@PAV12@0VCIsInventoryCategory@2@ABUrandom_access_iterator_tag@0@@Z
// Retail: Fable.exe @ 0x005C06C3, 112 bytes, `ret 8`: __fastcall (/Gr template TU) -- first in ecx, last in edx (kept in ebx), the 4-byte
//   predicate BY VALUE and the unused category tag on the stack. Only caller: the non-const find_if forwarder 0x005C08D3.
// Body: four inlined predicate calls per trip (`cmp [ecx+1Ch],esi`, stride `push 2Ch ; pop edi`), then the 3/2/1 remainder switch; size TU.
//   Byte-identical twin of 0x005C0653 (the const instantiation, NF88): this one serves the NON-const NInventory::CTCInventoryBase members
//   (AddItemToInventory 0x005BF32D, RemoveItemFromInventory 0x005BF3C6, AddInventoryItem 0x005BF4A3, ...), so the iterator is CCategory*.
// Identity (NF93, q11239): the predicate is NInventory::CIsInventoryCategory {Index} (Ego_r.pdb layout; FableWin.pdb
//   ??RCIsInventoryCategory@NInventory@@QBE_NABVCCategory@1@@Z and the Dinkumware counterpart ??$_Find_if@...CIsInventoryCategory...).
//   Descriptive by library (STLport). Was spelled over the made-up predicate CIsCategoryWithDefIndex and a local NInventory::CCategory model.
// Port (NF93): one explicit instantiation of stlport/fable_algorithm.h (NF88 opt-ins FABLE_RETAIL_STLP_FIND_IF_RANDOM_ACCESS(_BODY)) over
//   engine/CCategory.h, the 0x005C0653 recipe with the non-const iterator.
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCATEGORY_ITEMQUERIES
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#define FABLE_RETAIL_STLP_FIND_IF_RANDOM_ACCESS
#define FABLE_RETAIL_STLP_FIND_IF_RANDOM_ACCESS_BODY
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
class CThing;                                   // engine/CCategory.h's PASS22 class names CIntelligentPointer<CThing>
#include "engine/CCategory.h"
#include "stlport/fable_algorithm.h"

template NInventory::CCategory* FABLE_FASTCALL std::__find_if(NInventory::CCategory*, NInventory::CCategory*,
    NInventory::CIsInventoryCategory, const std::random_access_iterator_tag&);
