# Name-fix agent NF74 (2026-11-17, round 7): finish seven engine-side ports NF68, NF70 and NF71 left blocked

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done" and "add another agent to
speed up naming queue completion". Round 7 has fixed about 1,700 rows. What is left is mostly rows whose wrong binding comes from a
local model of a class that has a shared header; moving the whole row onto the shared headers is the work. NF66, NF67 and NF68
each ran out of budget before finishing these. NF72 (follow-up entries) runs beside you.

Read first:
- the landing READMEs `rebuild/candidates/manual/batches/2026-11-17_landing_nf66/`, `_nf67/`, `_nf68/`;
- each agent's records `rebuild/candidates/manual/naming/2026-11-17_agentNF66/` (and NF67, NF68): STATUS.tsv, APPLIED.tsv, NOTES.md,
  LESSONS.md, hdr/README.md. Their drafts are in their scratch dirs `$S/w22/agent<NFxx>/work/`;
- `docs/journal/2026-11/EGOD_FINDINGS.md` and `rebuild/candidates/manual/pending/2026-11-17_egod_parked/lane/egod_locals.py`: Ego_d.pdb
  (at `$S/dl_debug/files/Ego_d.pdb`, read it only with `python3 -I`) has the TLC source's named locals, parameter names and scope
  blocks for most functions, and `ghidra_out/egod_carved_functions.tsv` gives each function's object file. Use them when a row's
  residue is a register or stack-slot tie: the real named locals are the known lever.

## Your slice
`$S/w22/agentNF74/BLOCKED.tsv` lists each row with the agent that blocked it and its STATUS line; `rows.txt` is the 7 rows (all locked
as NF74). NF71 had six of them but gave none a grade; its notes are in `rebuild/candidates/manual/naming/2026-11-17_agentNF71/`
(STATUS.tsv, NOTES.md) and NF68's in `naming/2026-11-17_agentNF68/`.
- 0x008C2550, 0x008DDAD0, 0x008E0060, 0x008F3630: NF63's drafts still model classes locally and keep fake stand-in bodies, so they need
  a fresh port onto the shared headers, not a missing define. 0x008F3630 serves nf41:e110 (GetSubDef<CSpecialEffectsDef> 0x00669580).
- 0x00B9E950, 0x00BD1F70 (NF68): the row classes have no shared header, and the bodies use local-model member names the shared headers
  spell differently. Write the class headers the rows need (new `rebuild/include/engine/<Class>.h` files in your overlay, laid out
  from `ghidra_out/struct_layouts_egor.tsv` with the PDB member names), then port the rows onto them.
- 0x006C9B70 (NF70, entry q10747): NF70 matched it with a new local `std::_Nonconst_traits`; the stlport headers define it. Port the
  row onto `stlport/fable_containers.h` / `fable_map.h` instead (opt-ins for what they lack). NF70's draft is in
  `rebuild/candidates/manual/batches/2026-11-17_landing_nf70/held/`.

**Seven rows only, so each can be finished. Closest first; spend the budget on each before the next.** A hand-back with rows at 0
grades is not accepted for this slice.

**Static asserts:** never move a `FABLE_STATIC_ASSERT` to another line (its typedef is named after `__LINE__`, and VC7.1's stack-slot
order can depend on it). After editing an overlay header run
`python3 /home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/lane_merge/pin_asserts.py /home/user/FableDecomp/rebuild/include <your dir>/hdr <header>`
and say in hdr/README.md what it pinned.

