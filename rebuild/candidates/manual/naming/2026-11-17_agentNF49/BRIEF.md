# Name-fix agent NF49 (2026-11-17, round 7): definition-manager templates and the special-effects callees

Owner: "Continue the naming fix queue with two agents until complete". Rounds 1-7 have respelled about 1,600 landed rows. The latest
landings are b2ece459a7 (NF46) and 69ed47f1aa (NF47). Read first:
- `rebuild/candidates/manual/batches/2026-11-16_landing_nf47/README.md`, a family-sliced round;
- `rebuild/candidates/manual/naming/2026-11-16_agentNF47/NOTES.md`.

## Your slice is a FAMILY, not a tag range
Each agent in round 7 gets a closed family of binder rows. **Every row must end `ported`, `not-a-binder` or `blocked`.**
`carry` is not allowed inside the slice.

NF48 is running beside you on the STLport vector members. The rows in `$S/w22/agentNF48/rows.txt` are its rows, and none of them is
in your worklist. Never touch an NF48 row; the lock files enforce this.

**Your 12 entries** are in `ENTRIES.tsv`: source half, entry, address, correct symbol, status and the previous agent's note.

**Definition-manager templates:**
- `CDefinitionManager::GetDef<T>` for CEngineThemeDef, CMessageEventDef, CCameraManagerSetDef and CCameraModeDef;
- `CDefClassBase::GetSubDef<T>` for CSpecialEffectsDef, CTavernGameSpotTheAdditionDef, CTavernGameCoinGolfDef and
  CTavernGameShoveHaPennyDef;
- `CAIStateGroupBase::PeekDefinitionManager` (protected const). It is two queue lines, nf41 e77 and nf44 12, for the same
  address.

**Special effects:**
- `CTCSpecialAbilities::CreateParticleEffect` (static, `SI`);
- `CSpecialEffectsDef::GetEffectFromString`.

The headers exist:
- engine/CDefinitionManager.h, CGameDefinitionManager.h, CDefClassBase.h, CDefClassBase_GetSubDef.h and CDefPointer.h;
- engine/CAIStateGroupBase.h, CSpecialEffectsDef.h and CTCSpecialAbilities.h.

Read their existing opt-ins before you add any. Earlier rounds landed the GetDef / GetSubDef explicit-instantiation pattern; grep
landed rows for `GetSubDef<` and `GetDef<` to find it.

**The worklist is already probed; do NOT re-probe the whole half.** `WORKLIST.tsv` has 54 distinct rows, 67 lines, with columns
entry, target, correct_symbol, binder_row and bound_symbol_at_probe.
- It comes from NF41's start-of-round probe and NF44's APPLIED binder lists, minus every row landed since and minus NF48's rows.
- Before editing a row, compile its repo copy once with `xrow.sh` to confirm it still binds the wrong spelling. If it no
  longer does, record it `not-a-binder` (already fixed).
- NF44-half lines have an empty bound_symbol column. Read the binding from your confirming compile.

**How to port:**
- **Verify each claim first:** PDB name sets, the callee body, and which template argument the caller's def type proves.
- **Typical stand-ins:**
  - local `GetSubDef` / `GetDef` members on a local CDefClassBase / CDefinitionManager model;
  - `?GetPDefinitionManager` stand-ins for PeekDefinitionManager;
  - local `CreateParticleEffect` / `Lookup` declarations.
- **Port each caller:**
  1. Include the shared header.
  2. Call the real template member (`GetDef<T>` / `GetSubDef<T>`).
  3. Delete the local model.
  4. Grade.

  VC7.1 quirk: NF42 found that some dependent-return templates reject explicit instantiation with C2785. If the instantiation
  form fails, use the header's existing pattern. Do not burn the budget: after 3 failed forms, mark the row blocked with the
  exact compiler error.

## Method: transformer first, then batch
NF44 ported 19 scratch-texture rows by proving the rewrite by hand on 2-3 rows and then applying it with a small script
(`$S/w22/agentNF44/work/scratch.py` is that script; read it).
1. Group the rows by stand-in shape. Start with the 16 e110 `GetSubDef<CSpecialEffectsDef>` rows.
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
- Work only in `$S/w22/agentNF49/`, using absolute paths. Grade in your private `S=<your dir>/pS`. Never run git. Never write
  under /home/user/FableDecomp.
- Run one compile at a time, always inside `timeout`, with at most 10 grades per row. Every Bash call must exit 0.
- If you change a row that EH funclets include, regrade those funclets (see Round-3 lessons).
- Count before you report: every number comes from your files.

## Row claims
Claim each row before you change it:

    mkdir $S/w22/nflocks/<addr> 2>/dev/null && echo NF49 > $S/w22/nflocks/<addr>/owner; true

- NF48 holds its rows as `NF48`. If a lock you need is NF48's, record the row `deferred: held by NF48` and move on.
- Header overlays: NF48 may add opt-ins to stlport/ headers and engine headers. Keep your
  macro names specific to the member (`FABLE_RETAIL_<CLASS>_<MEMBER>`), so the coordinator can merge both overlays.

## Hand-back (in $S/w22/agentNF49/)
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
