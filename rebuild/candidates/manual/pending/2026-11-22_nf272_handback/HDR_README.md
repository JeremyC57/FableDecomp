# NF272 header overlay (2026-11-22): two add-only opt-ins; bases in ../orig/ (copied from the repo tree, still current at hand-back)

1. engine/CWaterPatchMesh.h, retail branch: `FABLE_RETAIL_CWATERPATCHMESH_OPERATORDELETE` (rows 0x00BEAAA0 / 0x00BEB4D0) adds one member, the class
   operator delete `static void FABLE_FASTCALL operator delete(void*)` (FableWin.pdb ??3CWaterPatchMesh@@SIXPAX@Z; no out-of-line copy in Ego_r)
   with an inline body that returns the block through NEngineInternal::GPLandscapeRenderer->MemoryManager->FreeVirtual (retail ??_G 0x00BEB4D0).
   The row must include engine/CEngineLandscapeMemoryManager.h (retail branch) and engine/CEngineLandscapeRenderer.h (_GPLANDSCAPERENDERER) first.
   Inserted after the CREATEFROMSTREAM block; the header's asserts are already `FABLE_STATIC_ASSERT_AT` (pin_asserts: 0 pinned). No object
   built here emits an out-of-line ??3CWaterPatchMesh (objdump: inlined into ??_G).
2. candidates/landscape_patch_update_foreground_compiled.h: `FABLE_RETAIL_LANDSCAPE_PATCH_UPDATE_FOREGROUND_RELEASEBUFFERS` (row 0x00BF4220 and
   its test) respells the header's existing local `CWaterPatchMesh` model's one member in place: `ClearTextures()` -> `ReleaseBuffers()`
   (0x00BEAA00); the old line stays on the #else path. pin_asserts pinned 10 asserts (lines 113..122 keep their names via FABLE_STATIC_ASSERT_AT).

Default paths are token-identical. Proof: lane_merge/equiv_check.py over the two headers' closure (tools/eqrows.py: 1,383 rows, every row whose
source or test includes either header transitively plus every `../src/compiled/` includer), repo tree v a copy with this overlay: 0 rows differ
(evidence/equiv.tsv empty). Control: the same copy with tokens planted inside each new opt-in AND on a default line of each header's branch
(CWaterPatchMesh.h `Build`, the candidates header's `class CBankFileAsyncData`): 9 lines differ (evidence/equiv_ctl.tsv): the seven
CWaterPatchMesh retail-branch includers (0x00BEB490, 0x00BF5A30, 0x00BEAB40, 0x00BEAE50, 0x00B761D0, 0x00B77370, 0x00B751F0) and 0x00BF4220's
source and test. No landed row defines either new macro, so none of those includers changes.
Other agents' overlays: NF270 holds only CCreatureActionBase.h / CCreatureAction_UnsheatheItemFromInventoryWithHasteAction.h; no overlap.
The extra file ../src/compiled/00/be/CWaterPatchMesh_CWaterPatchMesh_00beaaa0.cpp is grading scaffolding only: a copy of rows/00beaaa0.cpp under
its post-rename file name, so 0x00BEB4D0's fold include resolves through FABLE_HDR_OVERLAY (hdr/../src/compiled/...); not a header.
