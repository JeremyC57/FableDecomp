# Hand chunk 883 (CTCInventoryMap .. CTCInventoryMap): 0 of 1 byte-exact

## What matched (0)
None. The one row is parked with the call sequence, size, prologue, four action groups, position phase, def/appearance
releases and final loop byte-identical.

## Parked for Fable 5.1 (1)
- 006096e0 CTCInventoryMap::ConstructLogbookListPC DIFFER(12832v12832): 17 of 591 call segments differ only in c2 register / const-0 piece choices -- count prologue zero (retail `xor ebx,ebx` before the CategoriesList clear call and 7 counter stores through ebx; ours zero in eax after the call, ebx cost 600 in c2prio), books-loop icon branch (retail subtitleText.Info in ebx, ours the manager piece prio 54 > Info 34), books-loop exit (retail carries a zero ebx to `push ebx`), COMBAT ring release `cmp [eax],ecx` vs `cmp [eax],0`, one extra `mov r,r` in the tail loop, frame 0x124 vs 0x11C (spill slots); first differing byte 0x0060AFA1 | tried: counters before clear (p13) / before viewport phase (p18), separate `it` decl (p14), chained zero init (p15), unnamed sprite cast (p16), orientation in its own block (p17), `*book` in place of `index` (p10), subtitleText declared early (p09), /d2inlT70..T74 and /d2inlS (T71 = T72 is the only budget giving 590 calls) | next: c2prio --trace on block 295 (the count prologue) for a source form that splits the counters' const-0 piece into one ebx web (e.g. the counters as an array or struct of 7 ints zeroed together), then the icon-branch manager piece
  Device: `/d2inlT71` (per-row flag; plain C++ inlines the count loop's `++it` that retail calls at 0x0047B3E0).

## Skipped
None.

## Label corrections
- 0x006096E0 catalog `_global::ConstructLogbookScreen` is descriptive: the PDB name is `CTCInventoryMap::ConstructLogbookListPC` (LABELS.tsv).
- 0x006166E0 landed as `CTCCarryable_OnKill_006166e0.cpp`: it is the CategoriesList map's `_Rb_tree::clear` (manifest says RbTree_Clear).
- 0x00615820 landed with `CUnproven*` types: it is the same map's member-template `find<ECategory>`.
- 0x0047C6A0 was bound as `Detach@CTCCarryable` by a reloc oracle while grading: the manifest's `_Rb_tree<ECategory, pair<..., vector<CHeroLogBookEntry>>>::_M_erase` is right.
- Review: the differ source declares an explicit specialization of the CategoriesList tree's `clear()` (`template<> void _Rb_tree<...>::clear();`) so the call stays out of line as in retail; check that this is acceptable or replace it with a fable_map.h opt-in at landing.
