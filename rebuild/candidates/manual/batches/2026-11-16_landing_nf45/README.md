# 2026-11-16 landing nf45: name-fix round 7, NF45 (47 landed rows ported onto the render-global headers)

Owner: "Continue the naming fix queue with two agents until complete".

Round 7 cuts the queue by class family instead of by tag range. NF45 took one closed family: 55 landed rows that read five
`NEngineInternal` render globals through stand-in globals or local class models. The worklist was NF44's start-of-round probe, so
NF45 did not re-probe. The five globals:
- GPRenderManager 0x01436E18;
- GPScreenEffectDisplacementRenderer 0x01436E38;
- GPOcclusionManager 0x01436E5C;
- GPShadowRenderer 0x01436E60;
- GPEngine 0x01436EA4.

Every row had to end ported, not-a-binder or blocked. NF46 runs beside it on the engine-data globals. Brief, worklist and records:
`naming/2026-11-16_agentNF45/`.

## Rows (55, all terminal; STATUS.tsv)

| Status | Rows |
|---|---|
| ported | 48 (47 changed here; 0x00B63530 was already ported by NF41's landing) |
| blocked | 7 (unchanged) |

- **Blocked, register tie (5):** 0x00B32AD0, 0x00BB8090, 0x00BAD2E0, 0x00BCB800 and 0x00B76BC0.
  - On the shared `CRenderStateManager::SetRenderState`, everything matches except one or two state sites. There, retail
    swaps the registers of the two bookmark masks.
  - The repo opt-ins `SETRENDERSTATE_DESIRED`, `BOOKMARK` and `ADDSTATE_DIRTY` leave the diff unchanged.
  - Up to 6 grades per row. These rows need a SetRenderState form that matches each site.
- **Blocked, authoring (2):** 0x00BF4570 and 0x00BE6F70. These July rows are built on `candidates/*_compiled.h` and call stand-in
  helpers. Compiled unchanged, they give 316 of 4,431 bytes and 198 of 2,654 bytes.

## Entries (APPLIED.tsv)
- **done:**
  - 30 (GPOcclusionManager);
  - 38 (GPEngine);
  - 82 (GPScreenEffectDisplacementRenderer).

  reloc_bindings has no other binder for these three.
- **blocked:**
  - 29 (GPRenderManager): 41 rows fixed; what is left is the 7 blocked rows above.
  - 31 (GPShadowRenderer): 0x00B33010 needs the CEngineScene RenderScene* declarations, and 0x00BF4570 needs authoring.
- **Gained one or more correct binders (not closed):**
  - 32: 0x00B25D80;
  - 33: 0x00BE1E70;
  - 78: 0x00B54310;
  - 79: 0x00B32660, 0x00B4D2C0 and 0x00B54310.

  0x01436E74 (GPSubPrimitiveRenderer) and 0x01436E1C (GPDisplayManager) are also respelled where these rows read them.
- **Left for NF46's family:** GPEngineWorldMap in 0x00B63010 / 0x00B77E20. `engine/CIDrawEnvironment.h` pulls in `engine/CArray.h`,
  which collides with each row's local CArray.

## Rows and checks
- **RENAMES:** no self renames. 174 changed bindings, read from a fresh compile of each original and each new source
  (`RENAMES_applied.tsv`).
- **Recheck (`logs/recheck_part0..2.log`):** all 47 graded from the repo after landing. All 47 are RELOCATION_MATCH, every test
  PASS.
- **Checks:**
  - funclet_includes_check reports 1,355 checked, 0 broken;
  - `naming/rebind.py` removed 167 binding lines and added 100;
  - bindings_multi_check is at its baseline (`NullCoord`).
- **Fallback branches removed:** 0x00B32230, 0x00B3B0C0 and 0x00B4D2C0 lose their `#ifndef _MSC_VER` branches, which held local
  stand-in models for non-MSVC parses. They now include the shared headers unconditionally, as most rows do. The coordinator
  read the removed branches and accepts the change.
- **Local models:** the pre-existing ones remain in the rows (179 SHARED_HEADER_AVAILABLE warnings in the recheck). No new ones
  were added.

## Headers (17 opt-ins in 5 files; lines added only, default path unchanged)
- engine/CRenderStateManager.h: 8 inline setters.
- engine/CRenderManagerCore.h: SetMaterial, DrawCurrentBuffersAsPointList, SetViewTransform and SetPerspectiveTransform.
- engine/CEngineScreenEffectDisplacementRenderer.h: `_STENCILRENDERINGCAPTUREBLOCK` and `_DISPLACEMENTPASSES`.
- engine/CEngineOcclusionManager.h: `_ADDFLARETEST` and `_RENDEROCCLUSIONBOX`.
- engine/CEngineShadowRenderer.h: `_SHADOWBUFFERRENDERSTATECAPTUREBLOCK`.

`land_naming_prep.sh` / hdrcheck2 found 0 removed lines and no default-path change. Details are in
`naming/2026-11-16_agentNF45/hdr/README.md`.

## Name fixes queued (`land1116fu`)
- **0x00A0AA10 SetMaterial:** the self row is a placeholder.
- **0x00B8D040 AddFlareTest:** the self row is a placeholder. Check which of the two PDB overloads the body is.
- **0x00B244F0:** a claim. 0x00B32660 binds it as `CEngine::PeekVideoOptionsDef`, but the catalog calls it GetPoolTextureSize.
- **0x00A0AA80 SetAWindow:** the self leaf is on a stand-in class.
- **0x00B86BB0 FinishDisplacementStencilRendering:** a propagated `AddToInterface` label.

## Rule slips (NF45)
- About 7 read-only Bash calls exited non-zero.
- APPLIED.tsv was written at the end of the round, not as NF45 went.
- 19 repo tests still use old stand-in names. They are self-contained and pass; the list is in NOTES.md.
