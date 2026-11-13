# DOSSIER 0x00BAA870 CEnginePrimitiveManagerRepeatedZSprites::RenderFromSortList (4600 bytes)

## 1. Identity (manifest label is RIGHT)
- Mangled: `?RenderFromSortList@CEnginePrimitiveManagerRepeatedZSprites@@AAEXAAVCEngineInternalPrimitiveRepeatedZSprite@@@Z`
  = private, non-virtual, `void RenderFromSortList(CEngineInternalPrimitiveRepeatedZSprite& sprite)` (not const).
- Sources: (a) ghidra_out/egor_pdb_names.tsv Ego_r 0x00A61800, in donor order CreateInternalPrimitive 0x00A61050 / AddRepeatedZSprite /
  ctor 0x00A61170 / dtor / **RenderFromSortList 0x00A61800** / GetSupportedTypes / RenderPrimitive / RenderNonsortedPrimitives / FrameEnd,
  which is exactly retail's order 0x00BAA820 CreateInternalPrimitive -> **0x00BAA870** -> 0x00BABA70 -> ctor 0x00BABB50 ... -> 0x00BAC0F0
  RenderNonsortedPrimitives; (b) FableWin.pdb same mangled name (ghidra_out/_fablewin_api.tsv; FableWin body 0x02ED3E30, 3250 b / 845 insns,
  debug); (c) only caller = 0x00BAC1E8 inside landed RenderNonsortedPrimitives (rebuild/src/compiled/00/ba/..._RenderNonsortedPrimitives_00bac0f0.cpp
  names it "RenderFromSortList 0x00BAA870"). Not a vtable slot.
## 2. Retail shape
- `sub esp,124h` FIRST, then `push ebp; push edi` (no frame pointer: /Oy on), `push ebx; push esi` only after the IsLoaded early-out
  (0x00BAA8AD/0x00BAA8B2): shrink-wrapped. Frame 0x124. `ret 4`. thiscall (this -> ebp, saved at [esp+60h] = [esp+68h] after 4 pushes).
- NO SEH/EH frame although the function owns destructible locals (CCountedPointer<const C3DMesh2>, CScopedShadersEnable,
  CScopedVertexShaderConstantLayout, CRenderStateScopedCaptureBlock): the TU is built WITHOUT /EHsc (same TU's ctor 0x00BABB50 and dtor
  0x00BABE60 have no EH frame either). `// Flags: none` (as sibling 0x00BBC460). No /GS cookie.
- Four epilogues (0x00BAA89F, 0x00BAA932, 0x00BABA49, 0x00BABA5B); the last two are the tail-duplicated ~CScopedShadersEnable branch.
- x87 heavy: fsqrt at 0x00BAB0D7 (`#pragma intrinsic(sqrt)` + <math.h> as sibling IsInsideFrustum 0x00BAA4E0), fcos at 0x00BAB159
  (intrinsic cos), `call _CIpow 0x00BFEB40` twice (pow(float,float) -> CRT _CIpow; `pow` NOT intrinsic: toolchain-library callee).
- Size idioms: none of optimize("s"); `lea ecx,[ecx]` / `lea ebx,[ebx]` alignment pads before loops (0x00BAA8BD, 0x00BAB93A).

