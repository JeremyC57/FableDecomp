# NF342 hand-back (Account A naming queue, 2026-11-24): stale-line sweep of T3_PORT_QUEUE.tsv

This is a read-only audit; it changed no rows. It follows NF339's method (`pending/2026-11-24_nf339_handback/`), widened from the 227
"self respell" lines to the whole queue. NF339's 138 stale lines were already closed by `batches/2026-11-24_landing_nf339_b1/T3_CLOSED_STALE.tsv`.

The queue was read at 09:14 with 3,742 lines. At 09:44, NF340 appended 99 lines (97 "pick-any debt (copy)" and 2 "audit"). They were
written minutes earlier and are counted as OTHER, so the queue now has 3,841 lines. The first 3,742 lines are byte-identical before and
after; the per-line sha1 was rechecked. Inputs: reloc_bindings.tsv and OPEN.tsv at 09:14, build_candidates.ps1 at 09:05. None of the
247 compiled rows behind a STALE or PARTIAL line changed after it was compiled; source sha1 was rechecked.

## Result
- **263 stale lines** (`STALE_T3.tsv`) on 189 addresses. Each one is proven by a HEAD compile.
  - **self, 249 lines** (239 self + 10 self-shaped lines with free-text units). Proof:
    - the row's HEAD object DEFINES the line's PDB symbol P;
    - the grade compares that same symbol, with MATCH / RELOCATION_MATCH and the test PASS;
    - no stand-in mangled symbol the line names is left in DEF or UND.
  - **binder, 14 lines.** Proof:
    - every named binder that binds the target binds only P there. This is read from the binder's graded function, with each relocation
      resolved against the retail bytes;
    - P is UND in every binder and is never a strong DEF;
    - the table has no non-P spelling left at the target;
    - the target row itself DEFINES P.
  - **Where P comes from.** For 192 lines, P is in the line's own text and is an exact member of the Ego_r / FableWin name sets. For the
    other 71, P is the target's agreed spelling, and each of those has an independent source:
    - NF339's STALE_T3 for that address (52 lines);
    - NF303's census evidence (5);
    - another queue line at the same address that gives P in its own text (5);
    - a stand-in quoted in the line for the same member (5);
    - checked by hand from the line's demangled wording (4: lines 8, 216, 293, 554).
  - **table, 0 lines.** All 4 nf333 table lines are still in `reloc_bindings.tsv`.
  - **By source:** nf319 67, nf320 63, nf322 33, flatten-switch1 28, nf317 21, nf323 16, nf318 9, nf324 5, nf303 5, nf289 5, nF325 3,
    nf321 2, access2 2, nf301 1, nf291 1, nf290 1, nf285 1.
  - **Tracker status.** 261 of the 263 sit on addresses the tracker counts as done. The other two are access2 0x00665E90 / 0x009D4A50:
    their self is done, and the target stays `split_fixable` only through caller spellings. Those two lines were not in NF339's 227-line
    ENTRIES, so there is no conflict with NF339. Their fix came with landing nf317.
  - **Other lines on stale addresses.** These were judged on their own text, and 7 stay open:
    - nf311 0x00CBFB7D and nf309 0x00D890A0: the binders still bind stand-ins;
    - nf322 0x00BBDDB0 / 0x00900D3D: class-model defects;
    - nf337 0x0045D2FC / 0x0045E55A / 0x004604CF: the D-3 Copy-helper swap.
- **15 partial lines** (`PARTIAL.tsv`; column done_and_left says what is left).
  - **The named binders are fixed but something else is left (11 lines):**
    - the target row still defines a stand-in: nf293 0x00A3AB90 / 0x00855AF0, nf297 0x009F1650 / 0x009FF8D0, nf307 0x00766050 /
      0x007660C0, nf298 0x00ADF2A0;
    - other binders still hold non-PDB table spellings: nf299 0x00A3A9C0 / 0x00A3AA00, nf307 0x004C0240, nf297 0x0040A740.
  - **Some named binders are fixed and others still bind stand-ins (3 lines):** nf293 0x0099F2E0, nf303 0x00431F10 / 0x00463950.
  - **nf299 0x0099B4D0 (1 line):** the three CWideString copies now DEFINE only the PDB IAE spelling; QAE is absent from their DEF.
    They are still strong same-TU copies (COMDAT NODUPLICATES).
