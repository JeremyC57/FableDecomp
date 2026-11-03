# Header needs left open by landing 2026-11-03b

The headers this landing installed are listed in `README.md`. The needs below are **not** installed: no row of this
landing needed them, or they conflict with the one-model rule. Rows that use them must be graded against the installed
headers first.

## Chunk 723 (rounds 2 and 3): what its parked rows must change
- **Render-state tables.** Use the PDB data names now in `engine/CRenderStateManager.h` (FABLE_RETAIL_CRENDERSTATEMANAGER
  branch): `D3DTextureUVModeTable` 0x01396DC8, `D3DAlphaFuncModeTable` 0x01396DD8, `D3DZBufferFuncModeTable` 0x01396DF8,
  `D3DTextureFilterModeTable` 0x01396E58, `D3DTextureMipMapModeTable` 0x01396E64, `D3DBlendOpModeTable` 0x01396ED0,
  `D3DAlphaBlendModeTable` 0x01396F68, `D3DPolyShadeModeTable` 0x01396F9C, `D3DPolyFillModeTable` 0x01396FA4,
  `D3DPolyCullModeTable` 0x01396FB0. Do not use the descriptive names `g_textureUVModeToD3D`, `D3DBlendOpTable`,
  `D3DAlphaTestFuncTable`. The enum names come from `engine/RenderStateEnums.h`.
- **CRenderStateManager data is private.** Use the `__forceinline` setters. Every bool setter stores
  `enable ? 1 : 0`, as retail does.
- **AttachTextureToStage.** `CRenderManagerCore::AttachTextureToStage` is declared only by default (0x009DA9F0 calls
  0x009A0CF0 out of line). The water passes that expand it must `#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS`
  before the include. All 35 of 723's landed rows and siblings are RELOCATION_MATCH on the final tree without it.
- **Not installed.** These 723 changes are not installed: typed members of CEngineWaterSettings (they would go under
  `FABLE_RETAIL_CENGINEWATERSETTINGS`), CEngineLightingManager, CEngineCamera, CEngineResourceManager, C2DExtentsF,
  CDisplayManager (beyond `GetD3DDevice`) and CMatrix3x4. Each needs its own class branch with the default unchanged,
  then hdrcheck2 and an includer recheck.
- **CGenericVertex.** The local CGenericVertex model in 723's sources and `engine/CTVertexRHWColSpecTex1.h` describe
  overlapping vertex layouts. Pick one PDB name per layout before anything shares it.

## Chunk 724 round 3
- **NCameraModes_CBase.h is not installed.** No landed row needs it: 0x008771A0 uses `engine/NCameraModes_CTools.h` only.
  Before anything that uses it lands, make it a `FABLE_RETAIL_NCAMERAMODES_CBASE` branch with the default unchanged.

## Chunk 726
- **NUISystem_CScrollBar.h is not installed.** No landed row includes it.

## Codex pass 13
- **Models not installed** (no landed row needs them; VertexShaderBank stays an opaque 0x128 block in
  CRenderManager2D): CBankFile, CShaderDataBank, CChecksumCacheFile, CDateAndTime, CPackedUIntArray,
  CShaderBankStateBlock, CBankStateBlock, CSystemFont.
- **Kept at the tree version:** CVertexShader, CSimpleMemoryPool, CInitBaseClass, CShaderBase, CPixelShader, CVectorMap,
  Direct3D9Types, C2DExtentsF and CVertexCompressionParams. The landed rows compile against these.
- **0x00501450 CWorldMap::GenerateRegionConnectivityGraph stays held.** It needs `CVectorMap::LowerBound` expanded in
  its size()/Compare-functor form. The tree model, shared by every CThing includer, is the end()-first / `operator<`
  form. Solving it needs either a per-TU opt-in proven not to move any other includer, or evidence that retail has two
  LowerBound forms.
