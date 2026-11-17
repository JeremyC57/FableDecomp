# NF90 header overlays (round 7, slice q11178..q11189)

Each overlay adds `#ifdef` opt-ins only. Each opt-in is named after the member it serves.

**Base copies.** The base copies in `../orig/` are the repo headers as they now stand, after the NF89 landing (0cc6bc1018); every one
equals the repo header byte for byte (`cmp`).

**Rebase onto NF89's landing.** The coordinator landed NF89 during this run, so `CCategory.h` and `NInventory_CTCInventoryBase.h` were
rebased onto the new repo text:
- I ran a three-way `diff3 -m` (mine / old base / repo), took the repo side of each conflict, and re-added only my blocks the repo
  lacks.
- My duplicate copies of `FABLE_RETAIL_CCATEGORY_ADDITEM` and `FABLE_RETAIL_CCATEGORY_GETPDEF` were dropped: those blocks are repo text
  now (NF89 copied them verbatim), so each exists once.
- My `engine/_quarantine/CInventoryCategoryDef.h` overlay was byte-identical to the landed repo header (NF89 copied
  `FABLE_RETAIL_CINVENTORYCATEGORYDEF_PARENTDEF` and pinned its 11 asserts), so it was deleted along with its base copy.
- `diff` of each rebased overlay against the repo shows additions only. `pin_asserts.py` pinned 0.

**Token identity.** Without its macros, each overlay preprocesses to the same token stream as the repo header. `../tokid.log` runs
`cpp -P` on landed includers with and without the overlay: the tokid.sh samples, plus, before the rebase, includers that define the
enclosing branch macros ALL_ITEMS_CHAIN_PASS22, CMODEBASE_VIRTUALS, CMODEDRAGONBASE_VSLOTS, STLP_MEM_FUN_REF_ORIGINAL_PASS22 and
CTCINVENTORYBASE_HIGHLIGHTEDITEM. The last block, after the rebase, gives 16/16 IDENTICAL.

**New macro names.** Every new macro name was grepped across `rebuild/` first; none was in use.

