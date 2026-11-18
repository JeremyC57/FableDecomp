# NF92 header overlays (round 7, q11206..q11224)

Each overlay is the current repo header (after NF91's landing at bacd85eae1) plus NF92 opt-in blocks. Each block is guarded by a
member-specific macro. Base copies are in ../orig/ and are byte-equal to rebuild/include as of hand-back.

Checks:
- Token identity: tokid.sh, which preprocesses up to 4 landed includers per header with and without the overlay, no opt-in defined.
  All IDENTICAL (work/tokid_final.txt).
- Asserts: pin_asserts.py was run on every overlay. 1 pin in engine/CTCFish.h; 0 elsewhere. No FABLE_STATIC_ASSERT moved.

| Header | Opt-in (NF92) | Adds | Rows |
|---|---|---|---|
| engine/CCategory.h | FABLE_RETAIL_CCATEGORY_SELECTHIGHLIGHTEDITEM | declaration of SelectHighlightedItem(bool&) | 0x005BD9B2 |
| engine/CModeDragonBase.h | FABLE_RETAIL_CMODEDRAGONBASE_ONDEACTIVATE_INLINE (with _MODEBASE and NF90's CMODEBASE_ONDEACTIVATE_INLINE) | the one inline body of NCreatureMode::CModeDragonBase::OnDeactivate, after the asserts | 0x008E72B0, 0x008E74F0 |
| engine/CTCFish.h | FABLE_RETAIL_CTCFISH_MOSTRECENTFISHWEIGHT | public static float MostRecentFishWeight (0x013BAE80) | 0x005BEDCB |
| engine/CTCInventoryItem.h | FABLE_RETAIL_CTCINVENTORYITEM_ISTONOTPERSISTUNTILQUESTCOMPLETED | `bool IsToNotPersistUntilQuestCompleted() const;` (0x005D9270) | 0x005BEDCB |
| engine/CTCInventoryItem.h | FABLE_RETAIL_CTCINVENTORYITEM_ISHEROTITLE | `static bool __fastcall IsHeroTitle(long defIndex);` (0x005D98F0) | 0x005BF32D |
| engine/CTCInventoryItem.h | FABLE_RETAIL_CTCINVENTORYITEM_INVENTORYCATEGORY_MEMBER | declaration of the const GetInventoryCategory() (0x005D91F0) | blocked draft work/005bf654_blocked.cpp only |
| engine/CTCInventoryTrade.h | FABLE_RETAIL_CTCINVENTORYTRADE_ADDITEMTOINVENTORY | the two AddItemToInventory overload declarations (slots 80 / 81) | 0x0059C6DD, 0x0059C710 |
| engine/NInventory_CTCInventoryBase.h | FABLE_RETAIL_NINVENTORY_CITEM_GETINVENTORYITEMDEF | `#include "engine/CDefPointer.h"` + `class CInventoryItemDef;` at file scope, then the private declaration `CDefPointer<const ::CInventoryItemDef> GetInventoryItemDef() const;` (0x005BD82E) | 0x005BDE50 |
| engine/NInventory_CTCInventoryBase.h | FABLE_RETAIL_NINVENTORY_CITEM_GETMAXIMUMNUMBEROFITEMS | `long GetMaximumNumberOfItems() const;` (0x005BD931) | 0x005BDDE4 |
| engine/NInventory_CTCInventoryBase.h | FABLE_RETAIL_CTCINVENTORYBASE_ADDINVENTORYITEM | private AddInventoryItem(const CItem&) declaration (0x005BF4A3) | 0x005BF4A3 |
| engine/CThing.h | FABLE_RETAIL_CTHING_DTOR | protected virtual ~CThing() declaration | 0x004C8310 |
| stlport/fable_containers.h | FABLE_RETAIL_STLP_LIST_REMOVE_NOINLINE | STLport _list.c remove(const _Tp&) body, noinline (excluded with REMOVE_DECL) | 0x004C8050 |
| stlport/fable_containers.h | FABLE_RETAIL_STLP_BITSET_RESET_DECL | bitset::reset(size_t) declared only (out of line 0x004CC0D0) | 0x004C9840 |
| stlport/fable_containers.h | FABLE_RETAIL_STLP_BITSET_RESET | bitset::reset / _Unchecked_reset inline (excluded with RESET_DECL) | 0x004C8310 |

NF91 overlap:
- NF91's overlay was read before any NInventory / CCategory / CTCInventoryBase member was added, and no member is declared twice.
- NF91's landed blocks are not repeated: CTCInventoryTrade TRADECATEGORY_BODY, NInventory RESURRECTIONITEM, and CHeroTitleDef TYPENAME
  (the CHeroTitleDef.h overlay was dropped as identical to the repo).

Rebase on bacd85eae1:
- CTCInventoryTrade.h and NInventory_CTCInventoryBase.h were merged with diff3, with no conflicts.
- orig/ was refreshed and asserts re-pinned (0 new).
- The NInventory / trade rows were regraded (logs/batch5.log, all PASS).

Added lines (non-comment `>` lines of `diff orig/<h> hdr/<h>`) for the NInventory / CCategory overlays, as the brief asks:

```
engine/CCategory.h
#ifdef FABLE_RETAIL_CCATEGORY_SELECTHIGHLIGHTEDITEM
 void SelectHighlightedItem(bool& close_input_process);                                        // 0x005BD9B2
#endif
engine/NInventory_CTCInventoryBase.h
#ifdef FABLE_RETAIL_NINVENTORY_CITEM_GETINVENTORYITEMDEF
#include "engine/CDefPointer.h"
class CInventoryItemDef;
#endif
#ifdef FABLE_RETAIL_NINVENTORY_CITEM_GETINVENTORYITEMDEF
private:
    CDefPointer<const ::CInventoryItemDef> GetInventoryItemDef() const;       // 0x005BD82E
public:
#endif
#ifdef FABLE_RETAIL_NINVENTORY_CITEM_GETMAXIMUMNUMBEROFITEMS
    long GetMaximumNumberOfItems() const;               // 0x005BD931
#endif
#ifdef FABLE_RETAIL_CTCINVENTORYBASE_ADDINVENTORYITEM
private:
 bool AddInventoryItem(const CItem& item);                                                    // 0x005BF4A3
public:
#endif
```
(The CTCFish.h diff also shows the pinned assert `FABLE_STATIC_ASSERT_AT(26, sizeof(CTCFish) == 0x1c);`, the pin_asserts rewrite of
the unmoved assert.)

New macro names checked against rebuild/src and rebuild/tests with `grep -rlw`; none is used by any landed row: STLP_LIST_REMOVE_NOINLINE,
CTHING_DTOR, CMODEDRAGONBASE_ONDEACTIVATE_INLINE, STLP_BITSET_RESET.
