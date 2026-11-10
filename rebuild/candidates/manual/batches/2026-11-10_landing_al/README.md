# 2026-11-10 landing al -- Codex pass 17's held CBankFileAsyncData constructor

0x009D5D30 ??0CBankFileAsyncData@@AAE@PAXKAAVCBankFileAsync@@@Z, vland --land RELOCATION_MATCH, test PASS. Held in landing ak for its
three `volatile bool` members; owner, 2026-11-10: "if retail used it and volatile doesnt cause portability issues, then it should be
fine" (now in docs/pipeline/ASM_RULES.md `volatile` and CLAUDE.md). Evidence: Ego_r type 0xc09d / FableWin 0xbd67 are LF_MODIFIER
volatile over bool on Finished +0x44, BufferLocked +0x45, AllowRestart +0x46 (Aborted +0x47 plain); retail accesses 0x009D54C0,
0x009D54CC, 0x009D5566. The qualifier lives once, in the new shared engine/CBankFileAsyncData.h, with its `// Retail-volatile:` line;
the row source only cites it. The behaviour test's host model drops the qualifier (no byte or behaviour role there).
Source: `pending/2026-11-10_codex_pass17/return_2026-11-10/held/` (kept as the record).