| Overlay | Opt-in macro | Rows using it | What it adds | Asserts pinned |
|---|---|---|---|---|
| `engine/CCategory.h` | `FABLE_RETAIL_CCATEGORY_SETSELECTEDITEMINDEX` | 0x005BF060, 0x005BF5A4 | `void SetSelectedItemIndex(long);` (Ego_r 0x0061F2FF / FableWin, retail 0x005BC479) | 0 |
| | `FABLE_RETAIL_CCATEGORY_UPDATEITEMSTATSFORTHING` | 0x005BF16E | `void UpdateItemStatsForThing(const CThing&);` (Ego_r 0x00621F4C / FableWin) | 0 |
| | `FABLE_RETAIL_CCATEGORY_ADDINVENTORYITEM` | 0x005BF103 | `bool AddInventoryItem(const CItem&);` (Ego_r 0x00621EE3 / FableWin) | 0 |
| | `FABLE_RETAIL_CCATEGORY_ADDITEM_THING` | 0x005BF5A4 | `bool AddItem(CThing&, bool, long);` (Ego_r 0x0062239E / FableWin) | 0 |
| | `FABLE_RETAIL_CCATEGORY_GETHIGHLIGHTEDITEM` | 0x005BEAF5 | `long GetHighlightedItem() const;` (Ego_r 0x0061F7B1 / FableWin, retail 0x005BC99C) | 0 |
| `engine/NInventory_CTCInventoryBase.h` (class NInventory::CItem) | `FABLE_RETAIL_NINVENTORY_CITEM_ISSLOTFULL` | 0x005BF19C, 0x005BF060, 0x005BF103, 0x005BF5A4 | `bool IsSlotFull() const;` (Ego_r 0x00620B75 / FableWin, retail 0x005BDE50) | 0 |
| | `FABLE_RETAIL_NINVENTORY_CITEM_UPDATEITEMSTATS` | 0x005BF16E | `void UpdateItemStats(const ::CThing&);` (Ego_r 0x00621B5D / FableWin, retail 0x005BEDCB) | 0 |
| | `FABLE_RETAIL_NINVENTORY_CITEM_ADDITEM` | 0x005BF060 | `void AddItem(long, long, long);` (Ego_r 0x00621A1C, retail 0x005BEC92) | 0 |
| | `FABLE_RETAIL_NINVENTORY_CITEM_ADDINVENTORYITEM` | 0x005BF103 | `void AddInventoryItem(const CItem&);` (Ego_r 0x00620B0A, retail 0x005BDDE4) | 0 |
| | `FABLE_RETAIL_NINVENTORY_CITEM_ADDITEM_THING` | 0x005BF5A4 | `void AddItem(::CThing&, long);` (Ego_r 0x00622353, retail 0x005BF556) | 0 |
| (class CTCInventoryBase, default branch, after HIGHLIGHTEDITEM) | `FABLE_RETAIL_CTCINVENTORYBASE_GETALLSELECTEDITEMS` | 0x005BEAF5 | `protected: void GetAllSelectedItems(std::vector<long>&) const; public:` (Ego_r 0x006213AB, IBE) | 0 |
| `engine/CTCCreatureModeManager.h` | `FABLE_RETAIL_CMODEBASE_ONDEACTIVATE_INLINE` (with CMODEBASE_VIRTUALS) | 0x00739E60, 0x008E74F0 | After the class: `inline void CTCCreatureModeManager::CModeBase::OnDeactivate() { Active = false; }` -- the one definition of the body (q11189). | 0 (its assert was already `_AT`) |
| `engine/CModeDragonBase.h` | `FABLE_RETAIL_CMODEDRAGONBASE_MODEBASE` | 0x008E74F0 | An alternative class body: `NCreatureMode::CModeDragonBase : public CTCCreatureModeManager::CModeBase` with virtual GetAnimFilter / OnActivate / OnDeactivate (declarations) and the Ego_r members Filter (CAnimFilterOfGroup, +0x1C) / PTarget (CIntelligentPointer<CThing>, +0x28), public as in the generated layout; the existing text (default and VSLOTS branches) becomes the `#else`. The `holder` / `active` asserts are wrapped in `#ifndef` the macro, and the `__vftable` assert's `#ifndef FABLE_RETAIL_CMODEDRAGONBASE_VSLOTS` became `#if !defined(..._VSLOTS) && !defined(..._MODEBASE)` (same preprocessing without the new macro). | 6 (`FABLE_STATIC_ASSERT` -> `_AT(46..53)`) |
| `engine/CThing.h` | `FABLE_RETAIL_CTHING_INITTC` | 0x004C9D60 | After the `AddTC` declaration: `private: void InitTC(CTCBase*); public:` (Ego_r 0x004C94D0 / FableWin 0x01CF5BD0 ?InitTC@CThing@@AAEXPAVCTCBase@@@Z, retail 0x004C73E0). Declaration only. | 0 (its asserts were already `_AT`) |
| | `FABLE_RETAIL_CTHING_TCSTOBEREMOVED` | 0x004C9D60 | Next: `private: static std::list<std::pair<CIntelligentPointer<CThing>, CTCBase*> > TCsToBeRemoved; public:` (Ego_d.pdb PUB32 ?TCsToBeRemoved@CThing@@0V?$list@..@std@@A, private static; Ego_r / FableWin name its dynamic initializer and atexit destructor; retail 0x013B8A24, initializer 0x0121A440). Declaration only; needs engine/CIntelligentPointer.h. | 0 |
| `stlport/fable_containers.h` (bitset) | `FABLE_RETAIL_STLP_BITSET_SET_DECL` | 0x004C9D60 | `bitset<_Nb>& set(size_t n);` declared only, after `test` (STLport 4.6 _bitset.h; retail calls the bitset<274> instance 0x004CC090 out of line from AddTC while it expands `test`). | 0 (no asserts in the file) |
| `stlport/fable_algorithm.h` | `FABLE_RETAIL_STLP_CONST_MEM_FUN_REF` | 0x005BD96B | STLport 4.6.2 `const_mem_fun_ref_t` and the const `mem_fun_ref` overload (FableWin.pdb names the instances of this TU); closes `namespace std` to include stlport/fable_containers.h for `unary_function`, then reopens it. | 0 (no asserts in the file) |

Withdrawn: an earlier `stlport/fable_containers.h` overlay (pair copy / `_Construct` forced inline for 0x004F9934) was replaced by the per-row
flag `/d2inlS22` (EXTRA_RULES inline-budget preference 2) and deleted. The `stlport/fable_containers.h` overlay above is a new copy that holds
only the bitset `set` declaration. The CThing.h and fable_containers.h overlays were checked with tokid.sh after these edits (8 includers,
IDENTICAL; `../tokid.log`, last block).

Used from the repo (no overlay copy any more): `FABLE_RETAIL_CCATEGORY_ADDITEM` (rows 0x005BF060), `FABLE_RETAIL_CCATEGORY_GETPDEF`
(0x005BE85B, NF90's text) and `FABLE_RETAIL_CINVENTORYCATEGORYDEF_PARENTDEF` (0x005BE85B, NF90's text, quarantined generated header).
