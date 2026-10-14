# Hand-authoring brief (FableDecomp, Linux Wine lane)

Goal: byte-exact ("matching") C++ reconstructions of small native-PC Fable.exe functions, compiled
with the VC7.1-era compiler and compared against the retail bytes. You author one source + one
behaviour test per function, iterate with the compiler until the bytes match, and write the results
into a payload directory. You do NOT land (the coordinator lands sequentially).

## Environment (every Bash call must start with these two lines; shell state does not persist)
    export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
    source $S/wine/env.sh; export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; cd /home/user/FableDecomp
Never run `cd` into the scratchpad; keep the repo as cwd. Never commit. Never modify files under rebuild/ or docs/.

## Tools
- Your chunk file (TSV: address, module, name, length, bytes) is given below. `bytes` is the retail
  body (trailing int3/nop padding is not part of the function).
- Disassemble: `python3 $S/w17/xdis.py <8-hex-va> <len-hex>` e.g. `python3 $S/w17/xdis.py 00724bf0 0x40`
  (a function's callees can be disassembled the same way; the exe is at "$S/dl/game/Fable The Lost Chapters/Fable.exe").
- Manifest labels for callees: `grep -P "^00c3b840\t" rebuild/manifest/functions.tsv` (columns: address, name, module,
  calling convention, return type, parameter count, parameter types...). Labels are propagated across
  byte-identical bodies and often wrong for small functions -- treat them as hints.
- Vtables: `grep -i "<vtable va without 0x>" rebuild/manifest/rtti-vtables.tsv` (class, vtable_va, slot, func_va) when present.
- Landed sources for reference and reuse (already byte-exact): `rebuild/src/compiled/<aa>/<bb>/*_<addr>.cpp`
  (shard by the first two bytes of the address). `grep -rl "0x00C3B840" rebuild/src/compiled | head` finds
  landed callers/callees. Catalog of landed addresses: `rebuild/oracles/auto-re-candidates.tsv` (column 1).
- Class headers from the PDB: `rebuild/include/engine/<Class>.h` (real member names and offsets). When a
  header exists for the class, `#include "engine/<Class>.h"` and use its members instead of inventing a
  local struct with pads (the include path is rebuild/include). If the header's layout disagrees with the
  bytes, fall back to a local descriptive model and say so in the comment.
- Grade one source: `python3 $S/tools/q.py <8-hex-addr> <src.cpp> [leaf] [--flags /EHsc ...]`
  Prints `MATCH` / `RELOCATION_MATCH` (good) / `DIFFER(ours v retail)` with a side-by-side disassembly /
  `SRC_FAIL` with the compiler errors. The oracle for your addresses is in `$S/batch/*_oracle.tsv`
  (your chunk is registered there). `leaf` defaults to the catalog name's last `::` component; pass the
  function's C++ name (the lander picks the largest symbol containing it) when the source names it differently.
  Address must be 8 hex digits without 0x. Flags to try when the default differs: `--flags /EHsc` (body
  starts with `push -1 ; push offset`), `--flags /GS`, `--flags /Oa`; size-optimised TUs use `#pragma optimize("s", on)`
  at the top of the source (the lander sweeps `#pragma optimize("s"/"t"/"y"...)` and those flags automatically
  when landing, so a source that matches under a pragma/flag is fine -- just record which one worked).
- Patterns and solved problems: read `docs/pipeline/GOTCHAS.md` (whole file) before starting, and skim
  `rebuild/candidates/manual/batches/2026-10-13_linux_b5/README.md` for the conventions of a hand batch.

## Rules
- Byte purity: no `__asm`, no `naked`, no `_emit`, no `#pragma pack` tricks that hide the real layout.
  Plain C++ that VC7.1 compiles to the retail bytes.
- Relocations (call targets, vtables, globals, string literals, float pools) are masked by the grader:
  declare callees/globals with descriptive names and the retail address in a trailing comment
  (`void Method();   // 0x00C3B840`). Member names: real ones from the engine header, else descriptive
  (`m_pTarget`, `m_count`), never `field_0x10`.
- Model `this` with the engine header when it exists; otherwise `struct CName { unsigned char _pad[0x10]; long m_x; ... };`
- Every source starts with a header comment: what the function does, retail address and length, the
  callees by address, and (if it needed one) the pragma/flag. Explain the shape in the comment like a
  careful reverse engineer would (why the layout is what it is).
- Behaviour test: a standalone program that exercises the same logic through a copy of the model
  (the test is compiled separately from the source), printing exactly `<addr>_TEST PASS` on success and
  `<addr>_TEST FAIL` + return 1 on failure, where <addr> is the 8-hex address (e.g. `00724bf0_TEST PASS`).
  Template:
      #include <cstdio>
      ...model + logic...
      int main()
      {
          ... if (wrong) { std::printf("00724bf0_TEST FAIL\n"); return 1; }
          std::printf("00724bf0_TEST PASS\n");
          return 0;
      }
- Loop prevention: at most 4 grading attempts per function. If it still differs, keep the best source
  (comment: `// Grade: DIFFER(ours v retail) -- <what still differs>`) and move on. Scheduling-only diffs
  (same instructions, different order; `pop ecx` vs `add esp,4`; `jmp` vs `call ; ret`) are usually
  compiler-version artefacts: don't burn attempts on them.
- Lessons that pay: a `bool` result with no `xor eax,eax` is `unsigned char`; a `mov al ; shr/and` getter is an
  `unsigned char` bitfield; `fld [a] ; fmul [b]` is `b * a`; `add esp,4` after a `__cdecl` call means the
  result is returned, `pop ecx` means dropped; a virtual call on `this` inside a ctor/dtor must sit in an
  inline helper; `__declspec(novtable)` drops a class's own vtable store; a `delete p` is a null test +
  dtor + `operator delete`; VC evaluates call arguments right to left; a by-value class argument with a user
  copy constructor is built in its stack slot (`push ecx ; mov ecx,esp ; mov [ecx],eax`).

## Output
Write `$S/w22/<your-dir>/payload.json` as {"result":{"authored":[ {address, module, name, source_cpp, test_cpp, pass_pattern, flags(optional list)} ... ]}}
including only the functions that graded MATCH/RELOCATION_MATCH (list the DIFFER ones with their best
source in `$S/w22/<your-dir>/differ/<addr>.cpp` and a one-line reason in `$S/w22/<your-dir>/NOTES.md`).
`module` = the class name (or `_global`), `name` = the C++ function name used in the source (the leaf the
lander will look for), `pass_pattern` = `<addr>_TEST PASS`, `flags` = extra flags if one was needed (e.g. ["/EHsc"]).
Also write `$S/w22/<your-dir>/oracle.tsv` (address, name as `module::name`, length, bytes) for the matched rows.
Final report: counts (matched / differ / skipped), the shapes you learned, and anything the coordinator must know.