- **3,578 lines are not shown stale** (3,841 - 263). Only some of them are proven open:
  - **15 partial lines** (above).
  - **79 compiled and still open (OPEN_CHECKED):**
    - 67 self rows that do not define P;
    - 9 binder lines whose binder still binds a stand-in or DEFINES P as a strong copy;
    - 3 nf326 unit rows whose self is still the stand-in.
  - **2,712 open by the pre-filter, not compiled.** In each, one of these still holds:
    - a named binder still witnesses a non-PDB spelling in `reloc_bindings.tsv`;
    - a stand-in the line names is still a table spelling;
    - the row's full `// Leaf:` is not P;
    - `self_symbols.tsv` measured another symbol, and the source has not changed since that 03:05 measurement.
  - **762 other**, not decidable by script:
    - shared-name clusters 237: the made-up names are still graded by the rows;
    - identity research 138: all 105 route4 addresses are still `rtti_conflict` / `rtti_identified` in the tracker;
    - debt 152, including NF340's 97 new lines;
    - drectve units 23;
    - same-TU bodies and owner Alloc copies 35;
    - held-by-ruling, rejected and family units;
    - free-text model / scope / type ports (nf294 / nf295 / nf296 / nf313 / nf314 ...);
    - 2 audit lines.
  - **10 with no exact PDB target (NO_P).** This includes rename1 0x00BDC180: its line records a rejected rename with empty from / to
    spellings (`batches/2026-11-24_landing_rename1/RESULTS.tsv`), and the row is itself a binder in open lines.

### By kind

| kind | lines | STALE | PARTIAL | OPEN_CHECKED | OPEN | UNPROVEN | NO_P | OTHER |
|---|---:|---:|---:|---:|---:|---:|---:|---:|
| binder | 1686 | 14 | 15 | 9 | 1637 |  | 9 | 2 |
| self | 1328 | 239 |  | 67 | 1022 |  |  |  |
| other | 760 |  |  |  |  |  |  | 760 |
| unit | 37 |  |  | 3 | 34 |  |  |  |
| self? | 24 | 10 |  |  | 13 |  | 1 |  |
| table | 4 |  |  |  | 4 |  |  |  |
| binder2 | 2 |  |  |  | 2 |  |  |  |
| **total** | **3841** | **263** | **15** | **79** | **2712** | **0** | **10** | **762** |

### By source tag

