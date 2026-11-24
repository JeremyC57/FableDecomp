# Landing: Codex row 0x00A4EFC0 (C3DAnimationSequenceData::InitialiseFromFileData), 2026-11-22

Source: the one-row Codex hand-off `pending/2026-11-22_codex_row_00a4efc0/` (returned files in `returned/`, user zip
"Fable_00A4EFC0_verified"). **Owner decision 2026-11-22: land as Inferred.**

## What landed
- Row `rebuild/src/compiled/00/a4/C3DAnimationSequenceData_InitialiseFromFileData_00a4efc0.cpp` + test, flags `/Gr`:
  **RELOCATION_MATCH, test PASS** (strict headers, dry run and land; `logs/vland_*.log`).
- The body is the Ego_d-exact text (AUTH7 / AUTH11). The 16-byte residue AUTH11 proved to be a c2 order tie closes with the
  CArray opt-in `FABLE_RETAIL_CARRAY_DEBUGGER_BOUNDS_PASS27` (engine/CArray.h): `operator[]` wraps the debug index check FableWin
  shows at 0322E450 / 0322C360 (`index < size()`, `IsDebuggerPresent`, break) in `if (0)`. The row source carries an
  `// Inferred:` line: that release spelling was chosen by grading finitely many candidate expansions; the shipping macro text is not attested.
- Header changes (`logs/headers.diff`): C3DAnimFileSequenceChunk.h, C3DAnimationSequenceData.h (static assert re-pinned to line 60),
  C3DKeyframe.h, CArray.h, CQuaternion.h, new stlport/fable_animation_byte_vector_owners.h. Codex's unused C3DVector.h DIAG branches were dropped.

## Proof
- Header proof: pin_asserts 0 pinned, assert_names_check 0 mismatches, equivalence over 57,021 landed rows: only 0x0047042A differs
  (`logs/equivalence_differing_rows.tsv`).
- Recheck on the landed tree (`logs/recheck_landed_tree.log`): 0x00A4EFC0, 0x0047042A and the users 0x00A50510 / 0x00AA0FA0 / 0x00BD2D90
  all RELOCATION_MATCH + PASS.
- Pick-any (`logs/leafrel.txt`, `logs/pickany_vs_retail.tsv`): leafrel 0 problems; 24 EQ, 2 NEQ:relocs that are byte-equal (relocations at
  0x00485B6C / 0x00485B18 under unbound spellings), 188 with no owner row.

## Held
- Two reloc_bindings first witnesses would give a target a second spelling (0x00A552B0 `_M_fill_insert<C3DVectorWord>` with the explicit
  allocator argument; 0x00485B6C `_M_insert_overflow<unsigned char>`): removed from the table, queued `land1122a4efc0`
  (`logs/reloc_bindings_held_second_spelling.tsv`).
- Name queue: Codex NAME_FIXES (InitialiseFromFileData replaces the old InitialiseFromChunk label); the AllocateType formal `elements`
  stays queued from auth11.
