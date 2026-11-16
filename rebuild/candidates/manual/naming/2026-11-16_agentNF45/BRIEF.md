# Name-fix agent NF45 (2026-11-16, round 7): the NEngineInternal render globals, as one closed family

Owner: "Continue the naming fix queue with two agents until complete". Rounds 1-6 respelled about 1,190 landed rows. The latest
landing is commit 190163133a (NF44). Read these first:
- `rebuild/candidates/manual/naming/2026-11-16_agentNF44/NOTES.md`, which covers the GP* families ported in round 6;
- `rebuild/candidates/manual/batches/2026-11-16_landing_nf44/README.md`.

## Your slice is a FAMILY, not a tag range
Round 7 changes how the queue is cut. Each agent gets one closed family of binder rows, and **every row in it must end ported or
blocked**. `carry` is not allowed inside the slice.

**Five globals.** Both PDBs prove these data symbols (Ego_r.pdb / FableWin.pdb `NEngineInternal` globals):

| Retail | Correct symbol | Queue line(s) |
|---|---|---|
| 0x01436E18 | `?GPRenderManager@NEngineInternal@@3PAVCRenderManager@@A` | 29 |
| 0x01436E38 | `?GPScreenEffectDisplacementRenderer@NEngineInternal@@3PAVCEngineScreenEffectDisplacementRenderer@@A` | 82 |
| 0x01436E5C | `?GPOcclusionManager@NEngineInternal@@3PAVCEngineOcclusionManager@@A` | 30 |
| 0x01436E60 | `?GPShadowRenderer@NEngineInternal@@3PAVCEngineShadowRenderer@@A` | 31 |
| 0x01436EA4 | `?GPEngine@NEngineInternal@@3PAVCEngine@@A` | 38 |

`ENTRIES.tsv` holds these five lines from NF44's APPLIED.tsv, with NF44's notes.

**The worklist is already probed: do NOT re-probe.** `WORKLIST.tsv` has 55 distinct binder rows, 59 lines. Its columns are
target, entry, correct_symbol, binder_row, probe_status, bound_symbol_at_probe.
- It comes from NF44's start-of-round probe, minus the 50 rows NF44 changed, so it is stamped as of 190163133a.
- `NOREL` means the row has no relocation at that target. Find out why: it may read the global through a computed address or
  another global, or it may be a stale probe hit. Then either fix the row or mark it `not-a-binder` with the reason.
- The dominant shape is 23 rows binding `?g_pRenderManagerCore@@3PAUCRenderManagerCoreView@@A` plus 11 + 3 on
  `?g_pRenderManagerCore@@3P[AU|AV]CRenderManagerCore@@A`.
  - These rows carry LOCAL CRenderManagerCore / CRenderManagerCoreView models: 24-57 line views with inline SetState /
    PrepareForRender bodies and local CShaderRenderManager / CArray / C4DVector.
  - Each one is a port onto engine/CRenderManager.h + CRenderManagerCore.h. NF44 showed that CRenderManagerCore sits at offset 0
    of CRenderManager2D / CRenderManager, so `NEngineInternal::GPRenderManager` reaches the same calls.
- **Other globals in the same row:** 21 rows in your slice also read other GP* globals, such as GPResourceManager 0x01436E98,
  GPCamera 0x01436EA0 and GPEngineWorldMap 0x01436E8C. `$S/w22/r7/row_target.tsv` lists every row → GP target, and
  `$S/w22/r7/gp_binders.tsv` has the full GP family (139 rows, 11 globals) with each target's correct symbol. When you port a
  row, fix every GP* global it reads, and list each extra queue line it serves in STATUS.tsv `entries`.

Shared headers to use:
- engine/CRenderManager.h, CRenderManager2D.h and CRenderManagerCore.h;
- engine/CEngineOcclusionManager.h, CEngineShadowRenderer.h, CEngineScreenEffectDisplacementRenderer.h, CTextureManager.h and
  CEngine.h;
- engine/CEngineResourceManager.h, plus NF44's landed opt-ins (`FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_*`,
  `FABLE_RETAIL_CENGINEMAP_GPENGINEWORLDMAP`).

## Method: transformer first, then batch
NF44 ported 19 scratch-texture rows by proving the rewrite by hand on 2-3 rows and then applying it with a small script
(`$S/w22/agentNF44/work/scratch.py` is that script; read it).
1. Group the 55 rows by stand-in shape. Start with the 23 `CRenderManagerCoreView` rows.
2. Port 2-3 rows of the group by hand, using these steps:
   - include the header, adding an `#ifdef` opt-in in your overlay for any member it lacks;
   - delete the local model;
   - read `NEngineInternal::GPRenderManager`.
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
- Work only in `$S/w22/agentNF45/`, using absolute paths. Grade in your private `S=<your dir>/pS`. Never run git. Never write
  under /home/user/FableDecomp.
- Run one compile at a time, always inside `timeout`, with at most 10 grades per row. Every Bash call must exit 0.
- If you change a row that EH funclets include, regrade those funclets (see Round-3 lessons).
- Count before you report: every number comes from your files.

## Row claims
Claim each row before you change it:

    mkdir $S/w22/nflocks/<addr> 2>/dev/null && echo NF45 > $S/w22/nflocks/<addr>/owner; true

- One row in your slice, 0x00B63530, is held by NF41. NF41 has finished, but its rows are not landed yet. Leave that row
  `deferred` and do not touch it.
- Rows held by NF44 are already landed. The coordinator releases those locks, but if you find an NF44 lock, the repo copy is
  current and you may take the row.

Another agent may start on a different family while you run; the lock files keep you apart.

## Hand-back (in $S/w22/agentNF45/)
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