## 3. Callees (address | landed row | PDB name | note)
| addr | landed file (rebuild/src/compiled/) | PDB / correct spelling |
|---|---|---|
| 0x00A23E40 | 00/a2/C3DMeshInfo_IsLoaded_00a23e40.cpp | ?IsLoaded@C3DMeshInfo@@QBE_NXZ |
| 0x00A256F0 | 00/a2/C3DMeshInfo_BeginLoading_00a256f0.cpp | ?BeginLoading@C3DMeshInfo@@QBEXK_N0@Z (called (0,false,false) twice) |
| 0x00A25E70 | 00/a2/C3DMeshInfo_GetMesh_00a25e70.cpp | ?GetMesh@C3DMeshInfo@@QBE_NAAV?$CCountedPointer@$$CBVC3DMesh2@@@@KK@Z |
| 0x00A36EC0 | 00/a3/Destval...COptimisedPrimitive_00a36ec0.cpp (COMDAT-folded name) | ??1?$CCountedPointer@$$CBVC3DMesh2@@@@QAE@XZ called OUT OF LINE (twice) -> CCountedPointer dtor must NOT expand: use CCountedPointer.h's FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY (or VISIBLE_CALLED_DTOR), as agent2059's ParticleMeshGroup row (it EXPANDED it there -- check which; here it is a call) |
| 0x00B46C80 | (not landed) | ?UpdateGlobalLightingStates@CEngineLightingManager@@QAEXXZ (twice) |
| 0x00B8B780 | (not landed) | ?RenderBillboard@CEngineBillboardGenerator@@QAEXPBVC3DMesh2@@PAVCEngineBillboard@@MMJ@Z |
| 0x00987FE0/0x009880E0 | 00/98/... | CShaderRenderManager::EnableVertexShaders / DisableVertexShaders (via inline CScopedShadersEnable) |
| 0x00988110/0x00988190 | 00/98/... | EnablePixelShaders / DisablePixelShaders |
| 0x0098B5E0 | 00/98/..._SetVertexShaderConstantLayout | ?SetVertexShaderConstantLayout@CShaderRenderManager@@QAEXJ@Z (via CScopedVertexShaderConstantLayout(2)) |
| 0x00988020 | ApplyVertexShader(CVertexShader*,bool) | ?ApplyVertexShader... (VertexShader +0x20, false) |
| 0x00988140 | ApplyPixelShader | (PixelShader +0x28) |
| 0x00BAA460 | 00/ba/CEngineInternalPrimitiveRepeatedZSprite_GetTransitionFade_00baa460.cpp | ?GetTransitionFade@...@@QBE?AVCEngineFadeDistance@@XZ (hidden-return, result at [esp+20h]) |
| 0x00989A60 | SetUserVertexShaderConstant | ?SetUserVertexShaderConstant@CShaderRenderManager@@QAEXKABVC3DVector@@M@Z (consts 0,3,4,5,6) |
| 0x00989B00 | SetUserVertexShaderConstant | ?...@@QAEXKMMMM@Z (consts 1,2,7,0xB,0xC) |
| 0x00987F70 | 00/98/CPreTransposedBoneMatrix_Set_00987f70.cpp | ?Set@CPreTransposedBoneMatrix@@QAEXABVCMatrix3x4@@@Z |
| 0x009896D0 | SetUserVertexShaderConstants | ?SetUserVertexShaderConstants@CShaderRenderManager@@QAEXKJPAM@Z (8,3,bone) and (0x10, ObjectCount, ObjectsEntries) |
| 0x0098B930,0x00988A20,0x00989BF0,0x00989640,0x00989760,0x009897C0,0x0098A760,0x00988290,0x00988A50 | 00/98/... | PrepareForRender's callees: SetVSConstants, UpdatePixelShader, UpdatePixelShaderConstants, UpdateWorldSpaceFogTransform, UpdateAmbient, UpdateFogColour, UpdateLightGlobals, ResetWorldTransform, UpdateCombinedProjectionTransform |
| 0x00B905A0 | (not landed) | ?PrepareOcclusionTest@CEnginePrimitiveManagerOcclusionBase@@IAEXAAVCEngineInternalPrimitiveOcclusionBase@@W4ERenderPassType@@AAK@Z (this, sprite, 2, query) |
| 0x00B8D090 | 00/b8/E2_E2_00b8d090.cpp "Nop_ret0" | **?StartOcclusionTest@CEngineOcclusionManager@@QAEXXZ** (Ego_r). Landed callers spell it BeginQuery (0x00BE79D0) -> NAME FIX |
| 0x00B8D0A0 | 00/b8/E2_E2_00b8d0a0.cpp "Nop_ret4" | **?EndOcclusionTest@CEngineOcclusionManager@@QAEXK@Z** (Ego_r 0x00A347B0). Landed spell EndQuery(long) -> NAME FIX (param unsigned long) |
| 0x00A058C0 | 00/a0/CRenderStateManager_RealiseRenderState_00a058c0.cpp | ?RealiseRenderState@CRenderStateManager@@QAEXXZ (inline DrawBuffersIndexed, on shaderMgr->RenderManager) |
| 0x00A05840 | 00/a0/..RestoreCaptureBlock | ?RestoreCaptureBlock@CRenderStateManager@@QAEXXZ (~CRenderStateScopedCaptureBlock) |
| 0x00BFEB40 | 00/bf/global_CIpow_00bfeb40.cpp | _CIpow (pow) |
| D3D vtbl | IDirect3DDevice9 +0x190 SetStreamSource, +0x104 SetTexture, +0x1A0 SetIndices, +0x148 DrawIndexedPrimitive (engine/IDirect3DDevice9.h) |

