# NF338 hand-back (2026-11-24): the 320 T1 entries re-read as T3 work

The coordinator's ruling lifted stay-off-T3 for the T3 lines that name my ENTRIES targets. Every one of the 320 targets is on such a line.
For each target, TRIAGE.tsv records the T3 tag, the recorded reason, and whether that reason still holds on today's tree (scripted re-read
plus compiled witness objects). STATUS.tsv has one line per entry with its T3 tag.

## Result
| status | entries |
|---|---|
| done | 62, 112, 186, 195, 223, 224 |
| done-partial | 38, 88, 119, 120, 149 |
| skipped-port (reason holds) | 205 |
| skipped-held (each wrong-spelling witness is itself reserved: T3 / NF336 / NF337 / Codex / B claim) | 99 |
| not-taken (named only in another line's prose) | 127, 128, 129, 168, 210 |

- **Rows changed:** 5, all binders, all compiled.
  - `0x00999110`, `0x0056FAD7` and `0x006116E0` are in `batches/b1`, which has the unit u62 = `0x0056FAD7` + `0x006116E0`.
  - `0x00405650` is in `batches/b2`, which carries one add-only header opt-in.
  - `0x00A3AA40` is alone in `batches/b3` (see D1).
  - `batches/README.md` gives the landing order.
- **Checks on every changed row:**
  - HEAD and new compiled at one path, objsec ALL_EQ (`.drectve` included, DEF sets equal; the only change is the renamed external);
  - link HEAD v new: the new spelling resolves, 0 duplicates;
  - `vland --recheck-landed` RELOCATION_MATCH PASS (`recheck_final.log`; the WARNs are pre-existing).
- **Table:**
  - `batches/b1/TABLE_CHANGES.tsv`: 1 delete, witness `0x0056FAD7`.
  - `batches/b3/TABLE_CHANGES.tsv`: 2 deletes, witness `0x00A3AA40`.
  - `batches/b2/TABLE_CHANGES.tsv`: 2 replaces, witness `0x00405650`, ref_scan rs3 NONE.
  - `TABLE_ONLY.tsv`: 6 deletes and 2 replaces whose witnesses are unchanged rows that already bind the PDB spelling (compiled).
  - All three files dry-run with `apply_table.py --dry`: misses 0, no new multi-target.
  - `TABLE_ONLY` and b3 converge in either order: the b3 lines for 119 / 120 are deletes, and TABLE_ONLY's `fn_a3a9c0` / `fn_a3aa00` replaces add the PDB lines.
- **T3 lines:** `T3_CLOSE.tsv` lists the 6 lines to close (done entries). `T3_KEEP_PARTIAL.tsv` lists the 5 lines to keep (partials, reason partly holds).
  - Close the 112 / 186 / 195 lines only once TABLE_ONLY is applied.
  - Close the 62 line with b1, and the 223 / 224 lines with b2.
  - `land_scripted --release-t3` cannot close these: their column-2 addresses are entry targets, not the accepted (binder) rows.

## Decisions needed
- **D1. `0x00A3AA40` is the column-2 address of nf285's line for entry 121.** That line's reason still holds: binder `0x00A3AC40`'s `_Retail` stand-in.
  - land_scripted therefore holds `0x00A3AA40` as T3. `--release-t3 nf285` would release it, but would also close a line whose reason still holds.
  - The row is therefore split into `batches/b3`, with the queue line verbatim in `b3/T3_READD.tsv`.
  - Options: land b3 with `--release-t3 nf285` and re-append that line, or land it by hand.
  - b1 and b2 need no `--release-t3`. The nf285 lines of 186 / 195 close by hand from `T3_CLOSE.tsv`; the nf285 line of 149 stays open (partial, `T3_KEEP_PARTIAL.tsv`).
  - The other four rows have no T3 mention (checked with land_scripted's own logic on today's queue).
- **D2. Stale open claim on `0x006116E0`.** COORDINATION_CLAIMS line 197 (`2026-11-20 A q6333,q6342,q12343 naming claimed ... NF214`) is still `claimed`.
  - `pending/2026-11-21_nf214_handback/README.md` says it LANDED 2026-11-21 as nf214.
  - land_scripted holds every address an open claim names, so this would hold `0x006116E0`, and with it unit u62.
  - Please close the claim with a `done` line before landing b1.
- **D3. b2 needs `batches/b2/hdr/fable_ime.h` copied into `rebuild/include/` before land_scripted runs.** Its recheck does not use an overlay.
  - The edit is add-only: `FABLE_RETAIL_CIME_CALLBACKS` declares the two PDB public statics in both CIME branches, plus `struct IMEUI_VERTEX;`.
  - Probe: the HEAD source compiled with the overlay is byte-identical to without it, apart from the COFF timestamp.
  - equiv_check over the 7 includers: 0 DIFF.
  - Planted control: CTL flags exactly the 7 includers' sources and tests. No header includes `fable_ime.h`, and the 00405650 test only names it in a comment.
  - A whole-tree equiv run (`work/eq/all.tsv`) was cut short: killed at hand-back, no `#DONE` line. It covered part of the tree with 0 DIFF; its 3 CTL lines are all includers (`0x012288B0`, `0x00851920`, `0x00405650`). The per-includer run above is the complete proof, given that no header includes `fable_ime.h`.
  - First attempt rejected: the first draft put the forward declaration above `FABLE_STATIC_ASSERT` (line 41). That shifted its `__LINE__` typedef name, and equiv showed 4 DIFFs (`evidence/equiv_includers_v1_REJECTED_line_shift.tsv`). rebuild_abi.h says stack-slot order can depend on those names. The forward declaration now sits after the assert, and the row edit is line-neutral (two comment lines joined), because the row has its own `FABLE_STATIC_ASSERT` at lines 77 / 78.
- **D6. Account B's pending `pending/2026-11-24_acctB_b24` has a new `hdr/00402510/engine/NUISystem_CIME.h`.** It is a second CIME class model, for row `0x00402510`.
  - It does not touch `fable_ime.h` or `0x00405650`.
  - If it lands, CIME is declared in two shared headers. The `FABLE_RETAIL_CIME_CALLBACKS` statics then belong in whichever header becomes canonical.
  - I did not edit or read beyond the file list (B's claim).
- **D4. Prose-only entries 127, 128, 129, 168 and 210 were not taken** (coordinator rule: listed here only).
- **D5. Entry 149's census binder `0x009994A0` was not compiled.** It is reserved by nf303 T3. By source it binds through engine/CAFile.h. 149 stays partial for that reason only.

## Per-entry notes on the in-place fixes
- **62:** the census reason ("free stand-in where the PDB has a member") was not an obstacle. Both binders already model `NUISystem::CManager` locally, so the PDB public static `OnOpenMenus` (Ego_r + FableWin) is declared there.
  - ref_scan rs2 found `0x006116E0` still binding the free spelling, so both rows land as unit u62.
  - `0x004830A1`, `0x0058578B` and `0x00622A00` already bind the PDB form (compiled).
- **119 / 120:** `0x00A3AA40`'s `ZAlloc` / `ZFree` became the PDB private statics `Alloc` / `Free`. Two sources:
  - Ego_r.pdb and FableWin.pdb `?Alloc@CCompressorZlib@@CAPAXPAXII@Z` / `?Free@CCompressorZlib@@CAXPAX0@Z`;
  - the table line from `0x00A3AA10`, whose compiled object already binds those exact spellings.
  - The partial part: `0x00A3AB00` still binds `ZlibAlloc` / `ZlibFree`, because its own self is the `CCompressorZlib_Retail` stand-in (a port).
- **Two sources for every respelled name:** Ego_r.pdb and FableWin.pdb both list them (`evidence/pdb_namesets_62_119_120_149.tsv` and `evidence/pdb_namesets_223_224.tsv`, also copied into each batch's `evidence/`).
- **0x00405650 is not fully PDB-spelled:** it still binds three free callbacks (`Callback_DrawRect` / `Callback_free` / `Callback_OnChar`) and the stand-in `NativeCIME::Reset`. They are not ENTRIES targets (no landed self rows), so they were left alone; the same opt-in could later declare them.
- **149:** `0x00999110` declared a free `GetCurrentWorkingDirectory`. It now calls `CAFile::GetCurrentPath()`, declared as a public static on its local CAFile model (Ego_r + FableWin `?GetCurrentPath@CAFile@@SI?AVCWideString@@XZ`). 14 other census binders bind the PDB form (compiled).
- **223 / 224:** see D3. Two sources for the names: Ego_r.pdb and FableWin.pdb both list them (`evidence/pdb_namesets_223_224.tsv`); the landed self rows' Leaf lines agree.
- **Stale table lines (38, 88, 112, 149, 186, 195, and 119 / 120's `fn_` lines):** each was proven from the witness object compiled at HEAD (NF309 Q4). The PDB line exists for every delete.
  - 88: ref_scan would be TOO_MANY on `CCharString`. A brace-aware scan finds one landed source declaring a `CCharString` inside `namespace std`: `0x004D0830`. Its HEAD object (compiled) does not reference `??0CCharString@std@@QAE@ABU01@@Z`.
  - The first grep missed `0x004D0830` because its pattern stopped at the first `}`.
  - 149: ref_scan rs4 reports TOO_MANY on the decorated C name. No landed source names `FableGetCurrentPath_00997F80`.

## The 205 skipped-port and 99 skipped-held
The script checked whether each line's reason still holds:
- a stand-in identifier is still in each free binder's code; or
- for the 46 "reread-needed" entries, the witness object, compiled at HEAD, still binds the recorded non-PDB spelling (`work/witbind.tsv`, basis column of TRIAGE).
  - What this measures: only the binding was compiled and checked. The recorded obstacle itself (a port, COMDAT rename, LNK2005 or DIFF) was read, not re-measured.
  - The same test alone would also have called 62 and 223 / 224 "holds". Those two had census-only "FREE stand-in" reasons, and fixing them in place proved the obstacle was not real.
  - Of the 46, the census-only reasons (WRONGCLS / NOTPDB, from census309) are the likeliest to hide more cheap in-place fixes of that kind: 202, 220, 228, 235, 236.

The reasons are the recorded measured or stand-in ports: class / namespace re-scopes that rename a self or a pick-any COMDAT, enum ports without a shared header, same-TU stand-in bodies (LNK2005 risk), and grade DIFFs. Each is a port, not a respell.

## Locks and slips
- **Locks released:** 630 census / control locks (`work/release.log`). Then, as the last step, the changed-row locks and the two ref_scan locks (`0x00404A60` / `0x00404E10`), so land_scripted does not hold them.
- **Census not compiled in full:** 530 census binders were locked and prechecked, but not compiled. The advisor narrowed the work to the reread-needed binders, whose witnesses were already compiled. 8 census prechecks failed (`work/cen_fail.txt`); they were not needed.
- **Slips:**
  - one `sleep 0` in a status command earlier in the session;
  - the 00999110 test was prechecked after its lock (the lock was taken first for a control compile);
  - the g5 script ran the new-source precheck and the grade in one background script, so the precheck did not gate the compile (it passed).
