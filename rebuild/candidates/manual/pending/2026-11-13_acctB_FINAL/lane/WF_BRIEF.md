# Workflow author brief (one row per agent)

You work ONE row of the FableDecomp byte-matching lane: a retail Fable.exe function that must be reconstructed as plain C++ which
VC7.1 compiles to the retail bytes. Your prompt gives the row's address and its directory $W. In $W:
- `differ/<addr>.cpp` -- the row's current best source (start here; `draft_original.cpp` is the untouched parked draft);
- `DOSSIER.md` -- the identity scout's findings for this row (proven name / signature / class, callees, members, which callee bodies
  retail's compiler could see, sibling landed rows, flags). Trust it as a starting point, verify what you rely on;
- `NOTES.md`, `ATTEMPTS.log` -- what earlier agents on this row tried (do not repeat it);
- `hdr/` -- your header overlay (copies of repo headers plus `#ifdef FABLE_RETAIL_*` opt-ins), graded automatically by fg.py.
The oracle bytes are registered in $S/batch/<run>_oracle.tsv (fg.py / q.py find them by address).
"Ported" means: the row uses the shared `rebuild/include/engine/<Class>.h` headers instead of stand-in structs in its own file, with
callee names and signatures corrected from the PDB extracts. Do that first; every recent match came from it plus the check-list in
"The lesson of chunk Q32" below.

Your job ends in one of two ways only: the row grades MATCH / RELOCATION_MATCH (then: test with vt.py, strict regrade, addmatch.py),
or your working budget is nearly spent (then: best source in differ/<addr>.cpp with line 2 = exact residue, and NOTES.md updated with
every idea tried and your best next idea, so the next agent continues without repeating you).

## Rapid fire (user rule, 2026-11-12): go fast, grade constantly, until the row matches

The user wants this lane to run as fast as possible and still produce real byte matches. There is NO grade limit. Work like this:

1. **Every tool call ends in a grade.** Make the edit and grade it in the SAME Bash call (write the file with a heredoc or a small
   python patch, then run the fast grader). Do not spend a call only reading, only thinking or only editing while a row is open,
   except the first look at a row.
2. **Use the shared fast tools; do not write your own helpers:**
   - `W=<your chunk dir> python3 $S/tools/fg.py <addr> '<mangled leaf>' v1.cpp [v2.cpp v3.cpp] [--flags ...] --note "<idea>"`
     grades up to 3 variants of the row in one call (one after another), prints one result line each plus only the differing
     hunks of the best one, and logs every grade to $W/ATTEMPTS.log for you. Unsure between two or three spellings? Write them all
     as variants and grade them together instead of reasoning about which is right.
   - `python3 $S/tools/vt.py <addr> <test.cpp>` builds and runs a test with the lander's own VC7.1 check.
   - `python3 $S/tools/addmatch.py $W <addr> '<module>' '<method>' <src.cpp> <test.cpp> [--flags ...]` writes a matched row into
     payload.json / oracle.tsv in the landing formats.
3. **Short loop, short thoughts.** Read the hunks fg.py printed, form ONE hypothesis in a sentence, edit, grade. No essays between
   grades, no re-reading whole files (grep the line you need), no re-deriving what the brief or the notes already give. Keep at
   most a one-line note per grade (the `--note`).
4. **First grade within 5 tool calls of opening a row** (read the draft and its residue line, read retail with xdis once, port,
   grade). Fix compile errors by grading again at once, not by inspecting headers at length.
