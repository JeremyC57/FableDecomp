// std::__find_if(const NInventory::CCategory* first, const NInventory::CCategory* last, NInventory::CIsInventoryCategory pred,
//   const random_access_iterator_tag&) -- STLport 4.6 _algo.c random-access find_if over the inventory categories: the first category whose
//   GlobalDefIndex equals the predicate's Index, else last
// Leaf: ??$__find_if@PBVCCategory@NInventory@@VCIsInventoryCategory@2@@std@@YIPBVCCategory@NInventory@@PBV12@0VCIsInventoryCategory@2@ABUrandom_access_iterator_tag@0@@Z
// Retail: Fable.exe @ 0x005C0653, 112 bytes, `ret 8`: __fastcall (/Gr template TU) -- first in ecx, last in edx (kept in ebx), the 4-byte
//   predicate BY VALUE and the unused category tag on the stack. Only caller: the find_if forwarder 0x005C08C0 (NF88, q11159).
// Body: four inlined predicate calls per trip (`cmp [ecx+1Ch],esi`: the retail CCategory is 0x2C bytes with GlobalDefIndex at +0x1C, the
//   stride `push 2Ch ; pop edi`), then the 3/2/1 remainder switch; size TU. Twin 0x005C06C3 is the non-const instantiation.
// Identity (NF88, q11159 follow-on): the predicate is NInventory::CIsInventoryCategory {Index} (Ego_r.pdb layout; FableWin.pdb
//   ??RCIsInventoryCategory@NInventory@@QBE_NABVCCategory@1@@Z and ??$_Find_if@PBVCCategory@NInventory@@VCIsInventoryCategory@2@@std@@
//   0x02376C93, the Dinkumware counterpart of this function). Descriptive by library (STLport). Was spelled over the made-up predicate
//   CIsCategoryWithDefIndex and a local NInventory::CCategory model (leaf ??$__find_if@PBVCCategory@NInventory@@UCIsCategoryWithDefIndex@@...).
// Port (NF88): one explicit instantiation of stlport/fable_algorithm.h with the NF88 opt-ins FABLE_RETAIL_STLP_FIND_IF_RANDOM_ACCESS(_BODY)
//   over engine/CCategory.h (PASS22 class + NF88 opt-in FABLE_RETAIL_CCATEGORY_ITEMQUERIES: the predicate, a friend of CCategory).
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

template const NInventory::CCategory* FABLE_FASTCALL std::__find_if(const NInventory::CCategory*, const NInventory::CCategory*,
    NInventory::CIsInventoryCategory, const std::random_access_iterator_tag&);
