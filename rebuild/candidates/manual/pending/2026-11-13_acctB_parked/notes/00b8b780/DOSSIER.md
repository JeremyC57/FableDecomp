# DOSSIER 0x00B8B780 CEngineBillboardGenerator::RenderBillboard (3921 bytes) -- identity scout

## 1. Retail shape (xdis 00b8b780 0xf61, $W/dis_all.txt)
- thiscall, `ret 14h` (5 dword args), returns void (0x00B8C6CE).
- Prologue `push ebp ; lea ebp,[esp-64h] ; sub esp,1A0h` = the BIASED ebp frame. ebp = esp_entry-0x68; args at
  [ebp+6Ch] mesh, [ebp+70h] billboard, [ebp+74h] float, [ebp+78h] float, [ebp+7Ch] long. Locals ebp-0x13C .. ebp+0x5F.
- /GS: `mov eax,[0139C8A8h] (__security_cookie) ; mov [ebp+60h],eax` at entry; epilogue `mov ecx,[ebp+60h] ; pop edi/esi/ebx ;
  call 0x00BFE9F9 (__security_check_cookie) ; add ebp,64h ; mov esp,ebp ; pop ebp ; ret 14h`. Saved regs pushed AFTER the first
  block: ebx, esi at 0x00B8B7B0/B1, edi at 0x00B8B7DB (interleaved with code). Arrays in the frame (CTVertexRHWColTex1[4],
  unsigned short[6]) are the /GS triggers.
- NO SEH / EH frame (no fs:0) although the frame holds objects with destructors (CRenderTarget, CSurface temporaries,
  CRenderStateScopedCaptureBlock, CScopedShadersEnable): build without /EHsc (lane default, `// Flags: none`, like the
  sibling vector-deleting dtor 0x00B8C790).
- Why a frame pointer at all: the function contains the GFFloatToLongFloor inline-asm block (0x00B8BED8..0x00B8BEFB `fld [ebp+48h] ;
  fsub [ebp+40h] ; fistp dword [ebp+4Ch] ; fild ; fadd [ebp-8Ch] ; fstp [ebp-84h] ; mov eax,[ebp+48h] ; cmp ; jne ; inc [ebp+4Ch]`),
  which forces ebp. Every landed GFFloatToLongFloor site with a small frame (0x00B46A30, 0x00B52990, 0x00A4DEA0, 0x00A52650) uses
  `#pragma optimize("y", off)` and gets `mov ebp,esp`; the BIASED form (`lea ebp,[esp-N]`) is what the lane gets from
  `#pragma optimize("s", on)` (NPlayerGuiCDrawPerceivers_Draw_0064499c.cpp note: "s on alone gives the biased frame (adding y off
  gives mov ebp,esp)"; 10+ CTCInventory rows). BUT this row is speed-shaped: 16-aligned entry, loop-alignment pad `lea esp,[esp]`
  at 0x00B8C13C before the loop head 0x00B8C140 (size TUs do not align loops). FIRST PROBE: no optimize pragma at all (asm forces
  ebp, /O2 may bias a frame this large); then optimize("s",on); LANDING_QUEUE 0x00581B33 / 0x005786B5 record biased-frame gaps.
- Size idioms present: `rep movsd` (12 dwords) for two CMatrix3x4 copies (0x00B8B969, 0x00B8B9C7) -- also /O2 behaviour for 48-byte
  struct copies, so not proof of optimize("s").
- GFFloatToLongFloor is asm (Retail-asm): the row must include engine/GFFloatToLongFloor.h and carry a `// Retail-asm:` line
  for 0x00B8BEDE `fistp dword ptr [ebp+4Ch]` (ASM_RULES per-site table). Its 0.5f / 1.0f frame constants are [ebp+40h] (stored
  0x00B8BEB0) and [ebp-8Ch] (stored 0x00B8BEBA).

