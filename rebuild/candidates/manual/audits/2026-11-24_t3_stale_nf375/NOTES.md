# NF375: second stale-line sweep of T3_PORT_QUEUE.tsv (Account A, 2026-11-24)

This sweep is read-only. It changed no repo file. Besides `git log` / `show`, I also ran read-only `git diff`, `status` and `rev-parse`,
which the brief did not list. Every proof
was compiled from the repo into `work/cen_*` with `recheck.py` + `vland.py --recheck-landed`.

**State read**
- HEAD at start: f73ce6f5a3 (17:40). The working tree held NF374's uncommitted nf374_b1 landing. Those 28 rows were all NF374 stay-off
  rows already.
- Queue at start: 3,663 lines (`work/queue_at_start.tsv`). During the sweep nf374 appended about 190 lines. The first 3,663 lines did not
  change, and every hand-back line's sha1 was rechecked at the end.
- None of the 393 compiled rows' sources changed before hand-back (`src_sha` rechecked). The table lines at the 7 STALE / PARTIAL targets
  are byte-identical to the start snapshot.

## Result

| | lines |
|---|---:|
| Checked by compile | **146**: 118 naming lines + 28 copy-debt lines |
| **STALE** (`STALE_T3.tsv`) | **2** |
| **PARTIAL** (`PARTIAL.tsv`, each with a proposed column-4 rewrite) | **5** |
| OPEN, confirmed by compile | 125, plus 1 OTHER (research line 3593) |
| UNPROVEN (`UNPROVEN.tsv`): a needed row is stay-off | 13 |

Every checked line, with its verdict and reason, is in `work/checked.tsv`.

**Compiles and checks**
- 393 row compiles in 3 rounds: 382 RELOCATION_MATCH PASS and 11 MATCH PASS (`work/census.tsv`).
- Both pick-any checkers ran on 74 kept objects (`evidence/pk/`).
- Hit rate: 7 of 146 lines changed (2 close, 5 rewrite).
  - Round 1, touched rows plus pre-filter candidates (215 rows): 1 STALE and 4 PARTIAL.
  - Round 2, copy emitters, the retail callers of the candidate copies, and the nf370..nf373 lines (130 rows): 1 STALE and 1 PARTIAL.
  - Round 3, nine binder-hit nf303 lines and 14 NF342-verdict spot checks (48 rows): 0. Every spot check gave NF342's verdict again.
- I stopped at 393 compiles.

### STALE (close)
1. **Line 470, nf297 0x009FF8D0 (GetFrame).**
   - The line's last open clause was "left: binder 0x009DCA80".
   - 0x009DCA80 and 0x00BCB800 now bind only `?GetFrame@CGraphicInfo@@QBE_NAAVCGraphicFrame@@KKK@Z`. It is UND in both objects, and
     neither references any other GetFrame@CGraphicInfo spelling.
   - The table holds only that P at the target.
   - The self 0x009FF8D0 defines and grades P; it has been const since nf370.
   - Limit: this is NF342's named-binder standard. Binders the line does not name were not compiled; the table names only 0x009DCA80.
   - P is in both Ego_r and FableWin. The line text says "both PDBs QBE".
   - Evidence: `evidence/lines/470.txt`.
2. **Line 3537, nf340 0x00B49DF0 (`vector<CManagerLocalLight*>::_M_insert_overflow` copy).**
   - Both landed rows that call 0x00B49DF0 in retail, 0x00B47BC0 and 0x00B47FD0, emit an EQ copy (216 B; pk_all EQ and old checker EQ).
   - The other 9 rows that name CManagerLocalLight emit no copy at that address.
   - The fix came with landing nf344_b1: 0x00B47BC0's /d2inlT29 became /d2inlT39, and its RESULTS line says "repaired 00b49df0". The
     line was never closed.

### PARTIAL (rewrite proposed in `PARTIAL.tsv` column proposed_reason)
- **1010, nf303 0x00780400.**
  - Census witness 0x006A8A80 already binds P `?IsOnFire@CTCFlammable@@QBE_NXZ`. Its table line `?HasFreeActionPoint@CTCActionPoints@@QBE_NXZ`
    was stale when NF303 counted it, because that source last changed on 2026-11-11 (88a15fb806).
  - Left: 0x0089BD90's stand-in, plus a TABLE_CHANGES delete.
