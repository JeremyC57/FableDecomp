// std::__find_if(NInventory::CItem* first, NInventory::CItem* last, CIsSameItemType pred, const random_access_iterator_tag&) -- STLport random-access find_if over a category's items: the first item the CIsSameItemType predicate accepts, else last
// Leaf: ??$__find_if@PAVCItem@NInventory@@VCIsSameItemType@@@std@@YIPAVCItem@NInventory@@PAV12@0VCIsSameItemType@@ABUrandom_access_iterator_tag@0@@Z
// Retail: Fable.exe @ 0x005C176A, 174 bytes, `ret 0Ch`: __fastcall (/Gr template TU) -- first in ecx, last in edx (spilled to
// the frame word [ebp-4]), the 8-byte CIsSameItemType {ItemType, OnlyFindNonFullSlots} BY VALUE and the unused tag on the
// stack. Only caller: the NON-const find_if forwarder @ 0x005C190B (landed), used by NInventory::CCategory::RemoveItem
// 0x005BF237 with CIsSameItemType(type, false). Byte twin of the const instantiation 0x005C1818.
// Callees: CIsSameItemType::operator()(const NInventory::CItem&) const @ 0x005C150B (out of line: `push esi ; lea ecx,[ebp+8] ;
// call` -- the predicate is addressed in its own by-value argument slot).
// Body: STLport 4.x _algo.c __find_if (four predicate calls per trip, the 3/2/1 remainder switch); size TU (`push 14h ; pop
// ebx` stride, `idiv`, ebp frame for the slot-relative `lea`).
// Name fix (NF100, round 9, q11237 follow-on): the old row instantiated the template on engine/CItem.h's global CItem and a local
//   CIsSameItemType (own leaf ??$__find_if@PAUCItem@@..., binding ??RCIsSameItemType@@QBE_NABUCItem@@@Z for 0x005C150B); it now sits on
//   NInventory::CItem (engine/NInventory_CTCInventoryBase.h) and the engine/CCategory.h ITEMQUERIES CIsSameItemType with the
//   stlport/fable_algorithm.h random-access __find_if body (recipe of the landed 0x005C0653), so it defines the spelling its landed caller
//   0x005C190B binds and binds 0x005C150B as ??RCIsSameItemType@@QBE_NABVCItem@NInventory@@@Z (Ego_r.pdb 0x006237C7 + FableWin.pdb 0x02376623).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCATEGORY_ITEMQUERIES
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#define FABLE_RETAIL_STLP_FIND_IF_RANDOM_ACCESS
#define FABLE_RETAIL_STLP_FIND_IF_RANDOM_ACCESS_BODY
#include "engine/NInventory_CTCInventoryBase.h"
#include "stlport/fable_algorithm.h"

template NInventory::CItem* FABLE_FASTCALL std::__find_if(NInventory::CItem*, NInventory::CItem*, CIsSameItemType,
    const std::random_access_iterator_tag&);
