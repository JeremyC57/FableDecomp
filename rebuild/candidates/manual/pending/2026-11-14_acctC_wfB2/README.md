# acctC workflow hand-back wfB2 (parked rows, 2026-11-14)

| address | name | bytes | grade | test |
|---|---|---|---|---|
| 0x005FF190 | CTCInventoryMap::CreateMapMarkers | 4,176 | RELOCATION_MATCH | PASS |

Verified twice (workflow verifier + coordinator, `coordverify.log`). No payload flags, no asm, shared headers only.
REVIEW ITEMS (owner test: byte match yes, behaviour same yes, portable yes; both devices are inline-decision devices):
- FITTED: four loop-entry guards (`if (i < regions.size()) for (...)`, `if (it != cards.end()) for (...)` x3) that emit no
  bytes; they set the call/expand split of CCountedPointer<CComponent>::Reset (7 called, 2 expanded). Same device as landed
  0x00607330. Flag alternative per the author: /d2inlT72 (not used).
- Inline-budget: FABLE_RETAIL_STLP_MAP_FORCEINLINE_DTOR (explicit `FABLE_FORCEINLINE ~map() {}` in stlport/fable_map.h,
  additive opt-in). Retail expands the map destructor at 0x00600136..0x0060019D. The source lacks the `// Inline-budget:`
  line: add it at landing, or rule the opt-in out (then the row is open again).
- The second `#pragma auto_inline(off)` pair wraps the row function itself (as 20 landed files do); no own evidence.
`open/`: 0x00603000 CreateMiniMapMarkers, DIFFER(9571v9571), 6 hunks left (NOTES.md).
