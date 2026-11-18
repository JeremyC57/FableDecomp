# Name-fix agent NF89 (2026-11-17, round 7): the nf87 follow-ups (14 entries, q11163..q11176)

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done", then "Decrease to 2 agents
at a time once next agent finishes". NF87 landed at bfb4aaeaa4. NF88 runs beside you. Read first:
- `rebuild/candidates/manual/batches/2026-11-17_landing_nf87/README.md`;
- `rebuild/candidates/manual/naming/2026-11-17_agentNF87/`: NOTES.md, LESSONS.md, STATUS.tsv, hdr/README.md, NAME_FIXES.tsv;
- `docs/journal/2026-11/EGOD_FINDINGS.md` (retail has no identical-code folding: two identical bodies at two addresses are two names).

## Your slice
Files in `$S/w22/agentNF89/`:
- `ENTRIES.tsv`: queue lines q11163..q11176, 14 entries. `WORKLIST.tsv`: 23 lines from reloc_bindings.tsv at bfb4aaeaa4. `rows.txt`: 51 rows
  locked as NF89.
- q11172 / q11173 are DATA symbols (CTCAIScratchpad's static gossip maps): settle the spelling from two sources and respell the binders.
- q11174..q11176: the `__partial_sort` rows whose `__introsort_loop` callers 0x00A35490 / 0x00A73910 / 0x00A73A00 still bind old spellings;
  port each whole family (introsort loop, partial sort, heap helpers) onto NF82 / NF85 / NF87's stlport/fable_containers.h opt-ins.
- **q11165..q11171 (NInventory CTCInventoryBase / CCategory accessors) come last.** NF88 is fixing the same NInventory_CTCInventoryBase.h header
  chain for q11153..q11159. Use NF87's landed opt-ins (`FABLE_RETAIL_CTCINVENTORYBASE_CATEGORYCOUNT`, `_CATEGORYWITHINDEX`,
  `FABLE_RETAIL_NINVENTORY_CCATEGORY_ACCESSORS`) first. Before adding anything, read NF88's overlay of that header (`$S/w22/agentNF88/hdr/engine/`, read-only) so you
  do not declare the same member twice; new opt-ins use member-specific names (`FABLE_RETAIL_<CLASS>_<MEMBER>`), and hdr/README.md says
  which lines you added so the coordinator can merge the two overlays.

Do the other entries in queue order first.

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
`$S/w22/agentNF87/LESSONS.md`, `$S/w22/agentNF86/LESSONS.md`, `$S/w22/agentNF85/LESSONS.md`. The tool scripts in
`$S/w22/agentNF87/` and `$S/w22/agentNF86/` (g.sh, vl.sh, pay.py, fpay.py, st.py, claim.sh) are a working set: copy them and
repoint them to your directory.

The main lessons, repeated here:
- No new local model of a class that has a shared header (that includes renaming a stand-in to the real class name, and including a
  header under a renaming macro). No cast addresses. No `__asm`, `naked`, `_emit`, and no `#error` probes in rows.
- Overlay headers carry `#ifdef` opt-ins only, with macro names specific to the member (`FABLE_RETAIL_<CLASS>_<MEMBER>`); a body
  edit must be token-identical without the opt-in, and you must say so in hdr/README.md. NF88 may add opt-ins to the same
  headers.
- Work only in `$S/w22/agentNF89/`, using absolute paths everywhere (a relative vland log path once wrote into the repo's `work/`).
  Grade in your private `S=<your dir>/pS`. Never run git (not even `git status`). Never write under /home/user/FableDecomp.
- Run one compile at a time, always inside `timeout`. Every Bash call must exit 0 (end `grep -c` / `diff` loops with `; true`).
- Write any file or note argument that contains backticks with a quoted heredoc (`<<'EOF'`), never inside double quotes.
- If you change a row that EH funclets include, regrade those funclets.
- Count before you report: every number comes from your files.
- Claim any extra row before changing it: `mkdir $S/w22/nflocks/<addr> 2>/dev/null && echo NF89 > $S/w22/nflocks/<addr>/owner; true`.
  If it is held by another agent, record it `deferred: held by <owner>`.

## Hand-back (in $S/w22/agentNF89/)
- rows/ (finished rows only; keep blocked drafts in work/); hdr/ and hdr/README.md;
- RENAMES.tsv (section 1 header line exactly as in NF44's brief, then `# Section 2`);
- STATUS.tsv (one line per row in rows.txt, with its terminal status);
- APPLIED.tsv (one line per entry in ENTRIES.tsv: done, done-already, rejected or blocked with the rows left);
- NAME_FIXES.tsv (address, correct_symbol, evidence, wrong_spellings_seen) for proven fixes outside rows.txt;
- REJECTED.tsv, NOTES.md (mark descriptive names) and LESSONS.md;
- recheck_final.log: one vland run over every changed row.

Report briefly: rows ported, not-a-binder, blocked and deferred; entries done and blocked; header opt-ins; rule slips.
