# Name-fix agent NF51 (2026-11-17, round 7): CAIStateGroupBase AI actions, NUISystem actions and NPlayerGui

Owner: "Continue the naming fix queue with two agents until complete". Round 7 has fixed about 360 caller rows so far. The latest
landings are c2c0b478b3 (NF48) and 2f6e1d5705 (NF49). Read first:
- `rebuild/candidates/manual/batches/2026-11-17_landing_nf49/README.md` and `naming/2026-11-17_agentNF49/NOTES.md`. NF49 moved
  several CAIStateGroup rows onto engine/CAIStateGroupBase.h, and added the `_CAIFILTERBASE_RESTORE_PROTECTED` opt-in;
- `naming/2026-11-16_agentNF44/NOTES.md`, the NUISystem ports onto NUISystem_CUIState.h / fable_map.h.

## Your slice is a FAMILY, not a tag range
Each agent in round 7 gets a closed family of binder rows. **Every row must end `ported`, `not-a-binder` or `blocked`.**
`carry` is not allowed inside the slice.

NF50 is running beside you on the STLport red-black tree internals. The rows in `$S/w22/agentNF50/rows.txt` are its rows, and none of
them is in your worklist. Never touch an NF50 row; the lock files enforce this.

**Your 16 entries** are in `ENTRIES.tsv`: source half, entry, address, correct symbol, status and the previous agent's note. The
main ones:
- **nf44 86 `CAIStateGroupBase::AIAction_MoveToPosProximity` (protected; 25 rows).** NF44's triage: each row declares it on a local
  CAIStateGroupBase or derived view with long / bool* signatures. The header opt-in `FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY`
  exists, but each row needs the retail CAIStateGroupBase branch (`_MEMBERS` / `_AIACTIONS`, enums, `bool&`, `const CThing*`) in
  place of its local model.
  - NF49's 0x009378F0 port is the worked example; read it in the repo.
  - Also: AIAction_PerformAction, AIAction_PumpCombatActions, AIAction_MoveToPreCalcRoute and
    `CAIStateGroup_WorkBase<CAIStateGroupBase>::StayInShop`.
- **nf44 2 `??_9CDrawBase@NPlayerGui@@$BA@AE`, the vcall thunk at 0x0043E420 (slot 0 = ClearPrimitivesIfNecessary; 14 rows).**
  - Verify the claim first: a vcall thunk is named by the slot it dispatches to and by its callers.
  - Several binders are 0x00C0A010 .. 0x00C24EE0, far from NPlayerGui. Check whether those callers really go through this thunk
    (ICF can fold identical `jmp [ecx+0]` thunks), and record what you find.
- **NUISystem:**
  - CDeletion ctors (`??0CDeletion@NUISystem@@QAE@XZ` and its copy ctor);
  - the `CActionParamSLONG` vftable (6 rows). Per NF44, each is a full port onto the NUISystem headers, because
    NUISystem_CActionParamSLONG.h pulls in NUISystem_CComponent.h;
  - `CCountedPointer<CAction>` / `CCountedPointer<CComponent>::ReleaseData`;
  - `map<FRONTEND_SCREEN, CComponent*>::operator[]`;
  - CFrontEndManager::CanStartAttractMode.
- **CPlayerGui / NPlayerGui:**
  - Add/RemovePerceivingThing;
  - CDrawAbilityButtons::DrawAbilityButton.

**The worklist is already probed; do NOT re-probe the whole half.** `WORKLIST.tsv` has 68 distinct rows, 75 lines, with columns
entry, target, correct_symbol, binder_row and bound_symbol_at_probe.
- It comes from NF41's probe and NF44's APPLIED binder lists, minus every row landed since and minus NF50's rows.
- Before editing a row, compile its repo copy once with `xrow.sh` to confirm it still binds the wrong spelling. If it no
  longer does, record it `not-a-binder`.
- NF44-half lines have an empty bound_symbol column. Read the binding from your confirming compile.

**How to port:** verify each claim first, from the PDB name sets, the callee body and the vtable slots. Then move each row onto the
shared header, delete its local model, and grade.
- Rows that model a dozen classes locally are ports. Do them within the 10-grade budget, or mark them blocked/port and name the
  classes that must move first.
- Do not burn the budget: after 3 failed forms on one row, mark it blocked with the exact compiler error or diff.

**Containers restart without warning (three times in the last day).** Write STATUS.tsv and APPLIED.tsv after EVERY row. The first
grade after a restart can time out while wineserver starts; retry once.

## Method: transformer first, then batch
NF44 ported 19 scratch-texture rows by proving the rewrite by hand on 2-3 rows and then applying it with a small script
(`$S/w22/agentNF44/work/scratch.py` is that script; read it).
1. Group the rows by stand-in shape. Start with the 25 nf44:86 MoveToPosProximity rows.
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
- Work only in `$S/w22/agentNF51/`, using absolute paths. Grade in your private `S=<your dir>/pS`. Never run git. Never write
  under /home/user/FableDecomp.
- Run one compile at a time, always inside `timeout`, with at most 10 grades per row. Every Bash call must exit 0.
- If you change a row that EH funclets include, regrade those funclets (see Round-3 lessons).
- Count before you report: every number comes from your files.

## Row claims
Claim each row before you change it:

    mkdir $S/w22/nflocks/<addr> 2>/dev/null && echo NF51 > $S/w22/nflocks/<addr>/owner; true

- NF50 holds its rows as `NF50`. If a lock you need is NF50's, record the row `deferred: held by NF50` and move on.
- Header overlays: NF50 may add opt-ins to stlport/ headers. Keep your
  macro names specific to the member (`FABLE_RETAIL_<CLASS>_<MEMBER>`), so the coordinator can merge both overlays.

## Hand-back (in $S/w22/agentNF51/)
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