## 2. Name, class, signature (two+ sources)
- Leaf: `?RenderBillboard@CEngineBillboardGenerator@@QAEXPBVC3DMesh2@@PAVCEngineBillboard@@MMJ@Z`
  = `void CEngineBillboardGenerator::RenderBillboard(const C3DMesh2*, CEngineBillboard*, float, float, long)` (public, non-virtual,
  non-const). Sources: (a) ghidra_out/egor_pdb_names.tsv 00a32ca0 (Ego_r.pdb); (b) ghidra_out/_fablewin_api.tsv +
  naming_stragglers/FableWin.exe.signatures.tsv 02dbe530 (FableWin.pdb, debug size 2558); (c) shape: 5 dword args = `ret 14h`
  with types matching use (arg1 read as C3DMesh2: BoundingSphere +0x10, Materials +0x8C, PrimitiveCount +0x90, Primitives +0x94;
  arg2 as CEngineBillboard: Texture +0, ZBuffer +4, Offset +8, Scale +0x14 = Ego_r layout exactly; arg3/arg4 used with fmul/fld
  = float; arg5 fed to SetAlphaRef = long); (d) `this` reads +0x10 VertexShader / +0x18 PixelShader = Ego_r CEngineBillboardGenerator;
  (e) sole caller CEnginePrimitiveManagerRepeatedZSprites::RenderFromSortList (0x00BAA870, not landed) calls it at 0x00BAA9A4 with
  ecx = [0x01436E4C] = g_pEngineBillboardGenerator (CEngine_Initialise_00b26360.cpp:117). Not virtual (no vtable slot).
- Manifest label `CEngineBillboardGenerator::RenderBillboard` is CORRECT.
- Parameter names: not recovered (no PDB locals dumped). Descriptive: mesh, billboard, arg3 (screen/size factor, multiplies),
  arg4 (distance-like, divides), alphaRef (to SetAlphaRef). Mip float = FrameWidth * prim.AverageTextureStretch * arg3 /
  (arg4 * MipmapSizeMultiplier).

