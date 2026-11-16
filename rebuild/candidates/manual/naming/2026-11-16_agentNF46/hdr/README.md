# NF46 header overlays (round 7)

Each file is a copy of the repo header of the same path (`rebuild/include/<path>`, as of 2026-11-16) plus the opt-ins below.
Every addition is inside `#ifdef FABLE_RETAIL_<CLASS>_<MEMBER>`; with none of these macros defined each header is token-identical to
the repo copy, except the two existing-line edits called out under CEnginePrimitiveTypeInfo.h and CEngineWorldMap.h (both keep the
original tokens when the new macro is undefined). Merge order with NF45: the macro names are member-specific, so blocks can be
merged side by side.

## engine/CEngine.h
- `FABLE_RETAIL_CENGINE_GETSCRATCHBUFFERTEXTUREMANAGER`: `virtual CGPUScratchBufferTextureManager* GetScratchBufferTextureManager();`
  (Ego_r / FableWin `?GetScratchBufferTextureManager@CEngine@@UAEPAVCGPUScratchBufferTextureManager@@XZ`, retail 0x00B23F60). Row 0x00B23F60.
- `FABLE_RETAIL_CENGINE_GETMEMORYPOOLS`: `virtual CMovableResourceMemoryPool* GetLoadingMemoryPool();` / `GetAnimationMemoryPool();`
  (`?GetLoadingMemoryPool@CEngine@@UAEPAVCMovableResourceMemoryPool@@XZ` 0x00B25C80, `?GetAnimationMemoryPool@CEngine@@UAE...` 0x00B25CC0).
  Rows 0x00B25C80 / 0x00B25CC0. Both are CIEngine overrides (declarations only, retail branch, no new slot).

## engine/CEngineResourceManager.h (retail branch)
- `FABLE_RETAIL_CENGINERESOURCEMANAGER_GETLOADINGMEMORYPOOL`: inline `CCountedPointer<CMovableResourceMemoryPool> GetLoadingMemoryPool()`
  (FableWin `?GetLoadingMemoryPool@CEngineResourceManager@@QAE?AV?$CCountedPointer@VCMovableResourceMemoryPool@@@@XZ`, +0x8E0).
- `FABLE_RETAIL_CENGINERESOURCEMANAGER_PEEKANIMATIONMEMORYPOOL`: inline `CCountedPointer<CMovableResourceMemoryPool> PeekAnimationMemoryPool() const`
  (FableWin `?PeekAnimationMemoryPool@CEngineResourceManager@@QBE?AV?$CCountedPointer@VCMovableResourceMemoryPool@@@@XZ`, +0x8F0).
  Outside `_CTOR` the two members are the header's opaque `unsigned char[8]`, so the bodies read them with `reinterpret_cast` of the member
  storage, as the existing PeekVSConstantLayoutRepeatedMesh / LightsBones accessors in the same header do (no address cast). Under
  `_CTOR` they return the typed members. Rows 0x00B25C80, 0x00B25CC0, 0x00B42750, 0x00B2C8E0.

## engine/CHandleBasedMemoryPool.h (FABLE_RETAIL_CHANDLEBASEDMEMORYPOOL branch)
- `FABLE_RETAIL_CHANDLEBASEDMEMORYPOOL_ALLOCATE`: `bool Allocate(long*, unsigned long, unsigned long);` and `void Free(long);`
  (FableWin `?Allocate@CHandleBasedMemoryPool@@QAE_NPAJKK@Z` 0x00A03180, `?Free@CHandleBasedMemoryPool@@QAEXJ@Z` 0x00A02B30). Declarations only.
  Row 0x00B2C8E0.

## engine/CGPUScratchBufferTextureManager.h
- `FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_ALLOCATEDEPTHBUFFERSURFACE`: `long AllocateDepthBufferSurface(long, const C2DExtentsI&,
  const CPixelFormat&, ESurfaceMultisampleType);` (Ego_r / FableWin `?AllocateDepthBufferSurface@CGPUScratchBufferTextureManager@@QAEJJABVC2DExtentsI@@ABVCPixelFormat@@W4ESurfaceMultisampleType@@@Z`,
  retail 0x00F37390) plus, under the same macro, `#include "engine/ESurfaceMultisampleType.h"` above the class. Row 0x00BABB50.

