# Asm cleanup round 2, targets 1 and 3 (2026-11-05). P = $S/plan/asm2. No repo edits, no git.
I started no bake; target 2 is the other agent's.

## Per target
| target | outcome | plain C++ | intrinsic |
|---|---|---|---|
| 0x00B44820 / 0x00B44A60 | helper `engine/CEngineLightingSSE.h` (w1) | 30 spellings x 3 flag sets each | 16 / 12 spellings |
| GFROR13 0x00497890 | helper kept, evidence now proven | 32 | 32 |
| MixKernelsSIMD (21 macros) | kept, proven | 30 x 3 per family (3 families) | 13 per macro |
| C3DBoundingBoxSSE | kept, proven | 30 x 3 | 10 |
| NVectorMathSSE (5 rows) | kept, proven | 60 x 3 | 51 |
| CMatrix4x4InterpolateSSE | kept, proven | 30 x 3 | 17 |
| CParticleProgramSSE | kept, proven | 30 x 3 | 13 |
| GFGetInterpolatedColourMMX | kept, proven | 30 x 3 | 10 |
| C3DCoordIStrayMaxps | kept, proven | 30 x 3 | 10 |

The ATTEMPTS tables are in w1/overlay/rebuild/candidates/manual/naming/2026-11-05_asm_helpers_r2/:
- ATTEMPTS_LIGHTING.tsv
- ATTEMPTS_GFROR13.tsv
- ATTEMPTS_SIMD_HELPERS.tsv
- ATTEMPTS_UNIVERSAL.tsv

The raw probe logs are in attempts/, and the spellings in probes/.

## Apply, in order (repo root, lane env sourced)
    python3 $S/plan/asm2/apply_asm.py $S/plan/asm2/w1     # 2 in place + 7 new
    bash    $S/plan/asm2/regrade_asm.sh w1
    python3 $S/plan/asm2/apply_asm.py $S/plan/asm2/w2     # 8 headers, comments only
    bash    $S/plan/asm2/regrade_asm.sh w2                # 53 rows
    patch -p1 < rebuild/candidates/manual/naming/2026-11-05_asm_helpers/purity_no_inline_asm.patch
