# Pick-any all-copies audit (agent NF340, Account A, 2026-11-24)

**Why:** Account B's B24C9 finding. `pickany_vs_retail.py` compares only the COMDAT copies whose names appear in reloc_bindings.tsv, and
`/d2inl*` flags change copies across the whole TU.

**What was built:** `tools/decomp_pipeline/linux_lane/lane_tools/pk_all/pk_all.py`. It finds each copy's retail address from four sources:
- (a) the row's own leaf relocations, using the grader's mapping;
- (a2) the same, followed transitively;
- (b) reloc_bindings;
- (c) self_symbols.

Copies are compared with relocations masked, as in the old checker. The old checker found 117 of the 127 problem copies; the 10 it missed
are anonymous-namespace (`?A0x`) instantiations.

**Rows audited:** all 227 landed `/d2inl` rows and a random sample of 300 other landed rows (seed 340).

## Results
| | /d2inl (227) | sample (300) |
|---|---:|---:|
| function COMDAT copies | 8,233 | 1,377 |
| EQ / NEQ / NOADDR | 1,089 / 395 / 6,749 | 206 / 70 / 1,098 |
| **NEQ copies the leaf calls** | **102 copies in 63 rows (28%)** | **25 copies in 18 rows (6.0%, 95% CI ~3.8-9.3%)** |
| the old checker also reports these | 92 of 102 (the 10 missed are all `?A0x`) | 25 of 25 |

Notes on the results:
- **Extrapolation:** the 6.0% sample rate suggests that about 3,400 of the ~56,900 non-`/d2inl` landed rows (range ~2,200-5,300)
  emit a leaf-called copy that differs from retail. The 63 `/d2inl` rows come on top of that.
- **The old checker's blind spot is small:** copies with no table line under their own name. That covers `?A0x` names, and stand-in
  spellings such as B's 0x00711A50.
- **`/d2inl` control:** 17 leaf-called NEQ copies in 15 rows are caused by the flag; dropping it makes them EQ. 75 copies are NEQ either
  way. The flag also repairs 16 copies, and 14 `/d2inl` rows still match without it.
- **Validation:**
  - B's 0x0070D940 case reproduces with a reduced probe: the `~vector` 0x00711760 and `_M_insert_overflow` 0x00711A50 copies come out NEQ
    under `/d2inlT67` without `DESTROY_AUX_FASTCALL`.
  - On five rows the old checker passes, every old EQ line is reproduced.
  - The full B source could not be built: B's 0x0070D940 overlay is not on disk.
- **Address sources:** the a / b sources never disagree. All 25 ADDR_CONFLICTS lines are byte-identical retail bodies at two addresses.

## Rulings (advisor decision, owner delegated)
- **Keep both checkers.** `pk_all.py` is added and `pickany_vs_retail.py` stays, because pk_all skips a row's own leaf. That is how the old
  checker surfaced the three identity conflicts below.
- **Gate for Account A's landings, starting now.** `verify_and_land.py` stays unchanged until B agrees.
  - **A row change:** run pk_all on HEAD and on the new object.
    - A newly leaf-called (sources a / a2 / o) NEQ copy holds the landing, and so does a leaf-called copy that turns NEQ.
    - A copy that was already NEQ is debt.
    - A NEQ backed only by a table address (b / c) is a warning.
  - **A new row:** any leaf-called NEQ copy holds it. This is the existing rule, and it is why B's 0x00839940 is held.
    Exception (advisor decision, owner delegated, 2026-11-24; EXTRA_RULES 2026-11-24 pick-any entry): the row lands as debt when ALL hold: (1) the NEQ copy equals a landed row's HEAD copy at object level (bytes, relocation symbols, COMDAT selection); (2) no fix is on record (measured copy-EQ texts all move the leaf); (3) every other leaf-called copy is EQ under both checkers on A's object; (4) the row carries a `// Pick-any:` note and the copy's T3 line lists it as an emitter. Where a fix exists the row still holds and takes it (0x00839940). First use: 0x00ED8680 (batches/2026-11-24_landing_acctB_b24d).
  - **Every CompilerFlags change** (`/d2inl*`, `/Os`, `/O1`, `/GS`) goes through this gate.
- **The debt is queued per copy, not per row.** Each line's column 2 is the copy's retail address and its reason lists the rows that emit it:
  97 copy groups from 127 copies (`DEBT_BY_COPY.tsv`). The repair unit is the header text or local model that emits the copy, not the row.
- **No lane-wide run for now.** At about 4 s per row it would starve the landing rechecks of CPU. Grouping by copy text and grepping for the
  rows that define it finds the affected rows without compiling.

## Side findings, queued
- **Three rows whose own leaf is bound to another address in reloc_bindings** (NAME_FIXES_QUEUE `nf340`):
  - 0x0090310E `??0CAIStateGroup_HoverHornetQueen` -> 0x00AB3B10;
  - 0x00CBD310 `?push_back@CScriptThingVector` -> 0x008ADF90;
  - 0x00643D8C `?AddHitByMelee@CPlayerGui` -> 0x00437990.
  A no-compile census of landed Leaf lines against table targets, finding the rest of this kind, is queued.
- **Old-checker defects:** its LEAF-REFERENCED tag is wrong on 10 NEQ lines (9 missed, 1 false), and it crashes on 0x012248B0.

**Caveat (coordinator, 2026-11-24):** the non-/d2inl rows are a 300-row SAMPLE, so DEBT_BY_COPY.tsv's per-copy emitter list is a floor, not a count (NF357 found 34 NEQ emitters of GetSubDef<CAppearanceDef> 0x0042B0A2 where this file lists 1). Never close a copy's T3 line on this file alone: confirm it with a retail-caller census (NF354 / NF358 method) first. The three lines closed on it on 2026-11-24 (0x00429C6F, 0x0042B312, 0x0042B040) are being re-checked by NF358.
