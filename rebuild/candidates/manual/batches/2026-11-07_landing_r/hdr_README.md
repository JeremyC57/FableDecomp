# hdr/ -- header overlay of chunk 839 (opt-in branches only; default paths textually unchanged)

Both files were copied fresh from rebuild/include at the moment of editing and re-diffed against rebuild/include before
hand-back (only added lines inside new `#ifdef` blocks; no existing line reflowed). Neither macro is defined by any landed
source (grep -rlw over rebuild/src/compiled is empty); the two sibling regrades in SIBLINGS.tsv stay RELOCATION_MATCH.

## stlport/fable_map.h -- `FABLE_RETAIL_STLP_TREE_ERASE_KEY`
Row 0x00775350 CTCShop::UpdateStock (parked). STLport 4.6 `_Rb_tree::upper_bound`, `equal_range`, the declaration-only
`erase(iterator, iterator)` (out of line in retail: 0x007775B0 for map<long,CShopItem>) and `_tree.c erase(const key_type&)`
(`equal_range` expanded in place, then `erase(first, last)`), plus `map::erase(const key_type&)`. Retail expands the
equal_range chain at both of UpdateStock's erase-by-key sites and passes the two iterators by value to 0x007775B0.

## stlport/fable_containers.h -- `FABLE_RETAIL_STLP_BINARY_SEARCH`, `FABLE_RETAIL_STLP_RANDOM_SHUFFLE`
Row 0x00775350. STLport 4.6 `__less(T*)`, random-access `distance` / `advance`, `__lower_bound`, `lower_bound`, `binary_search`
(retail's listed-types probe: `sar 2` distance, `sar 1` half, `lea first,[middle*4+4]`, `len - half - 1`), and
`__random_number` (`rand() % n`, _STLP_NO_DRAND48: `call rand; cdq; idiv`), `__iter_swap_aux` / `iter_swap`,
`random_shuffle(first, last)`. Both chains are expanded in place in retail (the row's residue is that VC7.1's budget runs out
before them here, see NOTES.md).