## engine/CEngineWorldMap.h (retail branch)
- `FABLE_RETAIL_CENGINEWORLDMAP_VISIBLEMAPLIST`: `std::list<CEngineMap*> VisibleMapList; // +0x2c` after MapList. EDIT OF EXISTING LINES: the two
  existing pads `_pad_0x2c[0x16c]` (GAMEWORLD branch) and `_pad_0x2c[0x194]` (plain branch) are wrapped as
  `#ifdef ..._VISIBLEMAPLIST  _pad_0x30[0x168] / [0x190]  #else <original line>  #endif`, so without the macro the tokens are the original.
  Rows 0x00B599C0, 0x00B59A30, 0x00B684F0.
- `FABLE_RETAIL_CENGINEWORLDMAP_GAMEWORLDMAP` (needs `_GAMEWORLD`): `const CIDrawWorldMap* PeekGameWorldMap() const` (FableWin
  `?PeekGameWorldMap@CEngineWorldMap@@QBEPBVCIDrawWorldMap@@XZ`) and the typed member `CIDrawWorldMap* GameWorldMap; // +0x19c`
  + `_pad_0x1a0[0x20]`. EDIT OF AN EXISTING LINE: `_pad_0x19c[0x24]` is wrapped `#ifdef ..._GAMEWORLDMAP <typed> #else <original> #endif`.
  Row 0x00B401E0.

## engine/CEnginePrimitiveTypeInfo.h
- `FABLE_RETAIL_CENGINEPRIMITIVETYPEINFO_GETPRIMITIVEMANAGER`. EDIT OF AN EXISTING LINE: the existing
  `#if defined(FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED) || defined(FABLE_RETAIL_CENGINEPRIMITIVEMANAGERTEXT_RENDER)` gains
  `|| defined(FABLE_RETAIL_CENGINEPRIMITIVETYPEINFO_GETPRIMITIVEMANAGER)` (plus a comment), exposing the same FableWin inline
  `?GetPrimitiveManager@CEnginePrimitiveTypeInfo@@QBEPAVCEnginePrimitiveManager@@J@Z` without the other two opt-ins. Without the new macro the
  condition is unchanged. Rows 0x00B917E0, 0x00BBC130, 0x00BBCF70, 0x00BC5A20, 0x00BC65D0, 0x00BE2310, 0x00BE23F0 (entry 33).

## engine/CEnginePrimitiveRendererSharedData.h
- `FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_DTOR`: `virtual ~CEnginePrimitiveRendererSharedData();` (retail 0x00B36EC0) in the
  RENDERSETTINGS branch. Row 0x00B33520.

## engine/CMeshDataBank.h
- `FABLE_RETAIL_CMESHDATABANK_PEEKREPEATEDMESH`: `const C3DMeshInfo* PeekRepeatedMesh(unsigned long) const;` (Ego_r / FableWin
  `?PeekRepeatedMesh@CMeshDataBank@@QBEPBVC3DMeshInfo@@K@Z`, retail 0x00A24290, landed CMeshDataBank_PeekRepeatedMesh_00a24290.cpp). Row 0x00BE27B0.
- `FABLE_RETAIL_CMESHDATABANK_PEEKPARTICLEMESH`: `const C3DMeshInfo* PeekParticleMesh(unsigned long) const;` (Ego_r / FableWin
  `?PeekParticleMesh@CMeshDataBank@@QBEPBVC3DMeshInfo@@K@Z`, retail 0x00A242A0). Row 0x00BC2400.

## engine/CEngineLandscapeRenderer.h
- `FABLE_RETAIL_CENGINELANDSCAPERENDERER_ONRELEASEDEFAULTPOOLRESOURCES`: `virtual void OnReleaseDefaultPoolResources();`
  (Ego_r `?OnReleaseDefaultPoolResources@CEngineLandscapeRenderer@@UAEXXZ`, retail 0x00B676A0). Row 0x00B676A0.