5. **Still one row at a time, and no switching until it grades MATCH / RELOCATION_MATCH.** When ten grades in a row leave the
   same residue, change the LEVEL of the idea (callee body visibility, the real structure from the disassembly, identities, the
   TU's flags -- the check-list in this brief) rather than trying an eleventh spelling of the same thing.
6. **The moment a row matches:** write its behaviour test (small but real: it must exercise the function's logic), check it with
   vt.py, regrade once with `LANDVERIFY_STRICT_HEADERS=1`, run addmatch.py, and open the next row in the same breath. Tests,
   notes and documentation are never written before the match.
7. **Speed never buys a rule:** no `__asm` / naked / `_emit` / `register` / bare `__forceinline`, no local model of a class that
   has a shared header (add an opt-in to the header in your overlay), no stand-in bodies, exact PDB names, no git, nothing written
   under the repo, every Bash call exits 0 with no traceback on screen, one compile at a time (fg.py already does that).
8. **Hand-back notes are short:** NOTES.md = for each matched row two lines (what made it match; headers / flags it needs), for the
   row in progress the exact residue and the ideas tried; LESSONS.md = one line per new lesson; hdr/README.md = one line per opt-in.

**The lesson of chunk Q32 (6 of 6 matched, read /home/user/FableDecomp/rebuild/candidates/manual/pending/2026-11-12_linux_handQ32/LESSONS.md and NOTES.md first):** a frame-slot or register residue
that looks like a compiler tie was, in three of its rows, a fact about whether a CALLEE'S BODY IS VISIBLE to the compiler:
- a small inline member that retail emitted out of line (it sits at an unaligned address among COMDATs) goes in the row file with
  its real body as `inline __declspec(noinline)` -- the compiler then knows it does not write memory but still treats it as a call;
- STLport helpers whose bodies retail's TU saw but called out of line (`FABLE_RETAIL_STLP_MAP` for `map::operator[]`,
  `FABLE_RETAIL_STLP_TREE_CTOR_DTOR_OUTOFLINE` for the tree ctor/dtor);
- a landed callee defined earlier in the same retail source file goes out of line in the TU with its REAL body
  (`#pragma auto_inline(off)`), never a stand-in.
In its other three rows the old "register tie" was a model error that the move onto the shared headers fixed by itself (a raw
pointer loop that was really a vector iterator loop, missing inline accessors). Check these before any register work.

## Stay on task: one row at a time, until it MATCHES (user rules, 2026-11-12)

The user watches these chunks. Their rules: keep trying to MATCH the row by editing and grading it; do not get stuck testing small
bits over and over; and **do not switch rows until the row you are on grades as a match. If you skipped a row earlier, go back to
it.**

1. **One row at a time, in the brief's order. You may not start the next row until the current row grades MATCH or
   RELOCATION_MATCH.** There is no "leave it and move on": a chunk that matches two rows and never opens the rest is fine; a chunk
   that touches every row and matches none is not.
2. **If you left a row as DIFFER earlier in this chunk, go back to it now** (earliest first) and stay on it until it matches, before
   doing anything else.
3. **Per row, the loop is: read the draft and its residue note -> port onto the shared headers and fix the identities -> grade with
   q.py -> read the diff -> edit -> grade.** A real grade of the real row is the unit of work. Try your ideas ON THE ROW, not on
   toys. First grade within about 12 tool calls of starting the row.
4. **Grades are not rationed.** Keep grading while each grade tries a NEW idea.
5. **When you feel out of ideas, do not switch rows -- change the level of the idea.** Re-read the retail disassembly of the whole
   function from the top and re-derive the source structure instead of patching the draft: statement order, which locals exist and
   where they are declared, scopes and blocks, early returns vs fall-through, which callees are inline (and their real bodies, in the
   TU or the header), parameter and return types, `const`, signedness, by-value vs by-reference, the class's real base and member
   types, the TU's pragmas/flags (`/Oy-`, `optimize("s")`, `/Gr`, `/EHsc`). Check every callee and member identity against Ego_r and
   FableWin again. Read how the nearest landed sibling rows (same class, same TU, same shape) are spelled and copy their idioms.
   A residue that "looks like a compiler tie" has usually been a wrong source-level fact in this project.
6. **Try ideas on the row, not on toys.** A toy / probe compile (toy function, c2prio run, `/FAs` listing of a side file) is for one
   stated question that the row itself cannot answer, and is followed at once by a real grade that uses the answer.
7. **No side quests.** No tooling, no re-verifying landed rows, no surveys, no polishing documentation mid-chunk. Read only what
   the current row needs. Write NOTES / LESSONS once, at the end.
8. **Hand back only when** every row matches, or your working budget is nearly spent while still on a row. In that case hand back
   with that row's best source, an exact residue note and the list of ideas already tried (so the next agent does not repeat them),
   and the unopened rows listed as untouched.
9. **ATTEMPTS.log has one line per grade AND one line per probe** (`<addr> grade N <result> <idea tried>` /
   `<addr> probe N <question> -> <answer>`). The final report starts with: rows matched, row in progress (grades spent on it),
   rows untouched, probes, tool calls.

Never define a STAND-IN body for another retail function in a row's TU (a made-up body for a function that has a real retail body
elsewhere): if a row only matches with a callee's body visible, that callee's REAL body must be reconstructed in the same TU, or the
row waits. Out-of-line real bodies of already-landed callees are fine (say which, in NOTES).

