# Hand chunk 2028 (CEngineOcclusionManager .. CEnginePrimitiveManagerRepeatedStaticMeshes): 1 of 2 byte-exact

## What matched (1)
- 0x00B8DF50 CEngineOcclusionManager::FrameEnd (RELOCATION_MATCH, 1626 bytes; test PASS g++ + VC7.1; read-only vland RELOCATION_MATCH PASS).
  Inside a CRenderStateScopedCaptureBlock: SetVertexShadersEnable(true, &old) / ApplyVertexShader(VertexShaderFlareTest) /
  SetPixelShadersEnable(false, &old), the diffuse-only state block, Z / colour writes off; per flare test: the inline
  CEngineCamera::ClipSphere (the landed CEngine::ClipSphere 0x00B24150 twin), world transform = camera inverse non-homogenous view
  matrix + flare position + PreScale(radius), PrepareForRender(1), tri fan of CircleSegments; MaxPixelCount = GFFloatToLongNear(pi * rx * ry)
  from the render-target size and HomogenousViewScaleX / Y; clipped flare -> 1; restores and RefreshD3DTransforms.
  Shape lessons: the translation copy is a C3DVector copy placed after the matrix copy, through SetTranslateOnly(const C3DVector&) forwarding to the
  float overload (Z read first); TransformPoint(const&, &) rows accumulated Z-term first; radiusX computed before radiusY.

## Parked for Fable 5.1 (1)
- 00b9a4a0 CEnginePrimitiveManagerRepeatedStaticMeshes::RenderPrimitive DIFFER(1674v1674): 0x00B9AA25 `mov byte ptr [esp+1Bh],cl` / 0x00B9AA56 `mov dl,byte ptr [esp+1Bh]` built with [esp+18h] (the expanded CalculateSubPrimitiveRenderFlags' `translucent` parameter home, the only residue; 510/512 exact, 512/512 frame-masked) | tried: named render_flags local, flags declared first in the helper (worse), for+break LOD loop (moves the ~CCountedPointer inline split), `<=` LOD compare (jp v je), box-distance sum orders | next: frame-packer trace (agent2003 tools/pk or hand886 phook) of that 1-byte node: retail puts it at the top byte of the B+8 dword (it seems to join a 4-byte slot opened by another node); passes 2-3 unused (context), no behaviour test yet.

## Skipped
- none.

## Names (two sources each; LABELS.tsv)
- Rows: PDB spellings from Ego_r.pdb + FableWin.pdb plus donor order (FrameEnd) / vtable slot 7 (RenderPrimitive).
- Callees: see LABELS.tsv / NAME_FIXES.tsv. Inline-only FableWin names used as written: CEngineCamera::ClipSphere(const C3DVector&, float),
  TransformWorldToView, PeekNonHomogenousViewToViewScaleX/Y, PeekViewTransform, CMatrix3x4::SetTranslateOnly(float, float, float),
  CEngineStateBlocks::ApplyDiffuseOnlyStateBlock, CEngineInternalPrimitiveBase::IsPersistent, CEngineInternalPrimitiveOcclusionBase::HasOcclusionInfo /
  IsOccluded, CEnginePrimitiveOcclusionManager::IsPrimitiveOccluded, C3DMeshInfo::GetLODError, CEnginePrimitiveRenderUtil static getters,
  CShadowClipVolume::ClipBoundingBox, CEngineShadowRenderer::PeekSceneData, CEnginePrimitiveManager::GetBlockOnLoad.
- Descriptive / unproven: PeekNonHomogenousViewToViewScaleX/Y returning HomogenousViewScaleX/Y is inferred (name + the scaled-view
  semantics; the debug bodies are not on this machine). ERenderMode / EBankLoadPriority enumerators unknown: forward enums with casts.
- Globals (no PDB data symbols; established spellings): 0x01436E14 NEngineInternal::GPShaderRenderManager, 0x01436E18 NEngineInternal::GPRenderManager
  (the definer CEngine::Initialise), 0x01436EA0 g_pEngineCamera (definer + NAME_FIXES_QUEUE c722; c2018 / c2024 used NEngineInternal::GPCamera -- pick one
  at landing), 0x01436E90 g_pEngineStateBlocks, 0x01436E58 g_pPrimitiveOcclusionManager (c639), 0x01436E60 g_pEngineShadowRenderer, 0x01436E74
  g_pEngineSubPrimitiveRenderer, 0x01436E78 g_pPrimitiveRendererSharedData (V-keyed, no queue decision yet), 0x01436E80 g_pEnginePrimitiveRenderer,
  0x01436E9C g_pEngineLightingManager, 0x013D2880 g_SSEAvailable, 0x013CB508 CRenderStateCaptureBlock::StateManager.

## Asm helper sites
- 0x00B8E357 bare `fistp dword ptr [esp+24h]` -> engine/GFFloatToLongNear.h. C++ attempts (probe/fragW.cpp, VC7.1 RTM /O2): (long)x, (int)x,
  (unsigned long)x, implicit conversion, named float + (long), (long)(x + 0.5f), (short)x, via double: all __ftol2; the same with /QIfist: fistp qword.
- 0x00B9A75F..0x00B9A799 hand SSE box distance (parked row) -> engine/C3DBoundingBoxSSE.h (its own attempts table).

## Header debt
- CEngineOcclusionManager / CEnginePrimitiveManagerRepeatedStaticMeshes / CEngineInternalPrimitiveRepeatedStaticMesh: new typed retail branches
  (opt-ins, hdr/README.md); CEnginePrimitiveRendererSharedData typed up to +0xC4 only.
- CMatrix3x4::TransformPoint(const&, &): this row needs the Z-term-first accumulated rows (opt-in _COPY_SUMS); the default body (E13*x first) is what
  another landed row needed -- two retail contexts of one inline body, review at landing.
- CShadowClipVolume::ClipBoundingBox duplicates the landed out-of-line row 0x00B9C2E0's body; its box-distance sum order there is Z, X, Y, the parked
  row 0x00B9A4A0 needs Z, Y, X (row-defined CalcShortestDistanceToPointSquared): same pattern as the TransformPoint note.
