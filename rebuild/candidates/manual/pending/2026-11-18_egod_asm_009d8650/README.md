# egod lane: 0x009D8650 MicroThread::MicroThreadSleep under the asm rules (2026-11-18) -- STAGED (owner-delegated decision 2026-11-18)

Owner: "Fix the naked asm row to follow our current rules." Identity ?MicroThreadSleep@MicroThread@@YIXXZ (Ego_d object order +
FableWin 0x02F6CA00 + Ego_r 0x00B2B980). A 36-byte stack switch: it writes esp (0x009D865D `mov esp,[0x013CA898]`) and fs:[0].

What is done (agent asm1, ~70 grades; files here):
- Rule 4 attempt table (ATTEMPTS_AND_DESIGN.md): 32 plain-C++ spellings and 12 intrinsic spellings, all graded, all DIFFER; no
  C++ or VC7.1 intrinsic writes esp or pushes / pops. A normal (non-naked) definition with inline asm gives 28-42 bytes (VC7.1 never
  saves ebp around inline asm and emits its own push ebx/esi/edi first), so retail's bytes need a function with no compiler prologue;
  Ego_d's /Od length 36 = retail confirms the original was written that way.
- Rule 3.2 shared helper header `hdrdiff/NEW_MicroThreadSleep.h` (proposed rebuild/include/engine/MicroThreadSleep.h): the
  macro-wrapped whole-function asm path FABLE_MICROTHREAD_SLEEP_DEFINITION, `// Retail-asm-helper:` and `// Retail-asm:` evidence,
  and a FABLE_PORTABLE twin (POSIX ucontext / Win32 fibre). The row `src/009d8650.cpp` only includes the header and invokes the
  macro: no __asm / naked / _emit in the row or its test.
- Bytes: RELOCATION_MATCH 36/36; behaviour test PASS (`test/009d8650.cpp`: two micro-threads and the retail shared-stack design;
  three mutant helpers fail). Dual-path test `portable/test_MicroThreadSleep.cpp`: VC7.1 asm, dual and portable PASS; g++ portable
  PASS (also g++ / clang -O0..-O3, -Os); g++ -m32 syntax check skipped (no 32-bit headers in the lane).

Why it is NOT staged as a match (both need the owner):
1. The asm path is a whole function (`__declspec(naked)` + `__asm ret`) inside the shared helper. ASM_RULES rule 5, CLAUDE.md
   "Byte purity" and crawl/purity.py treat naked as an oracle bake: today purity grades the row `naked_asm` and read-only vland says
   NOT_GENUINE. `proposals/purity_whole_function_helper.diff` would allow naked + ret in a helper header carrying a
   `// Retail-asm-function: <retail address>` line (rows still cannot spell them); applied, the row grades retail_asm and vland gives
   RELOCATION_MATCH PASS; no other classification changes over 56,612 sources and 31 helper headers. Not applied (owner rule change).
2. The FABLE_PORTABLE twin keeps the protocol (park this side, publish a token in PGlobalStackPointer, resume the other side; each
   side keeps callee-saved registers, locals and its own exception handlers; same interleaving) but NOT identical semantics: the
   token is a ucontext_t* / fibre handle instead of an esp into a 24-byte frame, CreateMicroThread 0x009D8710 and (on Win32)
   SwitchToMicroThread 0x009D87F0 need their own portable branches, and the POSIX same-address stack copy relies on frame layout
   C++ does not guarantee (works on x86-64 GCC / Clang, not on ABIs such as AArch64).
Names found on the way (NAME_FIXES.tsv): 0x013CA898 = PGlobalStackPointer (Ego_d + Anniversary PDB; landed code says
SavedStackPointer), plus the TU's other statics (two mapped by declaration order only: verify before respelling).

## Decision and staging (2026-11-18)
The owner delegated the decision ("match retail, behave exactly like retail, be portable"). The coordinator accepted the
whole-function asm helper case (docs/pipeline/ASM_RULES.md "Whole-function asm helpers"; tools/decomp_pipeline/crawl/purity.py now
accepts naked + ret only in a helper header carrying `// Retail-asm-function:`; re-classifying all 56,612 row sources and 31 helper
headers changes nothing). An independent audit confirmed the identity and rule compliance and strengthened the test (values kept in
esi / edi / ebx across each switch; fs:[0] chain head checked), and the coordinator re-verified: RELOCATION_MATCH, test PASS, graded
retail_asm (`coordverify.log`, no pragma fallback).

Portability statement: the retail path is byte-identical and behaviour-tested. The FABLE_PORTABLE twin is protocol-equivalent, not
identical: POSIX ucontext (token = a ucontext_t* on the parked side's stack) / Win32 fibres (token = fibre handle, plus a
running-fibre variable retail does not have); retail's 24-byte saved frame and its fs:[0] handling are not reproduced. It is not
usable in a portable build yet: CreateMicroThread 0x009D8710 needs a portable first-entry branch (makecontext / CreateFiber) and, on
Win32, SwitchToMicroThread 0x009D87F0 needs a branch that skips its stack-image copy; the POSIX stack copy relies on frame layout
(tested on x86-64 g++ / clang++ -O0..-Os, not guaranteed by C++ or on AArch64). Follow-up tracked in ../2026-11-17_egod_parked.
Names: 0x013CA898 PGlobalStackPointer, 0x013CA894 PActiveScript, 0x013CA89C ThreadStackSize meet the two-source rule (Ego_d data
record + donor order / spacing / types matching retail use); 0x013CA8A0 / 0x013CA8A4 are order-only (do not respell yet).
