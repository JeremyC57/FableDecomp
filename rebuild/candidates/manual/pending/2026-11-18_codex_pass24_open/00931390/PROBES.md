# Pass 24 compiler probes

| Candidate | Result | Object bytes | Retail bytes | First difference | Strict instruction hunks |
| --- | --- | ---: | ---: | --- | ---: |
| complete ai calls | DIFFER | 3863 | 3863 | 0x009313AB | 29 |
| const component out | DIFFER | 3948 | 3863 | 0x00931398 | 172 |
| crime direct speech home | DIFFER | 3863 | 3863 | 0x009313AB | 49 |
| crime original look speed | DIFFER | 3863 | 3863 | 0x009313AB | 29 |
| crime owner list ref | DIFFER | 3863 | 3863 | 0x009313AB | 29 |
| crime shared home direct | DIFFER | 3863 | 3863 | 0x009313AB | 49 |
| crime speech retry control | DIFFER | 3863 | 3863 | 0x009313AB | 29 |
| crime standalone route result | DIFFER | 3863 | 3863 | 0x009313AB | 29 |
| crimefit 01 original owner locals | DIFFER | 3948 | 3863 | 0x00931398 | 172 |
| crimefit 02 const home lookup reference | DIFFER | 3948 | 3863 | 0x00931398 | 172 |
| crimefit 03 adjacent container test and front | DIFFER | 3948 | 3863 | 0x00931398 | 172 |
| crimefit 04 move ranges by value | DIFFER | 3948 | 3863 | 0x00931398 | 172 |
| crimefit 05 insert door at end | DIFFER | 3918 | 3863 | 0x00931398 | 159 |
| crimefit 06 place first range holder before guard | DIFFER | 3948 | 3863 | 0x00931398 | 172 |
| crimefit 07 separate body of unlocked guard | DIFFER | 3948 | 3863 | 0x00931398 | 172 |
| crimefit 08 guard then do for doors | DIFFER | 3863 | 3863 | 0x009313AB | 28 |
| crimefit 09 use scope for door pointer | DIFFER | 3948 | 3863 | 0x00931398 | 172 |
| crimefit 10 direct owner accessors | DIFFER | 3948 | 3863 | 0x00931398 | 172 |
| crimefit 11 inside unique lock receiver | DIFFER | 3948 | 3863 | 0x00931398 | 172 |
| direct ref second loop | DIFFER | 3948 | 3863 | 0x00931398 | 172 |
| do while door loop | DIFFER | 3863 | 3863 | 0x009313AB | 28 |
| donor door component | DIFFER | 3948 | 3863 | 0x00931398 | 172 |
| donor door without home alias | DIFFER | 3948 | 3863 | 0x00931398 | 176 |
| empty vector test | DIFFER | 3948 | 3863 | 0x00931398 | 172 |
| front instead of index | DIFFER | 3948 | 3863 | 0x00931398 | 172 |
| list reference first only | DIFFER | 3948 | 3863 | 0x00931398 | 172 |
| no component aliased read | COMPILE_FAIL | — | 3863 | — | — |
| original ret lifetime | DIFFER | 3948 | 3863 | 0x00931398 | 170 |
| original virtual update | COMPILE_FAIL | — | 3863 | — | — |
| package baseline | PREPROCESS_FAIL | — | 3863 | — | — |
| real lock navigation | COMPILE_FAIL | — | 3863 | — | — |
| restored filter header | COMPILE_FAIL | — | 3863 | — | — |

The selected result is PARKED (3863 object bytes, 3863 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
