# Name-fix agent NF47 (2026-11-16, round 7): game-component singletons and CBankFile::FindIndexBySymbolCRC

Owner: "Continue the naming fix queue with two agents until complete". Rounds 1-7 have respelled about 1,360 landed rows; the
latest landings are b9dacad520 (NF41) and 8872d59ed4 (NF45). Read first:
- `rebuild/candidates/manual/naming/2026-11-16_agentNF41/NOTES.md` (your entries come from NF41's half);
- `rebuild/candidates/manual/batches/2026-11-16_landing_nf45/README.md` (the first family-sliced round).

## Your slice is a FAMILY, not a tag range
Each agent in round 7 gets closed families of binder rows. **Every row must end `ported`, `not-a-binder` or `blocked`.**
`carry` is not allowed inside the slice.

NF46 is running beside you on the NEngineInternal engine-data globals. The 84 rows in `$S/w22/agentNF46/rows.txt` are its rows, and
none of them is in your worklist. Never touch an NF46 row; the lock files enforce this.

**Your three entries** (`ENTRIES.tsv`, NF41's APPLIED lines with its notes):

| Retail | Correct symbol | Entry | Wrong rows |
|---|---|---|---|
| 0x013B86A0 | `?PMainGameComponent@CMainGameComponent@@1PAV1@A` (protected static; inline `CMainGameComponent::Get()`, engine/CMainGameComponent.h:219) | e18 | 34 |
| 0x013B871C | `?m_pNewFrontendGameComponent@CNewFrontendGameComponent@@0PAV1@A` (private static; inline `Get()`, engine/CNewFrontendGameComponent.h:85) | e19 | 6 |
| 0x009CC410 | `?FindIndexBySymbolCRC@CBankFile@@QBEKK@Z` (Ego_r.pdb 0x00B1A5C0, FableWin.pdb 0x02F978C0; engine/CBankFile.h:226 opt-in) | e74 | 23 |

**The worklist is already probed; do NOT re-probe the whole half.** `WORKLIST.tsv` has 57 distinct rows, 63 lines, with columns
entry, target, correct_symbol, binder_row and bound_symbol_at_probe.
- It comes from NF41's start-of-round probe (`$S/w22/agentNF41/work/wrong.tsv`), minus every row landed since then (NF41's own 61
  and NF45's 47) and minus NF46's rows.
- Before editing a row, compile its repo copy once with `xrow.sh` to confirm it still binds the wrong spelling. If it no
  longer does, record it `not-a-binder` (already fixed) with the commit that fixed it, if you can tell.

**How to port each family:**
- **e18 / e19 (singletons):** each row models the singleton under a made-up struct or global name (`g_A`,
  `g_pEngine_013b86a0`, `g_13b871c` ...). Include engine/CMainGameComponent.h / CNewFrontendGameComponent.h, read the member
  through `CMainGameComponent::Get()` (or the static), and map every local offset to the real member using the header and
  ghidra_out/struct_layouts_egor.tsv.
  - Rows that also model other classes locally may collide with the header. Port what you can within the budget.
  - Otherwise mark the row blocked/port and name the colliding model.
  - NF41 already moved 0x004083E0 onto the header in 2 grades; read it as the pattern.
- **e74 (CBankFile::FindIndexBySymbolCRC):** 17 rows bind `?LookupValue@CDataBank@NGameText@@QBEKK@Z`, and others bind
  `CTagTable::GetTagFromCRC`, `CSpeechMap::LookupValue`, `CTextBank::LookupValue`, `CBankFileAsync::LookupValue` and similar.
  **Verify the claim first:**
  1. Read the body at 0x009CC410 and check whether those classes are CBankFile subclasses or members in the PDB layouts.
  2. If a caller really calls a DIFFERENT PDB function whose body is byte-identical (an ICF fold), that caller keeps its own true
     name. Folding gives one retail address, but the source called what it called. Record it `not-a-binder: ICF, true callee X`
     with the PDB evidence.
  3. Only callers whose PDB-proven callee is CBankFile::FindIndexBySymbolCRC (reached through a CBankFile base or member) are
     respelled.

## Method: transformer first, then batch
NF44 ported 19 scratch-texture rows by proving the rewrite by hand on 2-3 rows and then applying it with a small script
(`$S/w22/agentNF44/work/scratch.py` is that script; read it).
1. Group the rows by stand-in shape. Start with the e18 singleton rows.
2. Port 2-3 rows of the group by hand, using these steps:
   - include the header, adding an `#ifdef` opt-in in your overlay for any member it lacks;
   - delete the local model;
   - read the real static or call the real member.
   Grade each row until it is RELOCATION_MATCH.
3. Turn the edit into a script, apply it to the rest of the group, and grade them all in one vland payload of 20-30 rows.
4. Fix the stragglers by hand.

## Terminal statuses (per ROW in STATUS.tsv, per entry in APPLIED.tsv)
- **Rows:** `ported` (graded, test PASS), `not-a-binder` (with reason), or `blocked`.
- **blocked** needs an exact reason, the grade count, and the model or header that must be ported first.
  - Example: 0x00B68610's local CRenderStateManager collides with engine/CRenderStateManager.h.
  - A row blocked with 0 grades is not accepted unless it is `authoring` or `library`.
- **Entries:**
  - `done` when every binder row of that global is ported or not-a-binder;
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
- Work only in `$S/w22/agentNF47/`, using absolute paths. Grade in your private `S=<your dir>/pS`. Never run git. Never write
  under /home/user/FableDecomp.
- Run one compile at a time, always inside `timeout`, with at most 10 grades per row. Every Bash call must exit 0.
- If you change a row that EH funclets include, regrade those funclets (see Round-3 lessons).
- Count before you report: every number comes from your files.

## Row claims
Claim each row before you change it:

    mkdir $S/w22/nflocks/<addr> 2>/dev/null && echo NF47 > $S/w22/nflocks/<addr>/owner; true

- NF46 holds its rows as `NF46`. If a lock you need is NF46's, record the row `deferred: held by NF46` and move on.
- Header overlays: NF46 may add opt-ins to the same headers, such as engine/CEngine.h. Keep your
  macro names specific to the member (`FABLE_RETAIL_<CLASS>_<MEMBER>`), so the coordinator can merge both overlays.

## Hand-back (in $S/w22/agentNF47/)
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
