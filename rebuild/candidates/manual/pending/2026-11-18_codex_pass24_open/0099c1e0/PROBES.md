# Compiler history

The current prefix lifetime leaves a larger frame and different ownership-counter and destructor scheduling. The public underlying append specialization and reference-count ownership are present.

| Source change or build tag | Result | Object bytes | Instruction hunks | First unequal byte |
|---|---|---:|---:|---:|
| package baseline | LEAF_REVIEW_REQUIRED | — | — | — |
| original substring api | DIFFER | 639 | 37 | +0x2 |
| native append public | DIFFER | 639 | 37 | +0x2 |
| share pointer on empty | DIFFER | 639 | 37 | +0x2 |
| donor mutable pointers | DIFFER | 639 | 37 | +0x2 |
| search then assign prev | DIFFER | 623 | 36 | +0x2 |
| original ctor body order | DIFFER | 639 | 33 | +0x2 |
| advance prev after appends | DIFFER | 629 | 23 | +0x2 |
| loop with search condition | DIFFER | 632 | 39 | +0x2 |
| named suffix lifetime | DIFFER | 643 | 39 | +0x2 |
| search first before ret str | DIFFER | 639 | 40 | +0x2 |
| store new owner before addref | DIFFER | 639 | 37 | +0x2 |
| isolated prefix scope | DIFFER | 613 | 38 | +0x2 |
| hoist needle length | DIFFER | 623 | 19 | +0x2 |
| main selected fixture | DIFFER | 613 | 38 | +0x2 |

Retained source and test coverage. Tests cover multiple, adjacent and absent matches, deletion, wide receiver sharing and aliasing. Empty search text would not advance in the native loop and is excluded.

## Additional main-chat comparisons

- main_end_after_with — DIFFER, 619 bytes, 49 strict instruction hunks.
- main_end_after_prev — DIFFER, 632 bytes, 39 strict instruction hunks.
- main_end_advance_after_with — DIFFER, 617 bytes, 52 strict instruction hunks.
- main_end_advance_after_dtor — DIFFER, 624 bytes, 44 strict instruction hunks.
- main_end_wcsstr_guard_prev — DIFFER, 619 bytes, 49 strict instruction hunks.
- main_end_save_end_first — DIFFER, 656 bytes, 45 strict instruction hunks.
- main_end_guard_dereference — DIFFER, 503 bytes, 26 strict instruction hunks.

The selected result is PARKED (613 object bytes, 609 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
