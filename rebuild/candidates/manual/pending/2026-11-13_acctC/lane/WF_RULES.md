# acctC workflow rules (read after your BRIEF.md; these are binding)

Goal of the owner: match as many reserved rows as possible, fast. Byte-exact VC7.1 (cl 13.10.3077) reconstruction of retail
Fable.exe, behaving exactly like retail, portable C++.

## Setup
    S=/tmp/claude-0/-home-user-FableDecomp/923afea2-3bc0-53db-a7ad-a8bbc78e03e2/scratchpad
    source $S/wine/env.sh; cd /home/user/FableDecomp
    W=$S/w22/<your batch dir>
    export FABLE_HDR_OVERLAY=$W/hdr        # in its OWN statement, after W is set (same-line expansion gives /hdr and cl spins)
Your batch dir holds BRIEF.md (rules, tools, grading, addmatch, hand-back files), rows.tsv, ref/<addr>/ (retail_dis.txt, any
earlier draft and notes), hdr/ (header overlay + README.md), and for parked rows prev/ (best drafts + notes of earlier agents).
Shared material: `$S/w22/wfcommon/lessons/LESSONS_*.md` (short; what closed earlier rows — skim the ones for your area),
`$S/w22/wfcommon/tools/<agent>/` (helper scripts of earlier agents: copy what you use into $W/tools and repoint paths;
C44: sd.py frame-masked structural diff, bk.py, rd.sh, er.sh, fixed rstr.py; C42: v.sh grade + call targets, vl.sh read-only
vland, vt.sh, cl.sh call targets, cp.sh destructor-copy map, ordk.sh / batch.sh x87 term-order sweeps, vtw.py vtable words;
C41: gg.sh, vg.sh, sw.sh, best.sh, nd.py, tr1.sh priorities; C35: calls.py, lin2.py, kh.py, lens.sh; C28: sbs.py, fo.py, mkr.py;
C26: inline-trace analysis krange.py / slack.py / late.py, fa.sh frame map; C15: efm.py frame-slot map; C20/C22: render-row
tools win.py, blk.py, od.py, pat.py), `$S/w22/wfcommon/style_small/` and `style_ui/` (matched sources + tests: style guides).
A grade takes about 2 s; sweeps are cheap, your context is not.

## Hand authoring is the default
The automated aids (draft porters, opt-in re-apply scripts, variant sweeps, forced-priority what-ifs) are optional. When a
draft will not port, a tool spins or errors, or a sweep stalls: stop using the tool and WRITE THE FUNCTION BY HAND from the
disassembly (windows of ~60 lines, writing the C++ for each window before reading the next) in the idioms of landed siblings
of the same class / translation unit (`grep -rl <Class> rebuild/src/compiled/`). Most matches in this lane were made that way.
A stale draft is a hint, not an asset: if it needs more than ~10 minutes of header porting, re-author instead.

## Order of work per row (cheap measurements before register work)
1. Prove the name (two sources: Ego_r + FableWin name sets, vtable slot, landed callers) and the function start + true
   extent (catalog names and lengths are labels; several rows here were fragments or mis-sized; if the extent differs rebuild
   the oracle row as BRIEF.md says and note it).
2. Twin diff: the landed/matched sibling's statement idioms and its exact opt-in set, pragmas and `// Flags:` line.
3. Structural measurements: call list by TARGET (q.py masks call targets: use cl.sh / calls.py / read-only vland), counts of
   destructor copies / rets / release copies (exit kind: break v continue v return v flag; arm order), jcc histogram
   (signedness, < v <=), frame size, where retail loads a vtable/member relative to the preceding call (named local v nested
   expression), which destructors lack the null store (scope nesting).
4. Helper texts: existing shared-header opt-ins first; FableWin /Od size signatures (fwsize.py) as a hint (FableWin's
   compiler differs on float code: sizes of x87 helpers are unreliable).
5. Inline decisions: one c2 inline trace; caller-size window; natural uniform text (STLport helper opt-ins).
6. Only then register allocation / frame slots: named locals where retail has a frame home or holds a value in a callee-saved
   register, none where it does not; declaration order from store order; out-locals `= 0` when retail zeroes the register;
   one reused result variable; a named object in its own `{ }` block shares slots with temporaries; same assignment in both
   arms of if/else-if; a same-TU callee's landed text defined above the row.
7. x87 term / operand order follows c2 symbol numbering: helper texts and sweeps of natural named-local variants.

