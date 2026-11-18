# Compiler history

Ego_d names data, what, prev, res and ret_str in CCharString::Replace. prev retains the segment start while res advances over CRLF. The original UID reader and section destructor both expand. Together these restore all four inlined search loops, the binary UID transfer, and the native cleanup.

| Source change or build tag | Result | Object bytes | Instruction hunks | First unequal byte |
|---|---|---:|---:|---:|
| const uid | COMPILE_FAIL | — | — | — |
| declare domain order | COMPILE_FAIL | — | — | — |
| declare passport first | COMPILE_FAIL | — | — | — |
| ego locals | COMPILE_FAIL | — | — | — |
| map lookup before call | COMPILE_FAIL | — | — | — |
| original input string | COMPILE_FAIL | — | — | — |
| recovered production draft | COMPILE_FAIL | — | — | — |
| rebased const uid | DIFFER | 2023 | 90 | +0x2 |
| rebased declare domain order | DIFFER | 2058 | 93 | +0x7 |
| rebased declare passport first | DIFFER | 2031 | 92 | +0x2 |
| rebased ego locals | DIFFER | 2023 | 90 | +0x2 |
| rebased map lookup before call | DIFFER | 2023 | 89 | +0x2 |
| rebased original input string | DIFFER | 1991 | 90 | +0x2 |
| rebased recovered production draft | DIFFER | 2023 | 90 | +0x2 |
| selected save state | DIFFER | 2023 | 90 | +0x2 |
| save reader original scope | DIFFER | 2023 | 90 | +0x2 |
| save visible section cleanup | DIFFER | 2132 | 93 | +0x2 |
| save inline threshold | DIFFER | 2547 | 119 | +0x2 |
| save inline depth | DIFFER | 2023 | 90 | +0x2 |
| save restore both | DIFFER | 2276 | 5 | +0x14D |
| save donor conversions | DIFFER | 2276 | 5 | +0x14D |
| save explicit temporary strings | DIFFER | 2263 | 49 | +0x36 |
| save selected string data | DIFFER | 2263 | 24 | +0x350 |
| save direct loop inputs | DIFFER | 2214 | 11 | +0x14D |
| save expand text branch | DIFFER | 2277 | 75 | +0x7 |
| replace egod donor | MASKED_MATCH_PENDING_AUDIT | 2263 | 0 | — |
| replace egod late previous | MASKED_MATCH_PENDING_AUDIT | 2263 | 0 | — |
| replace egod text at search | DIFFER | 2263 | 8 | +0x34D |
| replace egod first search common | DIFFER | 2268 | 43 | +0x9 |
| replace egod selected receiver | MASKED_MATCH_PENDING_AUDIT | 2263 | 0 | — |
| replace egod selected needle | DIFFER | 2263 | 16 | +0x350 |
| replace egod res next data | DIFFER | 2276 | 43 | +0x9 |
| save clean selected | MASKED_MATCH_PENDING_AUDIT | 2263 | 0 | — |

Retained source and test coverage. The direct fixture covers hero/world, binary/text, both optional sections, zero entries, UID values above 32 bits, duplicate keys, CRLF removal and UNATTACHED.

The selected result is RELOCATION_MATCH (2263 object bytes, 2263 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