## 3. Callees (retail addr -> landed row; PDB spelling; flags)
| addr | landed row | PDB name / note |
|---|---|---|
| 0x0099A2D0 | 00/99/CBaseClass_CBaseClass_0099a2d0.cpp | ??0CBaseClass (CRenderTarget's inline ctor calls it). BeginOutlineRendering 0xb89a20 calls it "CBase" in comments -- PDB is CBaseClass |
| 0x0099A2E0 | 00/99/CBaseClass_CBaseClass_0099a2e0.cpp | ??1CBaseClass@@UAE@XZ (end of inline ~CRenderTarget) |
| 0x009F9DB0 | CTexture_GetDimensions_009f9db0 | ?GetDimensions@CTexture@@QBE?AVC2DExtentsI@@K@Z (hidden result; `.X` read) |
| 0x009F9E00 | CTexture_GetSurfaceLevel_009f9e00 | ?GetSurfaceLevel@CTexture@@QBE?AVCSurface@@J@Z (CSurface by value) |
| 0x009F9DF0 | CTexture_GetNoLevels_009f9df0 | ?GetNoLevels@CTexture@@QBEJXZ (called twice: pre-test + loop bottom) |
| 0x00B27EE0 | CEngine_GetScratchZBufferD24S8_00b27ee0 | ?GetScratchZBufferD24S8@CEngine@@QAEPAUCSurfaceNative@@K@Z, on [0x01436EA4] |
| 0x00A10E90 | CRenderTarget_AttachTarget_00a10e90 | **NAME FIX**: landed "AttachTarget" (depth overload) -- FableWin ?AttachZBuffer@CRenderTarget@@QAEXABVCSurface@@@Z (writes +0x14 ZBufferSurface, Ego_r CRenderTarget) |
| 0x00A10E60 | CRenderTarget_AttachTarget_00a10e60 | ?AttachTarget@CRenderTarget@@QAEXABVCSurface@@@Z (+4 RenderToSurface) |
| 0x00A10E10 | CRenderTarget_DetachTarget_00a10e10 | ?DetachTarget@CRenderTarget@@QAEXXZ |
| 0x009F2E20 | CSurface_Uninitialise_009f2e20 | ?Uninitialise@CSurface@@QAEXXZ (inline ~CSurface: vtable 0x0122F84C store + this call) x4 |
| 0x009BF160 | CDisplayManager_SetRenderTarget_009bf160 | ?SetRenderTarget@CDisplayManager@@QAE_NAAVCRenderTarget@@@Z = bool(CRenderTarget&). BeginOutlineRendering 0xb89a20 declares `void SetRenderTarget(CRenderTarget*)` -- disagrees with PDB |
| 0x009D8CF0 | (none; functions.tsv ClearRenderTarget/CRenderManager2D) | ?ClearRenderTarget@CRenderManager2D@@QAEXABVCRGBColour@@K@Z on GPRenderManager 0x01436E18 (engine/CRenderManager2D.h:65). 0xb89a20 calls it "CRenderManagerCore::Clear" -- wrong |
| 0x00988350 | CShaderRenderManager_SetViewTransform | ?SetViewTransform@CShaderRenderManager@@QAEXABVCMatrix3x4@@@Z |
| 0x00988490 | CShaderRenderManager_SetProjectionTransform | ?SetProjectionTransform@CShaderRenderManager@@QAEXMMMMMMMMMMMMMMMM@Z (16 floats, identity) |
| 0x009881F0 | CShaderRenderManager_SetWorldTransform | ?SetWorldTransform@CShaderRenderManager@@QAEXABVCMatrix3x4@@@Z |
| 0x00988020 | CShaderRenderManager_ApplyVertexShader | ?ApplyVertexShader@CShaderRenderManager@@QAEXABVCVertexShader@@_N@Z (bool = false: `push ebx`, ebx=0) |
| 0x00988140 | CShaderRenderManager_ApplyPixelShader | ?ApplyPixelShader@CShaderRenderManager@@QAEXABVCPixelShader@@@Z |
| 0x0098B930 0x00988A20 0x00989BF0 0x00989760 0x009897C0 0x0098A760 0x0098A540 0x00988A50 | CShaderRenderManager_* | SetVSConstants / UpdatePixelShader / UpdatePixelShaderConstants / UpdateAmbient / UpdateFogColour / UpdateLightGlobals / UpdateWorldSpaceLighting / UpdateCombinedProjectionTransform: all private (AAE), reached only through the inline PrepareForRender(flags) |
| 0x00BA48F0 | CEnginePrimitiveManagerStaticMeshesBase_SetupStaticBlocksNoMorph | static __fastcall bool (C3DPrimitive2&, const C3DMesh2&, C3DTextureStages&, long, const CGraphicDataBank*, bool) |
| 0x00B9D670 | ..._SetupFirstTextures | static __fastcall void (C3DPrimitive2&, C3DTextureStages&) |
| 0x00BA1110 | ..._RenderMeshPrimitiveBlocks | ?RenderMeshPrimitiveBlocks@...@@SIXAAVC3DPrimitive2@@ABVC3DTextureStages@@_N2@Z (false,false) |
| 0x009880E0 / 0x00988190 / 0x00987FE0 / 0x00988110 | CShaderRenderManager_Disable/Enable{Vertex,Pixel}Shaders | QAEXXZ on SingletonInstance 0x013BC470 (CScopedShadersEnable inline) |
| 0x00A0AA30 | CRenderManagerCore_SetVertexFormat | ?SetVertexFormat@CRenderManagerCore@@QAEXK@Z (0x144) |
| 0x00A0AEA0 | CRenderManagerCore_DrawAsIndexedTris | ?DrawAsIndexedTris@CRenderManagerCore@@QAEXPAPBVCTexture@@KPBXJJPAXJ@Z (7 args) |
| 0x009BF220 | CDisplayManager_SetRenderTargetAsBackScreen | ?SetRenderTargetAsBackScreen@CDisplayManager@@QAE_NXZ |
| 0x00B2FC50 | CEngineCamera_RefreshD3DTransforms | ?RefreshD3DTransforms@CEngineCamera@@QAEXXZ on g_pEngineCamera 0x01436EA0 |
| 0x00A05840 | CRenderStateManager_RestoreCaptureBlock | ?RestoreCaptureBlock@CRenderStateManager (inline ~CRenderStateScopedCaptureBlock) on 0x013CB508 |
| 0x00BFE9F9 | global_securitycheckcookie | __security_check_cookie |
| vtbl+0x190 (x2), +0x1A0, +0x104 | IDirect3DDevice9 | SetStreamSource / SetIndices / SetTexture (inline SelectVertexBuffer, FlushRender) |

## 4. Members / globals touched
- this (CEngineBillboardGenerator): +0x10 VertexShader (CVertexShader), +0x18 PixelShader (CPixelShader) (Ego_r). engine/CEngineBillboardGenerator.h
  FABLE_RETAIL_CENGINEBILLBOARDGENERATOR keeps them opaque (`_pad_0x10[0x10]`): needs an overlay opt-in naming them, or local model.
  The landed ctor/dtor rows (0x00B8B630, 0x00B8C7C0) model CShaderBase/CVertexShader/CPixelShader LOCALLY because CVertexShader.h /
  CEngineBillboardGenerator.h are flattened donors -- DisplayMirroredTextures uses engine/CVertexShader.h + CPixelShader.h with
  FABLE_RETAIL macros; copy that.
- C3DMesh2 (engine/C3DMesh2.h; base C3DMeshStats via FABLE_RETAIL_C3DMESH2_HELPERPOINTS): BoundingSphere +0x10 (C3DBoundingSphere: Center
  +0, Radius +0xC; private in C3DMeshStats -> FableWin ?GetBoundingSphere@C3DMeshStats@@QBE?AVC3DBoundingSphere@@XZ by VALUE, retail
  copies the 16 bytes to [ebp+34h..40h]); MaterialCount... Materials +0x8C (C3DMeshMaterial, 0x30), PrimitiveCount +0x90,
  Primitives +0x94 (C3DPrimitive2, retail 0xA0 bytes). FABLE_RETAIL_STATICBLOCKS_ORIGINAL_ACCESSORS gives GetMaterial(i).
- C3DPrimitive2 (engine/C3DPrimitive2.h): MaterialIndex +4, AverageTextureStretch +0x1C (float), VertexBuffer +0x30
  (scoped_ptr<CVertexBufferWin32>; retail 4 lower than Ego_r 0x34). CVertexBufferWin32: PVertexBuffer +4, Stride (byte) +0x10.
- C3DMeshMaterial: DiffuseMapBankIndex +0x10 (ulong).
- CEngineBillboard (no header; Ego_r 0x18): Texture +0 (CTexture*), ZBuffer +4 (CSurface*), Offset +8 (C3DVector), Scale +0x14 (float).
- CRenderTarget (no header; Ego_r 0x24 : CBaseClass): RenderToSurface +4, ZBufferSurface +0x14 (CSurface). vtable 0x0129C800. Local
  model exists in CEngineScreenEffectOutlineGlow_BeginOutlineRendering_00b89a20.cpp (copy it, fix SetRenderTarget signature).
- CSurface (engine/CSurface.h FABLE_RETAIL_CSURFACE; vtable 0x0122F84C; PD3DSurface +4, AllocationSource +8, PAllocatedMemory +0xC;
  inline ctor zeroes +4/+8 only; inline virtual dtor = vtable store + Uninitialise). FABLE_RETAIL_CSURFACE_FORCEINLINE_DTOR exists.
- CDisplayManager (0x01436E1C NEngineInternal::GPDisplayManager): SetRenderTarget not declared in engine/CDisplayManager.h (overlay add);
  FABLE_RETAIL_CDISPLAYMANAGER_BACKSCREEN gives SetRenderTargetAsBackScreen.
- CRenderStateManager (engine/CRenderStateManager.h FABLE_RETAIL_CRENDERSTATEMANAGER, DeviceState at +0x2818): every `[eax+28xx]` block
  is the inline SetRenderState(value, DeviceState.X) (CStateInfo 0x14: Current +0, Desired +4, BookmarkMask +8, InList byte +0x10;
  manager CurrentBookmarkMask +0x2814, captured list +0x08/+0x2808, update list +0x2008/+0x280C). Offsets:
  +0x28CC AlphaTestEnable, +0x28B8 AlphaBlendEnable, +0x28E0 AlphaRef, +0x28A4 AlphaTestFunc, +0x2944 FogEnable, +0x2868 ZBufferEnable,
  +0x3344 TextureArgument1[0], +0x3204 TextureOperation[0], +0x3484 TextureAlphaArgument1[0], +0x32A4 TextureAlphaOperation[0],
  +0x2DA4 TextureMipmapLODBias[0]. Tables: 0x01396DE8 = D3DAlphaFuncModeTable[ALPHA_TEST_GREATER=4], 0x01396EF0 =
  D3DTextureArgumentModeTable[TEXTURE_ARG_TEXTURE=2], 0x01396E74 = D3DTextureOperationModeTable[TEXTURE_OP_SELECT_ARG1=1].
- CRenderManagerCore (GPRenderManager 0x01436E18): PD3DDevice +0x3CF0, TextureStagesInUse +0x3CB8, CurrentlySelectedTextures +0x3D00
  (FlushRender inline). Macros FABLE_RETAIL_CRENDERMANAGERCORE, FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS,
  FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASINDEXEDTRIS_USER (ColourFilter row) -- check which DrawAsIndexedTris overload the latter declares.
- CShaderRenderManager (engine/CShaderRenderManager.h, GPShaderRenderManager 0x01436E14): CurrentVSConstantLayout +4, D3DDevice +0x1D4,
  PSConstTable +0x3CC (C4DVector array; index 2 = +0x20), UpdateFlags +0x3D8; SingletonInstance 0x013BC470 with VertexShadersEnabled
  +0x390 (=0x013BC800), PixelShadersEnabled +0x391 (=0x013BC801). PrepareForRender / SetPixelShaderConstant / SetVertexShadersEnable
  are header inlines (FABLE_FORCEINLINE).
- CEngineCamera (g_pEngineCamera 0x01436EA0): NoScaleViewTransform +0x114 -- FABLE_RETAIL_CENGINECAMERA + _NOSCALEVIEWTRANSFORM;
  RefreshD3DTransforms via FABLE_RETAIL_CENGINECAMERA_REFRESHD3DTRANSFORMS.
- CEngineResourceManager (NEngineInternal::GPResourceManager 0x01436E98): GraphicBanks[0] +0x954; PeekGraphicBank(EGraphicBankNo) inline.
  CGraphicDataBank::GraphicList (vector of CGraphicInfo 0x2C, begin +0x1E0 / end +0x1E4); CGraphicInfo.Frames +0x28;
  CGraphicInfoFrame.GraphicInfo +0xC; CGraphicHeader.FrameWidth +6 (ushort). PeekGraphic / PeekGraphicInfoFrame bodies are written
  in SetupStaticBlocksNoMorph_00ba48f0.cpp lines 31-38 (FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS) -- the exact double range check
  (`cmp ; jae` then `sbb ; and`) of retail 0x00BAE68..0x00BAE99.
- Primitive renderer (0x01436E78, g_pPrimitiveRenderer): MipmapSizeMultiplier +0xB0 (float; Ego_r CEnginePrimitiveRendererSharedData,
  offsets equal before +0x268) -- not in engine/CEnginePrimitiveRendererSharedData.h FABLE_RETAIL_..._MEMBERS (overlay add).
  PDB accessor: FableWin ?GetMipmapSizeMultiplier@CEnginePrimitiveRenderUtil@@SIMXZ (static fastcall float, a base-class helper; inline).
- 0x0143B990: 256-byte top-bit table (landed spelling g_highestSetBitTable, descriptive; filled by CEngineInlineTableBuilder ctor
  0x00BE2A80). Accessor = FableWin ?GFGetTopBitInByte@@YIKE@Z (inline, unsigned long __fastcall (unsigned char)).
- Constants: 1.0f 0x0122DED8, 0.0f 0x0122DEDC, 0.5f 0x0122F59C, -2.0f 0x01246050.

## 5. Visibility / inline expansions (all no out-of-line call in retail)
- CRenderStateScopedCaptureBlock (engine/CRenderStateCaptureBlock.h): ctor = StartCaptureBlock(this) expansion 0x00B8B7AB..0x00B8B7DF,
  dtor = RestoreCaptureBlock call 0x00B8C6B8 (last).
- CScopedShadersEnable(false,false) (engine/CScopedShadersEnable.h) at [ebp+4Ch]: ctor 0x00B8C0EF..0x00B8C116, dtor 0x00B8C64F..0x00B8C68A.
- CRenderTarget ctor/dtor inline; CSurface ctor/dtor inline (temporaries at [ebp+50h], [ebp-0CCh]).
- CShaderRenderManager::PrepareForRender(flags) inline 0x00B8BF80..0x00B8C029: tests present = 4(SetVSConstants), 1, 2, 4(Ambient),
  0x20000, 0x400, 0x200 (WorldSpaceLighting), 0x80 -> flags has 0x10 and 0x1, none of 0x400/0x40/0x80/0x200/0x100/0x8/0x20/0x4:
  flags = 0x11 (bit 2 is unused by PrepareForRender, so 0x13 is byte-identical).
- CShaderRenderManager::SelectVertexBuffer(CVertexBufferWin32&) (0x00B8BF5B..0x00B8BF7A) and CRenderManagerCore::FlushRender()
  (0x00B8C06A..0x00B8C0E0): bodies in DisplayMirroredTextures_00ba1b00.cpp lines 76-109 (FABLE_FORCEINLINE, same TU style).
- SetPixelShaderConstant(2, C4DVector(1,1,1,1)) inline 0x00B8BD90..0x00B8BDD4 (temp C4DVector at [ebp+50h]).
- CMatrix3x4 inlines: SetIdentity (FABLE_RETAIL_CMATRIX3X4_SETIDENTITY: zeros stored E43..E12 then E33,E22,E11 -- exact match at
  0x00B8B898..0x00B8B906), copy (`rep movsd`), multiply (see OUTLINE), GetTranslation.
- GFFloatToLongFloor (asm header), GFGetTopBitInByte, PeekGraphicInfoFrame, GetBoundingSphere, GetMipmapSizeMultiplier.
- Same-TU neighbours: ctor 0x00B8B630 (just before), PeekSceneFilterFlags 0x00B8C780, ??_E 0x00B8C790, ??_G 0x00B8C7A0, ??1 0x00B8C7C0
  (after). SetupBillboardTextures (FableWin 13 bytes) has no retail copy (inline, not called here).

## 6. Sibling / idiom rows
- CEngineScreenEffectColourFilter_RenderColorFilteredQuad_00b5af80.cpp: closest idiom donor (capture block + CScopedShadersEnable + render
  state setters + DrawAsIndexedTris + GFFloatToLongNear asm); macros FABLE_RETAIL_SCOPED_PTR, _CDISPLAYMANAGER(+_BACKSCREEN),
  _CRENDERMANAGERCORE(+_FORCEINLINE_HELPERS, _DRAWASINDEXEDTRIS_USER), _CRENDERSTATEMANAGER_STENCIL_INLINE, _CTEXTURE, _CRGBFLOATCOLOUR,
  _CPIXELFORMAT, _CENGINECAMERA(+_REFRESHD3DTRANSFORMS). No pragmas, no Flags line.
- CEnginePrimitiveManagerStaticMeshesBase_DisplayMirroredTextures_00ba1b00.cpp: PrepareForRender / SelectVertexBuffer / FlushRender /
  ApplyVertexShader+ApplyPixelShader; macros FABLE_RETAIL_CRGBFLOATCOLOUR, _CRENDERMANAGERCORE_FORCEINLINE_HELPERS,
  _CENGINEPRIMITIVERENDERUTIL_SETTEXTUREFACTOR, _CENGINEPRIMITIVERENDERERSHAREDDATA_MEMBERS, _CENGINEPRIMITIVERENDERER.
- CEnginePrimitiveRenderUtil_SetupOutlineSmoothAlphaStencilMaskStates_00bf01e0.cpp: the same SetAlphaTestEnable / AlphaBlendEnable /
  AlphaRef / AlphaTestFunc(GREATER) / FogEnable run with FABLE_RETAIL_CRENDERMANAGERCORE + _CRENDERSTATEMANAGER_OUTLINE_INLINE.
- CEngineScreenEffectOutlineGlow_BeginOutlineRendering_00b89a20.cpp (same 0x00B8xxxx neighbourhood): local CSurface / CRenderTarget /
  CRGBColour models (fix the SetRenderTarget / ClearRenderTarget spellings).
- CEngineLightingManager_CalculateFogTransformMatrix_00b451d0.cpp: CMatrix3x4 12-float ctor + PreMultiply over NoScaleViewTransform
  (FABLE_RETAIL_CMATRIX3X4_PREMULTIPLY); CEngineLightingManager_LookupSmallObjectLightCache_00b46a30.cpp: GFFloatToLongFloor.h user
  with `#pragma optimize("y", off)`.
- The billboard ctor/dtor rows 0x00B8B630 / 0x00B8C7C0 (local shader models; `// Flags: none` on 0x00B8C790).

## NAME_FIXES candidates
- 0x00A10E90 CRenderTarget::AttachTarget -> CRenderTarget::AttachZBuffer (FableWin ?AttachZBuffer@CRenderTarget@@QAEXABVCSurface@@@Z; writes ZBufferSurface +0x14).
- 0x009BF160 declared `void SetRenderTarget(CRenderTarget*)` in 0x00B89A20 -> PDB `bool SetRenderTarget(CRenderTarget&)`.
- 0x009D8CF0 called `CRenderManagerCore::Clear` in 0x00B89A20 -> `CRenderManager2D::ClearRenderTarget(const CRGBColour&, unsigned long)`.