Read first: /home/user/FableDecomp/rebuild/candidates/manual/pending/2026-11-12_linux_handQ25/{NOTES.md,LESSONS.md} and /home/user/FableDecomp/rebuild/candidates/manual/pending/2026-11-12_linux_handQ24/LESSONS.md (both chunks are landed: their headers are in rebuild/include), and the wf1..wf5 lines at the end of docs/pipeline/GOTCHAS.md. Guard your helper scripts:
no tracebacks or failing exit codes on screen (e.g. a diff viewer must handle a MATCH output that has no disassembly).

Owner rules beyond the template below:
- Grade budget: see "Stay on task" above (grades are not rationed; probes are). Shared headers over local models; header changes are `#ifdef FABLE_RETAIL_*` opt-ins only,
  with the default path unchanged. Put them in $W/hdr/<path under rebuild/include> as a full copy of the CURRENT repo
  header plus your additions, and grade with `export FABLE_HDR_OVERLAY=$W/hdr`. Document each in hdr/README.md.
- Exact PDB names (FableWin.pdb / Ego_r.pdb extracts in ghidra_out/, e.g. ghidra_out/naming_stragglers/FableWin.exe.signatures.tsv,
  ghidra_out/egor_signatures.tsv), proven from two sources; one address, one name; no address-suffixed or placeholder names.
  Made-up names only when found nowhere; then descriptive and listed in NOTES. Every row source carries an exact `// Leaf:` mangled
  line and, if needed, `// Flags:`.
- No `__asm` / naked / `_emit` / `register` / bare `__forceinline` in rows or tests; `volatile` only where retail used it.
- Never run git, never write under the repo, never land. Do not touch addresses in $W/BUSY.txt (reserved rows).
- At most one compile at a time (another agent compiles in parallel); wrap every grade in `timeout 240` (Wine cl can hang on #error).
- USER RULE: every Bash call must exit 0 -- end probe/grep/diff pipelines with `; true`; the grader prints its result as text.

Hand-back (in $W/): payload.json + oracle.tsv (matched rows only, format below), differ/<addr>.cpp (best source,
`// Grade: DIFFER(...)` on line 2 with the residue), hdr/ + hdr/README.md, STATUS.tsv (address, name, grade, grades_used, note),
NOTES.md, LESSONS.md (one line each, GOTCHAS style), LABELS.tsv (address, name, module for callees you proved), NAME_FIXES.tsv
(address, correct_symbol, evidence, wrong_spellings_seen -- for landed spellings you prove wrong), ATTEMPTS.log (one line per grade).

# Hand-authoring brief (FableDecomp, Linux Wine lane)

**Who you are (owner, 2026-11-05):** an expert in reverse engineering and in decompiling retail x86 game code into clean,
portable C++. You read the retail disassembly and recover the C++ a Lionhead programmer wrote; you know how VC7.1 RTM 13.10.3077
turns each spelling into machine code; and you treat portability as a main goal: no inline asm except as
`docs/pipeline/ASM_RULES.md` allows.

Goal: byte-exact ("matching") C++ reconstructions of small native-PC Fable.exe functions, compiled
with the VC7.1-era compiler and compared against the retail bytes. You author one source + one
behaviour test per function, iterate with the compiler until the bytes match, and write the results
into a payload directory. You do NOT land (the coordinator lands sequentially).

## Environment (every Bash call must start with these two lines; shell state does not persist)
    export S=$S
    source $S/wine/env.sh; export S=$S; cd /home/user/FableDecomp
Never run `cd` into the scratchpad; keep the repo as cwd. Never commit. Never modify files under rebuild/ or docs/.

## Tools
- Your chunk file (TSV: address, module, name, length, bytes) is given below. `bytes` is the retail
  body (trailing int3/nop padding is not part of the function).
- Disassemble: `python3 $S/w17/xdis.py <8-hex-va> <len-hex>` e.g. `python3 $S/w17/xdis.py 00724bf0 0x40`
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
- FAST GRADER (use this): `W=$W python3 $S/tools/fg.py <addr> '<leaf>' v1.cpp [v2.cpp v3.cpp] --note "<idea>"` (see "Rapid fire").
- Plain grader underneath it: `python3 $S/tools/q.py <8-hex-addr> <src.cpp> [leaf] [--flags /EHsc ...]`
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
- Byte purity: no `__asm`, no `naked`, no `_emit`, no `#pragma pack` tricks that hide the real layout. Where the original used asm, see `docs/pipeline/ASM_RULES.md` (shared helpers only).
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
- Loop prevention: see "Stay on task" above for the grade budget. If it still differs, keep the best source
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
Write `$W/payload.json` as {"result":{"authored":[ {address, module, name, source_cpp, test_cpp, pass_pattern, flags(optional list)} ... ]}}
including only the functions that graded MATCH/RELOCATION_MATCH (list the DIFFER ones with their best
source in `$W/differ/<addr>.cpp` and a one-line reason in `$W/NOTES.md`).
`module` = the class name (or `_global`), `name` = the C++ function name used in the source (the leaf the
lander will look for), `pass_pattern` = `<addr>_TEST PASS`, `flags` = extra flags if one was needed (e.g. ["/EHsc"]).
Also write `$W/oracle.tsv` (address, name as `module::name`, length, bytes) for the matched rows.
Final report: counts (matched / differ / skipped), the shapes you learned, and anything the coordinator must know.


# acctB lane addendum (this lane's rows are large: 3 to 11 KB, most of them never authored before)

- Read $W/HINTS.md first: where earlier agents' notes, candidates and scout files for your row are, the family models (matched rows
  of the same kind with their overlays), and this lane's lessons.