## 4. Globals
- 0x01436E80 g_pEnginePrimitiveRenderer (CEnginePrimitiveRendererSharedData view: BlockOnLoad +0x04, ZSpriteDrawDistanceFactor +0x8C) -- GetTransitionFade row comment; engine/CEnginePrimitiveRendererSharedData.h FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_MEMBERS.
- 0x01436E88 (byte +0x24 gates billboard regeneration; labelled g_pLocalDetailGenerator_01436e88 by 0x00BE2740 -- member unproven, CHECK).
- 0x01436E9C NEngineInternal::GPLightingManager (CEngineLightingManager*; ShadowMode +0x4694, inline SetEnableFadedShadowFactor -> FadedShadowDiffuseFactorEnabled +0xCE / StateUpdateFlags +0xE4 |= 1). Opt-in FABLE_RETAIL_CENGINELIGHTINGMANAGER (+_MEMBERS for ShadowMode).
- 0x01436E4C CEngineBillboardGenerator* (RenderBillboard this). engine/CEngineBillboardGenerator.h (FABLE_RETAIL_CENGINEBILLBOARDGENERATOR).
- 0x013BC470 CShaderRenderManager::SingletonInstance (VertexShadersEnabled 0x013BC800 = +0x390, PixelShadersEnabled 0x013BC801, layout index 0x013BC478 = +8 PeekVertexShaderConstantLayout).
- 0x013CB508 CRenderStateCaptureBlock::StateManager. 0x01436E14 NEngineInternal::GPShaderRenderManager. 0x01436E18 NEngineInternal::GPRenderManager (render-state setters). 0x01436EA0 g_pEngineCamera. 0x01436E98 (g_pDisplayManager per sibling 0x00BAC0F0 / g_pResourceManager per labels -- conflict) +0x2AAC = a CTexture (second texture stage). 0x01436E5C g_pOcclusionManager (CEngineOcclusionManager). 0x013CE558 GFastOneOverSqrtTable.
- CRenderStateManager tables: 0x01396DE8 = D3DAlphaFuncModeTable[4] (=5, GREATER); 0x01396DC8 = D3DTextureUVModeTable[0]; 0x01396E58 = D3DTextureFilterModeTable[0].
- Float consts: 1.0 0x0122DED8, 0.0 0x0122DEDC, 3.0 0x0122DED4, 0.5 0x0122F59C, -0.5 0x01238798, 1/(4pi) 0x012A52C0, TWO_PI (double) 0x0124F2B8,
  0.01 0x01230A5C, 1/64 0x0125B24C (float) and 0x012A2788 (DOUBLE 0.015625), -1/64 0x012A41B0, -1.0 0x0122DEE0, 1/256 = imm 0x3B800000.

