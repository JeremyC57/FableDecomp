# 2026-11-19 landing nf93a: naming round 8, NF93 part 1 (12 rows, q11225..q11442 slice 1)

Owner: "start the 3 naming agents on the naming queue". Round 8 runs NF93, NF94 and NF95 on disjoint slices of q11225..q11442
(claim in `COORDINATION_CLAIMS.tsv`; row list `naming/2026-11-19_round8_rows.tsv`). NF93 stopped for context after 9 of its 63 entries;
this lands that part. A fresh NF93 continues the slice from q11261. Brief and records: `naming/2026-11-19_agentNF93_part1/`.

## Entries (9 of 63 terminal)
- **done (4):** q11232 0x005C14A6 `NInventory::CPerItemData::CPerItemData` (catalog name fixed; the self row is now the nf86 pick-any scaffolding
  on the header inline ctor, one COMDAT); q11244 0x004DB88A STLport `_Rb_tree<long, pair<const long, CDefString>>::clear`; q11250 0x0086BE40
  (binder 0x0086BF70 onto the header); q11256 0x00753B40 `CTCLook::IsCurrentlyTurningViaAnimation` made const (QBE) with its binders 0x00753C70 / 0x006655A0.
- **blocked (4), annotated in the queue:** q11239 (0x005BF654 port; 0x005BF414 held by NF95), q11242 (identity: unreferenced ~_Rb_tree / ~map twins),
  q11247 (0x005BF654 DIFFER 297v297, a callee-saved register permutation), q11251 (one definition: 0x0086BE40 already defines the PDB spelling; fix
  written up in NOTES).
- **deferred (1):** q11415 0x00C88E60, Account B's row (B reports a CReverbFnsSSE vs CDspOps naming question coming in its hand4d NAME_FIXES).

## Rows (12): 9 self renames (RENAMES_applied.tsv, 8 file moves) + 3 caller respellings
0x004DB88A, 0x005BDFB1, 0x005BEC92, 0x005BF3C6, 0x005C06C3, 0x005C08D3, 0x005C14A6, 0x006655A0, 0x00753B40, 0x00753C70, 0x0086BF70, 0x008C2980.
Recheck from the repo after landing (`logs/recheck.log`): 10 RELOCATION_MATCH + 2 MATCH, every test PASS.

## Headers (4 opt-ins + 1 shared block, token-identical without them)
- engine/CCategory.h: `FABLE_RETAIL_CCATEGORY_SELECTITEMOFTYPE`, and `FABLE_RETAIL_CCATEGORY_REMOVEITEM` (NF94's block, copied verbatim so NF94's
  landing is a no-op for it).
- engine/CRandomAppearanceMorph.h: `FABLE_RETAIL_CRANDOMAPPEARANCEMORPH_GETRANDOMMORPHSET`.
- engine/CCreatureAction_CastWillForceSpell.h: `FABLE_RETAIL_CCREATUREACTION_CASTWILLFORCESPELL_CLASS` (generated flat class kept under `#else`; its 5
  asserts pinned with FABLE_STATIC_ASSERT_AT at their old lines 83..87).
- engine/CCombatAction_WillForcePushInto.h: `FABLE_RETAIL_CCOMBATACTION_WILLFORCEPUSHINTO_FINISHACTION`.
- Includer recheck: every landed row that includes these headers directly or through another header (935 rows): `logs/includer_recheck.txt`.

## Checks
- `naming/rebind.py`: removed 34 binding lines, added 29; `bindings_multi_check.py` at its baseline (`NullCoord`).
- `funclet_includes_check.py`: 1,355 checked, 0 broken. No changed address has an UNMATCHED_ROWS line.
- Synced with FullDecomp-B before landing (fast-forward to 5d9c4c76e6; log entries only).

## Queued (`land1119nf93`, 2)
0x004DA27F `_Rb_tree<long, pair<const long, CDefString>>::_M_erase` (CANDIDATE: other callers' tree types unproven) and the 0x005BF414 binder of
0x005C08D3 (NF95's row).

## Rule slips (NF93 NOTES)
Four Bash calls exited non-zero; one duplicated grade; some section-2 old spellings read from source, not a baseline compile.
