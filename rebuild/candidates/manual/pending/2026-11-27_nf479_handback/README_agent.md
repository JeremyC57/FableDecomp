# NF479 hand-back: Codex pass 29 name fixes (159 lines)

All 159 lines have an outcome in `STATUS.tsv`: 24 done (some partial), 17 done-already, 12 codex-row only, 100 blocked, 6 deferred. The count of
REJECTED.tsv lines is separate: it holds 1 rejected claim (line 108's anonymous hash). Line numbers are the data-line index of CODEX_NAME_FIXES.tsv.
Built on the repo as it stood after the Codex pass 29 landing (orig/ re-copied when each run dir was built, 2026-11-27; census includes the new Codex rows).

## Land order (each batch is a land_scripted run dir; all rows ACCEPT)
| batch | rows | what | Codex lines |
|---|---|---|---|
| b1 | 3 | WordWrapText self (private const) + its binder; PWordWrapFont private static (2 binders); registry line removed | 39, 41 |
| b2 | 3 | SetCreatureAsAttemptedToPerformedAction self (by reference) + its only old-spelling binder (one unit); InitialiseAsDepthBuffer self (ESurfaceMultisampleType) | 7, 156 |
| b3 | 26 | data binders: CRC32TAB (7), CCharString::NoStrings (10), NEngineInternal::GPLocalDetailGenerator (9 rows), NEngineInternal::GPEngineEnvironment (0x00B26360) | 6, 64, 126, 135 |
| b4 | 13 | self respells (access / const / reference / namespace / class) | 2, 4, 75, 99, 100, 102, 107, 110, 113, 114, 136, 139, 142 |
| b5 | 3 | binders: GetStyleCardsListUIComponentPC; the anonymous-namespace GetPBestTargetToFaceForCreature (2); stale WriteFloat line delete (hand) | 43, 94, 108 |

Each batch: `rows/`, `orig/` (HEAD row + test), `head/payload.json` (+ oracle.tsv), `RESULTS.tsv`, `TABLE_CHANGES.tsv` (only lines witnessed by a
row of the run), `HAND_TABLE_CHANGES.tsv` (b3, b5: rewitness lines onto rows outside the run + b5's stale delete), `SELF_SYMBOLS.tsv`,
`CATALOG_MANGLED.tsv`, `CATALOG_CHANGES.tsv` (b4), `UNITS.tsv`, `REGISTER_REMOVE.tsv` (b1), `evidence/` (vland HEAD / new logs, pk).
No tests changed in the final batches; no headers changed (no overlay).
Apply per batch in order b1..b5: land_scripted (rows + TABLE_CHANGES), then `apply_table.py HAND_TABLE_CHANGES.tsv`, SELF_SYMBOLS,
CATALOG_CHANGES, REGISTER_REMOVE. Units (UNITS.tsv, leader address): b1's three rows; b2 0x006EA540 + 0x007E5B90; b3 0x009CC0B0 + 0x009D1320 + 0x009D4DB0.
The HAND rewitness lines (b3: 4, b5: 1) point the table at held rows that still bind the old spelling (RULINGS_NEEDED R4): drop them if R4 is no;
the batches land without them (the moved rows' own lines are in TABLE_CHANGES).

## Gates (all batches)
- vland `--recheck-landed` HEAD v new, one LANDVERIFY_WORK path: b1 3/3, b2 3/3, b3 26/26, b4 13/13, b5 3/3 MATCH / RELOCATION_MATCH PASS
  on both sides (with each row's test). New-v-HEAD vland warnings: only 0x00687D10's STANDIN_LEAF text changes (the catalog fix in b4 clears it).
- objproof: bytes and relocation offsets same in every defined COMDAT, only the intended symbols renamed, `.drectve` same
  (`evidence_sandbox/changes.tsv`: self / reloc lines only; the one `self_DIFF` is 0x006D1DE0's pick-any IsMarriedToThePlayer copy, see below).
- Caller census (retail E8/E9 + absolute refs + table witnesses + source grep, 712 HEAD objects' symbol tables incl. the Codex rows):
  every landed binder of a changed self spelling moves in the same batch; table lines whose witness moves are deleted, replaced, or
  rewitnessed onto a landed row that still binds the old spelling (`evidence_sandbox/census2.tsv`).
- Binder rows carry `; target <addr>` in RESULTS.
- pickany_vs_retail + pk_all HEAD v new: no change except 0x006D1DE0's copy of CTCWife::IsMarriedToThePlayer, NOADDR -> EQ at its owner
  0x006CEC80 (16 bytes; the new copy is EQ as the pick-any rule asks). No flip on an already-NEQ copy (`evidence_sandbox/reachflip.txt`).
- Sandbox (shadow repo; apply_table only with FABLE_ROOT=<sandbox>; repo table checksum unchanged): every TABLE / HAND line applied, 0 misses;
  bindings_multi_check 1 -> 1 (the pre-existing NullCoord line); descriptive_check 2018 -> 2017 lines, 0 FAIL; s_unit_check 205 checked, 0 failed.

## Tracker dry run (naming_status.py --out <sandbox>): nothing worse
18 addresses -> agree_pdb: 0x0048DC8B, 0x00687D10, 0x006D1DE0, 0x006EA540, 0x00759870, 0x00886E80, 0x00887830, 0x008BFDA0, 0x00993EE0,
0x009AE150, 0x009BAD60, 0x009BBA40, 0x00A2E470, 0x00A2E490, 0x00B201D0, 0x00B21E90, 0x00BF9AA0, 0x013B87AC (agree_descriptive), and
fewer names at 0x0097E140 (3 -> 2), 0x009F32A0 (4 -> 3), 0x0129A168 (6 -> 2), 0x01436E88 (3 -> 2). Tracker table: `evidence_sandbox/TRACKER_EFFECT.tsv`
(15 split_fixable, 1 split_unknown, 1 agree_unverified, 1 agree_descriptive -> agree_pdb; 4 keep their status with fewer names).

## Other hand-back files
STATUS.tsv (159 lines), NAME_FIXES.tsv (catalog changes + 5 catalog-less addresses for indexing), REJECTED.tsv, RULINGS_NEEDED.tsv (R1..R5),
LOCKS.txt (final owners: 49 held by NF479 for the ACCEPT batches (48 rows + 0x00993EE0 for b5's hand delete), every other claimed lock released), NOTES.md (details, follow-ups, drafts),
tools/, work/drafts/ (parked rows).
