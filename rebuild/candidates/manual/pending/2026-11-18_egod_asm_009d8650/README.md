# egod lane: 0x009D8650 MicroThread::MicroThreadSleep under the asm rules (2026-11-18) -- HELD FOR OWNER DECISION

**Owner, 2026-11-18:** "As long as it does not affect matching or portability, it should be fine." The rule change (ASM_RULES "Whole-function
asm helpers", crawl/purity.py) stands. This row still waits on portability: the FABLE_PORTABLE twin must work on its own (portable
branches for CreateMicroThread 0x009D8710 / SwitchToMicroThread 0x009D87F0; no same-address stack copy that relies on frame layout).

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
