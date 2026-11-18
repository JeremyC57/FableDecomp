# Name-fix agent NF82 (2026-11-17, round 7): the nf80 follow-ups: the 45 remaining operator< binders and the CCharString tree rows

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done", then "Decrease to 2 agents
at a time once next agent finishes". NF78-NF81 landed (last at 1784dbce59). NF83 runs beside you. Read first:
- `rebuild/candidates/manual/batches/2026-11-17_landing_nf80/README.md` and `naming/2026-11-17_agentNF80/` (NOTES.md, LESSONS.md,
  hdr/README.md, NAME_FIXES.tsv): how NF80 gave `CBasicString<char>::operator<` (0x00429950) one definition in `fable_string.h`
  (`FABLE_RETAIL_CBASICSTRING_LESS_BODY`, a template member; the self row adds the explicit instantiation) and moved 13 binders onto it;
- `docs/pipeline/GOTCHAS.md`, the nf80 line (one definition; clear `LINK=` before a link test);
- `docs/journal/2026-11/EGOD_FINDINGS.md` (retail has no identical-code folding).

## Your slice
Files in `$S/w22/agentNF82/`:
- `ENTRIES.tsv`: queue lines q10989..q11008 (NF80's follow-ups), 20 entries. `WORKLIST.tsv`: landed rows that bind an entry address under
  another spelling (reloc_bindings.tsv at 1784dbce59).
- `LESS_BINDERS.tsv`: **q11007**, the 45 landed rows that still call 0x00429950 under a stand-in spelling (26 `??MCBasicStringChar@@QBE_NABU0@@Z`,
  5 `??MCBasicStringChar@@QBE_NABV0@@Z`, 2 `??MCCharStringData@@QBE_NABV0@@Z`, 1 `??MCBasicStringChar@std@@QBE_NABU01@@Z`, 11
  `?Less@CCharString@@QAE_NPAV1@@Z`). reloc_bindings.tsv does not show them; NF80 found them with nftools/callers.py. Each row must move its
  string model onto `fable_string.h` (`FABLE_RETAIL_CCHARSTRING` + `FABLE_RETAIL_CBASICSTRING_LESS_BODY`, and the other fable_string.h opt-ins
  NF80 used); a rename alone is not a fix. Group them by stand-in (the 26 share one), prove the edit on 2-3, script it.
- q11008: the held ToCharString ports 0x0049124E / 0x006C37D0 (NF80's matched drafts in `batches/2026-11-17_landing_nf80/held/` rename a
  member inside a local CWideString model; port them onto fable_string.h's `FABLE_RETAIL_CWIDESTRING_TOCHARSTRING_BODY` instead).
- `rows.txt`: 77 rows locked as NF82.

**Order:** the respelling entries q10989..q11006 first (mostly self rows of CCharString trees and the CTCBase registry heap helpers), then
q11008, then the 45 binders. Up to 10 grades per row. You may overlay the root `fable_string.h` (opt-in additions only, token-identical
without them; prove it on 3-4 includers as NF80 did, `tokid.log`).

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
`$S/w22/agentNF80/LESSONS.md`, `$S/w22/agentNF82/LESSONS.md`, `$S/w22/agentNF81/LESSONS.md`. The tool scripts in
`$S/w22/agentNF80/` and `$S/w22/agentNF82/` (g.sh, vl.sh, pay.py, fpay.py, st.py, claim.sh) are a working set: copy them and
repoint them to your directory.

The main lessons, repeated here:
- No new local model of a class that has a shared header (that includes renaming a stand-in to the real class name, and including a
  header under a renaming macro). No cast addresses. No `__asm`, `naked`, `_emit`, and no `#error` probes in rows.
- Overlay headers carry `#ifdef` opt-ins only, with macro names specific to the member (`FABLE_RETAIL_<CLASS>_<MEMBER>`); a body
  edit must be token-identical without the opt-in, and you must say so in hdr/README.md. NF83 may add opt-ins to the same
  headers.
- Work only in `$S/w22/agentNF82/`, using absolute paths everywhere (a relative vland log path once wrote into the repo's `work/`).
  Grade in your private `S=<your dir>/pS`. Never run git (not even `git status`). Never write under /home/user/FableDecomp.
- Run one compile at a time, always inside `timeout`. Every Bash call must exit 0 (end `grep -c` / `diff` loops with `; true`).
- Write any file or note argument that contains backticks with a quoted heredoc (`<<'EOF'`), never inside double quotes.
- If you change a row that EH funclets include, regrade those funclets.
- Count before you report: every number comes from your files.
- Claim any extra row before changing it: `mkdir $S/w22/nflocks/<addr> 2>/dev/null && echo NF82 > $S/w22/nflocks/<addr>/owner; true`.
  If it is held by another agent, record it `deferred: held by <owner>`.

## Hand-back (in $S/w22/agentNF82/)
- rows/ (finished rows only; keep blocked drafts in work/); hdr/ and hdr/README.md;
- RENAMES.tsv (section 1 header line exactly as in NF44's brief, then `# Section 2`);
- STATUS.tsv (one line per row in rows.txt, with its terminal status);
- APPLIED.tsv (one line per entry in ENTRIES.tsv: done, done-already, rejected or blocked with the rows left);
- NAME_FIXES.tsv (address, correct_symbol, evidence, wrong_spellings_seen) for proven fixes outside rows.txt;
- REJECTED.tsv, NOTES.md (mark descriptive names) and LESSONS.md;
- recheck_final.log: one vland run over every changed row.

Report briefly: rows ported, not-a-binder, blocked and deferred; entries done and blocked; header opt-ins; rule slips.
