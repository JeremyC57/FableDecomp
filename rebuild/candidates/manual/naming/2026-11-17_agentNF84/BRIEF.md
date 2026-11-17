# Name-fix agent NF84 (2026-11-17, round 7): the nf83 follow-ups (12 entries, q11044..q11055) and 23 rows NF64 left blocked

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done", then "Decrease to 2 agents
at a time once next agent finishes". NF83 landed at f076fa85da. NF82 runs beside you. Read first:
- `rebuild/candidates/manual/batches/2026-11-17_landing_nf83/README.md`, `_nf64/README.md`;
- `rebuild/candidates/manual/naming/2026-11-17_agentNF83/` and `_agentNF64/`: NOTES.md, LESSONS.md, STATUS.tsv, hdr/README.md;
- `docs/journal/2026-11/EGOD_FINDINGS.md` (retail has no identical-code folding: two identical bodies at two addresses are two names).

## Your slice
Files in `$S/w22/agentNF84/`:
- `ENTRIES.tsv`: queue lines q11044..q11055 (tag `land1117fu`), 12 entries; `WORKLIST.tsv`: landed rows that bind an entry address under
  another spelling (reloc_bindings.tsv at f076fa85da). NF83 found most of these by reading sources, not grading: grade before you trust.
  - q11054 (0x006DC020): a name collision. 0x00451AC6 defines the EDamageAttribute `_Rb_tree` destructor (its callers bind it so); the landed
    0x006DC020 (NF81, descriptive, no caller) defines the same symbol. Find 0x006DC020's real identity from its body, neighbours in
    `ghidra_out/egod_carved_functions.tsv` and the PDBs (the implicit `map<EDamageAttribute,float>` destructor, the owning class's destructor,
    ...), re-author it under that name, or REJECT with what you found. Never leave two rows defining one symbol.
  - q11055 (0x00860130): find a second source for `CBrainOutput::SetAsPerformAction` (Ego_r / FableWin name sets, callers, strings).
- `BLOCKED.tsv`: 23 rows NF64 left blocked (later agents did not touch them), each with NF64's STATUS line: `port` rows need their local
  models moved onto shared headers; `authoring` rows need the body re-authored (0x005BEAD5 / 0x005BF30D are mid-function fragments: say what
  the containing function is and mark them blocked if the row can not stand alone). 0x008E7E70 and 0x00EDC720 were blocked again by NF78
  (its STATUS says why). Up to 15 grades each.
- `rows.txt`: 45 rows locked as NF84.

Do the 12 entries first in queue order, then the blocked rows closest-first.

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
`$S/w22/agentNF80/LESSONS.md`, `$S/w22/agentNF84/LESSONS.md`, `$S/w22/agentNF81/LESSONS.md`. The tool scripts in
`$S/w22/agentNF80/` and `$S/w22/agentNF84/` (g.sh, vl.sh, pay.py, fpay.py, st.py, claim.sh) are a working set: copy them and
repoint them to your directory.

The main lessons, repeated here:
- No new local model of a class that has a shared header (that includes renaming a stand-in to the real class name, and including a
  header under a renaming macro). No cast addresses. No `__asm`, `naked`, `_emit`, and no `#error` probes in rows.
- Overlay headers carry `#ifdef` opt-ins only, with macro names specific to the member (`FABLE_RETAIL_<CLASS>_<MEMBER>`); a body
  edit must be token-identical without the opt-in, and you must say so in hdr/README.md. NF82 may add opt-ins to the same
  headers.
- Work only in `$S/w22/agentNF84/`, using absolute paths everywhere (a relative vland log path once wrote into the repo's `work/`).
  Grade in your private `S=<your dir>/pS`. Never run git (not even `git status`). Never write under /home/user/FableDecomp.
- Run one compile at a time, always inside `timeout`. Every Bash call must exit 0 (end `grep -c` / `diff` loops with `; true`).
- Write any file or note argument that contains backticks with a quoted heredoc (`<<'EOF'`), never inside double quotes.
- If you change a row that EH funclets include, regrade those funclets.
- Count before you report: every number comes from your files.
- Claim any extra row before changing it: `mkdir $S/w22/nflocks/<addr> 2>/dev/null && echo NF84 > $S/w22/nflocks/<addr>/owner; true`.
  If it is held by another agent, record it `deferred: held by <owner>`.

## Hand-back (in $S/w22/agentNF84/)
- rows/ (finished rows only; keep blocked drafts in work/); hdr/ and hdr/README.md;
- RENAMES.tsv (section 1 header line exactly as in NF44's brief, then `# Section 2`);
- STATUS.tsv (one line per row in rows.txt, with its terminal status);
- APPLIED.tsv (one line per entry in ENTRIES.tsv: done, done-already, rejected or blocked with the rows left);
- NAME_FIXES.tsv (address, correct_symbol, evidence, wrong_spellings_seen) for proven fixes outside rows.txt;
- REJECTED.tsv, NOTES.md (mark descriptive names) and LESSONS.md;
- recheck_final.log: one vland run over every changed row.

Report briefly: rows ported, not-a-binder, blocked and deferred; entries done and blocked; header opt-ins; rule slips.