- **1134, nf303 0x009F7250.**
  - The only census binder, 0x009F7330, binds P `?ClearAllQueries@CVsyncCallbackTimer@@AAEXXZ` and no longer references Teardown.
  - Left: only the stale table line `?Teardown@CPairedNodeLists@@QAEXXZ 009f7250 009f7330` (a delete). After that the line closes.
- **3336, nf326 0x00CD3D2E (unit).**
  - The self and the 0x00D7AA30 witness have been on the 4-argument PDB ??A since nf350.
  - Left: the CCutsceneCastMap binders 0x00E2A900 and 0x00E80D60. That is exactly the remainder of nf311 line 1650, so line 3336 could
    instead be merged into it.
- **3560, nf340 0x009AC700 (`map<CCharString,CCharString>::operator[]` copy).**
  - The line lists no emitter. The nf353 note says the copy is "resolved by owner".
  - Census of the 27 landed retail callers: six still emit a leaf-called NEQ copy (159 B v 149, both checkers LEAF-REFERENCED):
    0x009A8150, 0x00D3A4A0, 0x00D7AE80, 0x00E55CE0, 0x00E57020, 0x00E64FB0. 21 emit none.
  - The rewrite lists those six. The line now lists no emitter; its nf353 note says 11 rows dropped the copy, and six still emit it.
- **3584, nf349 0x0042AF3C.**
  - The self stand-in clause (GetDef_0042af3c) is done: nf360 / nf363.
  - Left: the SIZE_BODY owner port held by NF356. The owner still defines P strongly (selection 1).

### Open, confirmed (125; no list, see `work/checked.tsv`)
- **What the judge's raw "PARTIAL" meant.** 17 binder lines came out "PARTIAL" from judge.py, but none of them is a rewrite. They were
  re-judged by hand against their text:
  - In 10 of them, the "fixed" binder is a P witness the line never named as open, or one the text already records as done (123, 163,
    381, 383, 515, 612, 1509, 1545, 1676, 3563).
  - The other 7 are unchanged from NF342's PARTIAL verdict, which ruling D3 keeps whole (469, 495, 555, 556, 1389, 1402, 1403).
  - All 17 are counted as open.
- **Copy lines:**
  - **3504 / 3507 / 3545 (GetSubDef copies).** The listed emitter is now EQ, but the lines were kept open for the local-body emitters. Those
    are still NEQ: pk_all on 0x0041E6D3, 0x004BFC50, 0x0077C160, 0x00861E60, 0x005C3180 and 0x005C770C. None of the local-body rows has
    changed since NF357.
  - **3468.** Its note "left: the 9 T3-held rows" is current. 0x00D45060 / 0x00D58490 / 0x00E8EF80 / 0x00EA59E0 dropped the copy; 6
    held rows still emit it NEQ.
  - **3588.** The insert copy at 0x00CD3CAE is still NEQ in those 6 held rows.
  - **3564 / 3565 (pickany_d2inlS15).** Their leaves still reference the NEQ resize copy at 0x00AFE910.
- **New lines (nf343..nf373).**
  - All 27 nf372 binder lines are open (six at ??0C3DAnimationUtil plus 21 singles): the binders still bind their stand-ins.
  - nf370 / nf371 / nf373: the self? lines 3651 / 3652 / 3658 / 3659 are open, and so is 3660. Five of its rows still strongly define
    ?VecDel@T@@; 0x0043DD40 instead defines ?CLandscapeBackgroundPatch_vector_deleting_destructor@@... (the line text says all six define
    VecDel).
  - nf349 3585, nf353 3594 and nf357 3597..3600 are open.
  - The nf345 / nf348 / nf354 / nf370 identity lines were not compiled; they need rulings, not respells.

### UNPROVEN (13; `UNPROVEN.tsv`)
- **Stay-off rows.** Each of these lines needs a stay-off row, either NF374's targets / binders or Account B's:
  - 155: 0x00736AE0
  - 1508: 0x004FEEC0
  - 1851 / 3595: 0x00454B4C, plus 0x0045C5F6 for 3595
  - 3596: 0x0045C5F6
  - 3618: 0x00839940
  - 2531: 0x00BDC180
  - 3582: B's 0x0070D940, which landed at 17:40
  - 3473..3476: emitters 0x00626110 / 0x00626750
  - 3538: 0x00CE7670

### Line numbers
- `line` is the line number in the 17:40 snapshot (`work/queue_at_start.tsv`). Since then, landing 3557df67dc (option S, 18:06) removed two
  0x00B80AD0 lines, and nf374 appended about 190.
- `line_now` is the line in the live queue at hand-back, found by sha. Close by sha.

