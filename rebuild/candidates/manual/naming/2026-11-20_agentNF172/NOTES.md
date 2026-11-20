# NF172 notes (2026-11-20, round 10: render / shader / texture / engine naming)

Census first: one compile probe of every landed REFS row (work/probe.tsv, summary work/census.txt), plus selfsym of the self rows
(work/selfsym.txt). Counts are in APPLIED.tsv / STATUS.tsv; the final recheck is recheck_final.log.

## Descriptive names
- No new made-up names. q5054 (libpk CRenderLayerWin32::CreatePooledEvent / UpdateSlot) is descriptive on both sides and left for a
  coordinator decision (APPLIED q5054).

## New local models / casts / flags
- New local models: none. In-place respells inside local models the rows already had at HEAD (no new class token):
  0x00499C50 (CProgressDisplay::RenderProgress access), 0x00BBC130 (RenderSingleSprite / RenderSingleSpriteTrail PAV -> AAV),
  0x00B51F00 (BuildBoxPositionMatrix access), 0x00B3B4F0 (FlushCache -> ChangeSceneForTextureBank).
- Casts of stand-in types to real ones: none. Flags lines: none.

## Self renames that move a row's file (coordinator renames the file at landing)
- 0x00B2FC00: AddExclusiveObserver_AddExclusiveObserver_00b2fc00.cpp -> CEngineCamera_SetGameWindow_00b2fc00.cpp.
- 0x00A0AA60: CTCInventoryBase_GetCanItemBeQuickAssigned_00a0aa60.cpp -> CRenderManagerCore_IsRendering_00a0aa60.cpp.
- 0x00A0AA90: CTCInventoryBase_SetViewport_00a0aa90.cpp -> CRenderManagerCore_SetAWindow_00a0aa90.cpp.
- 0x009E3620: global_FindFormatInfoIndexFromD3DFormat_009e3620.cpp -> CPixelFormat_FindFormatInfoIndexFromD3DFormat_009e3620.cpp.
- Others keep their file names (0x00499880, 0x00B846E0, 0x00B3AFF0, 0x00BE04C0, 0x00B5E700, 0x00B5EA30 already carry the right
  class/method). EH funclets: none of the changed rows is included by any other compiled source (grep of each basename).

## Existing splits seen, not created (left as they are)
- 0x01437884 (the SetupSeaBackgroundShaderConstantsAndFog static guard): 0x00B70230 binds ??_B?6??...@56 (inline-function guard),
  the self row 0x00B7C510 ?$S1@?6??...@4IA (out-of-line guard). Compiler-chosen; flag for the coordinator.
- 0x01436EAC: three descriptive spellings (g_pEngineSurfaceCompositionManager / g_pResourceBank_01436eac / g_pSurfaceComposition_01436eac).
- 0x00BE0930 (const find): self ?find@...QBE (non-template) + ?Find@CRetailBlockSet@@... (0x00BE0480). D1 member-template family in
  NAME_FIXES; my draft work/00be0930_family_draft_MATCH.cpp grades MATCH. 0x00BE0480 also needs the IsBlockLoaded QAE/QBE family
  (0x00BDC180 via candidates/landscape_background_lod_compiled.h, 0x00BF8030, 0x00BF84C0 QAE; 0x00BDE390, 0x00BDE860 QBE; PDB is QBE).
- 0x00BC04F0: AAV (self, 0x00BBC130) + PAV (0x00BAD2E0, render-state family). Count unchanged.

## Tests
- Rewritten to link the real row object (no class model; linker /alternatename aliases + recording stubs, as NF161):
  0x00A0AA60, 0x00A0AA90, 0x00B2FC00, 0x00B846E0, 0x00B3AFF0, 0x00B23BC0. All other changed rows keep their landed (standalone) tests.
- A linked rewrite for 0x00BE0400 failed LNK2019: an /alternatename whose source is the ~430-character STLport find symbol was not
  honoured (the shorter aliases all work). The landed test was kept (work/00be0400_test_linkfail.cpp).

## Blocked / decisions for the coordinator
- q5054: see APPLIED. Choose (a) rename the callers' SStateSlot local model to CNodePool + change their own second parameter to
  CPooledEvent** (moves ApplySlotA / ApplySlotB leaves; referencers 0x00C3E330 / 0x00C43B30), or (b) keep the UpdateSlot spelling as a
  recorded descriptive split.
- q4811 / q11909: rows left in the render-state family (0x00BAD2E0, 0x00BB8090), per the brief.
- q11898: rows left 0x00A68060 (local CRenderStatistics stand-in: needs a real port, not a respell) and the self row 0x00A0AA70.
- q11918 / q11978: open (not started; census in APPLIED).
- q11906 / q11910: authoring (legacy 0x00BF4570; naked asm bake 0x009BEF80).

## Rule slips
- The census probe (work/probe.tsv) compiled three acctB-locked rows read-only in my own pS directory (0x00A685E0, 0x00A68910,
  0x00B99300) and the selfsym pass none; none of them was edited, graded for hand-back or put in any payload.
- One pdiff run for 0x00B5EA30 started after my transformer had failed (no work file): no cl ran on my source; counted in STATUS.
