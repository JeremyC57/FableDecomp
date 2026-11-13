# Chunk 3019 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (chunk 3019, 0x006D1250) A key passed by reference to STLport set find / insert stays uncaptured (forwarded in a register across an
  unrelated call, its frame word shared with a dead CDefPointer) only when find AND the whole insert chain are visible
  (FABLE_RETAIL_STLP_MAP + _MAP_ORIGINAL_INSERT_BODY + a visible find). Retail still CALLS find (0x00616580) at all three sites; where
  VC7.1's cost rule expands the visible inline find at hot sites while expanding other helpers retail also expands at similar scores,
  the device is a noinline-but-visible body (hdr opt-in FABLE_RETAIL_STLP_TREE_CONSTFIND_TEMPLATE_NOINLINE, Inline-budget line).
  Diagnose with a noinline probe header first: it isolates "visibility" from "inline decision" in two grades.
- (chunk 3019, 0x006D1811) `FrameNoticed[change > 0 ? 10 : 11] = GetFrame(); FrameNoticed[change > 0 ? 11 : 10] = 0;` evaluates the
  second test after the call (change spilled to the frame); retail's two fcom tests ahead of the call are two named index locals
  before the call statement.
- (chunk 3019) Fix residues bottom-up: the tattoo-site find order that chunk 2020 measured as "swaps this / hero globally" gave
  retail's allocation at once after the fatness tail (the last block) was right. A late block's shape changes the allocator's view of
  the whole function; re-test rejected levers after each fix.
- (chunk 3019) c2 inline cost score per callee fits sc = c1 * M * (1/f + 2) / 3 with a callee-specific M (set<long> find 1.44,
  CVectorMap LowerBound 1.69, insert_unique 1.22, _M_find 1.43) independent of the caller size -- useful to see quickly whether any
  frequency change can split two callees (here it cannot: the tattoo find site is 1.25x the morph LowerBound site's frequency).
- (chunk 3019) A `while` loop in place of a `for` drops the static frequency after it (LowerBound morph site 6.5 -> 1.5) but also
  changes allocation and frame size of the whole function; `continue` early-outs raise loop-body frequency.

Review at landing:
- 0x006D1250 needs this chunk's hdr/ (chunk 2020's overlay + stlport/fable_map.h with FABLE_RETAIL_STLP_TREE_CONSTFIND_TEMPLATE_NOINLINE).
- vland RELOC_CONFLICT for ??$find@J@ (0x00616580 here, 0x00B2EB20 in landed 0x00B2D9F0): two byte-identical retail copies of one
  template instance; decide how reloc_bindings records a symbol with two addresses.
- reloc_bindings binds 0x00616580 as `?find@CAnimSet@@QAE?AUCAnimSetIterator@@ABJ@Z` (landed 0x0075D8B0 stand-in) -> NAME_FIXES.tsv.
- The row uses an Inline-budget device (noinline find); the plain-C++ alternatives tried are listed in NOTES.md.
