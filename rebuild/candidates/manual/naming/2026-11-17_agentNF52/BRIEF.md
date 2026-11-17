# Name-fix agent NF52 (2026-11-17, round 7): class statics, singletons, vftables and CBasicString<char>

Owner: "Continue the naming fix queue with two agents until complete". Round 7 has fixed about 460 caller rows so far. The latest
landings are 2f6e1d5705 (NF49) and 8ea6b873eb (NF50). Read first:
- `rebuild/candidates/manual/batches/2026-11-16_landing_nf47/README.md`. NF47 ported the CMainGameComponent static family, and its
  data-symbol pattern is the model for yours;
- `rebuild/candidates/manual/batches/2026-11-17_landing_nf50/README.md`.

## Your slice is a FAMILY, not a tag range
Each agent in round 7 gets a closed family of binder rows. **Every row must end `ported`, `not-a-binder` or `blocked`.**
`carry` is not allowed inside the slice.

NF51 is running beside you on the CAIStateGroupBase AI actions, NUISystem and NPlayerGui. The rows in `$S/w22/agentNF51/rows.txt`
are its rows, and none of them is in your worklist. Never touch an NF51 row; the lock files enforce this.

**Your 14 entries** are in `ENTRIES.tsv`: source half, entry, address, correct symbol, status and the previous agent's note.

**Data symbols.** Binders declare these globals under made-up names or types; respell each one through the shared header's
declaration.
- `CShaderRenderManager::SingletonInstance` (11 rows);
- `CMainGameComponent::EventDispatch` (7);
- `COpinionReactionManager::AttitudeStrings` (7);
- `g_pScriptRunner` (6);
- `CTBaseSingleton<CSkeletalMorphResourceManager>::PClass`;
- `CCreatureActionBase::RegisteredActions`;
- `CWorldMap::LineCollisionAround4Square`;
- `CEngineInternalPrimitiveBase::CurrentInterpolationInfo`;
- the `CSegmentList` and `CTCInventoryMagic` vftables.

**Functions:**
- `CBasicString<char>::Compare` (static fastcall, 10 rows) and `CBasicString<char>::operator==(const char*) const` (3 rows), through
  `fable_string.h`;
- `CTCTavernGame::PositionMesh`;
- `CEnginePrimitiveManagerStaticMeshesBase::AddStaticMesh`.

**Verify each claim first:** PDB name sets, the data address, the access and storage class encoded in the mangling (0 = private,
1 = protected, 2 = public static; 3 = global), and constness.
- **Vftable binders.** Rows that construct or test an object by storing its vftable address need the class's real vftable symbol.
  That comes from declaring the class with its real virtuals from the shared header, never from a cast address.
- **`fable_string.h`.** It is a root `#pragma once` header, and NF41 broke other TUs by overlaying it. Do NOT overlay it. If a
  needed declaration is missing there, mark the row blocked with the exact declaration needed.

**The worklist is already probed; do NOT re-probe the whole half.** `WORKLIST.tsv` has 60 distinct rows, 61 lines, with columns
entry, target, correct_symbol, binder_row and bound_symbol_at_probe.
- It comes from NF41's probe and NF44's APPLIED binder lists, minus every row landed since and minus NF51's rows.
- Before editing a row, compile its repo copy once with `xrow.sh` to confirm it still binds the wrong spelling. If it no
  longer does, record it `not-a-binder`.
- NF44-half lines have an empty bound_symbol column. Read the binding from your confirming compile.

**Port method:** include the shared header, delete the local model or extern, and grade.
- Do not burn the budget: after 3 failed forms on one row, mark it blocked with the exact compiler error or diff.
- Rows that model a dozen classes locally are ports: do them within 10 grades, or mark them blocked/port and name the classes.

**Containers restart without warning (four times in the last day).** Write STATUS.tsv and APPLIED.tsv after EVERY row. The first
grade after a restart can time out while wineserver starts; retry once.

## Method: transformer first, then batch
NF44 ported 19 scratch-texture rows by proving the rewrite by hand on 2-3 rows and then applying it with a small script
(`$S/w22/agentNF44/work/scratch.py` is that script; read it).
1. Group the rows by stand-in shape. Start with the 11 SingletonInstance rows.
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
- Work only in `$S/w22/agentNF52/`, using absolute paths. Grade in your private `S=<your dir>/pS`. Never run git. Never write
  under /home/user/FableDecomp.
- Run one compile at a time, always inside `timeout`, with at most 10 grades per row. Every Bash call must exit 0.
- If you change a row that EH funclets include, regrade those funclets (see Round-3 lessons).
- Count before you report: every number comes from your files.

## Row claims
Claim each row before you change it:

    mkdir $S/w22/nflocks/<addr> 2>/dev/null && echo NF52 > $S/w22/nflocks/<addr>/owner; true

- NF51 holds its rows as `NF51`. If a lock you need is NF51's, record the row `deferred: held by NF51` and move on.
- Header overlays: NF51 may add opt-ins to engine headers (CAIStateGroupBase.h, NUISystem_*.h). Keep your
  macro names specific to the member (`FABLE_RETAIL_<CLASS>_<MEMBER>`), so the coordinator can merge both overlays.

## Hand-back (in $S/w22/agentNF52/)
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