| source | lines | STALE | PARTIAL | OPEN_CHECKED | OPEN | UNPROVEN | NO_P | OTHER |
|---|---:|---:|---:|---:|---:|---:|---:|---:|
| nf303 | 703 | 5 | 2 |  | 681 |  | 1 | 14 |
| nf324 | 623 | 5 |  | 24 | 583 |  |  | 11 |
| flatten2 | 237 |  |  |  |  |  |  | 237 |
| nf311 | 154 |  |  | 1 | 152 |  | 1 |  |
| nf319 | 146 | 67 |  | 8 | 71 |  |  |  |
| nf291 | 108 | 1 |  |  | 105 |  | 2 |  |
| route4-identity | 106 |  |  |  |  |  |  | 106 |
| nf320 | 105 | 63 |  | 4 | 38 |  |  |  |
| nf340 | 99 |  |  |  |  |  |  | 99 |
| nf322 | 86 | 33 |  | 3 | 48 |  |  | 2 |
| nf308 | 76 |  |  |  | 75 |  |  | 1 |
| nf321 | 72 | 2 |  | 7 | 63 |  |  |  |
| nf309 | 70 |  |  | 2 | 67 |  |  | 1 |
| nf285 | 64 | 1 |  | 2 | 58 |  | 2 | 1 |
| nf317 | 62 | 21 |  | 4 | 36 |  |  | 1 |
| nF325 | 56 | 3 |  | 3 | 50 |  |  |  |
| nf289 | 56 | 5 |  |  | 50 |  | 1 |  |
| nf326 | 54 |  |  | 5 | 48 |  |  | 1 |
| nf294 | 48 |  |  | 1 | 20 |  |  | 27 |
| nf334 | 48 |  |  |  | 33 |  |  | 15 |
| nf287 | 45 |  |  |  | 42 |  | 2 | 1 |
| nf298 | 41 |  | 1 | 1 | 11 |  |  | 28 |
| nf312 | 40 |  |  |  | 38 |  |  | 2 |
| nf301 | 37 | 1 |  |  | 36 |  |  |  |
| nf302 | 37 |  |  |  | 35 |  |  | 2 |
| nf323 | 37 | 16 |  | 2 | 9 |  |  | 10 |
| nf310 | 36 |  |  |  | 32 |  |  | 4 |
| nf295 | 34 |  |  |  | 16 |  |  | 18 |
| nf318 | 34 | 9 |  | 1 | 24 |  |  |  |
| nf314 | 32 |  |  |  | 13 |  |  | 19 |
| nf306 | 30 |  |  |  |  |  |  | 30 |
| flatten-switch1 | 29 | 28 |  |  | 1 |  |  |  |
| nf296 | 29 |  |  |  |  |  |  | 29 |
| nf297 | 29 |  | 3 |  | 26 |  |  |  |
| nf313 | 29 |  |  |  | 22 |  |  | 7 |
| nf305 | 27 |  |  |  | 27 |  |  |  |
| alias1 | 24 |  |  | 1 | 23 |  |  |  |
| nf300 | 24 |  |  |  | 23 |  |  | 1 |
| nf328 | 23 |  |  |  |  |  |  | 23 |
| access2 | 22 | 2 |  | 4 | 16 |  |  |  |
| nf290 | 22 | 1 |  |  | 20 |  |  | 1 |
| nf307 | 20 |  | 3 |  | 16 |  |  | 1 |
| nf331 | 20 |  |  |  |  |  |  | 20 |
| access1 | 17 |  |  | 2 | 15 |  |  |  |
| nf293 | 15 |  | 3 |  | 7 |  |  | 5 |
| access3 | 14 |  |  |  | 14 |  |  |  |
| nf304 | 13 |  |  |  | 12 |  |  | 1 |
| nf299 | 11 |  | 3 |  | 5 |  |  | 3 |
| pickany_d2inlS15 | 11 |  |  |  |  |  |  | 11 |
| flatten1 | 10 |  |  |  |  |  |  | 10 |
| nf315 | 10 |  |  |  | 8 |  |  | 2 |
| nf288 | 8 |  |  | 1 | 6 |  |  | 1 |
| nf292 | 7 |  |  | 1 | 4 |  |  | 2 |
| tracker | 7 |  |  | 1 | 6 |  |  |  |
| access4 | 6 |  |  |  | 6 |  |  |  |
| nf286 | 5 |  |  |  | 4 |  |  | 1 |
| nF327 | 4 |  |  | 1 | 3 |  |  |  |
| nf283 | 4 |  |  |  | 1 |  |  | 3 |
| nf333 | 4 |  |  |  | 4 |  |  |  |
| nf337 | 4 |  |  |  |  |  |  | 4 |
| nf330 | 3 |  |  |  | 2 |  |  | 1 |
| binders1 | 2 |  |  |  | 2 |  |  |  |
| nf284 | 2 |  |  |  | 1 |  |  | 1 |
| nf316 | 2 |  |  |  | 2 |  |  |  |
| nf335 | 2 |  |  |  |  |  |  | 2 |
| acctB_b24B6 | 1 |  |  |  |  |  |  | 1 |
| acctB_b24C8 | 1 |  |  |  |  |  |  | 1 |
| flatten-local2 | 1 |  |  |  | 1 |  |  |  |
| flatten4 | 1 |  |  |  | 1 |  |  |  |
| nf336 | 1 |  |  |  |  |  |  | 1 |
| rename1 | 1 |  |  |  |  |  | 1 |  |
| **total** | **3841** | **263** | **15** | **79** | **2712** | **0** | **10** | **762** |


