# Compiler probes

| Candidate | Status | Object bytes | Retail bytes | First difference | Strict instruction hunks |
| --- | --- | ---: | ---: | --- | ---: |
| card reference | DIFFER | 726 | 716 | 0x0059FC22 | 12 |
| compare card first | DIFFER | 706 | 716 | 0x0059FC22 | 12 |
| compare sentinel once | DIFFER | 710 | 716 | 0x0059FC7B | 14 |
| egod alias free | DIFFER | 706 | 716 | 0x0059FC22 | 12 |
| loop index count | DIFFER | 726 | 716 | 0x0059FC22 | 12 |
| package baseline | DIFFER | 725 | 716 | 0x0059FC7B | 9 |
| remove card alias | DIFFER | 706 | 716 | 0x0059FC22 | 12 |
| remove tattoo alias | DIFFER | 726 | 716 | 0x0059FC22 | 12 |
| specialfit 01 donor names with near native shape | DIFFER | 725 | 716 | 0x0059FC7B | 9 |
| specialfit 02 tattoo card ref with original index | DIFFER | 725 | 716 | 0x0059FC7B | 9 |
| specialfit 03 card getter once before empty branch | DIFFER | 722 | 716 | 0x0059FC7B | 11 |
| specialfit 04 card index in loop scope | DIFFER | 725 | 716 | 0x0059FC7B | 10 |
| specialfit 05 card pointer as reference lifetime | DIFFER | 725 | 716 | 0x0059FC7B | 9 |
| specialfit 06 morph pointer lifetime extends to card | DIFFER | 725 | 716 | 0x0059FC7B | 9 |
| specialfit 07 count through reference | DIFFER | 725 | 716 | 0x0059FC7B | 9 |
| specialfit 08 tattoo index as loop operand | DIFFER | 726 | 716 | 0x0059FC22 | 12 |
| specialfit 09 retain card pointer for direct conflict | DIFFER | 724 | 716 | 0x0059FC22 | 33 |
| specialfit 10 original dual def initialization | DIFFER | 838 | 716 | 0x0059FC1C | 38 |
| test vector size | DIFFER | 702 | 716 | 0x0059FC7B | 16 |

The selected result is PARKED (725 object bytes, 716 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
