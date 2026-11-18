# Compiler history

At 00671B06 the byte temporary and flag storage enter AL and CL in the opposite order while updating IsIndestructible. The corresponding shift and mask reverse as well.

| Source change or build tag | Result | Object bytes | Instruction hunks | First unequal byte |
|---|---|---:|---:|---:|
| package baseline | DIFFER | 1449 | 2 | +0x507 |
| shared transfer layout | DIFFER | 1449 | 2 | +0x507 |
| negated store guard | DIFFER | 1447 | 9 | +0x506 |
| bool store via conditional | DIFFER | 1447 | 9 | +0x506 |
| comparison keep old when equal | DIFFER | 1455 | 10 | +0x507 |
| ref receiving call | DIFFER | 1449 | 2 | +0x507 |
| byte representation read | DIFFER | 1449 | 2 | +0x507 |
| first case class receiver | DIFFER | 1449 | 2 | +0x507 |
| floatless bool equality | DIFFER | 1454 | 6 | +0x4D7 |
| original scopes double negation | DIFFER | 1453 | 6 | +0x4D7 |
| storage union adapter | DIFFER | 1449 | 2 | +0x507 |
| unit mask before copy | DIFFER | 1453 | 6 | +0x4D7 |
| callee overload visible | DIFFER | 1449 | 2 | +0x507 |
| each flag explicit | DIFFER | 1449 | 2 | +0x507 |
| each flag pointer | DIFFER | 1449 | 2 | +0x507 |
| main final models | DIFFER | 1449 | 2 | +0x507 |

Retained source and test coverage. The fixture checks each transferred field in order, each of eleven flag names and defaults, bit writeback, and sorted/unique versus binary-preserved navigator arrays.

The selected result is PARKED (1449 object bytes, 1450 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
