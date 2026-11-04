# Plan phase 3, wave w3 (2026-11-04): CArray onto the shared header (608 rows)

## Result
- **608 landed rows** now include `engine/CArray.h` instead of a local copy. 360 of them are the empty-vector variants.
  - hdrmig3 now also removes namespace-scope STLport helpers and a local placement new when the header already defines them. That
    raised the wave from 393 rows to 608.
  - No emitted symbol changed and no header changed.
- **Coordinator re-grade:** vland `--recheck-landed` gives 608 / 608 PASS, 87 MATCH + 521 RELOCATION_MATCH, with the same parity as
  before (`logs/regrade_vland.log`).
  - gate.py: 551 PASS. The other 57 have "no usable index entry", the same index-refresh ERR as in w2.
  - In the plan agent's run against a good index, gate.py passed 608 / 608.
- **Deferred:** 681 (`DEFERRED.tsv`):

  | Reason | Rows |
  |---|---|
  | compile_fail | 329 |
  | local_std (local map/set/_Rb_tree models) | 203 |
  | bitset_index | 59 |
  | bytes_differ | 59 |
  | stray_change | 31 |

## Local copies still left after phase 3 (plan agent's count)

| Class | At phase start | Now |
|---|---|---|
| CVectorMap | 2,348 | 1,077 |
| C3DVector | 3,450 | 1,363 |
| CArray | 1,320 | 685 |
