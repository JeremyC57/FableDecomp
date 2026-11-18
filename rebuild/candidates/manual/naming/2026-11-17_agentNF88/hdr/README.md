# NF88 header overlays (round 7, slice q11150..q11162)

Each overlay adds `#ifdef` opt-ins only, named after the member they serve. Base copies (the repo headers as of this run) are in `../orig/`;
at hand-back all four base copies still equal the repo headers byte for byte (`cmp`). Without its macros each overlay preprocesses to the
same token stream as the repo header: `../tokid.log` runs `cpp -P` on landed includers with and without the overlay (tokid.sh samples: 4
CPersistTraits.h, 3 CTCInventoryItem.h, 2 fable_algorithm.h and 1 CCategory.h includer; plus 4 includers that define
`FABLE_RETAIL_STLP_FIND_IF_INPUT`, the branch the fable_algorithm.h opt-in sits in): all IDENTICAL. No landed row defines
`FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22` (the branch the CCategory.h opt-in sits in). `pin_asserts.py` was run after every
edit; what it pinned is listed per header. The new macro names were grepped across `rebuild/`: no existing use.

| Overlay | Opt-in macro | Rows using it | What it adds | Asserts pinned |
|---|---|---|---|---|
| `engine/CPersistTraits.h` | `FABLE_RETAIL_CPERSISTTRAITS_HEROSTATINCREASEDATA_BINARYOUT` | 0x006DC450 | After the NF81 CComboMultiplierData opt-in, the same shape for CHeroStatIncreaseData: `template<> class CPersistTraits<CHeroStatIncreaseData>` with DefaultVal, the value-initialising ctor and the writer declaration `TransferBinaryOut(CDataOutputStream&, const CHeroStatIncreaseData&)` (Ego_r 0x0058AD90 / FableWin 0x01F0B550; retail 0x006DB0F0). Declarations only. | 0 (no asserts in the file) |
| `engine/CCategory.h` | `FABLE_RETAIL_CCATEGORY_ITEMQUERIES` (inside the existing PASS22 `NInventory::CCategory` block, i.e. with `FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22`) | 0x005C08C0, 0x005C0653, 0x005BE839, 0x005BE8B1, 0x005BF1D7, 0x005BEAD5, 0x005BE8D3, 0x005BF30D, 0x005BF2ED, 0x005BDF08 | Three parts under the one macro: (1) forward declarations of `NInventory::CPerItemData` / `CIsInventoryCategory`; (2) in the class, the Ego_r / FableWin member declarations GetBoughtPrice 0x005BE839, GetPerItemDataForItem 0x005BE8B1, IsSpaceInCategoryForItem 0x005BF19C, GetAmountOfItemsContainable 0x005BF1D7, GetNumberOfItemsOfTypeInCategory 0x005BDE96 (public const), the private const GetFirstEmptySlot 0x005BD96B / GetSlotWithItemsOfType 0x005BE85B, and `friend class CIsInventoryCategory` (the predicate reads the private GlobalDefIndex inline; no CCategory accessor exists in either PDB), then `public:` again; (3) after the class, the two find_if predicates with their Ego_r layouts: `NInventory::CIsInventoryCategory {Index}` (inline ctor and `operator()`) and the global `CIsSameItemType {ItemType, OnlyFindNonFullSlots}` (inline ctor; `operator()` declared, out of line at 0x005C150B). No layout change to CCategory. | 1 (`FABLE_STATIC_ASSERT(sizeof(CCategory)==44)` -> `FABLE_STATIC_ASSERT_AT(67, ...)`) |
| `stlport/fable_algorithm.h` | `FABLE_RETAIL_STLP_FIND_IF_RANDOM_ACCESS` (+ nested `_BODY`), inside the existing `FABLE_RETAIL_STLP_FIND_IF_INPUT` block before the find_if definitions | 0x005C08C0 (decl), 0x005C0653 (body); the callers use the decl through FIND_IF_CALLED_PASS23 | STLport 4.6 _algo.c random-access `__find_if(first, last, pred, const random_access_iterator_tag&)` (FABLE_FASTCALL): declaration only, or with `_BODY` the four-per-trip loop and the 3/2/1 remainder switch (the body the landed 0x005C0653 already had, moved unchanged). find_if picks it for pointer iterators over the input-iterator overload. | 0 (no asserts in the file) |
| `engine/CTCInventoryItem.h` | `FABLE_RETAIL_CTCINVENTORYITEM_GETBOUGHTPRICE` (nested after `_CONNECTTOINVENTORY`) | 0x005D8E70 | `long GetBoughtPrice() const;` (Ego_r 0x00559DD0 / FableWin 0x01E009B0 ?GetBoughtPrice@CTCInventoryItem@@QBEJXZ, retail 0x005D8E70). Declaration only. | 0 (its asserts were already `_AT`) |

Notes:
- The CCategory.h PASS22 block names `CIntelligentPointer<CThing>` without declaring CThing; rows that include it without CTCBase.h first
  forward-declare `class CThing;` (0x005C08C0, 0x005C0653). Not changed in the header (a body change, not an opt-in).
- NF87 also overlays `engine/NInventory_CTCInventoryBase.h`; NF88 does not touch that header (every CTCInventoryBase member these rows define
  was already declared there: GetBoughtPrice unconditionally, GetPerItemDataForItem / GetAmountOfItemsContainable / IsSpaceInInventoryForItem /
  GetNumberOfItemsOfTypeInInventory under PERITEMDATA / CONTAINABLE / BUILDINGTHING / SELECTEDOWNED). NF87's `FABLE_RETAIL_NINVENTORY_CCATEGORY_ACCESSORS`
  block declares a members-only NInventory::CCategory when ALL_ITEMS_CHAIN_PASS22 is NOT defined, so it cannot meet these rows (which all
  define it).