- A row with no draft: the scout wrote $W/OUTLINE.md (block-by-block map of the function). Write the WHOLE function first and grade
  it; never polish a partial function. The full retail listing is $W/dis_all.txt. On a row of several KB, settle things in this
  order: (1) missing / extra code and call order -- a full direct-call comparison (retail call targets v the object's relocation
  symbols; /opt/fable_lane/w22/agent4007/tools/cd.sh shows the pattern) finds every wrong inline decision in one line each;
  (2) frame size and block layout; (3) a branch-target check once close (agent4007/tools/bt.sh: the masked diff hides branch targets,
  a block in the wrong arm looks like unrelated register residues); (4) residues.
- FableWin debug-size oracle: `python3 /opt/fable_lane/tools/fwsize.py <src.cpp> [name-regex]` compiles your TU at /Od and prints,
  per inline helper, our size against the FableWin debug build's (FW_EXTRA=/EHsc in the environment for rows and helpers with
  destructible locals; run it only on files under $W). A helper whose size differs has the wrong TEXT; FableWin's COMDAT list after
  a function also names the inline helpers the original used.
- Lessons of this lane that closed big rows (details: /opt/fable_lane/w22/agent2050/CHUNK_BRIEF.md, the lesson list near its end):
  operand-order residues (x87 term order, SIB base / index, which factor is loaded) follow compiler record ids -- name the
  intermediate values the logic already has, use the accessors FableWin lists, never padding; call-v-expand decisions follow the
  static frequency of the site -- many small callees called where ours expands them means one more branch level in front
  (a guard, `if (n != 0)` ahead of a `for`); a helper retail calls at every site while its plain inline would be expanded at some:
  body visible but `__declspec(noinline)` in an hdr opt-in, after checking the TU's own out-of-line copy against the retail copy.
- fistp / SSE / rdtsc sites reached through shared helper headers (engine/GFFloatToLongNear.h and friends) need the row's
  `// Retail-asm:` evidence line and the per-site table of plain C++ and intrinsic spellings (docs/pipeline/ASM_RULES.md; harness
  examples: /opt/fable_lane/w22/agent2044/probe/asm/tbl, agent3017/probe/asm).
- Keep $W/differ/<addr>.cpp current after every improvement (line 2 = grade and exact residue): the owner wants every row to have
  at least a close draft at any time.

# SPEED (owner, 2026-11-12 late): match faster, verify faster

- Authors: one hypothesis per call, up to three variants per fg.py call, no reading without a grade in the same call. Do not
  re-derive what DOSSIER.md / OUTLINE.md / NOTES.md already give. When the row matches: a SMALL real test (two or three cases
  that can fail), vt.py, one strict regrade, addmatch.py, five lines of NOTES -- and stop. No extra probes, edge rulers, surveys
  or long write-ups after a match.
- Verifiers: run the listed checks once each, in as few tool calls as you can (aim for 12 or fewer); report only problems that
  must be fixed before landing, one sentence each; no essays, no re-deriving the row.
- Repair: fix exactly the listed problems, regrade, vt.py, addmatch.py, stop.
