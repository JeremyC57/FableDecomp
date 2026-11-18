# Name-fix agent NF68 (2026-11-17, round 7): finish engine-side ports NF63 and NF65 could not reach (18 rows)

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done" and "add another agent to
speed up naming queue completion". Round 7 has fixed about 1,250 rows. What is left of the queue is mostly rows that every earlier
agent marked `blocked/port`: their wrong binding comes from a local model of a class that has a shared header (a local CThing,
CPersistContext, CTCInventoryBase chain, CMap, CQuestManager, ...), and moving the whole row onto the shared headers was over the
10-grade budget of a naming pass. That port is now your job. NF63 and then NF65 had these rows: between them they ported 30 (landings nf63 and nf65), but both
ran out of time before reaching the rest. For each row here NF63 compiled one scripted draft and recorded its first error, and NF65
inspected some and wrote the exact opt-ins or header decisions they need. Read `$S/w22/agentNF65/STATUS.tsv`, its NOTES.md and
`$S/w22/agentNF63/STATUS.tsv` / NOTES.md. Their overlay headers are landed (last at ae79542f45). Several drafts stopped on a missing
retail-model macro (`FABLE_RETAIL_DEF_CLASS_BASE`, `FABLE_RETAIL_CDEFSTRINGTABLE`, `FABLE_RETAIL_NSCRIPT`): define it first. Three
drafts (0x00B76BC0, 0x00B7FA20, 0x00BB8090) stopped on the same CRGBFloatColour.h / C4DVector constructor error: solve it once.
0x00E12580 and 0x00EDA8C0 were NF60's (pGlobalScriptDef) rows, blocked on inlining differences; read NF60's notes. NF66 (game-side
ports) and NF67 (the engine-side rows below 0x00970000) run beside you.

**Lesson from NF62 and NF63: do not spread one draft over every row and stop.** Take the closest rows first and carry each to a
terminal grade (RELOCATION_MATCH, or 15 grades with the exact remaining error) before starting the next. A row with one draft grade
at hand-back is not finished.

## Your slice: 18 unfinished engine-side rows at or above 0x00970000
`$S/w22/agentNF68/BLOCKED.tsv` lists each row with the agent that blocked it and that agent's note (grade, entries, reason, and
usually the classes to port first). The full records are in `rebuild/candidates/manual/naming/2026-11-1?_agent<that agent>/`
(STATUS.tsv, APPLIED.tsv, NOTES.md, and their `work/attempts/` drafts where the note names one; those live in the agent's scratch
directory `$S/w22/agent<that agent>/work/`). `rows.txt` is the 18 rows. Authoring / identity rows and rows held by other agents
are not in it.

