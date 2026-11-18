# Pass 24 compiler probes

The table records compiler invocations. A repeat build or a compilation failure is not an additional source-shape attempt.

| Source or recheck | Result | Object / retail bytes | First differing offset | Instruction hunks |
|---|---|---:|---:|---:|
| shared wife gift | DIFFER | 387 / 386 | +0x22 | 4 |
| loop top result | DIFFER | 387 / 386 | +0x22 | 4 |
| do until wait stops | DIFFER | 387 / 386 | +0x22 | 4 |
| break on wait completion | DIFFER | 387 / 386 | +0x22 | 4 |
| original post test wait | DIFFER | 387 / 386 | +0x22 | 4 |
| nested callback continue | DIFFER | 387 / 386 | +0x22 | 4 |
| named postcondition | MASKED_MATCH_PENDING_AUDIT | 386 / 386 | — | 0 |
| named speech object | MASKED_MATCH_PENDING_AUDIT | 386 / 386 | — | 0 |
| accepted middle a | MASKED_MATCH_PENDING_AUDIT | 386 / 386 | — | 0 |
| accepted 22 | MASKED_MATCH_PENDING_AUDIT | 386 / 386 | — | 0 |

The selected result is RELOCATION_MATCH (386 object bytes, 386 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
