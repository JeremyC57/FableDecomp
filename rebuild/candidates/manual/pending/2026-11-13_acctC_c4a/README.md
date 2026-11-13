# acctC lane, hand-back c4a (2026-11-13): 1 row matched, 10,254 bytes -- staged, NOT landed

| address | name | length | grade | test |
|---|---|---:|---|---|
| 0x0060EEB0 | CTCInventoryMap::ConstructPCMapList | 10,254 | RELOCATION_MATCH | PASS |

Re-verified by the lane coordinator against `FullDecomp` at 55262db83 (read-only `vland.py`, `LANDVERIFY_STRICT_HEADERS=1`, overlay =
only the changed headers the row includes; `coordverify.log`: 1 win, no RELOC_IDENTITY / RELOC_CONFLICT, only catalogue RELOC_NAME
label warnings). No flags. Real length. No oracle file here. The row resisted the acctB lane and agent C1 (about 125 grades); agent
C4 closed it in about 135 more.

## What closed it (details in NOTES.md / LESSONS.md)
1. A loop: `if (IsOnWorldMap()) push_back(..); else if (*it == hero region) push_back(..);` -- two identical arms that VC7.1 merges.
2. The guild loop runs on its own `unsigned long j` counter (fixes the swapped frame words).
3. Teleport branch: six FindChild results in six named locals (fixes the SIB order at 0x0060FE86).
4. Compare through the shared `FABLE_RETAIL_CBASICSTRING_COMPARE_BODY` (no row-local template).
5. Quest-card loop through `GetTC(&quest_card)` + `FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN`: retail calls the non-const LowerBound
   0x0040F020 (PeekTC gave equal masked bytes but bound the const 0x004365B0 -> RELOC_CONFLICT).

## Header diffs (`hdrdiff/`, all new `FABLE_RETAIL_*` opt-ins, moved asserts pinned; table in hdrdiff/README.md)
CRegion.h, CTCInventoryMap.h, CTCQuestCard.h, CWorld.h, CWorldMap.h, NUISystem_CUIState.h. The agent's whole overlay passed
`equiv_check.py` over all 56,187 landed rows (0 differing) before C4's two row-2 opt-ins were added; re-run it when applying.

## Decisions / review items
- **`if (regions.empty())` guard (src line ~266): ACCEPTED as an inferred statement** (owner rule 2026-11-12). `regions` is always
  empty at that point, so the guard emits no code and cannot change behaviour on any compiler; without it the row is 10,476 bytes
  (wrong inline split), and four equivalent spellings leave one x87 operand order wrong. It is portable C++. No PDB / string
  evidence for the exact spelling; the comment in the source says so.
- Same-TU conflict with 0x005FF190 CreateMapMarkers (still open, `open_005ff190/`: DIFFER 4096v4176, all 133 call targets right,
  register / frame allocation differs): that row currently wants `FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND` on and COMPARE_BODY
  off, the opposite of this row. A LowerBound text of inline cost 96..106 would serve both; agent C5 is on it. The two row-2
  opt-ins (`CCountedPointer.h` RESET_ELSE_NULL, `fable_map.h` FORCEINLINE_DTOR) are kept in `open_005ff190/`, not part of this
  hand-back; this row matches without them.
