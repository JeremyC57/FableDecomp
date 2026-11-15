# Codex pass 22 reservation (2026-11-14, owner): 100 rows handed to Codex

**Returned and released 2026-11-15 (owner):** Codex's hand-back is in `return/` (START_HERE_CLAUDE.md, STATUS.tsv: 87 RELOCATION_MATCH,
6 PARKED, 7 EXTENT_WRONG). 76 rows landed in landing gb (`../../batches/2026-11-15_landing_gb/`). `RESERVED.tsv` is renamed
`RELEASED.tsv`, so the remaining rows are back in the normal pool (UNMATCHED_ROWS.tsv points at their files here):
- `return/matched_test_port/`: 11 rows Codex matched but not landable yet: 9 whose behaviour tests use a per-test `test_include/`
  directory the repo harness lacks (port the test, then land), the catch funclet 0x004A1C41 inside one of them, and 0x00C29A3C
  (Dinkumware `vector::_Insert_n`, needs the vendor STL include order).
- `return/parked/`: the 6 PARKED rows with Codex's closest source, test and probe notes.
- `return/extent_wrong/`: the 7 EXTENT_WRONG reservations (interiors of landed functions; BOUNDS_FIX.tsv).
- `return/landed_evidence/`: naming and probe notes of the 76 landed rows. Name fixes held for review:
  `return/NAME_FIXES_CONFLICTS_WITH_QUEUE.tsv`.

`RESERVED.tsv` lists them. They are off-limits to every other lane until this list is released (renamed `RELEASED.tsv`).
None overlaps the acctC reservation (`../2026-11-13_acctC/RESERVED.tsv`).

| kind | rows | bytes | what they are |
|---|---:|---:|---|
| parked | 50 | 81,296 | the 50 largest parked rows whose closest draft on record is the same size as retail (1,088-2,531 bytes), excluding the 128 rows of the earlier Codex passes released on 2026-11-13; `closest_version` is the draft, `note` its grade |
| never_worked | 50 | 24,134 | the 50 largest rows nobody has authored (21-3,344 bytes): 22 are `Catch@` / `Unwind@` EH funclets, about a dozen start at unaligned addresses and are probably tails of the function before them, the rest are whole functions with hint labels |

Lengths: for parked rows the retail length from the recorded grade; for never-worked rows the distance to the next known function
start with trailing int3 / nop padding removed (unchecked extents: Codex is asked to confirm each and report wrong ones).

## The package
`build.py` builds `fabledecomp_codex_pass22_2026-11-14.zip` in the scratch area (not committed; about 23 MB). It reads `rows.tsv`
(the selection, fixed here so a rebuild does not reshuffle it), needs `FABLE_EXE` only to measure extents and find callers, and
writes no retail bytes. Contents:
- `PROMPT_CODEX.md`, `README_CODEX.md` (copies here), `ROWS.tsv`;
- `rows/<addr>/`: ROW.md (extent, callers, landed neighbours), the neighbours' sources, and for parked rows the closest draft plus
  every earlier source and note on record;
- `include/`: every shared header at HEAD;
- `names/`: bindings, PDB name sets, PDB types, vtables, established spellings, the name-fix queue, label files, landed names and
  the landed address -> source map;
- `docs/`: the rules, GOTCHAS, the matching playbook and the compiler research, the header merge-and-proof steps, the hand-back
  rules from landing fy, `UNMATCHED_ROWS.tsv`, `PARKED_INDEX.tsv`;
- `lessons/`: 391 hand-back LESSONS / NOTES files since 2026-11-10, every landing README since 2026-11-11, the naming passes, the
  Codex pass 15-21 returns, the acctB dossiers of large rows;
- `tools/`: `cut_row.py`, `xref.py`, `lane_merge/pin_asserts.py`, `lane_merge/equiv_check.py`.

The return is graded and landed by the coordinator as before; header changes come back as old-text / new-text pairs (README_CODEX
"Header changes").
