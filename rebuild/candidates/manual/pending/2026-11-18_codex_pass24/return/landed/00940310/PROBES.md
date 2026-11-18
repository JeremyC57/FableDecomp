# Pass 24 compiler probes

The table records compiler invocations. A repeat build or a compilation failure is not an additional source-shape attempt.

| Source or recheck | Result | Object / retail bytes | First differing offset | Instruction hunks |
|---|---|---:|---:|---:|
| package baseline | LEAF_REVIEW_REQUIRED | — / 494 | — | — |
| real task speech | DIFFER | 510 / 494 | +0x8 | 10 |
| checked pick action | DIFFER | 520 / 494 | +0x8 | 11 |
| exit in crop scope | DIFFER | 493 / 494 | +0x8 | 10 |
| retain move result | DIFFER | 483 / 494 | +0x8 | 12 |
| shared local cleanup | DIFFER | 493 / 494 | +0x8 | 10 |
| donor pick result | DIFFER | 489 / 494 | +0x8 | 10 |
| both finish branches | DIFFER | 493 / 494 | +0x8 | 10 |
| assign clear with null | DIFFER | 493 / 494 | +0x8 | 10 |
| inverted precondition | DIFFER | 493 / 494 | +0x8 | 10 |
| bind component result | DIFFER | 493 / 494 | +0x8 | 10 |
| crop guard condition | DIFFER | 521 / 494 | +0x8 | 12 |
| single lookup assignment | DIFFER | 493 / 494 | +0x8 | 10 |
| null crop on task failure | DIFFER | 493 / 494 | +0x8 | 10 |
| acceptance early return | DIFFER | 493 / 494 | +0x1F | 11 |
| function scope crop | DIFFER | 493 / 494 | +0x1F | 2 |
| member typed lookup | DIFFER | 493 / 494 | +0x1F | 11 |
| pcrop copy init | DIFFER | 493 / 494 | +0x1F | 2 |
| typed lookup and delayed crop | DIFFER | 493 / 494 | +0x1F | 2 |
| separate member guard | MASKED_MATCH_PENDING_AUDIT | 494 / 494 | — | 0 |

The selected result is RELOCATION_MATCH (494 object bytes, 494 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
