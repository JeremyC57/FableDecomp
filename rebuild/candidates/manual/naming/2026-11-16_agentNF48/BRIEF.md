# Name-fix agent NF48 (2026-11-16, round 7): STLport vector members reached through local stand-in containers

Owner: "Continue the naming fix queue with two agents until complete". Rounds 1-7 have respelled about 1,480 landed rows. The latest
landings are 8872d59ed4 (NF45) and b2ece459a7 (NF46). Read first:
- `rebuild/candidates/manual/naming/2026-11-16_agentNF41/NOTES.md` (your entries come from NF41's half);
- `rebuild/candidates/manual/naming/2026-11-16_agentNF42/NOTES.md` and `2026-11-16_agentNF44/NOTES.md` (STLport map / list
  ports onto stlport/fable_map.h and fable_containers.h, and their opt-ins);
- `rebuild/candidates/manual/batches/2026-11-16_landing_nf46/README.md` (a family-sliced round).

## Your slice is a FAMILY, not a tag range
Each agent in round 7 gets a closed family of binder rows. **Every row must end `ported`, `not-a-binder` or `blocked`.**
`carry` is not allowed inside the slice.

NF47 is running beside you on the game-component singletons and CBankFile lookups. The rows in `$S/w22/agentNF47/rows.txt` are its
rows, and none of them is in your worklist. Never touch an NF47 row; the lock files enforce this.

**Your 16 entries** (`ENTRIES.tsv`, NF41's APPLIED lines with its notes) are STLport `std::vector<T>` members:
- operator=: float, long, CDefString, CArenaRoundDef, CConversation and CFireHeartPatternDef;
- erase: CEnginePrimitiveHandle, CCharString, CScriptThing and EHeroAbility;
- resize: unsigned long, EHeroAbility and CCharString;
- ~vector<CScriptThing>, push_back<EHeroAbility> and reserve<const CThing*>.

The callers bind them through local stand-in containers such as `??4CFloatVector`, `??4COpinionSet`, `?Erase`, `?EraseRange`,
`??1CVector_008ac970` and `?Destroy`.

**The worklist is already probed; do NOT re-probe the whole half.** `WORKLIST.tsv` has 87 distinct rows, 93 lines, with columns
entry, target, correct_symbol, binder_row and bound_symbol_at_probe.
- It comes from NF41's start-of-round probe, minus every row landed since then and minus NF47's rows.
- Before editing a row, compile its repo copy once with `xrow.sh` to confirm it still binds the wrong spelling. If it no
  longer does, record it `not-a-binder` (already fixed).

**How to port:**
- Retail is STLport 4.6.2. The repo models it in `rebuild/include/stlport/` (fable_containers.h, fable_map.h, ...), with opt-ins
  for out-of-line members. Both PDBs are Dinkumware builds, so these names are descriptive-by-library: say so in NOTES.
- Never invent template arguments the callers do not prove.
- **Verify each claim first.** Read the callee body (STLport `operator=` / `erase` / `resize` shapes) and check that the element
  type matches what the callers store. A queued claim that does not hold goes to REJECTED.tsv.
- **Then port each caller:**
  1. Replace the local container model with `std::vector<T>` from the shared STLport header.
  2. Use the header's opt-in so the member is declared, not defined inline, where retail calls it out of line.
  3. Grade.

  A local model that other code in the row also depends on (a whole local namespace std, local allocators) is a port. Do it
  within the 10-grade budget, or mark the row blocked/port and name the model.
- **Overlaying the STLport headers:** NF41 overlaid the root `#pragma once` fable_string.h and broke other TUs. Overlay
  `stlport/*.h` the same way as engine headers (a full copy plus `#ifdef` opt-ins), and recheck one unrelated includer before you
  hand back.
- **Classes:** where a stand-in class wraps the vector as a member of a real class (for example `COpinionSet`), use that class's
  engine header if it has one.

## Method: transformer first, then batch
NF44 ported 19 scratch-texture rows by proving the rewrite by hand on 2-3 rows and then applying it with a small script
(`$S/w22/agentNF44/work/scratch.py` is that script; read it).
1. Group the rows by stand-in shape. Start with the 23 e119 `vector<float>::operator=` rows (`CFloatVector` stand-in).
2. Port 2-3 rows of the group by hand, using these steps:
   - include the header, adding an `#ifdef` opt-in in your overlay for any member it lacks;
   - delete the local model;
   - call the real STLport member.
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
- **Terminal rule:** if a row needs a whole other class ported first (more than 10 grades), mark it blocked/port and name that
  class. It goes to a port list, so do not sink the budget into it.

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
- Work only in `$S/w22/agentNF48/`, using absolute paths. Grade in your private `S=<your dir>/pS`. Never run git. Never write
  under /home/user/FableDecomp.
- Run one compile at a time, always inside `timeout`, with at most 10 grades per row. Every Bash call must exit 0.
- If you change a row that EH funclets include, regrade those funclets (see Round-3 lessons).
- Count before you report: every number comes from your files.

## Row claims
Claim each row before you change it:

    mkdir $S/w22/nflocks/<addr> 2>/dev/null && echo NF48 > $S/w22/nflocks/<addr>/owner; true

- NF47 holds its rows as `NF47`. If a lock you need is NF47's, record the row `deferred: held by NF47` and move on.
- Header overlays: NF47 may add opt-ins to engine headers. Keep your
  macro names specific to the member (`FABLE_RETAIL_<CLASS>_<MEMBER>`), so the coordinator can merge both overlays.

## Hand-back (in $S/w22/agentNF48/)
- rows/;
- hdr/ and hdr/README.md;
- RENAMES.tsv (section 1 header line exactly as in NF44's brief, then `# Section 2`);
- STATUS.tsv (one line per worklist row, with its terminal status);
- APPLIED.tsv (the five entries, plus any extra queue lines you closed);
- REJECTED.tsv, NOTES.md and LESSONS.md;
- recheck_final.log: one vland run over every changed row.

Report briefly:
- rows ported, not-a-binder, blocked and deferred;
- entries done and blocked;
- header opt-ins;
- rule slips.
