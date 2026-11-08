# Chunk 879 worklog

## Extents (verified with xdis)
- 0x00502E90 CWorldMap::PopulateRegionWithVillagers: 0x00502E90..0x005063C6 = 13622 bytes (last `ret 4` 0x005063C3, int3 pad,
  next function 0x005063D0). Early `ret 4` at 0x005048ED is an inner return tail. One function.
- 0x0058AF10 CTCInventory::OpenPCOptionsMenu: `leave; ret` at 0x0058E1C3, one 17-entry jump table 0x0058E1C4..0x0058E208
  (switch on [esi] cmp 0x10 at 0x0058D8DE). Next function at 0x0058E208. 13048 bytes incl. the table. One function.

## Sources of evidence
- Ghidra decompile of 0x502E90: ghidra_out/decomp_teleport_point_scripts.c lines 1..2629.
- PDB: ?PopulateRegionWithVillagers@CWorldMap@@QAEXAAVCThing@@@Z (egor + fablewin), ?OpenPCOptionsMenu@CTCInventory@@QAEXXZ.

## Log

### 0x00502E90 progress (session 2)
- v1 compiles after overlay additions: CWorldMap.h (FABLE_RETAIL_CWORLDMAP_POPULATE decl), CMainGameComponent.h
  (FABLE_RETAIL_CMAINGAMECOMPONENT_WORLDSEED inline GetWorldSeed), CWorld.h (inline PeekDefinitionManager under _POPULATE).
- v2: body nested in `if (GetSubDef(&peopleDef)) { ... }` (retail jumps to the shared end on failure) -> seg0 fixed.
- v3: phase 1 in retail order (`!= 3` non-guard first; discarded GetHomeBuilding evaluations per arm), inline
  GetHomeBuilding/GetWorkBuilding bodies -> call sequence of phase 1 matches except list push_back inline (retail calls 0x4AC4B0).
- v5: phase-1 loop with `++it` at the end of the body (not in the for-increment): phase 2's push_back / list size sites go
  out of line exactly as retail (an extra preceding `for`-increment loop raises VC7.1's inline willingness downstream; probes e1-e17).
- v6: `float totalWeight` named -> retail `fld 1.0; fdiv st(1)`; school declared before phase 2. md 3569 / mdf 2311.
- Remaining inline residues: phase 1 list push_back (retail out of line, ours inline at every T), phase 9 CreateRelevant
  (retail out of line, ours inline; a scoped inline_depth(0) frees budget so phase 10 inlines instead), phase 12 CBasicString
  operator== (retail calls 0x004A93C0), map::erase(key) inline in retail (ours calls _Rb_tree::erase(key)).
- Tools: tools/ph.py (per-progress-segment diff/calls), tools/isites.py (unlogged inline-site summary), tools/cc.py.

### 0x00502E90 progress (session 3)
- v14: b05 (`long remaining = homeSpaces; while (remaining--)`) + erase(key) forceinline at both map and tree levels
  (fable_map.h `FABLE_RETAIL_STLP_TREE_ERASE_KEY_CALLS`). Phase 13 now calls only equal_range / erase(range), as retail does.
- v16: scoped `#pragma inline_depth(0)` on the phase-1 list push_backs; operands are hoisted into locals first. The phase-1
  call sequence now matches.
- v13 / v17 / v18 / v20: pragmas on P9 / P10 / P11. Outlining P9 cascades: the P11 inits, P12's vector copy and P14's
  SetHomeBuilding become inline. Best of these: md 3039.
- v21 (best): v16 with noGhosts / noGuards declared after the vectors, before homeSpaces. md 2902 / mdf 1434.
- b01-b10, c01-c04, d1-d6 and e1-e4 (declaration order, discarded calls, loop forms): none beat v21.
- Parked: differ/00502e90.cpp (from v21). 0x0058AF10 is not drafted; see NOTES.md for resume notes.
