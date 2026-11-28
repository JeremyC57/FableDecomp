# 0x00F277F0 vector<CHeroSoulsCreatureDef>::_M_fill_insert -- ESCALATE (AUTH30, 2026-11-27)
- Bytes: c0 (Codex, two overlay opt-ins, /Og- /d2vol5 + `#pragma optimize("g",on)`) RELOCATION_MATCH 493/493.
- Leaf-referenced NEQ: `_M_insert_overflow` (same vector), 371 B v 325 at 0x00F27A10.
  - The landed owner row 0x00F27A10 is a local STLport model with FABLE_FORCEINLINE helpers (local-model debt).
  - Landed 0x00F27710 (resize, same vector) does not emit the copy at HEAD, so there is no HEAD witness.
- vftable-reached NEQ: `??_GCHeroSoulsCreatureDef` (slot 0 -> 0x00F270B0), 54 v 30.
  - 0x00F270B0 is landed as `CLandscapeBackgroundPatch::vector deleting destructor` and is UNSETTLED in NAME_ANSWERS. RTTI puts it
    in CHeroSoulsCreatureDef's slot 0: a naming conflict to settle (NAME_CONFLICTS.tsv).
- The fix needs the owner's helper shape in the shared fable_containers.h (an opt-in) without moving this leaf. That is a port of
  the owner row first.
Grades: c0 only.