## Pace (breadth first)
Per row: author → grade → up to ~30 grades of residue work → if not matched and not visibly closing, save the best source in
differ/ with a 5-line residue note in NOTES.md and GO ON to the next row. Return to near-misses only when every row of the batch
has a graded draft (or was shown to be blocked). A residue that is a pure register-pick / reload-home / alias-analysis question
with equal calls and length and no known closer is parked on sight. Never spend the whole run on one row. A matched row needs
its behaviour test PASS and addmatch (BRIEF.md) before you move on. Write NOTES.md / STATUS.tsv as you go: a successor with a
fresh context continues from your files, and a container restart must lose nothing.

## OWNER'S ACCEPTANCE TEST (2026-11-14) — read this before the "Hard rules"; it overrides stricter wording in your prompt
The owner's main question before accepting any device or fitted spelling is:
  1. Does it BYTE MATCH retail (MATCH / RELOCATION_MATCH)?
  2. Does it BEHAVE THE SAME as retail (behaviour test PASS; the construct cannot change what the function does)?
  3. Is it PORTABLE code (plain C++ other compilers accept; MSVC-only switches are flags or pragmas other compilers ignore)?
If all three are yes, the construct may be handed back. So, when natural spellings have been tried and a row is otherwise
right, these LAST-RESORT devices are allowed (prefer them in this order, use the least, and mark every one):
  a. a fitted but ordinary spelling (named local, redundant-but-real guard such as `if (it != end) for (...)`, statement order,
     nested braces) — mark `FITTED` in the header comment;
  b. an inline control for a pure inline-decision residue (`// Flags: /d2inlT<N>` / `/d2inlS<N>`, scoped `#pragma inline_depth`,
     `#pragma auto_inline`) with an `// Inline-budget:` line (owner decision 2026-11-08);
  c. for c2 record-count ties that only flip an operand order (test/or, load order, SIB base/index, x87 commutative order):
     k unused named locals (smallest k), as the repo's own author prompts allow ("as a last resort only: adding k = 0..7
     unused named locals re-keys it"; 20 landed rows do this) — mark with a `// Fidelity:` comment saying what it re-keys;
  d. dead release asserts through the overlay's `fable_assert.h` (`FABLE_RETAIL_ASSERT`, `// Inferred:`), count stated.
