# Landing RP1 (Account A, 2026-11-24): the CVsyncCallbackTimer header re-port, 29 rows, four days late

RP1 handed back on 2026-11-20 (COORDINATION_LOG: "RP1 handed back (29 rows; 0x009C0E50 waits for option B)"). Its landing prep began
at 08:45 that day, stopped, and left no stated reason. The hand-back stayed in session scratch, and its claim line (status
handed-back) and 30 lane locks went on holding its rows: unit1, unit2 and unit3 each held the 0x004166E2 / 0x00417001 unit because of
them. Advisor decision (owner delegated, 2026-11-24): land it rather than release it, re-based on the true base, with the checks below.
The hand-back is now in the repo: `pending/2026-11-24_rp1_handback/`.

## What it does
- `engine/CVsyncCallbackTimer.h` gains add-only opt-ins: `CVsyncCallbackTimerInfo` (Ego_r layout, 0x30 bytes, a size assert),
  nested `CPendingQuery`, and the dtor / GetInfo / OnPreDeviceReset / OnPostSwapScreens declarations, each behind its own
  `FABLE_RETAIL_CVSYNCCALLBACKTIMER_*` macro.
- 29 rows drop their row-local CVsyncCallbackTimer / CVsyncCallbackTimerInfo / IDirect3DQuery9 models for the shared header: the
  `list<CPendingQuery>` STLport instances at 0x009F76F0..0x009F7EC0, GetInfo and the Info ctor, the CDisplayManager callers
  0x009BEEB0 / 0x009BF2D0 / 0x009BF500 / 0x009BFB30 / 0x009C01C0, 0x009BE870, and 0x004166E2. Each row's graded symbol is unchanged.
- Names: RP1's only rename (0x009F73C0 OnPresent -> OnPostSwapScreens, Ego_r + FableWin) was landed meanwhile by nf296; the table binds
  the PDB spelling. 0x009BEEB0's test was still on the old local name, so it takes RP1's respelled test.

## Re-base
- True base: the claim commit fd00a40a60 (08:02; RP1's locks date from 08:01, its files from 08:45). HEAD's CVsyncCallbackTimer.h equals RP1's `orig/` copy,
  so the header applies as is.
- 27 rows are unchanged since the base and take RP1's text. Two rows changed after it and were three-way merged (base / HEAD / RP1):
  - 0x009BEEB0 (nf296): RP1's header include replaces the local model that nf296 had respelled;
  - 0x009BFB30 (nf297 / nf314 / ea72dc6eef): RP1's `(_D3DFORMAT)` cast combined with HEAD's `GetClosestDepthBufferFormat` respell.
- Not landed: 0x009C0E50 (blocked until option B, unchanged); 0x009BF7E0 was a check row only.

## Gates
- `pin_asserts.py`: nothing to pin; the only new assert is inside the new opt-in. `assert_names_check`: 0 mismatches.
- Whole-tree equivalence (57,146 rows): 4,250 differ in the raw run, all path-only `__FILE__` lines (now common in shared headers).
  Rerun with the tree path normalised (`logs/equivalence_path_normalised.tsv`): only 26 rows differ, all RP1's own rows.
- Recheck on the landed tree (`logs/recheck.log`, VC7.1): 29 of 29 good.

## Bookkeeping
- Claim line closed (`done`), 30 RP1 lane locks released. The 0x004166E2 / 0x00417001 unit goes back to the unit route's wave 4, which
  regenerates its edit on this text.
