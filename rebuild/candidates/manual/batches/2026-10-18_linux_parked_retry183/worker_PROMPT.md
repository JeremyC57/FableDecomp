# Current overrides: Sol6.1 MEDIUM retry202F
Own ONLY retry202f_sol/chunk.tsv four rows00417418,00418b7a,0041d8e7,0041d98a. Read ../SESSION_RULES.md, HISTORY_AUDIT.tsv, ROOT_HISTORY_SEARCH.json and preserved Claude brief below. Scratch only here. Each row has4 UNCERTAIN prior reservations, maximum6 new source/declaration/flags/compiler-failure grades, totalcap10. Do not reset, auto-sweep or exceed. Freeze immutable source/hash/full command/effective flags before every invocation. Compiler failures count. Identical-source/flags verification is separate, precompiler infra excluded only with proof. All earlier work preserved. Exact READY candidates stop new grades until root review.
Actual headers mandatory including quarantine; explicit native ABI projections with evidence allowed. No fake virtual padding, volatile/alignment or byte injection/asm. Genuine instantiated constructor signatures/hierarchies; native PMF4 and full host width. Verify actual emitted COFF virtual signature order, VC reverses overload groups. Pointee GetRefCount/Release signed long, stored IVRefCount unsigned, Delete const. Counted refs0/fastcall callback4/data8 distinct intelligent object0/refs4. Native allocator identity and every relocation/string throughNUL independently audited. CTCBase donor editor methods incorrect: current corrected native table ../astra239_whole/CTCBASE_NATIVE_CORRECTION.tsv.
Expanded actual source/header fixtures hostC++98/O2 and nativeVC7.1 plus noelide with genuine owning copies. State coverage limits and genuine external dependency preconditions. Give READY source/test immutable hashes/grades/bytes/fixtures/native evidence, then continue whole four. Whole section root independently verifies/lands before replacement. Preserve full snapshots and counts, parking last resort within cap. No children/git/commits/canonical/sharedheader/sharedtool edits/processkilling/Winerestart. Every shell source work/codex-lane/wine/env.sh, export S=/workspace/FableDecomp/work/codex-lane, cwdrepo. Existing root sharedWine reaper/socket only; require_escalated for socket. q.py timeout200; xdis length HEX. Fiveothers own whole236/239/240/241/242. This replaces completed202E, root179committed94a62dc56.

# Hand-authoring brief (FableDecomp, Linux Wine lane) -- wave 25, chunk 231

Goal: byte-exact ("matching") C++ reconstructions of small native-PC Fable.exe functions, compiled
with the VC7.1-era compiler and compared against the retail bytes. You author one source + one
behaviour test per function, iterate with the compiler until the bytes match, and leave the results
in your directory. You do NOT land and you do NOT do the final re-grade: the coordinator stages your
work for a later session (the coordinator) that re-grades every row, runs the tests and lands it.

## Environment (every Bash call must start with these two lines; shell state does not persist)
    export S=/workspace/FableDecomp/work/codex-lane
    source $S/wine/env.sh; export S=/workspace/FableDecomp/work/codex-lane; cd /workspace/FableDecomp
Never `cd` into the scratchpad; keep the repo as cwd. Never commit. Never modify anything under the repo
(rebuild/, docs/, tools/ are read-only for you).

## Wave 25 rules (16 agents share this lane at the same time -- read first)
- You are one of 16 agents, each on its own 40-row chunk. Work ONLY inside $S/w22/agent231. Never write to another
  agentNN directory, $S/tools, $S/batch, $S/wine/prefix or the repo, and never run git.
- NEVER kill processes (no pkill, killall, kill, `wineserver -k`): the other agents' compiles would die. Wrap every
  grade in `timeout 200` (the scripts below already do); a hung compile ends there. A source with a missing brace can
  make cl.exe spin until that timeout -- check braces before grading.
- At most 4 grading attempts per body (an attempt = a source change + a grade). Then move the best source to
  differ/<addr>.cpp (header line `// Grade: DIFFER(ours v retail) -- <what still differs>`) and write its hand-over
  line (format below). Scheduling-only diffs (same instructions in another order, register swaps, `pop ecx` vs
  `add esp,4`) are usually compiler-build artefacts: park them after one or two tries instead of burning four.
- A row whose bytes contain int3/nop padding followed by ANOTHER function body within the row length covers two
  functions: skip it (STATUS `SKIP`, one line in NOTES.md).
- A row counts as matched only if q.py printed MATCH or RELOCATION_MATCH for the exact file that sits in src/.
- Work through the rows in order, cheapest first is fine; write the test as soon as a body matches.

