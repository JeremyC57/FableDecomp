# Compiler history

The EDI-to-EBX copy straddles the block-width guard at 00F3AC0C in the opposite order. The selected function otherwise has the full retail extent.

| Source change or build tag | Result | Object bytes | Instruction hunks | First unequal byte |
|---|---|---:|---:|---:|
| supplied | COMPILE_FAIL | — | — | — |
| package baseline | DIFFER | 605 | 2 | +0x1A |
| guarded countdown | DIFFER | 605 | 19 | +0x11 |
| same tu countdown | DIFFER | 605 | 2 | +0x1A |
| same tu egod | DIFFER | 605 | 19 | +0x11 |
| param remaining | DIFFER | 602 | 59 | +0x2 |
| guard then for | DIFFER | 605 | 19 | +0x11 |
| count with postdecrement | DIFFER | 602 | 61 | +0x2 |
| count as loop bound | DIFFER | 587 | 33 | +0x2 |
| give call both arms | DIFFER | 636 | 31 | +0x2 |
| blocksize less n | DIFFER | 636 | 31 | +0x2 |
| ptr walk index | DIFFER | 605 | 2 | +0x1A |
| guard return zero | DIFFER | 619 | 20 | +0x11 |
| delay by initial subtraction | DIFFER | 605 | 2 | +0x1A |
| wide comparison at top | DIFFER | 605 | 2 | +0x1A |
| sized column before call | DIFFER | 605 | 4 | +0x1A |
| single return on wide loop | DIFFER | 605 | 2 | +0x1A |
| clamped call then counter | DIFFER | 602 | 38 | +0x4 |
| countdown first increment | DIFFER | 605 | 2 | +0x1A |
| guard shared n | DIFFER | 602 | 38 | +0x4 |
| index aggregate assignment | DIFFER | 605 | 2 | +0x1A |
| pointer after guard | DIFFER | 605 | 19 | +0x11 |
| clamp implicit loop value | DIFFER | 605 | 19 | +0x11 |
| main range selected | DIFFER | 605 | 2 | +0x1A |

Retained source and test coverage. Checks zero counts, widths, strided values, recursion into subblocks, and equal/unequal encoding costs.

The selected result is PARKED (605 object bytes, 605 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
