# NF189 header overlay

## stlport/fable_map.h (base copy: orig/stlport/fable_map.h, from the repo working tree at start)
- Added `FABLE_RETAIL_STLP_TREE_FIND_LONG_LONG_SIZE_BODY` (q4385, row 0x006D0630) after the nf184 `FABLE_RETAIL_STLP_LESS_CCHARSTRING_SIZE_BODY`
  block at the end of the file: an explicit specialization of `_Rb_tree<long,pair<const long,long>,..>::find<long>` (STLport body,
  `__declspec(noinline)`) between `#pragma optimize("s", on)` and `#pragma optimize("", on)`. Fidelity: scaffolding, not retail
  (EXTRA_RULES 2026-11-20 size-pragma scaffolding). The three checks: (1) 0x006D0630 grades RELOCATION_MATCH with and without the macro,
  and its defined-symbol set is the same both ways; only the find<long> COMDAT changes (21 -> 20 bytes); (2) owner row 0x005937E1 sets
  `#pragma optimize("s", on)` itself; (3) the comment says the block ends with `optimize("", on)` and 0x006D0630 carries no row-head
  optimize pragma.
- Additive, member-specific macro; the default path is token-identical (the block is inside `#ifdef`, after the last
  FABLE_STATIC_ASSERT; pin_asserts pinned 0). Landed includers: 975 rows under rebuild/src/compiled include stlport/fable_map.h; no landed
  row defines the new macro, so none changes. Read the other agents' overlays first: NF188 / NF190 do not overlay stlport/fable_map.h.
- Added `FABLE_RETAIL_STLP_MAP_FIND_ULONG_LONG_SIZE_BODY` (q4386, row 0x005D66BA) right after it: an explicit specialization of
  `map<unsigned long,long>::find` (STLport 4.6 _map.h body `return _M_t.find(__x);`, inline, callers still expand it) between
  `#pragma optimize("s", on)` and `#pragma optimize("", on)`. Fidelity: scaffolding, not retail. Checks: (1) 0x005D66BA grades
  RELOCATION_MATCH with and without the macro; only the map::find COMDAT changes (22 -> 20 bytes; every other COMDAT byte-equal);
  (2) owner row 0x005D8548 sets `#pragma optimize("s", on)` itself; (3) the comment says the block ends with `optimize("", on)`.
  0x005D66BA does carry row optimize pragmas (`("y", off)`, `("s", on)`), but it includes stlport/fable_map.h BEFORE them, so the
  header's reset happens first and the row's own pragmas still govern its body (the row grades the same either way). Flag for the
  coordinator: this is the one place the "no row-head optimize pragma" condition is met by include order rather than by absence.
- Default path token-identical; no landed row defines either macro; pin_asserts pinned 0 (no FABLE_STATIC_ASSERT after the insertions).