## 5. Members (shared headers; use them, no local pads)
- this = engine/CEnginePrimitiveManagerRepeatedZSprites.h (retail 0xC4): VertexShader +0x20, PixelShader +0x28, VertexBuffer +0x38
  (CVertexBufferWin32: PVertexBuffer +0x3C, Stride byte +0x48), IndexBuffer +0x6C (PD3DIndexBuffer +0x70, PrimitiveType +0x78),
  BillboardTexture +0x8C (CEngineBillboard; .Texture read at +0x8C), NonStippleAlphaFactor +0xB0, EnableZSpriteAlpha +0xB4,
  VerticalControlPointWeightingFactor +0xB8, ControlPointWeightingPower +0xBC, CurrentRenderedMesh +0xC0. Header has only the
  VIRTUAL_DTOR opt-in and opaque byte arrays for the containers -> the author needs an overlay with typed members
  (CVertexShader/CPixelShader/CVertexBufferWin32/CIndexBuffer/CEngineBillboard) + base CEnginePrimitiveManagerOcclusionBase (PrepareOcclusionTest
  is a protected base member: `this` must derive from engine/CEnginePrimitiveManagerOcclusionBase.h).
- sprite = engine/CEngineInternalPrimitiveRepeatedZSprite.h: Mesh +0x50, ObjectsEntries +0x5C, ObjectCount +0x60, MinDistance +0x8C,
  MaxScale +0x90; base CEngineInternalPrimitiveBase (FadeDistance +0x14/+0x18, AlphaRef uchar +0x28) via CEngineInternalPrimitiveOcclusionBase
  (0x48..0x50). Header models bases as pads -> overlay typed base needed (FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE exists).
- C3DMeshInfo (engine/C3DMeshInfo.h): MeshData +0x0C (CMeshData: LODMeshes[4] +0x08), BoundingBox +0x10 (min.z +0x18, max.z +0x24),
  BoundingSphere +0x28 (Center +0x28/2C/30, Radius +0x34), LODDataSizes[4] +0x50. Inline GetBestCurrentDetailLevel/HasLOD exist only in
  overlay /opt/fable_lane/w22/agent2028/hdr/engine/C3DMeshInfo.h (FABLE_RETAIL_C3DMESHINFO_LODS, needs _BOUNDINGBOX) -- copy it.
- CEngineCamera (engine/CEngineCamera.h): +0x14..+0x20 (4 floats), +0x54 C3DVector (position), +0x60 float (FOV), +0xD4..+0xE0
  (HomogenousViewScaleX..), NoScaleViewTransform +0x114, InverseNoScaleViewTransform +0x144 (FABLE_RETAIL_CENGINECAMERA_NOSCALEVIEWTRANSFORM; check +0x14/+0x54/+0x60 names in struct_layouts_egor.tsv).
- CRenderStateManager.h (FABLE_RETAIL_CRENDERSTATEMANAGER): DeviceState +0x2818; states used: AlphaTestEnable 0x28CC, AlphaRef 0x28E0,
  AlphaTestFunc 0x28A4, SpecularEnable 0x29E4 (1), FogEnable 0x2944 (1), TextureUVModeU[1] 0x2EF8, TextureUVModeV[1] 0x2F98,
  TextureFilterModeMag[1] 0x3178, TextureFilterModeMin[1] 0x30D8. StartCaptureBlock inline (+0x2808/+0x2814/BookmarkDummyState +0x3A24).
  NOTE retail emits `cmp DesiredState,value; je` = default (non EARLY_RETURN) SetRenderState body is fine; check vs 0x00BBC460.
- CShaderRenderManager.h: RenderManager +0x1D0, D3DDevice +0x1D4, WorldMatrixIsIdentity +0x1E8, UpdateFlags +0x3D8, PrepareForRender inline.
- CRenderManagerCore.h: TextureStagesInUse +0x3CB8, PD3DDevice +0x3CF0, CurrentlySelectedTextures +0x3D00 (AttachTextureToStage expanded:
  FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS).

