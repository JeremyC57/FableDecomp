# Compiler history

Portable SSE intrinsics and the scalar four-diagonal path produce different operand scheduling and instruction selection starting in the SIMD arm.

| Source change or build tag | Result | Object bytes | Instruction hunks | First unequal byte |
|---|---|---:|---:|---:|
| package baseline | PREPROCESS_FAIL | — | — | — |
| intrinsic axis sequence | DIFFER | 2696 | 58 | +0x1A |
| unrolled simd corners | DIFFER | 2745 | 70 | +0x1A |
| direct matrix receiver | DIFFER | 2696 | 82 | +0x1A |
| scalar halves assign | DIFFER | 2696 | 95 | +0x1A |
| copy scalar diagonals | DIFFER | 2696 | 83 | +0x1A |
| center before diagonals | DIFFER | 2714 | 70 | +0x11 |
| sse pointer advance | DIFFER | 2699 | 71 | +0x1A |
| incremental scalar minimum | DIFFER | 2662 | 85 | +0x1A |
| loop independent extrema | DIFFER | 2696 | 58 | +0x1A |
| consolidated scalar expansion | DIFFER | 2696 | 65 | +0x1A |
| main selected fixture | DIFFER | 2696 | 58 | +0x1A |

Retained source and test coverage. The actual two branches are compared to an independent enumeration of all eight corners, including in-place output, 48 configurations, degenerate extents, rotation, shear, scaling and translation.

The selected result is PARKED (2696 object bytes, 2735 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
