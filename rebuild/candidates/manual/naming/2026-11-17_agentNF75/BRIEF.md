# Name-fix agent NF75 (2026-11-17, round 7): the follow-up entries queued at nf70-nf73 and the assert audit (35 entries, 73 rows)

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done" and "add another agent to
speed up naming queue completion". Round 7 has fixed about 1,900 rows. NF70-NF73 landed (last at b636ade3e9). NF74 (seven blocked
engine ports) runs beside you. Read first:
- `rebuild/candidates/manual/batches/2026-11-17_landing_nf70/README.md`, `_nf71/`, `_nf72/`, `_nf73/` and
  `2026-11-17_assert_pin_audit/README.md`;
- `rebuild/candidates/manual/naming/2026-11-17_agentNF70/` .. `_agentNF73/`: NOTES.md, LESSONS.md, hdr/README.md;
- `docs/journal/2026-11/EGOD_FINDINGS.md` (retail has no identical-code folding and the linker keeps one copy per decorated name: two
  identical bodies at two addresses are always two different names).

## Your slice
Files in `$S/w22/agentNF75/`:
- `ENTRIES.tsv`: `rebuild/candidates/manual/naming/NAME_FIXES_QUEUE.tsv` lines q10910..q10944 (tag `land1117fu`; `q<n>` is the
  0-based line). 35 entries.
- `WORKLIST.tsv`: landed rows that bind an entry address under another spelling (reloc_bindings.tsv at b636ade3e9). 45 lines.
- `rows.txt`: the 73 rows you may change, all locked as NF75. 0x006C9B70 is held by NF74 (HELD.txt): record it
  `deferred: held by NF74`.

Entries that are more than a rename (read their queue lines):
- **0x00596CCE / 0x00597006** (NF73's held rows, drafts in `batches/2026-11-17_landing_nf73/held/`): both match, but bind
  `CCountedPointer<NUISystem::CList>::ReleaseData` / `ShareData` at 0x0059A994 / 0x0059AA86, while landed rows bind those names at
  0x00429288 / 0x00429486. Find out what 0x0059A994 and 0x0059AA86 really are (which `CCountedPointer<T>` instance, or which member:
  callers, the TU's neighbours, the type each caller passes, Ego_d's object file in `ghidra_out/egod_carved_functions.tsv`), fix the
  catalog name of whichever is wrong (RENAMES section 1), then land the two rows with bindings that put one name at one address.
  Their NAME_FIXES callers (0x00597B20 / 0x0059A238, the IAE self spelling) follow.
- **0x006C4430** (held at nf72, draft in `batches/2026-11-17_landing_nf72/held/`): reach FadingThings without the offset cast (a
  `friend` opt-in in engine/CTCFadeOutAndIn.h, or another honest form).
- **0x008F70F0** CMap::AddThemeDefIndexToPalette: make the member protected in engine/CMap.h's `FABLE_RETAIL_CMAP_THEMEPALETTE` (an
  opt-in edit; the default path must stay token-identical), so the row defines the PDB's `IAEEK`; regrade it and its callers.
- **0x00429950** (q10763, `CBasicString<char>::operator<` one definition): needs the root fable_string.h; agents do not edit it.
  Record it `blocked/header` with the exact change (NF70's is in `batches/2026-11-17_landing_nf70/held/q10763_change.txt`).

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
to a terminal grade before starting the next. **Budget:** up to 10 grades per row; after 3 failed forms on one row mark it blocked
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
`$S/w22/agentNF72/LESSONS.md`, `$S/w22/agentNF73/LESSONS.md`, `$S/w22/agentNF70/LESSONS.md`. The tool scripts in
`$S/w22/agentNF72/` and `$S/w22/agentNF73/` (g.sh, vl.sh, pay.py, fpay.py, st.py, claim.sh) are a working set: copy them and
repoint them to your directory.

The main lessons, repeated here:
- No new local model of a class that has a shared header (that includes renaming a stand-in to the real class name, and including a
  header under a renaming macro). No cast addresses. No `__asm`, `naked`, `_emit`, and no `#error` probes in rows.
- Overlay headers carry `#ifdef` opt-ins only, with macro names specific to the member (`FABLE_RETAIL_<CLASS>_<MEMBER>`); a body
  edit must be token-identical without the opt-in, and you must say so in hdr/README.md. NF74 may add opt-ins to the same
  headers.
- Work only in `$S/w22/agentNF75/`, using absolute paths everywhere (a relative vland log path once wrote into the repo's `work/`).
  Grade in your private `S=<your dir>/pS`. Never run git (not even `git status`). Never write under /home/user/FableDecomp.
- Run one compile at a time, always inside `timeout`. Every Bash call must exit 0 (end `grep -c` / `diff` loops with `; true`).
- Write any file or note argument that contains backticks with a quoted heredoc (`<<'EOF'`), never inside double quotes.
- If you change a row that EH funclets include, regrade those funclets.
- Count before you report: every number comes from your files.
- Claim any extra row before changing it: `mkdir $S/w22/nflocks/<addr> 2>/dev/null && echo NF75 > $S/w22/nflocks/<addr>/owner; true`.
  If it is held by another agent, record it `deferred: held by <owner>`.

## Hand-back (in $S/w22/agentNF75/)
- rows/ (finished rows only; keep blocked drafts in work/); hdr/ and hdr/README.md;
- RENAMES.tsv (section 1 header line exactly as in NF44's brief, then `# Section 2`);
- STATUS.tsv (one line per row in rows.txt, with its terminal status);
- APPLIED.tsv (one line per entry in ENTRIES.tsv: done, done-already, rejected or blocked with the rows left);
- NAME_FIXES.tsv (address, correct_symbol, evidence, wrong_spellings_seen) for proven fixes outside rows.txt;
- REJECTED.tsv, NOTES.md (mark descriptive names) and LESSONS.md;
- recheck_final.log: one vland run over every changed row.

Report briefly: rows ported, not-a-binder, blocked and deferred; entries done and blocked; header opt-ins; rule slips.