ASM has EXTRA rules on top of the three questions (docs/pipeline/ASM_RULES.md), and the three questions still apply to it:
where retail itself used asm (bytes no VC7.1 C++ spelling emits, cited by address + instruction), use an intrinsic first, else
an isolated macro-wrapped kernel in a SHARED HEADER with a `FABLE_PORTABLE` twin and a dual-path test (that is what keeps it
portable and behaving the same), never asm for a compiler residue, never `__asm` / naked / `_emit` in a row source or test.
Still NOT acceptable, because they fail the test: `volatile` tricks (except where retail used it, ASM_RULES), stand-in or stub callee bodies (not the real code), forced c2 priorities or any diagnostic build,
a local class model where a shared header exists (the lander's strict-header check rejects it), a fitted text in a SHARED header
that changes behaviour or contradicts a PDB. A last-resort device never replaces finding the real statement structure: a row
with wrong calls, wrong length or structural hunks is not "one ruler away".

## Hard rules
- No `__asm` / naked / `_emit` in a row or test (retail asm only as a shared-header macro kernel with a FABLE_PORTABLE twin and
  a dual-path test, docs/pipeline/ASM_RULES.md); no `volatile` tricks; no stand-in bodies; (dead locals / guards only as the marked last-resort devices above); no compiler flag or pragma without byte evidence or a landed same-TU sibling carrying it;
  no helper text that contradicts a PDB.
- CORRECTION (coordinator, 2026-11-14; this replaces any "never /d2inl / inline_depth" wording in your prompt): the owner's
  decision of 2026-11-08 (tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md, "inline-budget residues may
  use an inline control") ALLOWS an inline control when the ONLY residue is an inline decision (a callee expanded at some
  sites and called at others, inline depth, caller-size cap) — never for register choice, block layout or frame slots. Order
  of preference: (1) plain C++ with retail-plausible control flow; (2) a per-row flag with the source untouched,
  `// Flags: /d2inlT<N>` or `/d2inlS<N>` (payload "flags"); (3) a scoped `#pragma inline_depth(N)` ... `#pragma inline_depth()`
  or `#pragma auto_inline(off)` ... `(on)` around the affected code. Conditions: MATCH / RELOCATION_MATCH, behaviour test PASS,
  shared headers as usual, pick the SMALLEST N that matches and say why, and mark it on line 2 (or right after the Leaf/Flags
  header): `// Inline-budget: <device> -- <which callee / sites; diagnostic evidence>`. Fitted header texts whose only purpose
  is to move an inline price (e.g. RESET_ELSE_*) and dead guards / rulers remain NOT acceptable: use the flag instead.
- Shared headers for `this` (`#include "engine/<Class>.h"`); a local class model only where the header cannot express the
  class and a landed sibling does the same (say so). Header additions ONLY as new additive `FABLE_RETAIL_*` opt-ins in your
  overlay (default text untouched), listed in hdr/README.md with evidence, moved asserts pinned with pin_asserts.py.
  Same-TU callee definitions go in the row source (text equal to the landed body), not in a header.
- Names in exact PDB spelling with two sources; descriptive names only when no original exists anywhere, marked in NOTES.
- Mark every spelling in your hand-back as EVIDENCED (retail bytes / twin / PDB) or FITTED (chosen only by grading).
- Never run `--land`, never commit or push or run git write commands, never touch another batch dir or the shared Wine
  processes beyond the documented tools; kill only your own processes, by PID. Other agents work concurrently in sibling dirs.
- Regrade any already-matched source whose included headers you change (`g++ -M` shows which) — in your own src/ and in
  `$S/w22/wfcommon/style_*` if you touch a header they include (report it; do not edit those files).

## Context discipline (every earlier agent stopped from context exhaustion, not from lack of ideas)
Never read whole notes, sources, traces or disassemblies: grep/sed the section. Every tool output under ~40 lines
(tail/head/grep/wc). One-line-per-variant grading scripts. Small patches, no re-reading of large files.

## Files to leave in your batch dir
src/<addr>.cpp + test/<addr>.cpp for matched rows, payload.json + oracle.tsv (addmatch.py's own format), differ/<addr>.cpp for
best non-matching sources, hdr/ + hdr/README.md, NOTES.md, LESSONS.md, STATUS.tsv (one line per row: address, grade, test,
grades used, state), ATTEMPTS.log, LABELS.tsv, NAME_FIXES.tsv.

## OWNER DECISION 2026-11-15 on device d (dead release asserts)
Dead asserts are accepted as they are, at any count, PROVIDED the row source carries a note saying why they are there:
an `// Inferred:` line in the header comment giving the count, what residue they close (addresses), and the evidence
(debug-build size gap, sibling rows). A row with FABLE_RETAIL_ASSERT and no such note is not handed back. Use only the
shared fable_assert.h macro (never a home-made form; `__assume` forms were measured and ruled out).

## OWNER GUIDANCE 2026-11-15: hand-authoring often beats automatic tools
Sometimes writing the source by hand is more likely to get a match than automatic tools. Scripted sweeps, combo generators
and ruler searches (k locals, assert counts, flag ranges) are for measuring, not for finding the source: several rows burned
hundreds of grades on sweeps and then closed in one step when an agent read the retail disassembly and wrote the block the way
a programmer would have (0x00B9AC30: about 1,100 swept grades failed; it closed by spelling one block like its matched
siblings). So: read the retail bytes and the matched same-class / same-TU siblings first, reason about what statement
structure produces them, and write that by hand. Reach for a sweep only to answer one specific measured question, and stop
sweeping when two sweeps in a row move nothing.

## OWNER RULING 2026-11-16: dead asserts inside an inline helper that retail plausibly asserted in
Dead asserts in an inline accessor whose debug build shows asserts (e.g. CGraphicDataBank::PeekGraphic) are accepted even
when their effect is on an inline decision, provided the `// Inferred:` note says so and inline controls were tried first.

## TIP 2026-11-16: Anniversary 360 line table as a statement-order source (third source, mark it as such)
The Anniversary Xbox 360 Test .xex is unencrypted. wfB5 rebuilt its image and disassembled the PowerPC code of a function
against the PDB line table, which gives the ORIGINAL statement order and local names; this took 0x009FDE80 from 13 register
hunks to 0. Scripts and a worked example: /tmp/claude-0/-home-user-FableDecomp/923afea2-3bc0-53db-a7ad-a8bbc78e03e2/scratchpad/w22/wfB5/probe/anniv/ (mkimg.py, ppcdis.py, bl.txt; mapping in wfB5/NOTES.md).
Use it when a row is stuck on statement order / control-flow shape. The 360 file was edited after TLC, so where it and the
retail PC bytes disagree the retail bytes win; cite it in the row header as third-source evidence.

## TIP 2026-11-16: check that every same-TU template / COMDAT callee is DEFINED in the row
0x006D0830 sat through eleven agents as a "global colouring-order" residue; it closed on the first grade once the template
copy retail compiled in the same TU (GetAllLeafThings<...>, COMDAT 0x006D4600) was defined in the row instead of only
declared: with the body visible the compiler knows the arguments do not escape. If your residue is a hoist / register choice
around a call to a template or inline-capable function that retail emitted next to the row, make its real body visible.
