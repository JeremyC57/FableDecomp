# Chunk 529 (agent529, 2026-10-28): 14 of 20 re-queued single functions landed (241-495 bytes)

Re-queue chunk of 20 rows of 241-495 bytes (0x00A46B50..0x00B41C30): CEngine rendering and clipping, quaternion slerp,
particle emitters, physics meshes, skeletal bone morphs and STLport containers. One Opus 5.5 agent at high effort.

- **Landed:** 11 rows with `vland.py --land` (1 MATCH 0x00B30480 CEngineCamera::ClipSphere, 10 RELOCATION_MATCH;
  0x00AE8C30 needs /Gr), and 3 rows as grade `retail_asm` (payload_retail_asm.json): 0x00AA3390
  CQuaternion::CalculateSlerpWeights, 0x00AA3560 CQuaternion::SetSlerpFromIdentity, 0x00B05620
  CParticle::GetInterpolatedOrientation inline the engine's `fld` + `fistp dword` float-to-long helper through
  GFFastArcCos / GFFastSin. Every test PASS under g++ and VC7.1; no RELOC_IDENTITY, CONFLICT or DUPLICATE.
- **Parked (6, differ/):** 0x00A56670 GFGetHermiteSplinePosition, 0x00A82BE0 `__adjust_heap` (scheduling), 0x00A90D60
  RemapBoneIndices (retail elides a temporary's dtor), 0x00B27D90 CEngine::Render (VC7.1 merges two bool reads into one
  word load), 0x00B3BC90 CEngineResourceManager::SetupEditorAnimation (frame slots), 0x00B41C30
  CEngineMap::PeekInterpolatedWaterDepth (scheduling).
- **Note:** 0x00A86CD0 compiles a same-TU noinline body for its callee Calculate 0x00A868C0 under the real name to fix the
  caller's frame; that body is a stand-in, not the retail function, and 0x00A868C0 stays unlanded.

Name fixes (NAME_FIXES.tsv, 38 rows, deferred): 0x00AF0380 `NParticleEngine::CParticleEmitter` ctor, 0x00AE91D0
`CCountedPointer<CParticleEmitter>::DeleteData`, 0x00A05840 CRenderStateManager::RestoreCaptureBlock, the
CShaderRenderManager Enable/Disable shader quartet 0x00987FE0..0x00988190, 0x0099B2C0 CWideString::ToCharString, the
CBankFile family (FindIndexBySymbol, AddEntry, Begin/EndUpdateEntries, UpdateEntry), 0x00B24850 SetDefaultRenderStates is
AAE. Catalog label "ApplyPartialAnimationAlpha" had been propagated across three different functions.
