# Compiler probes

| Candidate | Status | Object bytes | Retail bytes | First difference | Strict instruction hunks |
| --- | --- | ---: | ---: | --- | ---: |
| package baseline | LEAF_REVIEW_REQUIRED | — | 1582 | — | — |
| port 01 shared headers | COMPILE_FAIL | — | 1582 | — | — |
| port 02 retail calls | COMPILE_FAIL | — | 1582 | — | — |
| port 03 typed ui calls | COMPILE_FAIL | — | 1582 | — | — |
| port final | DIFFER | 1581 | 1582 | 0x00589414 | 56 |
| shape 04 original hair def base | DIFFER | 1487 | 1582 | 0x00589407 | 54 |
| shape 05 tattoo arm breaks | DIFFER | 1487 | 1582 | 0x00589407 | 54 |
| shape 06 grouped categories | DIFFER | 1265 | 1582 | 0x00589407 | 76 |
| shape 07 style reference in body area | DIFFER | 1487 | 1582 | 0x00589407 | 54 |
| shape 08 body mask combined | DIFFER | 1492 | 1582 | 0x00589407 | 68 |
| shape 09 hair arm outer negated | DIFFER | 1487 | 1582 | 0x00589407 | 54 |
| shape 10 counted vector erase inline | DIFFER | 1487 | 1582 | 0x00589407 | 54 |
| shape 11 scoped ui subtitles | DIFFER | 1487 | 1582 | 0x00589407 | 54 |
| shape 12 cdefclassbase getsubdef body | DIFFER | 1468 | 1582 | 0x00589430 | 41 |
| shape 13 cdefpointer release pointee | DIFFER | 1487 | 1582 | 0x00589407 | 54 |
| shape 14 stlp vector dtor forceinline | DIFFER | 1487 | 1582 | 0x00589407 | 54 |
| shape 15 ccountedpointer copy decl only | DIFFER | 1468 | 1582 | 0x00589430 | 41 |
| shape 16 direct body area branch | DIFFER | 1487 | 1582 | 0x00589407 | 61 |
| shape 17 direct area with same tu subdef | DIFFER | 1478 | 1582 | 0x00589407 | 55 |
| shape 18 default def release | DIFFER | 1612 | 1582 | 0x00589407 | 66 |
| shape 19 same tu subdef default def release | DIFFER | 1581 | 1582 | 0x00589414 | 56 |
| shape 20 same tu subdef specific release | DIFFER | 1581 | 1582 | 0x00589414 | 56 |
| shape 21 exact same tu specialization | DIFFER | 1581 | 1582 | 0x00589414 | 56 |
| style retry 01 original title and subtitle scopes | DIFFER | 1581 | 1582 | 0x00589414 | 56 |
| style retry 02 original default hair scope | DIFFER | 1560 | 1582 | 0x00589430 | 33 |
| style retry 03 clump null and invalid tattoo | DIFFER | 1560 | 1582 | 0x00589430 | 33 |
| style retry 04 switch arm tattoo break | DIFFER | 1581 | 1582 | 0x00589414 | 56 |
| style retry 05 retain menu as const pointer | DIFFER | 1581 | 1582 | 0x00589414 | 56 |
| style retry 06 add retained parent receiver | DIFFER | 1581 | 1582 | 0x00589414 | 56 |
| style retry 07 retain item vector const references | DIFFER | 1581 | 1582 | 0x00589414 | 56 |
| style retry 08 construct title before classification | DIFFER | 1581 | 1582 | 0x00589414 | 56 |
| style retry 09 retain one ui bool | DIFFER | 1581 | 1582 | 0x00589414 | 56 |
| style retry 10 retain count as reference | DIFFER | 1581 | 1582 | 0x00589414 | 56 |
| style retry 11 use do while for count | DIFFER | 1578 | 1582 | 0x00589414 | 55 |
| style retry 12 case four test before hair | DIFFER | 1560 | 1582 | 0x00589430 | 33 |
| style retry 13 default test before hair | DIFFER | 1560 | 1582 | 0x00589430 | 33 |
| style retry 14 exception case subdef moved into guard | DIFFER | 1564 | 1582 | 0x00589430 | 34 |
| style retry 15 default subdef moved into guard | DIFFER | 1560 | 1582 | 0x00589430 | 33 |
| style retry 16 four null guard first then merge | DIFFER | 1564 | 1582 | 0x00589430 | 40 |
| style retry 17 both null guards first then merge | DIFFER | 1572 | 1582 | 0x00589430 | 38 |

The selected result is PARKED (1564 object bytes, 1582 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
