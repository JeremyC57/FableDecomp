# 2026-11-17 landing nf78: name-fix round 7, NF78 (16 rows: the nf74-nf75 follow-ups and five NF70 ports; one row held)

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done" and "add another agent
to speed up naming queue completion".

NF78 took the 11 entries queued at the nf74 and nf75 landings (q10945..q10955), their binders, and eight rows NF70 had left blocked.
Brief, list and records: `naming/2026-11-17_agentNF78/`.

## Rows (STATUS.tsv: 26 = the 25 slice rows + 0x0059A238, claimed through the lock)

| Status | Rows |
|---|---|
| ported, landed | 16: 0x008C1600, 0x007C0FB0, 0x004F97B3, 0x00812170, 0x006DCFA0, 0x004E7720, 0x004F914A, 0x009F53C0, 0x00597B20, 0x00599D5C, 0x0059A238, 0x007ADBF0, 0x00ADD020, 0x009EC5E0, 0x00D8F6D0, 0x00E5F2B0 |
| ported, held | 1: 0x0063A970 (`held/`, below) |
| not-a-binder | 5: 0x008AC060, 0x004F9660, 0x009F5170, 0x00429288, 0x00570332 (its reloc_bindings destructor line was stale) |
| blocked | 4: 0x0081EE60 (DIFFER 174 v 207 even unmodified), 0x00EDC720 (the two-view CScriptThing problem, 4 grades), 0x008E7E70 and 0x00A37300 (baseline grade only; STATUS names the header work each needs) |

- Five of NF70's eight blocked rows are ported: 0x007ADBF0, 0x00ADD020, 0x009EC5E0, 0x00D8F6D0, 0x00E5F2B0.
- **Held: 0x0063A970** (RELOCATION_MATCH + PASS at hand-back; source and test in `held/`). It calls SetEngineThemeAt by the protected
  `IAE` spelling both PDBs give, while the landed 0x0081EE60 still defines the public `QAE` one, so landing it alone would break that
  link. It lands with q10951 (queued).
- 0x008C1600 keeps eight local names from the landed row's fitting (world, creature, search, last, me, distance, too_close, self);
  they are not PDB names and the row marks them FITTED. 0x008C2550 still carries an older copy of the same function (queued).

## Entries (APPLIED.tsv, 11)
8 done (q10945..q10950, q10952, q10954), 2 done-already (q10953, q10955), 1 blocked (q10951, 0x0081EE60).

## Rows and checks
- **Section 1: 10 self renames (10 file moves).** Coordinator correction: 0x004F97B3's module is respelled to the catalog spelling of
  its sibling 0x00453554 (`map<EDamageAttribute,float>`).
- **Headers (6 opt-ins in 5 files; additions only):** `stlport/fable_map.h` (`FABLE_RETAIL_STLP_TREE_COPY_CTOR_BODY`,
  `_MAP_COPY_CTOR_BODY`), `stlport/fable_containers.h` (`_LIST_ASSIGN_BODY`), `engine/CHitParameters.h` (`_COPY_CTOR`),
  `engine/CPersistTraits.h` (`_BOOL_BINARYIN_NOINLINE`) and `candidates/world_map_theme.h` (`FABLE_RETAIL_CMAP_ENGINETHEME_PROTECTED`, one
  assert pinned at line 101; used by the held row). NF78's base copies were identical to the repo. `lane_merge/assert_names_check.py`:
  0 mismatches against 49b943df65. Details: `naming/2026-11-17_agentNF78/hdr/README.md`.
- **Funclets:** none affected (1,355 checked, 0 broken).
- **Recheck (`logs/recheck.log`):** all 16 landed rows graded from the repo after landing: 16 RELOCATION_MATCH, every test PASS.
- **Checks:** `naming/rebind.py` removed 51 binding lines and added 40; bindings_multi_check is at its baseline (`NullCoord`).

## Name fixes queued (`land1117fu`, 14)
NF78's NAME_FIXES.tsv (12: CMap's protected theme accessors 0x0081EE60 / 0x0081EFC0 / 0x0081F090 / 0x0081F170, CFrontEndManager members,
the CComboMultiplierData and EDamageAttribute tree `_M_erase` / `clear` rows and others), plus the held 0x0063A970 and the
0x008C2550 copy of GetValidRushPosition.

## Rule slips
- One Bash call exited 1 (a helper run outside the repo directory, then rerun).
- The `/d2inlT` bisect for 0x00EDC720 was not tried; its inline-depth probe stayed in a draft.