Per row:
1. Read the blocking note and the entry it serves (the queued correct symbol is in that agent's APPLIED.tsv / ENTRIES.tsv).
2. **Port the whole row onto the shared headers:** delete the local models of every class that has a header
   (`rebuild/include/engine/<Class>.h`, `stlport/*`, `fable_string.h`, `libpk/*`), include the headers with the macros they need,
   and add `#ifdef` opt-ins in your overlay for members the headers lack (declarations, or bodies only where retail inlines them).
   Local models remain only for classes with no shared header. The wrong binding must become the queued PDB spelling.
3. Grade until RELOCATION_MATCH / MATCH with the test PASS. Rows that share a class chain (the five CMap rows 0x0081FA20 / 0x0081FA90 / 0x008200B0 / 0x00820100 /
   0x00820140; the libpk CSoundEvent / CBankProcessor / CSegmentFile rows at 0x00C1..-0x00C4..; the CThing / CThingManager rows;
   the 17-19-class AI and combat rows NF53 listed) are one group: port the chain once and reuse it.
4. If the row's own self name is wrong too (the note says so), re-author it under the PDB spelling and put it in RENAMES section 1;
   its landed callers in rows.txt follow in the same pass, others go to NAME_FIXES.tsv.

**Budget for this slice: up to 15 grades per row** (not 10), because whole-row ports are the work. Start with the groups that
unblock the most rows and entries. A row that still fails after 15 grades stays `blocked/port` with the exact compiler error or
DIFFER, the forms you tried, and the class or header change that would unblock it. A blocker that needs a change to a header's
default path or to the root `fable_string.h` is `blocked/header` with the exact change proposed in NOTES.md (agents do not make
those changes; the coordinator does).

**Every row must end `ported`, `not-a-binder` or `blocked`.** `carry` is not allowed. For each entry a row serves, say in
APPLIED.tsv whether the entry is now done (all its binders ported or not-a-binder) or still blocked, and by which rows.

## Method: transformer first, then batch
NF44 ported 19 scratch-texture rows by proving the rewrite by hand on 2-3 rows and then applying it with a small script
(`$S/w22/agentNF44/work/scratch.py` is that script; read it).
1. Group rows that share a stand-in (for example the CResource list rows) and port them together.
2. Port 2-3 rows of the group by hand, using these steps:
   - include the header, adding an `#ifdef` opt-in in your overlay for any member it lacks;
   - delete the local model;
   - call the real member or template.
   Grade each row until it is RELOCATION_MATCH.
3. Turn the edit into a script, apply it to the rest of the group, and grade them all in one vland payload of 20-30 rows.
4. Fix the stragglers by hand.

## Terminal statuses (per ROW in STATUS.tsv, per entry in APPLIED.tsv)
- **Rows:** `ported` (graded, test PASS), `not-a-binder` (with reason), or `blocked`.
- **blocked** needs an exact reason, the grade count, and the model or header that must be ported first.
  - Example: 0x00B68610's local CRenderStateManager collides with engine/CRenderStateManager.h.
  - A row blocked with 0 grades is not accepted unless it is `authoring` or `library`.
- **Entries:**
  - `done` when every binder row of that entry is ported or not-a-binder;
  - otherwise `blocked` with the rows left and why.
- **Terminal rule for this slice:** the port IS the work, so spend up to 15 grades on it. Only after that (or when the blocker is
  a header default-path change) mark it blocked/port or blocked/header and name exactly what is missing.

## Also carry the lessons
These sections of `$S/w22/agentNF44/BRIEF.md` apply unchanged; read them:
- "Lessons of earlier passes";
- "Disk";
- "Round-3 / Round-2 / Round-1 lessons";
- "Per entry";
- "Hard rules";
- "Lane";
- "Hand-back".
NF44's own LESSONS.md and `$S/w22/agentNF41/LESSONS.md` are the newest.

The main lessons, repeated here:
- No new local model of a class that has a shared header. That includes renaming a stand-in to the real class name, and including
  a header under a renaming macro.
- No cast addresses.
- Overlay headers carry `#ifdef` opt-ins only. A body edit must be token-identical without the opt-in, and you must say so in
  hdr/README.md.
- Work only in `$S/w22/agentNF68/`, using absolute paths. Grade in your private `S=<your dir>/pS`. Never run git. Never write
  under /home/user/FableDecomp.
- Run one compile at a time, always inside `timeout`, with at most 15 grades per row in this slice. Every Bash call must exit 0.
- If you change a row that EH funclets include, regrade those funclets (see Round-3 lessons).
- Count before you report: every number comes from your files.

## Row claims
Claim each row before you change it:

    mkdir $S/w22/nflocks/<addr> 2>/dev/null && echo NF68 > $S/w22/nflocks/<addr>/owner; true

- NF66 and NF67 hold their rows as `NF66` / `NF67`; none is in your rows.txt. NF65's locks on your rows are released. If a lock you need is held by another agent,
  record the row `deferred: held by <owner>` and move on.
- Header overlays: NF66 and NF67 may add opt-ins to the same headers. Keep your macro names specific to the member
  (`FABLE_RETAIL_<CLASS>_<MEMBER>`), so the coordinator can merge both overlays.

## Hand-back (in $S/w22/agentNF68/)
- rows/;
- hdr/ and hdr/README.md;
- RENAMES.tsv (section 1 header line exactly as in NF44's brief, then `# Section 2`);
- STATUS.tsv (one line per row in rows.txt, with its terminal status);
- APPLIED.tsv (one line per entry the rows serve: done, or blocked with the rows left);
- NAME_FIXES.tsv (address, correct_symbol, evidence, wrong_spellings_seen) for proven fixes outside rows.txt;
- REJECTED.tsv, NOTES.md and LESSONS.md;
- recheck_final.log: one vland run over every changed row.

Report briefly:
- rows ported, not-a-binder, blocked and deferred;
- entries done and blocked;
- header opt-ins;
- rule slips.
