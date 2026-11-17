# Name-fix agent NF80 (2026-11-17, round 7): the 17 rows NF75 left blocked; first the one definition of CBasicString<char>::operator<

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done" and "add another agent to
speed up naming queue completion". NF76 and NF77 landed (last at 7c68fb20d9). NF78 and NF79 run beside you. Read first:
- `rebuild/candidates/manual/batches/2026-11-17_landing_nf75/README.md` and `_nf70/README.md`;
- `rebuild/candidates/manual/naming/2026-11-17_agentNF75/NOTES.md` (sections on q10920, q10923, q10931, q10939, q10943), its LESSONS.md and
  hdr/README.md; `_agentNF70/NOTES.md` on q10763;
- `docs/journal/2026-11/EGOD_FINDINGS.md` (retail has no identical-code folding).

## Coordinator ruling for this slice: you may overlay the ROOT header `rebuild/include/fable_string.h`
Earlier briefs kept agents out of root headers; this one lifts that for fable_string.h only (q10763 and q10939). Same rules as any
overlay: additions only, each under a new `#ifdef FABLE_RETAIL_*` opt-in named for the member, token-identical without the opt-in,
asserts pinned, base copy in `orig/fable_string.h`, every change explained in hdr/README.md. Put it at `<your dir>/hdr/fable_string.h`
(the overlay path mirrors rebuild/include). fable_string.h has hundreds of includers, so prove "token-identical without the opt-in"
by preprocessing three or four landed includers with and without your overlay (no opt-in defined) and diffing the output.

## Your slice
Files in `$S/w22/agentNF80/`:
- `BLOCKED.tsv`: NF75's 17 blocked rows with its STATUS lines (what each needs). `ENTRIES.tsv`: the six queue entries they belong to
  (q10763, q10920, q10923, q10931, q10939, q10943).
- `q10763_change.txt`: the held fable_string.h change for 0x00429950 (`FABLE_RETAIL_CBASICSTRING_LESS_BODY` / `_DEFINE`). It is a draft:
  grade it, fix it, and follow it only as far as it holds.
- `LESS_BINDINGS.tsv`: the 13 landed rows that call 0x00429950, each under its own stand-in spelling; one (0x0042AAB3) already binds
  the right `??M?$CBasicString@D@@QBE_NABV0@@Z`.
- `rows.txt`: 21 rows locked as NF80: the 17 plus 0x009EFB80 (carries a second auto_inline(off) definition of operator<) and
  0x006C31B0 / 0x00756350 / 0x007ACC90 (NF70 says they carry the same copy). Those four are yours only so the one definition holds;
  take each to a terminal status, and mark it not-a-binder if it does not define or bind operator<.

**Order:** (1) the operator< group: fable_string.h opt-in, the self row 0x00429950 on it, 0x009EFB80 drops its copy, then the 11 binders
of q10923 moved onto fable_string.h (they model CBasicString / CCharString locally); (2) q10939 0x0099B2C0 (the inline accessor in
fable_string.h); (3) q10943 0x005907AB (NUISystem::CDeletion); (4) q10931 0x009C8060; (5) q10920 0x00835C80 (full port; last).
Up to 15 grades per row. Where this order differs from "Order of work" below, follow this one.

**Static asserts:** never move a `FABLE_STATIC_ASSERT` to another line. After editing an overlay header run
`python3 /home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/lane_merge/pin_asserts.py /home/user/FableDecomp/rebuild/include <your dir>/hdr <header>`
and say in hdr/README.md what it pinned. Keep base copies of the headers you overlay in `<your dir>/orig/` (outside hdr/).

Two kinds of work, often in the same entry:
1. **Self row:** the row AT the entry address has the wrong own name. Re-author it under the correct spelling (usually by moving it
   onto its shared header) and put it in RENAMES section 1 (address, module, name, mangled_leaf, old_catalog_name, old_leaf,
   evidence). `module` + `name` is the catalog name: give the real class and member (no `Class::` prefix in `name`). For STL
   instances use the module spelling the catalog already uses for that container. For a leaf-only change on a row whose catalog
   name is already right, repeat the old catalog name.
2. **Binder rows:** landed rows that call the entry address under another spelling. Respell them (section 2 of RENAMES: caller
   row, old symbol, new symbol, callee address), taken from before/after compiles.

