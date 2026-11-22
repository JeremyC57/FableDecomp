# NF273 overlay headers

## engine/CCreatureAction_UnsheatheItemFromInventory.h (entry 1, row 0x006A0E50)
- Base: the repo working tree copy (orig/engine/CCreatureAction_UnsheatheItemFromInventory.h, byte-equal to rebuild/include at copy time;
  re-compared at hand-back: see NOTES.md).
- Added (add-only, inside the retail branch `#else` of `#ifndef FABLE_RETAIL_CCREATUREACTIONBASE`, under `public:`, right after the second
  constructor declaration): `#ifdef FABLE_RETAIL_CCREATUREACTION_UNSHEATHE_COPYCTOR` -> the declaration
  `CCreatureAction_UnsheatheItemFromInventory(const CCreatureAction_UnsheatheItemFromInventory& other);` with a two-line comment.
  Evidence: Ego_r.pdb 0x006C57A0 + FableWin.pdb 0x02211700 ??0CCreatureAction_UnsheatheItemFromInventory@@QAE@ABV0@@Z (public); retail body
  0x006A0E80 (landed row), bound so in reloc_bindings (witness 0x006A0E50). Declaration only: no slot, no layout change. Without it the
  row's `new T(*this)` would expand an implicit copy constructor.
- Default path: token-identical (the new macro is defined only by rows/006a0e50.cpp and rows/006a0e50_test.cpp; grep of rebuild/: no hit).
- pin_asserts.py: pinned 1 (the size assert below the insertion is now FABLE_STATIC_ASSERT_AT(44, ...), so its typedef name is unchanged).
- Landed includers (direct): 0x005CA68E, 0x006666A0, 0x00694430, 0x0069F0F0, 0x006A0150, 0x006DF800, 0x006DF860, 0x006DEEE0; through
  CCreatureAction_UnsheatheItemFromInventoryWithHasteAction.h / CCreatureAction_UnsheatheToShowToSheatheTrophy.h also 0x0069FCD0 (+ test),
  0x006A02B0, 0x008CDD40. None defines the new macro.
- Whole-tree equiv_check (tools/eq.sh: repo rebuild/include v work/tree_new = the repo tree with this one pinned file replaced; every landed
  source and test): 57,125 rows checked, 0 differ, DONE 0 (evidence/equiv_e1.log). orig/ re-compared with the repo after the run and at hand-back: equal.
