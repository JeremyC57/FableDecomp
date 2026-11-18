# Compiler history

From 00A57210 the optional output tests and temporary x87 values differ. The public const leaf and optional output pointee types agree with both donors.

| Source change or build tag | Result | Object bytes | Instruction hunks | First unequal byte |
|---|---|---:|---:|---:|
| package baseline | LEAF_REVIEW_REQUIRED | — | — | — |
| shared line | DIFFER | 652 | 13 | +0x73 |
| inplace normalize named inputs | DIFFER | 673 | 33 | +0x2 |
| length from magnitude | DIFFER | 602 | 31 | +0x2 |
| square magnitude then sqrt | DIFFER | 602 | 31 | +0x2 |
| point offset before guard | DIFFER | 687 | 45 | +0x2 |
| dot in rotated order | DIFFER | 673 | 33 | +0x2 |
| path length before distance | DIFFER | 673 | 34 | +0x2 |
| component reciprocal | DIFFER | 673 | 33 | +0x2 |
| distance result one local | DIFFER | 673 | 33 | +0x2 |
| full branch return | DIFFER | 673 | 34 | +0x2 |
| original local order | DIFFER | 703 | 34 | +0x2 |
| squared rhs nested | DIFFER | 673 | 34 | +0x2 |

Retained source and test coverage. Checks both optional output arguments, endpoint and interior projections, degenerate lines and the clipping radius.

The selected result is PARKED (652 object bytes, 654 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
