# Retry 10: formulation-gap rows (inline callees, real headers, sibling TUs)

Workspace `$W` = /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/retry10
Lane preamble for EVERY Bash call:
  export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh; export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; cd /home/user/FableDecomp
Prime Wine once at the start: `timeout 120 wine cmd /c exit >/dev/null 2>&1 </dev/null`.

You are reconstructing parked functions from native PC `Fable.exe` (Fable: The Lost Chapters, VC7.1) so that the
compiled bytes match retail exactly. Repo: `/home/user/FableDecomp`. Read `CLAUDE.md`, then `docs/pipeline/GOTCHAS.md`
(search for "inventory", "inline-once", "RTM"), then the `LESSONS.md` of the chunk each row came from
(`$S/w22/agent<chunk>/LESSONS.md`; the chunk number is in `$W/rows.tsv`).

What is already known, do not re-derive it. These rows were parked after ordinary rewriting was exhausted. Each has a
best source and a one-line reason in `$W/src/<addr>.cpp` (line 1 `// Grade:`; the same text is in `$W/rows.tsv`).
Every one gives identical bytes under cl 13.10.3077 (the retail compiler), 4035 and 3052, so the gap is in the source
text, not the compiler. Expression shuffling, renaming temporaries, `lea`/`add` swaps and pragma sweeps have been
tried. Do not spend your budget repeating them.

The rows: EVERY parked row of the project, 1,040 in `$W/rows.tsv` (address, module, name, length, batch, reason).
The first 24 are the inventory band of chunk 209 and the three inline-once destructor rows 00d17090, 0066bd00,
00e32200: do those first, then continue down the list. Work by reason family rather than by address: pick a row,
then grep `rows.tsv` for the other rows with the same reason wording (same callee, same class, same shape) and do them
together, because a callee body or header fix that lands one row usually lands its siblings. Retail disassembly: the
first 24 rows are in `$W/dis_all.txt`; for any other row run `python3 $W/xdis.py <addr> <hexlen>` (length from
rows.tsv). This is a long job: keep STATUS.tsv current after every row, keep NOTES.md one line per unmatched row, and
run finish.py whenever you have a few matched rows so the landing session can land them while you continue. Expect to
be stopped and relaunched; on relaunch, read STATUS.tsv and continue with the first row it does not list.

The three strategies that are not yet exhausted, in this order:

1. Supply the callee bodies. Where the reason mentions inline-once, a release sequence, a destructor, or "retail
   computes it twice", retail saw a callee body in the TU that we only declare. Find the callee in the catalog
   (`rebuild/build_candidates.ps1`, source under `rebuild/src/compiled/`) or reconstruct it from the retail disassembly
   (`$W/dis_all.txt` for the rows themselves; `python3 $W/xdis.py <addr> <hexlen>` for any other address). Put its full
   definition in the same TU as the parked function, written as retail would have had it (same header, same member
   types). Keep the parked function's own text unchanged on the first attempt; only the TU contents change.
2. Replace local models with engine headers. If the source declares `struct X { char pad[...]; ... }` and
   `rebuild/include/engine/X.h` exists, switch to the header. Member types and alignment are an input to the frame
   allocator.
3. Author sibling functions as one TU. When several parked rows are methods of the same class, or share a helper (the
   inventory accessors), build them together with their shared helpers in one file and grade each address from that
   object (copy the same TU text to each `src/<addr>.cpp`; the grader picks the function by its leaf name).

Workflow per row. Grade with `bash $W/g.sh <addr> <leaf> [--flags ...]` (RTM compiler by default; prints MATCH /
RELOCATION_MATCH / DIFFER(built v retail) on line 1, then the side-by-side listing); `python3 $W/udiff.py <(bash
$W/g.sh <addr> <leaf>) | grep -v "^ "` shows only the differing instructions. The leaf is the `// Leaf:` line of the
source if present, else the function's own name (destructors: `??1Class`). Keep any `// Flags:` line. A row counts as
done only at MATCH or RELOCATION_MATCH. For each done row, write a self-contained behaviour test `$W/test/<addr>.cpp`
that copies the function body verbatim, stubs callees so calls and arguments are observable, exercises every branch,
and prints `<addr>_TEST PASS`; confirm with `python3 $W/tall.py <addr>`. No `__asm`, `naked`, `_emit`, `volatile`; no
`#include` of repo files by relative path in tests; `#include "engine/<Class>.h"` in sources where a header exists.
Line 1 of every matched source: `// <Module>::<Name>(<params>) -- <what it does>` (finish.py reads it).

Budget rule. Up to five serious attempts per strategy per row (fifteen per row); a new attempt means a different hypothesis about what retail saw in the TU, not a re-run. If a row still differs after that, write one
precise line to `$W/NOTES.md` saying which strategy got closest, the remaining byte delta, and the specific
instruction that differs (not "register colouring"; name the register and the value). Then move on.

Record as you go. Append `<addr>\t<MATCH|RELOCATION_MATCH|DIFFER(nvm)>\t<leaf>\t<one-line note>` to `$W/STATUS.tsv`
after each row so progress survives a restart. If `STATUS.tsv` exists when you start, skip rows already marked
matched. At the end run `python3 $W/finish.py` to build `payload.json` and `oracle.tsv` for the matched rows (it takes
rows that have both `src/` and `test/` files, so only matched rows may have a test).

Rules. Never run git. Never write under `rebuild/` or `docs/`. Never kill processes. Never repeat a failing command
more than twice with the same input. Report back with the `STATUS.tsv` contents, the list of callees you reconstructed
(address, name, where you got the body), any wrong catalog labels you found with the evidence, and one lesson per
matched row that a later agent could reuse (put them in `$W/LESSONS.md` too).