### Not examined (no compile this sweep)
- **NF342-verdict lines with no touched row, carried forward.** 10 pre-filter-CAND lines kept NF342's verdict: 118, 364, 367, 369, 443,
  559, 624, 1100, 1672 and 3327 (OPEN_CHECKED / PARTIAL there). The 14 lines I spot-checked from the same group all came out as NF342
  said.
- **Copy-debt lines with no touched emitter:** 72 nf340 lines and 9 pickany_d2inlS15 lines.
- **Today's copy lines:** the nf358 REOPENED lines and the nf359 / nf360 lines.
- **Remaining pre-filter OPEN lines:** about 2,700 binder / self lines where the table, Leaf or self_symbols still shows the obstacle,
  plus the 806 OTHER lines (clusters, identity research, debt, held, drectve).
- **No acctB_b25 line** exists in the queue.
- **The stop rule.** After 150 compiles the hit rate was 2 closes and 4 rewrites; round 2 found the second close (3537) through the copy
  census. Round 3 (48 rows) found nothing, so I stopped at 393. That is about one close per 200 compiles.

## Surprises / findings for the coordinator
1. **Line 3560 is not resolved.** The line now lists no emitter, but six landed rows still emit a NEQ leaf-called copy of 0x009AC700. One
   of them, 0x00E55CE0, is an nf353-held row; the other five are outside nf353's family port. The fix (header text or local model) is
   not measured.
2. **Stale table lines whose witness no longer binds them:**
   - `?HasFreeActionPoint@CTCActionPoints@@QBE_NXZ 00780400 006a8a80`
   - `?Teardown@CPairedNodeLists@@QAEXXZ 009f7250 009f7330`

   NF303's census counted both of these as open stand-ins. It read the table, not the objects.
3. **Pre-filter blind spot (NF342's limit, confirmed).** The pre-filter keys on the table, which keeps one witness per spelling. Of the
   lines it called OPEN, compiling found real progress in 1010 and 1134; 470 was found the same way.
4. **0x0043DD40** (line 3660) defines a different stand-in from the one the line names.

## Rules / process
- **Locks.**
  - Owner NF375 on all 393 compiled rows, claimed only from missing or `released`, with the ledger at `work/locks_held.txt`.
  - Order per row: test precheck, then claim, then source precheck, then compile.
  - All 393 are released (written `released`).
  - Skipped rows under a foreign live lock: none. The only live foreign locks were COORD 00402510 / 00d11930 and NF344 0064b1ac /
    00e4fa20, and none of them was needed.
- **Stay-off list** (`work/stayoff.txt`, 943 rows): NF374's addrs.txt, plus the binder addresses in TARGETS.tsv `spellings`; NF376's five
  rows; Account B's claimed rows and pending/*acctB* rows; the 28 rows of the in-flight nf374_b1 tree. The stay-off check is enforced
  inside `census.py pay`, not only when the lines are picked.
- **Prechecks.** Clang `-fsyntax-only` on HEAD source and test for every row.
  - 74 tests and 16 sources failed clang only on VC7.1 system headers (cwchar / functional / ostream) or OWN MSVC-only syntax. All passed
    D1 `cl /Zs` (`work/pre_zs.log`).
  - All 393 passed one of the two.
- **Process.** No sleep loops beyond waiting on done files. Each vland chunk ran with `timeout 900` on its own launch line. Object folders
  were removed with clean.py after the census was read; evidence was kept.

## Files
- `STALE_T3.tsv` (line = snapshot line, line_now = live line by sha)
  - Columns: line, line_now, sha, source, address, unit, reason (the queue line verbatim), kind, proof, proof_files.
  - Close by sha (the first 12 hex digits of the sha1 of the full line); line_now is where that sha sits in the live queue now.
- `PARTIAL.tsv`: the same leading columns, then done_and_left, proposed_reason (a replacement for column 4) and proof_files.
- `UNPROVEN.tsv`; `work/checked.tsv` (all 146 checked lines); `work/verdicts.tsv` (judge.py over the whole queue).
- `evidence/census/<addr>.json` (393 HEAD objects); `evidence/pk/<addr>.pkall.tsv` + `.pkold.txt`; `evidence/lines/<line>.txt` (the 7
  STALE / PARTIAL lines).
- `work/manual.tsv` and `work/copy_manual.tsv`: hand verdicts, each with its reason. `work/rewrites.tsv`.
- `tools/` (README.txt lists the NF375 additions).
