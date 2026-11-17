# Name-fix agent NF91 (2026-11-17, round 7): the nf89 follow-ups (16 entries, q11190..q11205)

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done", then "Decrease to 2 agents
at a time once next agent finishes". NF89 landed at 0cc6bc1018. NF90 runs beside you. Read first:
- `rebuild/candidates/manual/batches/2026-11-17_landing_nf89/README.md`, `_nf88/README.md`;
- `rebuild/candidates/manual/naming/2026-11-17_agentNF89/`: NOTES.md, LESSONS.md, STATUS.tsv, hdr/README.md, NAME_FIXES.tsv, sweep_result.tsv;
- `docs/pipeline/GOTCHAS.md`: the nf80, nf86 and nf88 lines (one definition; the `// Fidelity: scaffolding` pick-any pattern).

## Your slice
Files in `$S/w22/agentNF91/`:
- `ENTRIES.tsv`: queue lines q11190..q11205, 16 entries. `WORKLIST.tsv`: 16 lines from reloc_bindings.tsv at 0cc6bc1018. `rows.txt`: 41 rows
  locked as NF91.
- q11204 (one definition): 0x0059C6B0 keeps a same-TU auto_inline(off) strong copy of GetCategoryIndexForTradeType, which is landed separately at
  0x0059B6F4. Give the header the body under a new opt-in, re-author the owner 0x0059B6F4 with the nf88 pick-any scaffolding, make 0x0059C6B0
  take the same inline body (it still needs the body visible in its TU), and prove one definition by linking the two objects with `LINK=`
  cleared (as NF88, `naming/2026-11-17_agentNF88/link/`).
- q11205 / q11200 (0x005BF77D): NF89's try3 (`$S/w22/agentNF89/work/005bf77d_try3.cpp`, opt-ins in `work/hdr_blocked/`) is DIFFER 324 v 334 on
  register choice only. Try first: the GetDef / GetSubDef template bodies under `#pragma auto_inline(off)`; then GetAllItems 0x005BF482's body
  visible in the TU through the pick-any scaffolding (never a strong copy).
- q11199 0x005A30A1: DIFFER 917 v 917 (two `stock[key]=1` stores tail-merged; NF89 STATUS).
- NInventory CCategory rows (q11195, q11197, q11203): NF90 also adds CCategory / CTCInventoryBase members. Read its overlay
  (`$S/w22/agentNF90/hdr/engine/`, read-only) before adding a member, so one is never declared twice; member-specific opt-in names; list
  your added lines in hdr/README.md.

Do the entries in queue order; q11204 and q11205 last.

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
`$S/w22/agentNF89/LESSONS.md`, `$S/w22/agentNF88/LESSONS.md`, `$S/w22/agentNF87/LESSONS.md`. The tool scripts in
`$S/w22/agentNF89/` and `$S/w22/agentNF88/` (g.sh, vl.sh, pay.py, fpay.py, st.py, claim.sh) are a working set: copy them and
repoint them to your directory.

The main lessons, repeated here:
- No new local model of a class that has a shared header (that includes renaming a stand-in to the real class name, and including a
  header under a renaming macro). No cast addresses. No `__asm`, `naked`, `_emit`, and no `#error` probes in rows.
- Overlay headers carry `#ifdef` opt-ins only, with macro names specific to the member (`FABLE_RETAIL_<CLASS>_<MEMBER>`); a body
  edit must be token-identical without the opt-in, and you must say so in hdr/README.md. NF90 may add opt-ins to the same
  headers.
- Work only in `$S/w22/agentNF91/`, using absolute paths everywhere (a relative vland log path once wrote into the repo's `work/`).
  Grade in your private `S=<your dir>/pS`. Never run git (not even `git status`). Never write under /home/user/FableDecomp.
- Run one compile at a time, always inside `timeout`. Every Bash call must exit 0 (end `grep -c` / `diff` loops with `; true`).
- Write any file or note argument that contains backticks with a quoted heredoc (`<<'EOF'`), never inside double quotes.
- If you change a row that EH funclets include, regrade those funclets.
- Count before you report: every number comes from your files.
- Claim any extra row before changing it: `mkdir $S/w22/nflocks/<addr> 2>/dev/null && echo NF91 > $S/w22/nflocks/<addr>/owner; true`.
  If it is held by another agent, record it `deferred: held by <owner>`.

## Hand-back (in $S/w22/agentNF91/)
- rows/ (finished rows only; keep blocked drafts in work/); hdr/ and hdr/README.md;
- RENAMES.tsv (section 1 header line exactly as in NF44's brief, then `# Section 2`);
- STATUS.tsv (one line per row in rows.txt, with its terminal status);
- APPLIED.tsv (one line per entry in ENTRIES.tsv: done, done-already, rejected or blocked with the rows left);
- NAME_FIXES.tsv (address, correct_symbol, evidence, wrong_spellings_seen) for proven fixes outside rows.txt;
- REJECTED.tsv, NOTES.md (mark descriptive names) and LESSONS.md;
- recheck_final.log: one vland run over every changed row.

Report briefly: rows ported, not-a-binder, blocked and deferred; entries done and blocked; header opt-ins; rule slips.
