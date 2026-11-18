# NF89 header overlays (round 7, q11163..q11176)

Six overlaid headers (five repo headers plus `engine/_quarantine/CInventoryCategoryDef.h`, a verbatim copy of NF90's overlay), 15 new
opt-in macros of mine plus two of NF90's taken verbatim (17 in all; see the merge note). The base copies in `../orig/` are the repo headers after the
nf88 landing (63faa94001); at hand-back all six still equal the repo headers byte for byte (`cmp`). Every edit is an `#ifdef` opt-in with a
member-specific macro; no existing line was changed. None of the macro names occurs in `rebuild/include` or `rebuild/src` (grep -w), and
every one is defined by at least one row in `../rows/`.

**Token identity** (`../tokid.log`, `../tokid.sh`): `cpp -P -D_MSC_VER=1310`, with and without `-I hdr`, whitespace-normalised. 46 of 46
IDENTICAL (re-run at hand-back): up to 4 landed includers per header (the quarantine header has one, 0x005C0121), 6 landed rows that define `FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22` (the
branch the CCategory.h opt-ins sit in), and every file in `../rows/` with its NF89 opt-in `#define` lines stripped.

**Asserts.** `pin_asserts.py /home/user/FableDecomp/rebuild/include <dir>/hdr <header>` ran after every edit and once more over all six at
hand-back: pinned 0 in all six (CTCInventoryTrade.h and the PASS22 block of CCategory.h were already `_AT`; CCategory.h's ten unpinned
asserts sit above every edit point; CTCInventoryQuests.h / CTCInventoryExperience.h / NInventory_CTCInventoryBase.h have no assert after an
edit point; the quarantine file is NF90's text).

**Merge note for NF90 / the coordinator.** Three blocks also exist in NF90's overlay; keep exactly ONE copy of each when merging (two
copies under one macro would redeclare the member or the class). One I wrote independently; two are NF90's text, copied verbatim:
- engine/CCategory.h `FABLE_RETAIL_CCATEGORY_ADDITEM` (mine, written independently; same macro and same declaration as NF90's):
  `bool AddItem(long itemDefIndex, bool silent, long price, long count); // 0x005BF060`.
- engine/CCategory.h `FABLE_RETAIL_CCATEGORY_GETPDEF`: `CDefPointer<const CInventoryCategoryDef> GetPDef() const { return PDef; }` (NF90's
  block text and comment, verbatim).
- engine/_quarantine/CInventoryCategoryDef.h: the whole file is NF90's overlay (`FABLE_RETAIL_CINVENTORYCATEGORYDEF_PARENTDEF`), `cmp`-equal to
  NF90's copy at hand-back; I added nothing to it.
No other member of mine is in NF90's overlay (checked at hand-back against NF90's CCategory.h and NInventory_CTCInventoryBase.h:
GetItemInSlotWithIndex, GetHighlightedItemIndex (NF90's GetHighlightedItem 0x005BC99C is a different member), GetCategoryThingAssociation,
GetUniqueItemCountInCategory, HighlightNextItem, GetNumberOfSlots, GetCategoryIdentifier, GetNextOccupiedSlot, ResetInventoryAnim,
GetNumberOfSlotsForCategory).

**Not in hdr/:** the blocked draft 0x005BF77D (`../work/005bf77d_try3.cpp`) needs two more opt-ins, kept out of hdr/ because no finished row
uses them: `FABLE_RETAIL_CRESURRECTIONITEMDEF` (engine/CResurrectionItemDef.h: the retail class over CSubDefClassBase, RTTI
`ghidra_out/rtti_bases.tsv`, with StatGetTypeName from FableWin.pdb + the retail literal 0x01243C94; the three original asserts pinned `_AT`)
and `FABLE_RETAIL_CTCINVENTORYBASE_RESURRECTIONITEM` (NInventory_CTCInventoryBase.h after CREATEOBJECT:
`CThing* GetResurrectionItemInInventory();`). Both are in `../work/hdr_blocked/engine/` (put it before hdr/ on the include path to rebuild the
draft), base copy in `../work/hdr_blocked_orig/`.

## engine/CCategory.h (all inside the PASS22 `NInventory::CCategory` class, after `GetAllItemsInCategory`, before NF88's ITEMQUERIES)
| Macro | Adds | Users |
|---|---|---|
| `FABLE_RETAIL_CCATEGORY_ITEMINSLOT` | `long GetItemInSlotWithIndex(long) const;` (0x005BC9B0, Ego_r + FableWin) and inline `long GetHighlightedItemIndex() const { return HighlightedItemIndex; }` (FableWin.pdb only, absent from Ego_r = inline; retail reads +0x24 in place) | 0x005A3581 |
| `FABLE_RETAIL_CCATEGORY_CATEGORYTHINGASSOCIATION` | `CThing* GetCategoryThingAssociation() const;` (0x005BD1AE). The PASS22 twin of NF87's `!PASS22` ACCESSORS declaration in NInventory_CTCInventoryBase.h: the two blocks are mutually exclusive | 0x005BD1AE |
| `FABLE_RETAIL_CCATEGORY_UNIQUEITEMCOUNT` | `long GetUniqueItemCountInCategory() const;` (0x005BC8AC) and `void HighlightNextItem();` (0x005BD054) | 0x005BC8AC, 0x005BD054 |
| `FABLE_RETAIL_CCATEGORY_NUMBEROFSLOTS` | `long GetNumberOfSlots() const;` (0x005BD1CF, Ego_r 0x00620102 + FableWin) | 0x005BD328 |
| `FABLE_RETAIL_CCATEGORY_CATEGORYIDENTIFIER` | `EInventoryCategoryIdentifier GetCategoryIdentifier() const;` (0x005BD1B3, Ego_r); the enum comes from the NInventory_CTCInventoryBase.h block below | 0x0059B740, 0x0056E632, 0x005AEBFE, 0x0059C6B0, 0x0059CF49, 0x0059D9AD |
| `FABLE_RETAIL_CCATEGORY_ADDITEM` | `bool AddItem(long itemDefIndex, bool silent, long price, long count);` (0x005BF060, Ego_r + FableWin `QAE_NJ_NJJ`) -- NF90 has the identical block, see the merge note | 0x0059C6B0 |
| `FABLE_RETAIL_CCATEGORY_GETPDEF` | NF90's block verbatim, after ADDITEM: inline `CDefPointer<const CInventoryCategoryDef> GetPDef() const { return PDef; }` (FableWin.pdb `QBE?AV?$CDefPointer@$$CBVCInventoryCategoryDef@@@@XZ`, inline in retail) -- see the merge note | 0x005BD054 |
| `FABLE_RETAIL_CCATEGORY_GETNEXTOCCUPIEDSLOT` | after GETPDEF: `private: long GetNextOccupiedSlot(long index) const; public:` (0x005BCF61; Ego_r 0x0061FE9B + FableWin `ABEJJ@Z`, private const) | 0x005BD054 |

## engine/NInventory_CTCInventoryBase.h
| Macro | Where / adds | Users |
|---|---|---|
| `FABLE_RETAIL_CCATEGORY_CATEGORYIDENTIFIER` (with `!FABLE_RETAIL_CTCINVENTORYBASE_ORIGINAL_COMPLETE_METHODS_PASS22`) | just before the PASS22 `#include "engine/CCategory.h"`: `namespace NInventory { enum EInventoryCategoryIdentifier {...}; }`, the exact text of the COMPLETE_METHODS block's enum line (that block defines it otherwise) | as above |
| `FABLE_RETAIL_CTCINVENTORYBASE_RESETINVENTORYANIM` | after the HIGHLIGHTEDITEM block: `void ResetInventoryAnim(long);` public (0x005BCD2D, Ego_r + FableWin `QAEXJ`) | 0x005A3581 |
| `FABLE_RETAIL_CTCINVENTORYBASE_NUMBEROFSLOTSFORCATEGORY` | after RESETINVENTORYANIM: `long GetNumberOfSlotsForCategory(long) const;` (0x005BD328, Ego_r 0x00620259 + FableWin) | 0x005BD328, 0x0056E5AB |

## engine/CTCInventoryTrade.h (new blocks just before `#ifdef FABLE_RETAIL_CTCINVENTORYTRADE_BUYPOSSIBILITY`, in this order)
| Macro | Adds | Users |
|---|---|---|
| `FABLE_RETAIL_CTCINVENTORYTRADE_BUYHIGHLIGHTEDITEM` | `public: void BuyHighlightedItem();` (0x005A3581) | 0x005A3581, 0x005AEBFE |
| `FABLE_RETAIL_CTCINVENTORYTRADE_ITEMCOUNTFORCATEGORY` | `public: long GetItemCountForCategory();` (0x0059B740) | 0x0059B740 |
| `FABLE_RETAIL_CTCINVENTORYTRADE_PROCESSTRADEACTION` | `public: virtual void ProcessTradeAction();` (slot-68 override, 0x005AEBFE), `virtual bool GetIsEnabled() const;` (slot-75 override), `void SellHighlightedItem();` (0x005A3436); needs `FABLE_RETAIL_CTCINVENTORYBASE_VTABLE`; overrides add no slot | 0x005AEBFE |
| `FABLE_RETAIL_CTCINVENTORYTRADE_TRADECATEGORY` | `public: long GetCategoryIndexForTradeType();` (0x0059B6F4) and `void RemoveAllItemsFromCategory(NInventory::EInventoryCategoryIdentifier);` (0x0059CF49); needs the NInventory enum (CCATEGORY_CATEGORYIDENTIFIER or COMPLETE_METHODS) | 0x0059C6B0, 0x0059CF49, 0x0059D9AD |

## engine/CTCInventoryQuests.h
- `FABLE_RETAIL_CTCINVENTORYQUESTS_GETSELECTEDCATEGORYASSOCIATION`: right after `StatGetInterfaceType`, outside the MEMBERS block:
  `CThing* GetSelectedCategoryAssociation();` (0x0061A040; Ego_r 0x00597A90 + FableWin). Non-virtual, no layout change. User: 0x0061A040.

## engine/CTCInventoryExperience.h
- `FABLE_RETAIL_CTCINVENTORYEXPERIENCE_TRAINABLESTATTYPE`: after `GetExperienceAbilityColour`: `EHeroTrainableStatType GetHeroTrainableStatType(long button);`
  (0x0056E632; Ego_r + FableWin `QAE?AW4EHeroTrainableStatType@@J@Z`; the landed caller 0x0057229B binds it). User: 0x0056E632.

## engine/_quarantine/CInventoryCategoryDef.h
- The whole file is a verbatim copy of NF90's overlay (`FABLE_RETAIL_CINVENTORYCATEGORYDEF_PARENTDEF`: `class CInventoryCategoryDef : public
  CParentDefClassBase`, SelectEmptySlots +0x45, WrapHighlightCursor +0x46). Base copy in `../orig/engine/_quarantine/`. Merge: take NF90's.
  User: 0x005BD054 (which also defines the existing repo macro `FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE` for retail's load-before-Release order).
