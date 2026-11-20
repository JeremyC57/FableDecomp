# NF172 header overlay (opt-ins only; base copies in ../orig/)

Every change is an add-only `#ifdef FABLE_RETAIL_<CLASS>_<MEMBER>` block; with the macro undefined the default path is token-identical
to the repo header, so none of the landed includers listed below changes. pin_asserts.py was run after each edit.

## engine/CEngineStaticMapFileBlockManager.h
- `FABLE_RETAIL_CENGINESTATICMAPFILEBLOCKMANAGER_RELEASEREQUEST`: declares `void ReleaseRequest(long block);` (0x00BE0400; Ego_r.pdb /
  FableWin.pdb `?ReleaseRequest@CEngineStaticMapFileBlockManager@@QAEXJ@Z`). Declaration only. Used by rows/00be0400.cpp.
- `FABLE_RETAIL_CENGINESTATICMAPFILEBLOCKMANAGER_GETBLOCKDATA_CONST`: declares `const unsigned char* GetBlockData(long block) const;`
  (0x00BE04C0; Ego_r.pdb 0x00A41E20 / FableWin.pdb 0x02DF74B0 `?GetBlockData@CEngineStaticMapFileBlockManager@@QBEPBEJ@Z`). Declaration only.
  Used by rows/00be04c0.cpp. (Named _CONST so it cannot collide with the unrelated `FABLE_RETAIL_CENGINESTATICMAPFILEBLOCKMANAGER_GETBLOCKDATA`
  of candidates/landscape_background_lod_compiled.h, which 0x00BDC180 defines.)
- pin_asserts: pinned 0 (the one assert is already `FABLE_STATIC_ASSERT_AT(26, ...)`).
- Landed includers (grep of rebuild/src/compiled): CEngineStaticMapFileBlockManager_AddRequest_00be06b0.cpp,
  CEngineMap_OpenDevelopmentStaticMap_00b42530.cpp, stdmap_map_00b442e0.cpp. None defines the new macro: unchanged.

## engine/CEngineCamera.h
- `FABLE_RETAIL_CENGINECAMERA_SETGAMEWINDOW` (inside the FABLE_RETAIL_CENGINECAMERA_POSITION class, right after `CEngineCamera(bool);`):
  declares `void SetGameWindow(const class C2DBoxF* window);` (0x00B2FC00; Ego_r.pdb 0x009D0510 / FableWin.pdb 0x02DC15E0
  `?SetGameWindow@CEngineCamera@@QAEXPBVC2DBoxF@@@Z`). Declaration only. Used by rows/00b2fc00.cpp.
- pin_asserts: pinned 0 (all asserts are `FABLE_STATIC_ASSERT_AT`).
- Landed includers: 99 rows include engine/CEngineCamera.h; none defines FABLE_RETAIL_CENGINECAMERA_SETGAMEWINDOW (grep: 0), so none changes.

## engine/CEngineResourceManager.h
- `FABLE_RETAIL_CENGINERESOURCEMANAGER_ONRESTORE` (private section, right after the `_ONRELEASE` block): declares
  `virtual bool OnRestoreDefaultPoolResources();` (0x00B3AFF0; Ego_r.pdb 0x009D9640 / FableWin.pdb 0x02DD2A20
  `?OnRestoreDefaultPoolResources@CEngineResourceManager@@EAE_NXZ`, vtable 0x012A1B3C slot 12). Declaration only. Used by rows/00b3aff0.cpp.
- pin_asserts: pinned 0. Landed includers: 157; none defines the new macro (grep: 0), so none changes.

## engine/CGPUScratchBufferTextureManager.h
- `FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_ENDSHUTDOWN` (right after `BeginShutdown`): declares `bool EndShutdown();` (0x00F36D30;
  Ego_r.pdb 0x00B3C1D0 / FableWin.pdb 0x0316CC20 `?EndShutdown@CGPUScratchBufferTextureManager@@QAE_NXZ`). Declaration only. Used by
  rows/00b3aff0.cpp.
- pin_asserts: pinned 0. Landed includers: 37; none defines the new macro, so none changes.

## engine/CEngineScreenEffectRadialBlurRenderer.h
- `FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER_RENDER` (MEMBERS mode, just before the existing `private:` ApplyRadialBlur block):
  declares `virtual void Render(const class CEngineComponentRenderParams&);` (0x00B5EA30, the CEngineComponent::Render override, no new
  slot) and, private, `void CaptureEnvironmentParameters();` (0x00B5BD70) / `bool UpdateWorldPositionTracking();` (0x00B5C120): Ego_r.pdb +
  FableWin.pdb spellings. Declarations only. Used by rows/00b5ea30.cpp.
- pin_asserts: pinned 0. Landed includers: 13; none defines the new macro, so none changes.

## engine/CEngine.h
- `FABLE_RETAIL_CENGINE_SETPERSISTENTPRIMITIVE` (FABLE_RETAIL_CENGINE class, right before the `_GETSCRATCHBUFFERTEXTUREMANAGER` block):
  declares the CIEngine slot-23 override `virtual void SetPersistentPrimitive(CEnginePrimitiveHandle&, const CEnginePrimitive&, unsigned long);`
  (0x00B23BC0; Ego_r.pdb / FableWin.pdb `?SetPersistentPrimitive@CEngine@@UAEXAAVCEnginePrimitiveHandle@@ABVCEnginePrimitive@@K@Z`). An override
  of CIEngine's pure slot 23: no layout or vtable change. Used by rows/00b23bc0.cpp.
- pin_asserts: pinned 0. Landed includers: 98; none defines the new macro, so none changes.
