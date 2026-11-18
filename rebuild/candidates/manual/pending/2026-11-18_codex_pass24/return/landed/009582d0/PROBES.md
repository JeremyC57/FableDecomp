# Compiler history

The native RTTI descriptor for the vtable at 01238C6C identifies CIntelligentPointer<CThing>. The look call takes 0.0027777f. The signed module static FRAMES_BETWEEN_CHECKING_IS_STILL_VALID is present in Ego_d and initialized to 30 in retail. Three calls from IsStillRunnable use the typed FindMostRecentDeed specialization.

| Source change or build tag | Result | Object bytes | Instruction hunks | First unequal byte |
|---|---|---:|---:|---:|
| real trespassing deeds | COMPILE_FAIL | — | — | — |
| shared intelligent compare | DIFFER | 1063 | 51 | +0xE |
| original typed smart pointer | DIFFER | 1035 | 56 | +0xE |
| typed mutable component lookup | DIFFER | 1097 | 4 | +0x78 |
| moving before home check | DIFFER | 1097 | 7 | +0x1F |
| moving between home and hero | DIFFER | 1097 | 6 | +0x44 |
| typed raw player assignment | DIFFER | 1091 | 7 | +0x74 |
| deed predicates at full expression | DIFFER | 1097 | 4 | +0x78 |
| shared home operand | DIFFER | 1081 | 11 | +0x24 |
| full loop condition | DIFFER | 1097 | 4 | +0x78 |
| toggle after action | DIFFER | 1096 | 35 | +0xA |
| use alive pointer once | DIFFER | 1068 | 10 | +0x78 |
| pretest repeat guard | DIFFER | 1097 | 7 | +0x1F |
| name the typed hero | DIFFER | 1097 | 4 | +0x78 |
| signed frame delta | DIFFER | 1094 | 13 | +0x78 |
| original thing assignment | DIFFER | 1098 | 0 | +0x2A9 |
| original thing assignment direct | DIFFER | 1098 | 0 | +0x2A9 |
| original typed temp | DIFFER | 1098 | 0 | +0x2A9 |
| named thing owner | DIFFER | 1098 | 0 | +0x2A9 |
| original getpointer setpointer | DIFFER | 1098 | 0 | +0x2A9 |
| native turn speed | MASKED_MATCH_PENDING_AUDIT | 1098 | 0 | — |
| original frame interval definition | MASKED_MATCH_PENDING_AUDIT | 1098 | 0 | — |

Retained source and test coverage. Fourteen state scenarios exercise movement, looking, exits, forced and throttled validity checks, missing components and the three deed predicates.

The selected result is RELOCATION_MATCH (1098 object bytes, 1098 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
