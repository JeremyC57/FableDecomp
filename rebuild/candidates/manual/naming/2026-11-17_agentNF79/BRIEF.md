# Name-fix agent NF79 (2026-11-17, round 7): the follow-ups queued at nf76-nf77 (19 entries, q10956..q10974)

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done" and "add another agent to
speed up naming queue completion". NF76 and NF77 landed (last at 7c68fb20d9). NF78 and NF80 run beside you. Read first:
- `rebuild/candidates/manual/batches/2026-11-17_landing_nf76/README.md`, `_nf77/README.md`;
- `rebuild/candidates/manual/naming/2026-11-17_agentNF76/`, `_agentNF77/`: NOTES.md, LESSONS.md, hdr/README.md;
- `docs/journal/2026-11/EGOD_FINDINGS.md` (retail has no identical-code folding: two identical bodies at two addresses are two names).

## Your slice
Files in `$S/w22/agentNF79/`:
- `ENTRIES.tsv`: `NAME_FIXES_QUEUE.tsv` lines q10956..q10974 (tag `land1117fu`), 19 entries. `WORKLIST.tsv`: the landed rows that bind
  an entry address under another spelling (reloc_bindings.tsv at 7c68fb20d9), 48 lines. Notes:
  - q10969 (0x013CA818) and q10973 (0x01436E9C) are DATA symbols. Settle each one's spelling from two sources (Ego_r.pdb / FableWin.pdb
    S_GDATA32 / S_LDATA32 names, the class's static member in the shared header) and respell the binders. If the Ghidra label is wrong,
    put the fix in NAME_FIXES.tsv (the coordinator edits ghidra_out/labels_*.tsv), do not edit ghidra_out.
  - q10970 asks for a PORT of 0x0057770A, the twin of 0x005778CA (NF76 ported 0x005778CA: copy its recipe from
    `naming/2026-11-17_agentNF76/` and the landed row).
  - q10974 lists five addresses with stand-in spellings and no claim: find each address's PDB name (two sources), then respell the
    binders. An address you cannot name from two sources goes to REJECTED.tsv with what you found.
- `rows.txt`: the 60 rows you may change, locked as NF79. `HELD.txt`: 0x0059A238 is held by NF78 (record it `deferred: held by NF78`).

Do the entries in queue order; the port (q10970) after the respellings. No row may end at 0 grades unless it is not-a-binder.

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
  edit must be token-identical without the opt-in, and you must say so in hdr/README.md. NF78 and NF80 may add opt-ins to the same
  headers.
- Work only in `$S/w22/agentNF79/`, using absolute paths everywhere (a relative vland log path once wrote into the repo's `work/`).
  Grade in your private `S=<your dir>/pS`. Never run git (not even `git status`). Never write under /home/user/FableDecomp.
- Run one compile at a time, always inside `timeout`. Every Bash call must exit 0 (end `grep -c` / `diff` loops with `; true`).
- Write any file or note argument that contains backticks with a quoted heredoc (`<<'EOF'`), never inside double quotes.
- If you change a row that EH funclets include, regrade those funclets.
- Count before you report: every number comes from your files.
- Claim any extra row before changing it: `mkdir $S/w22/nflocks/<addr> 2>/dev/null && echo NF79 > $S/w22/nflocks/<addr>/owner; true`.
  If it is held by another agent, record it `deferred: held by <owner>`.

## Hand-back (in $S/w22/agentNF79/)
- rows/ (finished rows only; keep blocked drafts in work/); hdr/ and hdr/README.md;
- RENAMES.tsv (section 1 header line exactly as in NF44's brief, then `# Section 2`);
- STATUS.tsv (one line per row in rows.txt, with its terminal status);
- APPLIED.tsv (one line per entry in ENTRIES.tsv: done, done-already, rejected or blocked with the rows left);
- NAME_FIXES.tsv (address, correct_symbol, evidence, wrong_spellings_seen) for proven fixes outside rows.txt;
- REJECTED.tsv, NOTES.md (mark descriptive names) and LESSONS.md;
- recheck_final.log: one vland run over every changed row.

Report briefly: rows ported, not-a-binder, blocked and deferred; entries done and blocked; header opt-ins; rule slips.
