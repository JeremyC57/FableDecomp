# Name-fix agent NF46 (2026-11-16, round 7): the NEngineInternal engine-data globals, as one closed family

Owner: "Continue the naming fix queue with two agents until complete". Rounds 1-6 respelled about 1,250 landed rows. The latest
landings are 190163133a (NF44) and b9dacad520 (NF41). Read these first:
- `rebuild/candidates/manual/naming/2026-11-16_agentNF44/NOTES.md`, which covers the GP* families ported in round 6, including
  NF44's scratch-texture transformer;
- `rebuild/candidates/manual/batches/2026-11-16_landing_nf44/README.md`.

## Your slice is a FAMILY, not a tag range
Round 7 changes how the queue is cut. Each agent gets one closed family of binder rows, and **every row in it must end ported or
blocked**. `carry` is not allowed inside the slice.

NF45 is running beside you on the render globals: GPRenderManager 0x01436E18 and 0x01436E38 / 0x01436E5C / 0x01436E60 / 0x01436EA4.
- NF45 owns the 55 rows in `$S/w22/agentNF45/rows.txt`. It also fixes every other GP* global those rows read, so they are NOT in
  your worklist.
- Never touch an NF45 row; the lock files enforce this.

**Your globals.** Both PDBs prove these data symbols (Ego_r.pdb / FableWin.pdb `NEngineInternal` globals):

| Retail | Correct symbol | Queue line(s) |
|---|---|---|
| 0x01436E78 | `?GPPrimitiveRendererSharedData@NEngineInternal@@3PAVCEnginePrimitiveRendererSharedData@@A` | 32 |
| 0x01436E84 | `?GPPrimitiveTypeInfo@NEngineInternal@@3PAVCEnginePrimitiveTypeInfo@@A` | 33 |
| 0x01436E8C | `?GPEngineWorldMap@NEngineInternal@@3PAVCEngineWorldMap@@A` | 34 |
| 0x01436E98 | `?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A` | 35 |
| 0x01436EA0 | `?GPCamera@NEngineInternal@@3PAVCEngineCamera@@A` | 37 / 79 |

`ENTRIES.tsv` holds these lines from NF44's APPLIED.tsv, with NF44's notes, plus 36 / 78 (GPLightingManager 0x01436E9C).
- Every remaining GPLightingManager binder is an NF45 row, so 36 / 78 are not yours to close.
- Record them in APPLIED.tsv as `deferred: binders are NF45 rows`.

**The worklist is already probed: do NOT re-probe.** `WORKLIST.tsv` has 84 distinct binder rows. Its columns are target, entry,
correct_symbol, binder_row, probe_status, bound_symbol_at_probe.
- It comes from NF44's start-of-round probe, minus the rows NF44 changed and minus NF45's rows. None of them changed in NF41's
  landing, so it is current as of b9dacad520.
- `NOREL` means the row has no relocation at that target. Find out why: it may read the global through a computed address or
  another global, or it may be a stale probe hit. Then either fix the row or mark it `not-a-binder` with the reason.
- **The dominant shape is GPResourceManager 0x01436E98 (48 rows),** in two groups:
  - 15 rows bind `?g_pEngineData@@3PAUCEngineData@@A`;
  - 9 bind `?g_pEngine_01436e98@@3PAUCEngineGlobal_01436e98@@A`;
  - the rest use other stand-ins.

  NF44 proved the access path for the +0x3190 scratch-texture rows: `NEngineInternal::GPResourceManager->GetGPUScratchBufferTextureManager()`
  on engine/CEngineResourceManager.h, with the landed `FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_*` opt-ins. Map each local
  CEngineData offset to the real CEngineResourceManager member using ghidra_out/struct_layouts_egor.tsv.
- After that: GPEngineWorldMap (13 rows, mostly `?g_pEngineState_01436e8c@@3PAUSEngineState_01436e8c@@A`), then
  GPPrimitiveRendererSharedData (10), GPPrimitiveTypeInfo (7) and GPCamera (6).
- **Other globals in the same row:** when you port a row, fix every GP* global it reads. `$S/w22/r7/gp_binders.tsv` has the full GP
  family with each target's correct symbol. List each extra queue line a row serves in STATUS.tsv `entries`.

Shared headers to use:
- engine/CEngineResourceManager.h and CGPUScratchBufferTextureManager.h, plus NF44's landed opt-ins (`FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_*`,
  `FABLE_RETAIL_CENGINEMAP_GPENGINEWORLDMAP`);
- engine/CEngineMap.h, CEngineWorldMap.h, CEnginePrimitiveRendererSharedData.h, CEnginePrimitiveTypeInfo.h and CEngineCamera.h.

## Method: transformer first, then batch
NF44 ported 19 scratch-texture rows by proving the rewrite by hand on 2-3 rows and then applying it with a small script
(`$S/w22/agentNF44/work/scratch.py` is that script; read it).
1. Group the 84 rows by stand-in shape. Start with the 15 `g_pEngineData` rows (GPResourceManager).
2. Port 2-3 rows of the group by hand, using these steps:
   - include the header, adding an `#ifdef` opt-in in your overlay for any member it lacks;
   - delete the local model;
   - read the `NEngineInternal::GP*` global.
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
- Work only in `$S/w22/agentNF46/`, using absolute paths. Grade in your private `S=<your dir>/pS`. Never run git. Never write
  under /home/user/FableDecomp.
- Run one compile at a time, always inside `timeout`, with at most 10 grades per row. Every Bash call must exit 0.
- If you change a row that EH funclets include, regrade those funclets (see Round-3 lessons).
- Count before you report: every number comes from your files.

## Row claims
Claim each row before you change it:

    mkdir $S/w22/nflocks/<addr> 2>/dev/null && echo NF46 > $S/w22/nflocks/<addr>/owner; true

- NF45 holds its rows as `NF45`. If a lock you need is NF45's, record the row `deferred: held by NF45` and move on.
- Header overlays: NF45 may add opt-ins to the same headers, such as engine/CEngine.h and CEngineResourceManager.h. Keep your
  macro names specific to the member (`FABLE_RETAIL_<CLASS>_<MEMBER>`), so the coordinator can merge both overlays.

## Hand-back (in $S/w22/agentNF46/)
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
