# rasm2 -- retail_asm / volatile verification of every open candidate (2026-10-27)

One Fable 5.1 agent (high effort) checked all 218 open rows any record called a retail_asm, inline-asm or volatile
candidate (`CANDIDATES.tsv` gives each row's source), under the owner's 2026-10-27 evidence rule (`crawl/purity.py`: no
instruction cap; the `// Retail-asm:` line cites a retail address and an instruction of the block; no naked/_emit; no
`ret` inside the asm). 44 rows landed here with `vland.py --land` (verify.log: 19 MATCH, 25 RELOCATION_MATCH, all tests
PASS, no RELOC_IDENTITY; every source grades `retail_asm`; the tests carry the same asm, which purity does not count).

| verdict | rows |
|---|---|
| retail_asm_landable (landed) | 44 |
| retail_asm_overcap (whole-function MASM/CRT asm with ret or naked: __ftol2, __alldiv, __allshr, __aulldvrm, MemMove, MicroThreadSleep) | 6 |
| volatile (00c34630 DecodeAdpcmToPcm, matched with two volatile members; in pending overcap/) | 1 |
| not_reproduced (parked, 44 with a best source in pending differ/) | 167 |

Landed families: the fnstcw/fldcw ceil/floor block (19 rows, 10 instructions with a jne, previously refused by the cap),
bare fistp / libpk truncation helpers (3), the libpk 2^x block (Pow10 x7, MillibelsToAmplitude x2), cpuid/pushfd
(0xC52AA0), DirectShow wxutil EnlargedUnsignedDivide (0xCB0160, the published `_X86_` block), and hand SIMD kernels
(SSE/SSE2/MMX, 11 rows; 0xC6D370/0xC6F0F0 need /GS).

Not volatile after all: 00c35dd0 (a dead re-test of a register). Unproven: 009919c0, 00991c10. Plain C++ after all:
006d63f0, 00645099, 00c6ac90, 00c6ad10 (the last two need the LTCG convention of 0x00C6AAE0). Boundary: 00ba1140 and
00bd71b0 are split by the manifest at an early `ret 0Ch`; the whole functions are 2344 and 2116 bytes
(rebuild/oracles/boundary-overrides-pending.tsv). Near misses worth a look: NOTES.md.