Verdicts:

| Verdict | Meaning |
|---|---|
| STALE / PARTIAL | As above. |
| OPEN_CHECKED | Compiled at HEAD; the fix is not there. |
| OPEN | The pre-filter shows the obstacle is still there; not compiled. |
| NO_P | No exact PDB spelling could be fixed for the line. |
| OTHER | Not decidable by script. |

Every line, with its verdict and reason, is in `work/verdicts.tsv`. The classification inputs are in `work/lines.tsv`.

## How
1. **Classify** (`tools/classify.py`). Each line gets one kind:

   | Kind | Meaning |
   |---|---|
   | self | The column-2 row's own spelling moves to P. |
   | binder | Column 2 is a table target; the binders bind P. |
   | unit | nf326 "outside the CScriptThing member unit" / "binders bind". That agent's STATUS says the open spelling may belong to the self or to a binder, so done means the whole target agrees. The queue copy of the reason is cut at about 220 characters, so the untruncated reason is read from nf326's STATUS.tsv. |
   | binder2 | nf324 "table binder of X still references S". |
   | table | nf333. |
   | other | Everything else. |

   P comes from the line text when it is an exact PDB member. Otherwise it is the tracker's `pdb_spelling` / `rtti`, the one PDB name that
   shares `?name@scope@@` with the old spelling the line quotes (the nf317 form), or the target's agreed spelling, which then needs
   corroboration (above). Binders come from three places:
   - the line text;
   - NF303's `evidence/census.tsv` for nf303 lines;
   - the agent's STATUS.tsv binder column for 3 lines (the nf289 / nf290 binders were checked against that column too).

   A family review fixed four misreadings before any verdict:
   - nf326 binder-obstacle lines look like self lines whose self already defined P;
   - "as 0x…" mentions are not binders;
   - target lines that name other rows without the word "binder" are binder lines;
   - "held" self lines (skipped-held / NF323 Q1) are self lines.
2. **Pre-filter**, with no compiles. Inputs: OPEN.tsv (09:14), `reloc_bindings.tsv`, the Leaf lines, and `self_symbols.tsv`. The last is
   used only for rows whose source is older than it; NF339 found it stale for rows changed after 03:05.
3. **Prove.** 351 addresses were compiled at HEAD with the catalog flags: a recheck.py payload, then `vland.py --recheck-landed`, in 9
   background chunks. Each chunk had `timeout 900` on its own launch line.
   - **Prechecks first.** Every HEAD source and test passed clang `-fsyntax-only` beforehand (`work/pre.log`). 16 files passed only
     through D1 `cl /Zs` (`work/pre_zs.log`). 13 of those failed clang only on `_assert`, `<cwchar>` or iostream. The other 3 failed in
     their own lines (see D5).
   - **No locks.** No compiled row was locked by another agent at payload time; every `cen_*/skipped.tsv` is empty.
   - **Evidence.** For each compiled row, `evidence/census/<addr>.json` holds the grade, the graded symbol, DEF / UND, the COMDAT
     selection of each DEF, and every relocation of the graded function resolved to its retail target.
4. **Calibration.**
   - **Positive.** All 62 queue lines at NF339's 30 stale addresses were judged on their own text, and all are STALE.
   - **Negative.** 28 controls all came out OPEN_CHECKED, so the sample has no false negatives. They were 25 random self rows the
     pre-filter called open through `self_symbols.tsv`, plus the 3 rows with a full Leaf != P changed since 03:05. Also compiled: 29 lines
     (12 rows) with no usable Leaf whose source changed after 03:05. None of them defines P.

