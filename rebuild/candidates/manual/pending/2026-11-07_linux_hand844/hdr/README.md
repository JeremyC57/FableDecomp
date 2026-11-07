# hdr/ -- header overlay of chunk 844 (opt-in branches only; default paths textually unchanged)

Two files, both the chunk 839 overlay (`pending/2026-11-07_linux_hand839/hdr/`) carried forward unchanged, because the
parked differ/00775350.cpp (CTCShop::UpdateStock) still defines the three opt-ins. Re-diffed against rebuild/include before
hand-back (2026-11-07 12:32): the repo copies do not carry the opt-ins yet (chunk 839 is pending), and the diff is additions
only inside new `#ifdef` blocks (fable_containers.h 625a626,672; fable_map.h 162a163,185 and 241a265,268); no existing line
reflowed. No landed source defines any of the three macros (grep -rlw over rebuild/src/compiled is empty), so no includer
needed a recheck; the four matched rows of this chunk do not include either header. If chunk 839 lands first, this overlay
becomes byte-identical to the repo copies and can be dropped.

## stlport/fable_map.h -- `FABLE_RETAIL_STLP_TREE_ERASE_KEY` (chunk 839, carried)
STLport 4.6 `_Rb_tree::upper_bound`, `equal_range`, the declaration-only `erase(iterator, iterator)` (out of line in retail:
0x007775B0 for map<long,CShopItem>), `_tree.c erase(const key_type&)` (equal_range expanded in place, then `erase(first, last)`)
and `map::erase(const key_type&)`. Used by differ/00775350.cpp only.

## stlport/fable_containers.h -- `FABLE_RETAIL_STLP_BINARY_SEARCH`, `FABLE_RETAIL_STLP_RANDOM_SHUFFLE` (chunk 839, carried)
STLport 4.6 `__less(T*)`, random-access `distance` / `advance`, `__lower_bound`, `lower_bound`, `binary_search`, `__random_number`
(`rand() % n`), `__iter_swap_aux` / `iter_swap`, `random_shuffle(first, last)`. Used by differ/00775350.cpp only.

## Header debt (not changed here, see NOTES.md)
- engine/CTCInGameMenu.h: the FABLE_RETAIL_CTCINGAMEMENU branch lacks Components +0x58 (vector<NUISystem::CComponent*>),
  SaveGameIndex +0xC8, FramesToWaitToSave +0xCC, PSaveInProgress +0xD0, SaveInProgressTime +0xD8 (the generated branch has them
  as opaque bytes); differ/0047ed76.cpp keeps a local model.
- engine/NUISystem_CComponent.h slot 31 is ChangePosition and slot 43 Initialise: the chunk 757/840 model of 0x0047ED76 called
  them SetPosition / Show_slot43; the differ source now uses the PDB names.
