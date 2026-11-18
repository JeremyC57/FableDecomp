# Compiler history

The old enemy and receiver occupy opposite frame words near 007F621C. The later component-selection paths retain additional differences.

| Source change or build tag | Result | Object bytes | Instruction hunks | First unequal byte |
|---|---|---:|---:|---:|
| package baseline | DIFFER | 1458 | 36 | +0x1F |
| parent first pos from parent | DIFFER | 1458 | 40 | +0xC |
| script overrides home late | DIFFER | 1458 | 39 | +0xB |
| old enemy before pos | DIFFER | 1458 | 38 | +0xC |
| turncoat from typed actor | DIFFER | 1458 | 40 | +0x1F |
| original search filter temporaries | DIFFER | 1454 | 35 | +0x1F |
| original player filter temporaries | DIFFER | 1471 | 90 | +0x2 |
| component through owner | DIFFER | 1461 | 45 | +0x15 |
| distance from actor position | DIFFER | 1468 | 91 | +0x2 |
| old enemy comparison input | DIFFER | 1430 | 62 | +0x0 |
| typed id frequency | DIFFER | 1455 | 37 | +0x1F |
| direct list push | DIFFER | 1458 | 36 | +0x1F |
| select one comparison assignment | DIFFER | 1431 | 72 | +0x0 |
| main attack selected | DIFFER | 1458 | 36 | +0x1F |

Retained source and test coverage. The selected-body fixture enumerates targeting flags and records target updates; its dependencies are controlled.

The selected result is PARKED (1458 object bytes, 1458 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