## File layout and scripts (already in your directory)
- `dis_all.txt` -- the retail disassembly of all 40 rows (read it first).
- `src/<addr>.cpp` -- ONLY sources that graded MATCH/RELOCATION_MATCH. `test/<addr>.cpp` -- the behaviour test of each
  src/ file. `differ/<addr>.cpp` -- the best source of each parked row. `probe/` -- scratch variants.
- `bash $S/w22/agent231/g.sh <addr> [leaf] [--flags ...]` grades src/<addr>.cpp and prints the side-by-side
  disassembly; `python3 $S/w18/udiff.py <(bash $S/w22/agent231/g.sh <addr>) | grep -v "^ "` shows only the differing lines.
  To grade a probe: `timeout 200 python3 $S/tools/q.py <addr> $S/w22/agent231/probe/<file>.cpp [leaf] [--flags ...]`.
- `bash $S/w22/agent231/gall.sh > $S/w22/agent231/grades.txt` grades every src/*.cpp (it honours `// Leaf:` and `// Flags:` lines).
- `bash $S/w22/agent231/tall.sh` compiles and runs every test with g++ (64-bit host: in tests use member access, never
  raw offsets, and never memset a polymorphic object). `python3 $S/w22/agent231/tall.py` compiles and runs every test
  with the lane's VC7.1 cl.exe under Wine (what the lander runs; keep tests plain C++98, no <string>/<vector> tricks).
  Every test must print `<addr>_TEST PASS` under both.
- `python3 $S/w22/agent231/finish.py` writes payload.json + oracle.tsv from src/ + test/ (run it LAST).
- Line 1 of every source: `// <Module>::<Name>(<params>) -- <what it does>`; finish.py takes the module and the leaf
  name from it. Add `// Leaf: <symbol substring>` (e.g. `??1CFoo` for a destructor whose TU also emits `??_G`, or a
  mangled name for an overload) and/or `// Flags: /EHsc` on their own lines when needed.

## Tools
- Your chunk file (TSV: address, module, name, length, bytes) is given below. `bytes` is the retail
  body (trailing int3/nop padding is not part of the function).
- Disassemble: `python3 $S/tools/xdis.py <8-hex-va> <len-hex>` e.g. `python3 $S/tools/xdis.py 00724bf0 0x40`
  (a function's callees can be disassembled the same way; the exe is at "$S/dl/game/Fable The Lost Chapters/Fable.exe").
- Manifest labels for callees: `grep -P "^00c3b840\t" rebuild/manifest/functions.tsv` (columns: address, name, module,
  calling convention, return type, parameter count, parameter types...). Labels are propagated across
  byte-identical bodies and often wrong for small functions -- treat them as hints.
- Callers of any address (call/jmp sites and absolute references): `python3 $S/w23/xref.py <hexva>` -- the quickest way to
  name a row whose manifest label is propagated (read the callers' landed sources or their vtable slots).
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

## Output (in $S/w22/agent231)
1. src/ + test/ for every matched row, differ/ for every parked row; then run gall.sh (all must be MATCH/RELOCATION_MATCH),
   tall.sh and tall.py (all PASS), then finish.py.
2. `STATUS.tsv`: header `address	status	bytes	test	note`, one line per chunk row; status MATCH | RELOCATION_MATCH |
   DIFFER | SRC_FAIL | SKIP; bytes `<ours>v<retail>`; test `PASS` (g++ and VC) / `FAIL` / `-`; a short note.
3. `NOTES.md`:
   - `# Hand chunk 231 (<first class> .. <last class>): <M> of 40 byte-exact`
   - `## What matched (<M>)` -- bullets by class: what each function does and the SHAPE lesson that made it match.
   - `## Parked for the coordinator (<K>)` -- one bullet per parked row in exactly this form:
     `- <addr> <Class::Name> DIFFER(<ours>v<retail>): <the differing instructions, one line> | tried: <forms tried> | next: <the next idea, or "lane gap: <kind>">`
   - `## Skipped` and `## Label corrections` (propagated manifest labels you found wrong).
4. `LESSONS.md`: `# Chunk 231 -- lessons from the authoring agent (fold into GOTCHAS at landing)`, then one bullet per
   codegen lesson worth adding to docs/pipeline/GOTCHAS.md (the source form -> the retail shape, with an address), then
   `Review at landing:` items the landing session must check (rows that needed more than 4 grades, local base models
   next to an existing engine header, errors you noticed in ALREADY-LANDED sources, catalog rows that hold several functions).
5. Final message (short): matched / parked / skipped counts, the parked addresses, and the lessons.



Current assignment is exactly the TWO rows in local chunk.tsv.

## Your authoritative assignment
Use only chunk.tsv in THISdirectory. Old chunk231 labels/table are obsolete.