## 6. Inline helpers retail expanded (and opt-ins)
- CScopedShadersEnable(true,true) / dtor: engine/CScopedShadersEnable.h (out-of-line copies 0x009D8AD0/0x009D8B40 exist, but retail EXPANDS here).
- CScopedVertexShaderConstantLayout(2) / dtor: engine/CScopedVertexShaderConstantLayout.h (expanded).
- CRenderStateScopedCaptureBlock: engine/CRenderStateCaptureBlock.h (expanded StartCaptureBlock / call RestoreCaptureBlock).
- CEngineLightingManager::SetEnableFadedShadowFactor(ShadowMode == 1) (FABLE_RETAIL_CENGINELIGHTINGMANAGER), twice.
- C3DMeshInfo::GetBestCurrentDetailLevel (overlay, above).
- GFFastOneOverSqrt (engine/C3DVector.h; five NEWTON_* spellings -- pick by scheduling; FableWin ?GFFastOneOverSqrt@@YIMM@Z).
- CRenderStateManager::SetAlphaTestEnable/SetAlphaRef/SetAlphaTestFunc/SetSpecularEnable/SetFogEnable/SetTextureUVModeU/V/
  SetTextureFilterModeMin/Mag (FORCEINLINE in header).
- CShaderRenderManager::PrepareForRender(flags) with flags containing 0x200 and none of 1,4,8,0x10,0x20,0x40,0x80,0x400 (only
  UpdateWorldSpaceFogTransform among the flag-gated ones appears) -- e.g. 0x200 (bit 2 etc. unprovable).
- CShaderRenderManager::SelectVertexBuffer + nine-arg DrawBuffersIndexed(textures, 2, VertexBuffer, 0, count*4, IndexBuffer, 0, count*2, PT_NULL)
  + six-arg DrawBuffersIndexed: bodies exactly as in landed 00/ba/CEnginePrimitiveManagerStaticMeshesBase_DisplayMirroredTextures_00ba1b00.cpp
  lines 74-97 (same-TU FABLE_FORCEINLINE definitions) or overlay FABLE_RETAIL_CSHADERRENDERMANAGER_SELECTVERTEXBUFFER (agent2059 hdr).
- Out of line (do NOT expand): ~CCountedPointer<const C3DMesh2> 0x00A36EC0, CPreTransposedBoneMatrix::Set, GetTransitionFade (same TU,
  defined earlier at 0x00BAA460 but retail CALLS it -- it is not inline in the class).

## 7. Siblings to copy idioms from
- 00/bb/CEnginePrimitiveManagerParticleGroup_RenderSubPrimitive_00bbc460.cpp (Flags: none): CRenderStateScopedCaptureBlock +
  CScopedShadersEnable + CScopedVertexShaderConstantLayout + lighting manager + FORCEINLINE_HELPERS.
- 00/ba/CEnginePrimitiveManagerStaticMeshesBase_DisplayMirroredTextures_00ba1b00.cpp: PrepareForRender + DrawBuffersIndexed forms.
- 00/ba/CEngineInternalPrimitiveRepeatedZSprite_IsInsideFrustum_00baa4e0.cpp (same TU: <math.h> + #pragma intrinsic(sqrt)).
- 00/ba/CEngineInternalPrimitiveRepeatedZSprite_GetTransitionFade_00baa460.cpp (CEngineFadeDistance local model; callee).
- /opt/fable_lane/w22/agent2059/src/00bc25f0.cpp + hdr/ (matched ParticleMeshGroup::RenderSubPrimitive: GetMesh flags 0x11 BlockOnLoad idiom,
  CCountedPointer handling, its #define list) and /opt/fable_lane/w22/agent2028 (RepeatedStaticMeshes::RenderPrimitive: GetBestCurrentDetailLevel).
