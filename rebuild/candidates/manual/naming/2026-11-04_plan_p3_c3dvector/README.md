# Plan phase 3, wave w2 (2026-11-04): C3DVector onto the shared header (2,087 rows)

## Result
- **2,087 landed rows** now include `engine/C3DVector.h` instead of a local copy. Lower-case `x/y/z` member spellings were changed to
  the header's `X/Y/Z`.
  - The rows cover variants 0-22 of the local copy (variant 0 alone is 1,189 rows).
  - No emitted symbol changed and no header changed, so no other includer needs a regrade.
- **Coordinator re-grade:** vland `--recheck-landed` gives 2,087 / 2,087 PASS, 264 MATCH + 1,823 RELOCATION_MATCH, with the same
  parity as before on every row (`logs/regrade_vland.log`).
  - gate.py in the coordinator run (`logs/regrade_gate.tsv`): 1,817 PASS. The other 270 fail with "no usable index entry": an index
    refresh run without the lane env had overwritten their baselines with ERR.
  - In the plan agent's run against a good index, gate.py passed 2,087 / 2,087.
- **Deferred:** 1,347 (`DEFERRED.tsv`):

  | Reason | Rows |
  |---|---|
  | Past the wave budget; pass hdrmig3 and wait in `READY_PHASE4.txt` | 530 |
  | compile_fail | 478 |
  | File is in another wave (one file, one wave) | 275 |
  | bytes_differ | 48 |
  | stray_change | 16 |

  - Most compile failures are local member names the header lacks (`GetLengthSquared`, `GetLength`, `Normalise`, ...).
  - 77 of them are a local `C2DCoordI` that collides with the header's include.