## Decisions needed
- **D1. Close the 263 lines in `STALE_T3.tsv` without a landing,** as NF339 D1 / landing nf339 did.
  - Delete each by `line` + `sha`, where sha is the first 12 hex digits of the sha1 of the full queue line. Line numbers are those of the
    09:14 file; NF340's lines were appended after them.
  - The `landed_by_candidates` column lists every later batch or hand-back that names the row, excluding the line's own landing. It
    checks rows.txt, RESULTS, TABLE_CHANGES, RENAMES and APPLIED. Same-day non-nf batches (alias1, access3, flatten_*) may predate the
    line.
  - Typical fixers:
    - self lines: nf322 / nf323 / nf325 / nf327 / flatten4;
    - binder lines: the nf292 / nf293 hand-backs and nf337 b1 / b4.
  - nf285 0x00997F80 names no landing: binder 0x00999230 is named only by the nf311 hand-back.
- **D2. The 5 nf324 CIEngine "held" lines** (0x00B236B0 / 0x00B236C0 / 0x00B236D0 / 0x00B236F0 / 0x00B23710). NF324 already marked them
  "triage (-): done: the landed row already defines the PDB symbol". The census confirms it, and landing flatten4 names the rows.
- **D3. PARTIAL lines:** either split each of the 15 into its done half and the rest, rewriting the line to what is left, or keep them
  whole.
- **D4. Strong-copy lines.** In 5 lines a binder binds P but DEFINES it as a strong same-TU copy (COMDAT selection 1): nf288 0x0055FB80,
  nf292 0x00A99110, nf309 0x0094FBB0 / 0x00F3B970, and nf299 0x0099B4D0. The first four are recorded OPEN_CHECKED and the last is
  PARTIAL; each line's own subject is the strong copy.
- **D5. Prechecks that passed only through `cl /Zs`** after clang errors in the file's own lines. NF339 used NF305 Q2 as precedent for the
  same kind of case.
  - H_004eb6e7 (target of STALE line 1294): the MSVC multi-word functional cast `unsigned int(expr)` at lines 64-65.
  - H_00997f80 (`wcscmp` on the short-wchar_t `CBasicStringW`) and T_00997f80 (explicit specialization of `compare` after
    instantiation): target of STALE line 8.
  - Please confirm that these count; otherwise lines 8 and 1294 move to UNPROVEN.

## Limits
- **OPEN lines were not compiled.** The table keeps one witness per spelling. A binder can still reference a retired stand-in that the
  table no longer shows, which could hide a PARTIAL line.
- **Binder STALE covers named binders only.** STALE for a binder line rests on the named binders, compiled; the table's clean state at the
  target; and the target row's DEF. Unnamed binders were not compiled. For the 14 binder STALE lines the table holds only P at the target,
  but a binder that the table does not witness could still bind a stand-in.
- **Other lines were not examined.** Cheap table / tracker checks only confirm route4 and flatten2 are still open.
- **Rule compliance.**
  - No git and no repo writes. `PYTHONDONTWRITEBYTECODE=1` was set, and recheck.py wrote only into my folder. Every wd was removed with
    `clean.py`.
  - No sleep that waited: one no-op `sleep 0` slipped into a command at 09:41.

## Files
- `STALE_T3.tsv`. Columns: line, sha, source, address, unit, reason (the verbatim queue columns), kind, pdb_symbol, proof,
  landed_by_candidates, remaining (tracker status).
- `PARTIAL.tsv`. Same columns, with done_and_left in place of proof.
- `work/verdicts.tsv` (all 3,841 lines), `work/lines.tsv`, `work/manual.tsv` (17 manual verdicts with reasons), `work/summary_tables.md`.
- `evidence/census/*.json` (351 HEAD objects) and `work/cen_*/vland.log` + payload.json.
- `tools/` (README.txt): common.py, classify.py, census.py + cen_run.sh, pre_rows.sh, zs_list.sh, judge.py, summary.py, nf.py.
