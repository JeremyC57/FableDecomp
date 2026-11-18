# Compiler history

The entity reference reload at 0074A512, loop scheduling, and the removal path differ. The direct array/position form is the closest retained port.

| Source change or build tag | Result | Object bytes | Instruction hunks | First unequal byte |
|---|---|---:|---:|---:|
| package baseline | DIFFER | 232 | 8 | +0x23 |
| assigned vec last element | DIFFER | 233 | 10 | +0x23 |
| assigned vec no extra index | DIFFER | 217 | 18 | +0x2 |
| assigned vec normal | DIFFER | 232 | 8 | +0x23 |
| index before nav last element | DIFFER | 235 | 13 | +0x23 |
| index before nav no extra index | DIFFER | 227 | 15 | +0x23 |
| index before nav normal | DIFFER | 234 | 11 | +0x2B |
| nav decl first last element | DIFFER | 233 | 11 | +0x23 |
| nav decl first no extra index | DIFFER | 217 | 16 | +0x2 |
| nav decl first normal | DIFFER | 232 | 9 | +0x23 |
| original last element | DIFFER | 233 | 10 | +0x23 |
| original no extra index | DIFFER | 217 | 18 | +0x2 |
| original normal | DIFFER | 232 | 8 | +0x23 |
| recount after last element | DIFFER | 253 | 14 | +0x17 |
| recount after no extra index | DIFFER | 243 | 24 | +0x2 |
| recount after normal | DIFFER | 252 | 14 | +0x17 |
| second alias input | DIFFER | 232 | 8 | +0x23 |
| second const start | DIFFER | 232 | 8 | +0x23 |
| second do after guard | DIFFER | 232 | 8 | +0x23 |
| second explicit navigation lookup | DIFFER | 232 | 8 | +0x23 |
| second for compensate | DIFFER | 241 | 21 | +0x3 |
| second navigation body | COMPILE_FAIL | — | — | — |
| second valid first | DIFFER | 232 | 10 | +0x23 |
| main final models | DIFFER | 232 | 8 | +0x23 |

Retained source and test coverage. The fixture compares the survivors against every reachability mask for candidate counts zero through eight.

The selected result is PARKED (232 object bytes, 234 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
