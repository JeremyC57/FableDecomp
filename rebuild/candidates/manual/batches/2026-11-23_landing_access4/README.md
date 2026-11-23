# Landing access 4 (Account A, 2026-11-23): 72 rows onto their PDB symbol by access / const / virtual / return edits (route 3a, full-Leaf rows)

Script: `tools/decomp_pipeline/linux_lane/lane_tools/access_fix.py --wide --virtual --ret`, landed with `land_scripted.py`. The edits,
identity rules and per-row proof are the same as access 3 (`batches/2026-11-23_landing_access3/README.md`).

- **What is new: the pool's self symbol.** A row's full mangled `// Leaf:` now takes precedence over the measured symbol. This adds the
  rows the measurement pass never measured (they already had a full Leaf) and stops already-landed rows from being re-selected. A run before
  the fix re-selected 40 rows landed earlier today; their regenerated sources were identical to HEAD, and nothing landed from that run.
- **Run `vr4`.** 807 candidates; 79 generated, 72 accepted (91%), 7 rejected (queued as `access4`), 458 skipped.

## Recheck
`logs/recheck.log` (landed tree, 72 rows): 72 MATCH / RELOCATION_MATCH PASS, graded symbol = PDB symbol (`land_scripted.py`).
