# Chunk 879 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- VC7.1's inline budget is spent in source order: forcing one early site out of line (`#pragma inline_depth(0)`) frees
  budget, so later sites that retail calls get expanded. The effect cascades, so an inline-budget residue in a large
  function is a whole-function quantity, not a per-site one (0x00502E90: outlining P9 makes P12 and P14 inline).
- A preceding loop written `for (...; ...; ++it)` raises VC7.1's inline willingness at later call sites. The same loop with
  `++it;` as the last statement of its body does not (0x00502E90 phase 1 -> phase 2 push_back / list::size stay out of line
  as retail).
- `#pragma inline_depth(0)` around a statement also outlines the trivial inline accessors inside it (CDefPointer /
  CIntelligentPointer operator->). Hoist those operands into named locals before the pragma'd statement.
- Retail keeps discarded accessor calls (`villager->GetHomeBuilding();`, `IsUnderScriptSupervision()`,
  `PeekPSimDefinition()`) as evaluations: deleting them changes block layout and makes the result worse (0x00502E90 b02).
- `float total = a + b + c; float n = 1.0f / total;` with a NAMED total gives retail's `fld 1.0 ; fdiv st(1)`. The unnamed
  expression gives `fdivr` (0x00502E90 phase 0).
- STLport map::erase(key) expanded in place with equal_range / erase(range) called (0x00514730 / 0x0051AF80) needs
  `__forceinline` on both `map::erase(const K&)` and `_Rb_tree::erase(const K&)`. Forcing only the tree level leaves
  map::erase out of line.
- /d2inlS had no effect on this row, and /d2inlT is non-monotonic over 13 KB. Bisecting T is not reliable for whole-function
  budget residues.

Review at landing:
- 0x00502E90 has 54 grades. Its TU copies CreateRelevantPersonAndUpdateStats (0x004FF750) verbatim, because VC7.1 expands it
  at the work-place sites. The landed 0x004FF750 source and the draft must stay in sync.
- Already-landed sources with wrong names: see NAME_FIXES.tsv.
  - Several are anonymous pad-struct `__fastcall` stand-ins that should be retyped to their real classes: AssignTarget*,
    CountReferences, GetMovementMode, SortTreeRecursively, and CRegionManager::PopDontPopulateNextLoadedRegion.
  - The 0x00662000 file is named GetMovementMode, but its function is spelled CountReferences.
- 0x0058AF10 was never attempted. Assign it to a fresh agent with the resume notes in NOTES.md.
