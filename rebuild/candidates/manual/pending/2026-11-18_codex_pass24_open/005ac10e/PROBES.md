# Compiler probes

| Candidate | Status | Object bytes | Retail bytes | First difference | Strict instruction hunks |
| --- | --- | ---: | ---: | --- | ---: |
| child from menu on append | DIFFER | 3543 | 3532 | 0x005AC139 | 43 |
| child from menu on first add | DIFFER | 3543 | 3532 | 0x005AC139 | 43 |
| children read for name | DIFFER | 3543 | 3532 | 0x005AC139 | 43 |
| egod names | DIFFER | 3532 | 3532 | 0x005ACB59 | 2 |
| function scope subtitle cast uses | DIFFER | 3541 | 3532 | 0x005AC128 | 211 |
| named category receivers | DIFFER | 3532 | 3532 | 0x005ACB59 | 2 |
| obtain text at call | DIFFER | 3526 | 3532 | 0x005AC115 | 67 |
| original scopes and viewport | DIFFER | 3532 | 3532 | 0x005ACB59 | 2 |
| package baseline | DIFFER | 3532 | 3532 | 0x005ACB59 | 2 |
| retained subtitle through raw ptr | DIFFER | 3542 | 3532 | 0x005AC142 | 142 |
| size from menu accessor | DIFFER | 3559 | 3532 | 0x005AC128 | 185 |
| subtitle function scope | DIFFER | 3532 | 3532 | 0x005ACB59 | 2 |
| tradefit 01 entry pointer use at append | DIFFER | 3538 | 3532 | 0x005AC115 | 123 |
| tradefit 02 entry ref use at append | DIFFER | 3538 | 3532 | 0x005AC115 | 123 |
| tradefit 03 entry pointer ref use at append | DIFFER | 3538 | 3532 | 0x005AC115 | 123 |
| tradefit 04 entry and frame references | DIFFER | 3532 | 3532 | 0x005ACB59 | 2 |
| tradefit 05 entry vector ref at append | DIFFER | 3532 | 3532 | 0x005ACB59 | 2 |
| tradefit 06 frame ref at append | DIFFER | 3532 | 3532 | 0x005ACB59 | 2 |
| tradefit 07 vector pointer later | DIFFER | 3532 | 3532 | 0x005ACB59 | 2 |
| tradefit 08 sublabel index unsigned | DIFFER | 3530 | 3532 | 0x005AC8B2 | 4 |
| tradefit 09 bounded vector ref | DIFFER | 3532 | 3532 | 0x005ACB59 | 2 |
| tradefit 10 explicit pointer arg subtitle | COMPILE_FAIL | — | 3532 | — | — |
| tradefit 11 use const reference for subtitle value | DIFFER | 3532 | 3532 | 0x005ACB59 | 2 |
| tradefit 12 menu child as named ref after name | DIFFER | 3541 | 3532 | 0x005AC115 | 149 |
| tradefit 13 named category slots | COMPILE_FAIL | — | 3532 | — | — |
| tradefit 14 explicit shop cash original scope | COMPILE_FAIL | — | 3532 | — | — |
| tradefit 15 late subtitle value ref | COMPILE_FAIL | — | 3532 | — | — |
| tradefit 16 title loop vector order | COMPILE_FAIL | — | 3532 | — | — |
| tradefit 17 named category slots | DIFFER | 3532 | 3532 | 0x005ACB59 | 2 |
| tradefit 18 explicit shop cash original scope | DIFFER | 3532 | 3532 | 0x005ACB59 | 2 |
| tradefit 19 late subtitle value ref | DIFFER | 3532 | 3532 | 0x005ACB59 | 2 |
| tradefit 20 title loop vector order | DIFFER | 3532 | 3532 | 0x005ACB59 | 2 |
| v receiver end statement | DIFFER | 3532 | 3532 | 0x005AC94B | 8 |
| viewport original pair | DIFFER | 3532 | 3532 | 0x005ACB59 | 2 |

The selected result is PARKED (3532 object bytes, 3532 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
