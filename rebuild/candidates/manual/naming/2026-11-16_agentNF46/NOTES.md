# NF46 notes (round 7, 2026-11-16): NEngineInternal GP* data-symbol binders

Worklist: 84 landed rows reading GPPrimitiveRendererSharedData 0x01436E78 (entry 32), GPPrimitiveTypeInfo 0x01436E84 (33),
GPEngineWorldMap 0x01436E8C (34), GPResourceManager 0x01436E98 (35), GPCamera 0x01436EA0 (37 / 79). Entries 36 / 78
(GPLightingManager) are deferred: their binders are NF45 rows.

Result (STATUS.tsv): 74 ported, 1 not-a-binder (0x00B6B0B0, already binds GPEngineWorldMap), 9 blocked, 0 deferred, 0 carry.
Every ported row grades RELOCATION_MATCH with the overlay and binds the PDB data symbol; tests in recheck_final.log.

## Blocked (9)
- Authoring (July behaviour lifts on rebuild/include/candidates/*_compiled.h that do not byte-match retail; 1 grade each):
  0x00B687D0, 0x00BE7D70, 0x00BF4130, 0x00BFE050 (entry 35); 0x00BDC510, 0x00BF4220, 0x00BF8810 (entry 79/37).
- blocked/port (1 grade each, first error recorded in STATUS.tsv; attempts in work/attempts/):
  0x00B7C510: CRGBFloatColour, C3DVector, C4DVector, CMatrix3x4, CArray (engine/CIDrawEnvironment.h pulls their headers; the row's shape
  rests on local models). 0x00B7FA20: CArray + CShaderRenderManager (and CRGBFloatColour / C4DVector / CMatrix3x4). The GP spelling for
  both is ready: `GPEngineWorldMap->PeekGameWorld()->DrawGetEnvironment()->PeekInterpolatedParams().ThemeChannels[0].Scalers[2]`.

## Self renames (RENAMES.tsv section 1) -- these move the row files
0x00BC5A20 (CEnginePrimitiveChangerManager, now virtual ERendered), 0x00B51F60 (CWeatherRendererMethods -> CEngineWeatherRenderer::ClipBoxToCamera,
file CWeatherRendererMethods_ClipBoxToCamera_00b51f60.cpp moves), 0x00B676A0 (QAE -> UAE), 0x00B23F60 (free function -> CEngine virtual),
0x00B25C80 / 0x00B25CC0 (static -> CEngine virtual). Old and new self symbols were taken from compiled objects. No EH funclet includes any
changed row (grep of rebuild/src/compiled and 01/21/*Unwind*), so no funclet needs repointing or regrading.
Self-symbol audit: for the other 68 ported rows the self symbol of the port equals the original's (work/audit.tsv, both from objects).
Section 2: 0x00B52120 (ClipLockedBoxToCamera) calls 0x00B51F60 through a stale local declaration; respell at landing. The other two callers
(0x00B54890 / 0x00B54E30) already define the PDB spelling in their TU. The other five renamed functions have no direct callers (virtual).

## Points the coordinator must know
- FITTED: 0x00BAD670 divides by `(float)(long)frame->GetWidth()` / `GetHeight()`: FableWin's GetWidth returns unsigned long, but retail
  converts signed (0x00BAD85F movzx + fild dword). 0x00BAD8A0 uses the plain unsigned conversion, which is what its retail does.
- 0x00B90270 binds GPResourceManager but keeps its pre-existing raw +0x29C8 reinterpret (= VShaderResources[1]) and its other renaming
  macros (C3DMeshInfo_PDB, CBankFile_PDB, CVertexShader_PDB, CShaderDataBank_PDB). Pre-existing renaming macros also remain in 0x00BCA770 /
  0x00BCC5F0 (CVertexShader_PDB, CShaderDataBank_PDB) and 0x00BBF380 / 0x00BC3E10 (the self manager class _PDB, CIndexBufferLayout): debt.
  I removed only the renaming macros over the headers I needed (CEngineResourceManager in 0x00B90270 / 0x00BCC5F0 / 0x00B5ECE0, CEngine in
  0x00BCA770, CPixelFormat in 0x00B5BF10 / 0x00B5ECE0 / 0x00B6D510).
- 0x00B90270 and 0x00BDD2F0 use the generated-layout branch of engine/CEngineResourceManager.h (members before the retail/PDB divergence:
  MeshBanks +0x10, GraphicBanks +0x954). 0x00BDD2F0 must: the retail branch's template CCountedPointer.h collides with its local counted pointer.
- Name debts left (not GP, not changed): 0x00B401E0 is really ?OnGameMapLoaded@CEngineWorldMap@@QAEXJ@Z (catalog leaf; `this` is the world
  map, +0x20 MapList) and its callee 0x00BDBFE0 is likely ?OnGameMapLoaded@CEngineLandscapeMap@@QAEXXZ; the derived self shells
  RadialBlurMethods (0x00B5BF10), CGlowRendererMethods (0x00B5ECE0), CEngineScreenEffectDisplacementRenderer_Init (0x00B86A70), whose PDB
  selves are ?InitialiseTextures@CEngineScreen*Renderer@@AAE_NXZ; 0x00B7C510's catalog leaf is SetupSeaBackgroundShaderConstantsAndFog
  (the row says Midground); 0x00B42750 still calls 0x00B3E820 as CStaticMapBankFile::SetMemoryPool (landed row: CBankFileAsync::SetLoadingMemoryPool).
- Callee respellings inside ported rows (now the PDB spellings): 0x00B2C4E0 CCharString(const char*, long) (was int);
  0x00B5ECE0 / 0x00B86A70 scratch-manager AllocteMemoryPage / AllocateTexture (were local AllocateMemoryPage / pointer-parameter variants);
  0x00BE27B0 CBankFile::FindIndexBySymbol + CMeshDataBank::PeekStaticMesh / PeekRepeatedMesh (0x00A24290; my first spelling
  PeekAnimatedMesh was wrong -- that is 0x00A242B0 -- caught by the first final run's RELOC_NAME and fixed before the logged run) (were CGraphicsBank::FindEntry / GetEntry /
  GetAnimatedEntry); 0x00BABB50 AllocateDepthBufferSurface with ESurfaceMultisampleType.
- Globals out of this family left as they were: 0x01436E1C, 0x01436E20, 0x01436E48, 0x01436E58, 0x01436E74, 0x01436E80, 0x01436EA8,
  0x01436EAC, 0x01436EB0.
- RELOC_NAME warnings in recheck_final.log: every callee spelling they name is already in the landed original (pre-existing), except
  GPCamera (the target symbol; the stale label is g_pEngineCamera).
- NF45's landing (8872d59ed4) touched none of my overlay headers; overlays re-diffed against the repo after it (only my opt-ins differ).

## Transformers (work/)
shader.py, dropcomp.py, sd.py, ti.py, env.py, rmm.py, scratch.py, gb.py (graphic bank), trails.py (sprite / weapon trails).