**Every row must end `ported`, `not-a-binder` or `blocked`.** `carry` is not allowed. A row already right is `not-a-binder` (say
why). An entry whose self row is already right and has no wrong binders is `done-already`.

**Verify every claim from two sources** before applying it (PDB name sets, the callee body, vtable slots, the header). Queue claims
come from one agent's evidence; a claim that does not hold goes to REJECTED.tsv with the reason. Do NOT re-probe the whole slice.

**Order of work.** The entries in queue order. Take each row
to a terminal grade before starting the next. **Budget:** up to 10 grades per entry row and 15 per port row; after 3 failed forms on one row mark it blocked
with the exact compiler error or diff. A row that models a dozen classes locally is blocked/port unless it moves within 10 grades;
name the classes that must move first.

**Containers restart without warning.** Write STATUS.tsv and APPLIED.tsv after EVERY row. The first grade after a restart can time
out while wineserver starts; retry once.

## Method: transformer first, then batch
NF44 ported 19 scratch-texture rows by proving the rewrite by hand on 2-3 rows and then applying it with a small script
(`$S/w22/agentNF44/work/scratch.py`). Group rows that share a stand-in or a callee respelling, prove the edit on 2-3, script it,
grade the rest in one vland payload of 20-30 rows, then fix the stragglers by hand.

## Also carry the lessons
These sections of `$S/w22/agentNF44/BRIEF.md` apply unchanged; read them: "Lessons of earlier passes", "Disk", "Round-3 /
Round-2 / Round-1 lessons", "Per entry", "Hard rules", "Lane", "Hand-back". Then the newest LESSONS.md files:
`$S/w22/agentNF77/LESSONS.md`, `$S/w22/agentNF76/LESSONS.md`, `$S/w22/agentNF75/LESSONS.md`. The tool scripts in
`$S/w22/agentNF77/` and `$S/w22/agentNF76/` (g.sh, vl.sh, pay.py, fpay.py, st.py, claim.sh) are a working set: copy them and
repoint them to your directory.

The main lessons, repeated here:
- No new local model of a class that has a shared header (that includes renaming a stand-in to the real class name, and including a
  header under a renaming macro). No cast addresses. No `__asm`, `naked`, `_emit`, and no `#error` probes in rows.
- Overlay headers carry `#ifdef` opt-ins only, with macro names specific to the member (`FABLE_RETAIL_<CLASS>_<MEMBER>`); a body
  edit must be token-identical without the opt-in, and you must say so in hdr/README.md. NF78 and NF79 may add opt-ins to the same
  headers.
- Work only in `$S/w22/agentNF80/`, using absolute paths everywhere (a relative vland log path once wrote into the repo's `work/`).
  Grade in your private `S=<your dir>/pS`. Never run git (not even `git status`). Never write under /home/user/FableDecomp.
- Run one compile at a time, always inside `timeout`. Every Bash call must exit 0 (end `grep -c` / `diff` loops with `; true`).
- Write any file or note argument that contains backticks with a quoted heredoc (`<<'EOF'`), never inside double quotes.
- If you change a row that EH funclets include, regrade those funclets.
- Count before you report: every number comes from your files.
- Claim any extra row before changing it: `mkdir $S/w22/nflocks/<addr> 2>/dev/null && echo NF80 > $S/w22/nflocks/<addr>/owner; true`.
  If it is held by another agent, record it `deferred: held by <owner>`.

## Hand-back (in $S/w22/agentNF80/)
- rows/ (finished rows only; keep blocked drafts in work/); hdr/ and hdr/README.md;
- RENAMES.tsv (section 1 header line exactly as in NF44's brief, then `# Section 2`);
- STATUS.tsv (one line per row in rows.txt, with its terminal status);
- APPLIED.tsv (one line per entry in ENTRIES.tsv: done, done-already, rejected or blocked with the rows left);
- NAME_FIXES.tsv (address, correct_symbol, evidence, wrong_spellings_seen) for proven fixes outside rows.txt;
- REJECTED.tsv, NOTES.md (mark descriptive names) and LESSONS.md;
- recheck_final.log: one vland run over every changed row.

Report briefly: rows ported, not-a-binder, blocked and deferred; entries done and blocked; header opt-ins; rule slips.
