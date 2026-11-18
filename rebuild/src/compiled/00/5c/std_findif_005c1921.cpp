// std::find_if<const NInventory::CItem*, CIsSameItemType> -- forwards to the random-access __find_if with an iterator-category tag
// Leaf: ??$find_if@PBVCItem@NInventory@@VCIsSameItemType@@@std@@YIPBVCItem@NInventory@@PBV12@0VCIsSameItemType@@@Z
// Retail: Fable.exe @ 0x005C1921, 22 bytes, `ret 8` (fastcall: first in ecx, last in edx, the 8-byte predicate on the
// stack). Catalog label std::vector<pair<long,CCountedPointer<CContainmentVolume>>>::_Ucopy is wrong. Ego_r.pdb names this
// instantiation (??$find_if@...@std@@YI..., YI = __fastcall), and its callee __find_if (0x005C1818); STLport passes the
// random_access_iterator_tag by const reference (`lea eax,[ebp+0Fh] ; push eax`). Only caller: NInventory::CCategory::GetSlotWithItemsOfType
// (const) 0x005BE85B, a CIsSameItemType predicate.
// Port (NF90, q11186): one explicit instantiation of stlport/fable_algorithm.h find_if (FABLE_RETAIL_STLP_FIND_IF_INPUT +
// FABLE_RETAIL_STLP_FIND_IF_RANDOM_ACCESS, the NF88 0x005C08C0 recipe) with the shared NInventory::CItem (engine/NInventory_CTCInventoryBase.h,
// 0x14 bytes, Ego_r.pdb) and CIsSameItemType (engine/CCategory.h, Ego_r.pdb layout {ItemType, OnlyFindNonFullSlots}); the local models
// of both are gone.
// Shape: size TU (ebp frame and `push dword ptr [ebp+..]` are the /O1 encodings).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCATEGORY_ITEMQUERIES
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#define FABLE_RETAIL_STLP_FIND_IF_RANDOM_ACCESS
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
class CThing;                                   // engine/CCategory.h's PASS22 class names CIntelligentPointer<CThing>
#include "engine/CCategory.h"
#include "stlport/fable_algorithm.h"

template const NInventory::CItem* FABLE_FASTCALL std::find_if(const NInventory::CItem*, const NInventory::CItem*, CIsSameItemType);
