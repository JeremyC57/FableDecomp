# Name-fix agent NF88 (2026-11-17, round 7): the nf86 follow-ups (13 entries, q11150..q11162) and one definition for three shared bodies

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done", then "Decrease to 2 agents
at a time once next agent finishes". NF86 landed at b090df8c93. NF87 runs beside you. Read first:
- `rebuild/candidates/manual/batches/2026-11-17_landing_nf86/README.md`, `_nf80/README.md` ("Open: CTCBase::AllocateTC"), `_nf81/README.md` (q10988);
- `rebuild/candidates/manual/naming/2026-11-17_agentNF86/`: NOTES.md, LESSONS.md, STATUS.tsv, hdr/README.md;
- `docs/pipeline/GOTCHAS.md`: the nf80 and nf86 lines (one definition; the `// Fidelity: scaffolding` pick-any pattern).

## Your slice
Files in `$S/w22/agentNF88/`:
- `ENTRIES.tsv`: queue lines q11150..q11162, 13 entries. `WORKLIST.tsv`: 14 lines from reloc_bindings.tsv at b090df8c93. `rows.txt`: 35 rows
  locked as NF88.
- q11150 (0x01436E9C) is a Ghidra label fix only: put the label line in NAME_FIXES.tsv.
- q11153..q11159 (the NInventory CTCInventoryBase / CCategory wrappers and their find_if): NF86 proved the names; porting needs the PASS22
  CCategory / NInventory_CTCInventoryBase.h header chain fixed first (NF86 STATUS). Fix it with opt-in additions only, then port.
- q11161: 0x00664670 is `CThingCreatureBase::SetCurrentAction(const CCountedPointer<CCreatureActionBase>&)` (Ego_r and FableWin); its catalog
  name `_Cons_val` is wrong.
- **q11162, one definition (coordinator ruling):** three owning rows define a header body as a strong copy through a `_DEFINE` macro while
  includers get the inline copy, which clashes at whole-image link (LNK2005): 0x00686F10 (`FABLE_RETAIL_CTCBASE_ALLOCATETC_DEFINE`),
  0x008C1600 (GetValidRushPosition's `_DEFINE`, header engine/CCombatAction_StrikeResponseJackOfBladesIntoRush_GetValidRushPosition.h) and
  0x0081F910 (CMap `GETSTRONGESTENGINETHEMEAT_DEFINE`). Re-author each owning row so it no longer defines `_DEFINE`: it includes the body
  inline like every includer and makes VC7.1 emit it as a pick-any COMDAT through an anonymous-namespace reference marked
  `// Fidelity: scaffolding, not retail` (the pattern of 0x00860150). Leave the `_DEFINE` macros in the headers (unused afterwards). Then
  prove one definition: link the owning row's object with one includer's object (0x00686FA0, 0x008C2550, a CMap includer) with `LINK=`
  cleared, and say so in NOTES. If a row stops matching that way, keep its landed form and mark it blocked with the diff.

Do the entries in queue order, q11162 last.

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
`$S/w22/agentNF86/LESSONS.md`, `$S/w22/agentNF85/LESSONS.md`, `$S/w22/agentNF84/LESSONS.md`. The tool scripts in
`$S/w22/agentNF86/` and `$S/w22/agentNF85/` (g.sh, vl.sh, pay.py, fpay.py, st.py, claim.sh) are a working set: copy them and
repoint them to your directory.

The main lessons, repeated here:
- No new local model of a class that has a shared header (that includes renaming a stand-in to the real class name, and including a
  header under a renaming macro). No cast addresses. No `__asm`, `naked`, `_emit`, and no `#error` probes in rows.
- Overlay headers carry `#ifdef` opt-ins only, with macro names specific to the member (`FABLE_RETAIL_<CLASS>_<MEMBER>`); a body
  edit must be token-identical without the opt-in, and you must say so in hdr/README.md. NF87 may add opt-ins to the same
  headers.
- Work only in `$S/w22/agentNF88/`, using absolute paths everywhere (a relative vland log path once wrote into the repo's `work/`).
  Grade in your private `S=<your dir>/pS`. Never run git (not even `git status`). Never write under /home/user/FableDecomp.
- Run one compile at a time, always inside `timeout`. Every Bash call must exit 0 (end `grep -c` / `diff` loops with `; true`).
- Write any file or note argument that contains backticks with a quoted heredoc (`<<'EOF'`), never inside double quotes.
- If you change a row that EH funclets include, regrade those funclets.
- Count before you report: every number comes from your files.
- Claim any extra row before changing it: `mkdir $S/w22/nflocks/<addr> 2>/dev/null && echo NF88 > $S/w22/nflocks/<addr>/owner; true`.
  If it is held by another agent, record it `deferred: held by <owner>`.

## Hand-back (in $S/w22/agentNF88/)
- rows/ (finished rows only; keep blocked drafts in work/); hdr/ and hdr/README.md;
- RENAMES.tsv (section 1 header line exactly as in NF44's brief, then `# Section 2`);
- STATUS.tsv (one line per row in rows.txt, with its terminal status);
- APPLIED.tsv (one line per entry in ENTRIES.tsv: done, done-already, rejected or blocked with the rows left);
- NAME_FIXES.tsv (address, correct_symbol, evidence, wrong_spellings_seen) for proven fixes outside rows.txt;
- REJECTED.tsv, NOTES.md (mark descriptive names) and LESSONS.md;
- recheck_final.log: one vland run over every changed row.

Report briefly: rows ported, not-a-binder, blocked and deferred; entries done and blocked; header opt-ins; rule slips.
