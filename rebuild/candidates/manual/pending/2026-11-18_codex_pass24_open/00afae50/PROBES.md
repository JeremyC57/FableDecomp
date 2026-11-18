# Compiler history

The accepted inline split removes the extra IsActive, GetRecordedPos and TransformPosIfLocal calls. The receiver allocation and frame differ from the prologue onward.

| Source change or build tag | Result | Object bytes | Instruction hunks | First unequal byte |
|---|---|---:|---:|---:|
| package baseline | COMPILE_FAIL | — | — | — |
| shared sprite | COMPILE_FAIL | — | — | — |
| shared graphics | COMPILE_FAIL | — | — | — |
| full shared | COMPILE_FAIL | — | — | — |
| particle ref | COMPILE_FAIL | — | — | — |
| count local | COMPILE_FAIL | — | — | — |
| age local | COMPILE_FAIL | — | — | — |
| inline limit | COMPILE_FAIL | — | — | — |
| original tick rate | COMPILE_FAIL | — | — | — |
| resource age local | COMPILE_FAIL | — | — | — |
| resource count local | DIFFER | 1832 | 88 | +0x0 |
| resource full shared | DIFFER | 1832 | 88 | +0x0 |
| resource inline limit | DIFFER | 1832 | 88 | +0x0 |
| resource original tick rate | DIFFER | 1832 | 88 | +0x0 |
| resource particle ref | DIFFER | 1832 | 88 | +0x0 |
| sprite visible frame | DIFFER | 1940 | 87 | +0x0 |
| sprite visible frame threshold | DIFFER | 2471 | 164 | +0x7 |
| sprite frames depth | DIFFER | 1940 | 87 | +0x0 |
| sprite expanded transforms | DIFFER | 2430 | 107 | +0x0 |
| sprite expanded all native | DIFFER | 2471 | 164 | +0x7 |
| sprite debug vector lifetimes | DIFFER | 2413 | 156 | +0xB |
| sprite selected shared | DIFFER | 2413 | 156 | +0xB |

Retained source and test coverage. Checks empty and expired particles, sprite and trail combinations, both delay ranges, timed animation, phase clamps, transformed positions, computed radii, and handle clears.

The selected result is PARKED (2413 object bytes, 2405 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
