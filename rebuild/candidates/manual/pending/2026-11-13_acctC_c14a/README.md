# acctC lane, hand-back c14a (2026-11-13): 2 rows matched, 19,889 bytes -- staged, NOT landed

| address | name | length | grade | test |
|---|---|---:|---|---|
| 0x0060EEB0 | CTCInventoryMap::ConstructPCMapList | 10,254 | RELOCATION_MATCH | PASS |
| 0x0060C900 | CTCInventoryMap::ConstructMapList | 9,635 | RELOCATION_MATCH | PASS |

Re-verified together by the lane coordinator against `FullDecomp` at 55262db83 (read-only `vland.py`, `LANDVERIFY_STRICT_HEADERS=1`;
`coordverify.log`: 2 wins, no RELOC_IDENTITY / RELOC_CONFLICT). No flags, real lengths, no oracle here. **This folder supersedes
hand-back c4a** (0x0060EEB0 alone), which is removed: the two rows are one retail TU with the same opt-in set, and land together.

## 0x0060C900 is ConstructMapList, not BuildMapTeleportUI
`?ConstructMapList@CTCInventoryMap@@QAEPAVCComponent@NUISystem@@PAV23@_NPA_N@Z` -- Ego_r.pdb 0x0065F950, FableWin.pdb 0x0208A2A0,
donor order next to ConstructPCMapList. reloc_bindings carries two wrong BuildMapTeleportUI manglings for it (NAME_FIXES.tsv). It is
the console twin of ConstructPCMapList: written from the matched sibling's text, matched on the second grade.

## What the two rows depend on (review items)
- **`if (regions.empty())` guard, in both rows: accepted as an inferred statement** (owner rule 2026-11-12). Always true at that
  point, emits no code, portable; without it the rows are 10,476 / 9,795 bytes (wrong inline split). Spelling is FITTED.
- **Both rows keep the same-TU body of CreateMapMarkers 0x005FF190 in the file.** That row is still open (`open_005ff190/`,
  DIFFER 4096v4176); when its text is settled both rows here must be regraded with it.
- NAME_FIXES.tsv also: 0x006A7140 is `CThingFilter_HasInformation::operator()(const CThingAICreature&) const` in both PDBs (a landed
  local model in 0x00617A00 spells it with `const CThing&`).

## Header diffs (`hdrdiff/`)
CRegion.h, CTCInventoryMap.h (incl. new `FABLE_RETAIL_CTCINVENTORYMAP_MAPLIST`), CTCQuestCard.h, CWorld.h, CWorldMap.h,
NUISystem_CUIState.h: new opt-ins only, moved asserts pinned; table in hdrdiff/README.md. The agent's whole overlay passed
`equiv_check.py` over every landed row (0 differing).

## Open rows of the same TU
- `open_00603000/`: 0x00603000 CreateMiniMapMarkers, DIFFER(9682v9571): all 303 call targets, block order, strings, loops and
  frame size right; register allocation differs from the first block. Its extra header opt-ins are kept there, not in `hdrdiff/`.
- `open_005ff190/`: 0x005FF190 CreateMapMarkers, DIFFER(4096v4176), same kind of residue. Agent C15 continues both.