Per row:
1. Read the blocking note and the entry it serves (the queued correct symbol is in that agent's APPLIED.tsv).
2. **Port the whole row onto the shared headers:** delete the local models of every class that has a header
   (`rebuild/include/engine/<Class>.h`, `stlport/*`, `fable_string.h`, `libpk/*`), include the headers with the macros they need, and
   add `#ifdef` opt-ins in your overlay for members the headers lack. Local models remain only for classes with no shared header.
   The wrong binding must become the queued PDB spelling.
3. Grade until RELOCATION_MATCH / MATCH with the test PASS: **up to 15 grades per row.** Take the closest rows first and carry each
   to a terminal grade (RELOCATION_MATCH, or 15 grades with the exact remaining error) before starting the next. A row with one
   draft grade at hand-back is not finished.
4. If the row's own self name is wrong too, re-author it under the PDB spelling and put it in RENAMES section 1; its landed callers
   in rows.txt follow in the same pass, others go to NAME_FIXES.tsv.

A blocker that needs a change to a header's default path or to the root `fable_string.h` is `blocked/header`, with the exact change
proposed in NOTES.md (agents do not make those changes; the coordinator does). **Every row must end `ported`, `not-a-binder` or
`blocked`;** `carry` is not allowed. For each entry a row serves, say in APPLIED.tsv whether the entry is now done or still blocked,
and by which rows.

## Also carry the lessons
These sections of `$S/w22/agentNF44/BRIEF.md` apply unchanged; read them: "Lessons of earlier passes", "Disk", "Round-3 / Round-2 /
Round-1 lessons", "Per entry", "Hard rules", "Lane", "Hand-back". Then the newest LESSONS.md files: `$S/w22/agentNF73/LESSONS.md`,
`$S/w22/agentNF71/LESSONS.md`, `$S/w22/agentNF68/LESSONS.md`. The tool scripts in `$S/w22/agentNF68/` (g.sh, vl.sh, pay.py, fpay.py,
st.py, claim.sh) are a working set: copy them and repoint them to your directory.

The main lessons, repeated here:
- No new local model of a class that has a shared header (that includes renaming a stand-in to the real class name, and including a
  header under a renaming macro). No cast addresses. No `__asm`, `naked`, `_emit`, and no `#error` probes in rows.
- Overlay headers carry `#ifdef` opt-ins only, with macro names specific to the member (`FABLE_RETAIL_<CLASS>_<MEMBER>`); a body edit
  must be token-identical without the opt-in, and you must say so in hdr/README.md. NF72 may add opt-ins to the same headers. Copy a
  header into your overlay from the repo at the moment you first change it.
- Work only in `$S/w22/agentNF74/`, using absolute paths everywhere (relative vland log paths have written into the repo's `work/`
  twice: always set the log path and `LANDVERIFY_WORK` to your directory). Grade in your private `S=<your dir>/pS`. Never run git
  (not even `git status`). Never write under /home/user/FableDecomp.
- Run one compile at a time, always inside `timeout`. Every Bash call must exit 0 (end `grep -c` / `diff` loops with `; true`).
- Write any file or note argument that contains backticks with a quoted heredoc (`<<'EOF'`), never inside double quotes.
- Keep blocked drafts in work/, never in rows/. If you change a row that EH funclets include, regrade those funclets.
- Count before you report: every number comes from your files.
- Claim any extra row before changing it: `mkdir $S/w22/nflocks/<addr> 2>/dev/null && echo NF74 > $S/w22/nflocks/<addr>/owner; true`.
  If it is held by another agent, record it `deferred: held by <owner>`.

## Hand-back (in $S/w22/agentNF74/)
- rows/; hdr/ and hdr/README.md;
- RENAMES.tsv (section 1 header line exactly as in NF44's brief, then `# Section 2`);
- STATUS.tsv (one line per row in rows.txt, with its terminal status);
- APPLIED.tsv (one line per entry the rows serve: done, or blocked with the rows left);
- NAME_FIXES.tsv (address, correct_symbol, evidence, wrong_spellings_seen) for proven fixes outside rows.txt;
- REJECTED.tsv, NOTES.md and LESSONS.md;
- recheck_final.log: one vland run over every changed row.

Report briefly: rows ported, not-a-binder, blocked and deferred; entries done and blocked; header opt-ins; rule slips.
