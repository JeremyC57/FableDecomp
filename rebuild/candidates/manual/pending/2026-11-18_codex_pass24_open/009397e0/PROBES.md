# Compiler history

The shared random rotation restores the native seed update before the external call. Around 009398C2 the seed pointer enters EDX where retail retains it in EDI.

| Source change or build tag | Result | Object bytes | Instruction hunks | First unequal byte |
|---|---|---:|---:|---:|
| package baseline | DIFFER | 569 | 1 | +0xD0 |
| shared rng | DIFFER | 573 | 50 | +0x6 |
| real shared rotation | DIFFER | 569 | 5 | +0xD0 |
| rotate defined after inline rng | DIFFER | 569 | 5 | +0xD0 |
| result local shared rotation | DIFFER | 567 | 7 | +0xD0 |
| cross call rotation decl | DIFFER | 573 | 50 | +0x6 |
| rot frame plus delay | DIFFER | 569 | 5 | +0xD0 |
| rot delay plus frame | DIFFER | 569 | 5 | +0xD0 |
| rot minimum then frame | DIFFER | 569 | 5 | +0xD0 |
| rot explicit split | DIFFER | 566 | 12 | +0xD0 |
| rot early seat test | DIFFER | 569 | 5 | +0xD0 |
| rot output in perform block | DIFFER | 565 | 11 | +0xD0 |
| main final models | DIFFER | 569 | 5 | +0xD0 |

Retained source and test coverage. The fixture covers seat release, sitting and mode checks, seed evolution, animation availability and idle-scheduler presence.

The selected result is PARKED (569 object bytes, 571 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
