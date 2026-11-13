# DOSSIER 0x00BB3E80 -- CEngineClothRenderer::RenderClothPrimitive (4540 bytes)

No prior draft ($W/prior empty; agent2069 never started this row; $W/hdr empty). The CLOSEST MODEL is
/opt/fable_lane/w22/agent2060/differ/00ba5260.cpp (StaticMeshesBase::RenderMeshPrimitive, 4056 b, DIFFER-close) with its
overlay /opt/fable_lane/w22/agent2060/hdr (README.md lists every opt-in). Copy that overlay into $W/hdr first: this row expands
the SAME inline helpers (GetAppropriateMipmap, SetupAlphaBlendingStatesFromRenderFlags, FlushRender, RestoreDefaultPrimitiveParameters,
scoped capture block, Start/EndOcclusionTest).

## 1. Retail shape (xdis 00bb3e80 0x11cc = $W/dis_all.txt)
- __thiscall, `ret 0x20` (8 dword args; twice: 0x00BB4508 and 0x00BB5039, the two copies of the destructor tail).
- `push ebp; mov ebp,esp; sub esp,48h`, then push ebx / esi (0xBB3E94/95, interleaved with the capture-block code) and push edi
  (0xBB3EB5). No SEH frame although two RAII locals have dtors => TU built without /EHsc -- siblings 0x00BBC460
  (same RAII pair) land with `// Flags: none`; agent2060 0x00BA5260 also no flags/pragma. No /GS cookie. No `optimize("s")` idioms
  (fild/fmul inline, full jcc forms). Frame pointer = lane default for these render TUs (siblings use no pragma).
- One x87 `fistp` site (0x00BB43F0) inside the expanded GetAppropriateMipmap: GFFloatToLongFloor (shared header
  rebuild/include/engine/GFFloatToLongFloor.h) -> needs the `// Retail-asm:` line + per-site table (ASM_RULES), exactly like
  agent2060's 0x00BA5519 line.

## 2. Identity (proven, 3 sources)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv 0x00a69da0) and FableWin (ghidra_out/_fablewin_api.tsv 0x017c3b8e):
  `?RenderClothPrimitive@CEngineClothRenderer@@QAEXABVCParticleSimulation@@ABVC3DClothPrimitive@@ABVC3DPrimitive2@@ABVCEngineSubPrimitiveMeshBase@@AAVCEngineInternalPrimitiveMeshBase@@ABVCMatrix3x4@@W4ERenderPassType@@_N@Z`
- Landed caller rebuild/src/compiled/00/b8/CEngineClothSimulationStaticMesh_Render_00b812f0.cpp (call 0x00B81426) cites the same
  mangled name and declares it; the other caller is 0x00B81612 (CEngineClothSimulationAnimatedMesh::Render 0x00B814B0, landed).
- Manifest label is RIGHT. Signature:
  `void CEngineClothRenderer::RenderClothPrimitive(const CParticleSimulation& simulation, const C3DClothPrimitive& cloth_primitive,
   const C3DPrimitive2& mesh_primitive, const CEngineSubPrimitiveMeshBase& sub_primitive, CEngineInternalPrimitiveMeshBase& primitive,
   const CMatrix3x4& transform, ERenderPassType pass, bool block_on_load)` -- public, non-virtual. The last bool ([ebp+0x24]) is
  NEVER read (0 references in the listing).
- Leaf line: `// Leaf: ?RenderClothPrimitive@CEngineClothRenderer@@QAEXABVCParticleSimulation@@ABVC3DClothPrimitive@@ABVC3DPrimitive2@@ABVCEngineSubPrimitiveMeshBase@@AAVCEngineInternalPrimitiveMeshBase@@ABVCMatrix3x4@@W4ERenderPassType@@_N@Z`
- Arg slots: [ebp+8] simulation, +0xC cloth_primitive, +0x10 mesh_primitive, +0x14 sub_primitive, +0x18 primitive, +0x1C transform,
  +0x20 pass, +0x24 block_on_load (unused).

## 3. Callees (retail addr -> landed file / name; PDB name)
| addr | how | landed row | name to use (PDB) |
|---|---|---|---|
| 0x00A05840 | thiscall on CRenderStateCaptureBlock::StateManager 0x013CB508 | 00/a0/CRenderStateManager_RestoreCaptureBlock_00a05840.cpp | CRenderStateManager::RestoreCaptureBlock (via `~CRenderStateScopedCaptureBlock`, engine/CRenderStateCaptureBlock.h) |
| 0x00987FE0 / 0x009880E0 | ecx=0x013BC470 | landed | CShaderRenderManager::EnableVertexShaders / DisableVertexShaders (inside CScopedShadersEnable, engine/CScopedShadersEnable.h) |
| 0x00988110 / 0x00988190 | ecx=0x013BC470 | landed | EnablePixelShaders / DisablePixelShaders (same) |
| 0x00BFEAE6 | cdecl | toolchain thunk | memmove (dead arm of the inlined STLport erase in vector clear) |
| 0x00464ACE | thiscall, 5 args | 00/46/stdvectorunsignedlong_Minsertoverflow_00464ace.cpp | std::vector<unsigned long>::_M_insert_overflow (push_back overflow) |
| 0x00BFEA70 x4 | | toolchain | __ftol2 (CRGBFloatColour::GetRGBColour inside CEnginePrimitiveRenderUtil::SetTextureFactor) |
| 0x00B46C80 x2 | ecx=g 0x01436E9C | NOT landed | ?UpdateGlobalLightingStates@CEngineLightingManager@@QAEXXZ (declared engine/CEngineLightingManager.h:93) |
| vtbl+0x74 (slot 29) on primitive | | | ?PeekRenderInfoMeshBase@...@@UBEPBVCEngineRenderInfoMeshBase@@XZ (slot 29 per CEngineInternalPrimitiveOcclusionBase.h opt-in note RESETOCCLUSIONINFO) |
| vtbl+0x80 (slot 32) on primitive | | | ?GetMorphInfo@CEngineInternalPrimitiveMeshBase@@UAEPAVCEngineInternalPrimitiveMorphInfo@@XZ |
| 0x00BA9360 | static fastcall (ecx material, edx surfaces, push mipmap, &preload, bank, false) | 00/ba/CEnginePrimitiveManagerMeshBase_GetDiffuseTexture_00ba9360.cpp | CEnginePrimitiveManagerMeshBase::GetDiffuseTexture (decl in engine/CEnginePrimitiveManagerMeshBase.h under FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_STATICBLOCKS) |
| 0x00BEFD90 | this, (effect, scale) | NOT landed | ?SetupOutlineSmoothStencilStates@CEnginePrimitiveRenderUtil@@QAEXABVCEngineMeshEffectOutline@@M@Z (pass 8) |
| 0x00BF01E0 | this, (effect) | 00/bf/..._SetupOutlineSmoothAlphaStencilMaskStates_00bf01e0.cpp | SetupOutlineSmoothAlphaStencilMaskStates (pass 9) |
| 0x00BF2240 | this, (effect, scale, bool, long 1) | NOT landed | ?SetupOutlineSmoothFillStates@CEnginePrimitiveRenderUtil@@QAEXABVCEngineMeshEffectOutline@@M_NJ@Z (pass 10) |
| 0x00BF0740 | this, (effect, scale) | NOT landed (parked 2026-10-22) | **SetupOutlineStencilStates** (pass 12 = RENDER_PASS_OUTLINE_STENCIL) |
| 0x00BF0B20 | this, (effect, scale) | NOT landed | **SetupOutlineFillStates** (pass 13 = RENDER_PASS_OUTLINE_FILL). FLAG: manifest + CEnginePrimitiveRenderUtil.h Q25 opt-in (`FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_OUTLINESTENCILSTATES`) call 0x00BF0B20 "SetupOutlineStencilStates" -- wrong: two addresses carry that name; evidence: pass enum (engine/CEngineRenderEnums.h 12 OUTLINE_STENCIL / 13 OUTLINE_FILL), Ego_r order/sizes (Stencil 0x410 ~ 0xBF0740 0x3E0; Fill 0x520 ~ 0xBF0B20 0x510). Put a NAME_FIXES.tsv line. |
| 0x00BF1030 | this, (effect, flags, pass) | 00/bf/CEnginePrimitiveRenderUtil_SetupStencilMask_00bf1030.cpp | ?SetupStencilMask@CEnginePrimitiveRenderUtil@@QAEXPBVCEngineMeshEffect@@KW4ERenderPassType@@@Z |
| 0x00988020 | ecx=g 0x01436E14, (shader, false) | landed | CShaderRenderManager::ApplyVertexShader(const CVertexShader&, bool) |
| 0x00988140 | ecx=g 0x01436E14 | landed | CShaderRenderManager::ApplyPixelShader(const CPixelShader&) |
| 0x00B4FC40 | ecx=g 0x01436E60, (1, transform, &params) | NOT landed | ?SetupShadowFuncRenderStates@CEngineShadowRenderer@@QAEXJABVCMatrix3x4@@ABUCColorBufferDepthPackParameters@1@@Z |
| 0x00B47390 | ecx=g 0x01436E9C, (transform) | NOT landed | ?SetupShaderSpotLight@CEngineLightingManager@@QAEXABVCMatrix3x4@@@Z |
| 0x00B8D090 | ecx=g 0x01436E5C | landed as `Nop_ret0_00b8d090` / local "BeginQuery" (00/be/..RenderWithOcclusion_00be79d0) | **?StartOcclusionTest@CEngineOcclusionManager@@QAEXXZ** (FableWin; agent2060 overlay `FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_OCCLUSIONTEST`) -- landed spellings disagree with PDB |
| 0x00B8D0A0 | (handle) | landed as `Nop_ret4_00b8d0a0` / "EndQuery" | **?EndOcclusionTest@CEngineOcclusionManager@@QAEXK@Z** |
| 0x00BB2540 | this, 14 args | NOT landed | ?RenderPolygons@CEngineClothRenderer@@AAEXABVCParticleSimulation@@ABVCParticleProgram@@ABVCMatrix3x4@@ABVCEngineLocalLightArray@@PAPBVCTexture@@K_N5K55KW4ERenderPassType@@AAVCEngineInternalPrimitiveMeshBase@@@Z (private) = (sim, program, transform, lights, const CTexture** textures, unsigned long count, bool, bool, unsigned long, bool, bool, unsigned long, ERenderPassType, CEngineInternalPrimitiveMeshBase&) |
| D3D device vtbl +0x190/+0x1A0/+0x104 | | | IDirect3DDevice9::SetStreamSource / SetIndices / SetTexture inside inline CRenderManagerCore::FlushRender (FableWin ?FlushRender@CRenderManagerCore@@QAEXXZ) |

## 4. Members / globals
Globals (spellings: most-used landed binding; PDB global names not found):
- 0x013CB508 CRenderStateCaptureBlock::StateManager (header static); 0x013BC470 CShaderRenderManager singleton (+0x390/0x391 = 0x013BC800/801 enable flags; via CScopedShadersEnable).
- 0x01436E18 render manager (CRenderManagerCore*; `NEngineInternal::GPRenderManager` in 0x00BBC460, g_pRenderManagerCore elsewhere).
- 0x01436E14 CShaderRenderManager* (`NEngineInternal::GPShaderRenderManager`). +0x391 = pixel shaders enabled (GetPixelShadersEnabled), +0x3CC constant buffer (+0x20 = constant 2), +0x3D8 dirty |= 2 (inline SetPixelShaderConstant; see DisplayMirroredTextures 0x00BA1B00).
- 0x01436E78 CEnginePrimitiveRendererSharedData* (`g_pPrimitiveRenderer` in 0x00BA1B00 / `g_pPrimitiveRendererSharedData` in 0x00B91140). Offsets == Ego_r PDB exactly: CurrentOcclusionLayer +0x78, MipmapSizeMultiplier +0xB0, OcclusionEnable +0xBD, PixelShaderDiffuseOnly +0xDC, PixelShaderDiffuseColourTextureDiffuseAlpha +0xE4, PixelShaderDiffuse +0xF4, PixelShaderOutlineSmoothFill +0x1FC, PixelShaderOutlineSmoothFillAlpha +0x204, CPixelShader* PixelShaderDiffuseFogShadow +0x224, PixelShaderDiffuseFogShadowOutputFactor +0x228. Header engine/CEnginePrimitiveRendererSharedData.h: FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_MEMBERS (+ agent2060 overlay `_MIPMAP`). FableWin statics on CEnginePrimitiveRenderUtil wrap these: GetCurrentOcclusionLayer, GetOcclusionEnable, GetMipmapSizeMultiplier (SI = static fastcall inline).
- 0x01436E5C CEngineOcclusionManager* g_pOcclusionManager (header engine/CEngineOcclusionManager.h default branch names CurrentFrame +0x4010, Entries +0x4014, EntriesSize +0x401C; FableWin AllocateOccluderHandle inline -- body in landed 0x00B91140 RenderBoundingBox).
- 0x01436E9C CEngineLightingManager* (NEngineInternal::GPLightingManager in 0x00BBC460). engine/CEngineLightingManager.h with FABLE_RETAIL_CENGINELIGHTINGMANAGER (+ _MEMBERS for ShadowMode +0x4694): FadedShadowDiffuseFactorEnabled +0xCE, CurrentThemeChannel +0xE0..E2, StateUpdateFlags +0xE4, SelfIlluminationAmbientPercentage +0xE8; inline SetEnableFadedShadowFactor / SetCurrentEnvironmentThemeChannel / SetSelfIlluminationAmbient / RestoreDefaultPrimitiveParameters ALREADY in the header.
- 0x01436EA0 CEngineCamera* g_pEngineCamera: camera position +0x54/+0x58/+0x5C (PeekCameraPos, FABLE_RETAIL_CENGINECAMERA_POSITION).
- 0x01436E60 CEngineShadowRenderer* (g_pShadowRenderer_01436e60 in sibling FrameStart 0x00BB1EF0; g_pEngineShadowRenderer elsewhere).
- 0x01396F78 / 0x01396F7C = CRenderStateManager::D3DAlphaBlendModeTable[4]/[5] (table 0x01396F68): SRC_ALPHA / INV_SRC_ALPHA.
- 0x01231724 1/255.0f; 0x01230014 255.0f; 0x0129BA3C DELTA_NOTIONAL_ZERO; 0x0143B990 g_highestSetBitTable (GFGetTopBitInByte).

Render states (CRenderStateManager::DeviceState at +0x2818, CStateInfo 0x14: DesiredState +4, BookmarkMask +8, DirtyListFlag +0x10;
StateStack +8 (8-byte elems), StateStackSize +0x2808, StateUpdateList +0x2008 / count +0x280C, CurrentBookmarkMask +0x2814):
+0x2840 ColourWriteEnable, +0x2854 ZBufferWriteEnable, +0x28B8 AlphaBlendEnable, +0x28CC AlphaTestEnable, +0x28E0 AlphaRef,
+0x291C SourceAlphaBlendMode, +0x2930 DestAlphaBlendMode, +0x2944 FogEnable, +0x29D0 TextureFactor, +0x29E4 SpecularEnable.
All via engine/CRenderStateManager.h inline setters (FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS; SetColourWriteEnable(bool)
inline = FABLE_RETAIL_CRENDERSTATEMANAGER_OUTLINE_INLINE; SetTextureFactor(unsigned long) inline body defined in-row as 0x00BA1B00 does).

Object members (struct_layouts_egor.tsv names; retail offsets):
- CEngineClothRenderer (`this`, saved at [ebp-8]); base CEnginePrimitiveRenderUtil (vptr +0). TriVertexShaders CArray<CVertexShader> +0x04
  (reads _M_start), TriVertexShadersShadow CVertexShader* +0x40, TriVertexShadersSpotShadow +0x44, TriVertexShaderOutline +0x54,
  TriVertexShaderOutlineSmoothFill +0x5C. NO shared header engine/CEngineClothRenderer.h; landed rows model it locally
  (full retail layout in 00/bb/CEngineClothRenderer_CEngineClothRenderer_00bb6730.cpp lines 70-100).
- C3DClothPrimitive (no header): ParticleProgram CCountedPointer<CParticleProgram> +0x04 (Data read), MaterialIndex +0x10.
- C3DPrimitive2 (engine/C3DPrimitive2.h; agent2060 overlay FABLE_RETAIL_C3DPRIMITIVE2_RENDER): BoundingSphere +0x0C (Center +0xC..+0x14, Radius +0x18), AverageTextureStretch +0x1C.
- CEngineSubPrimitiveMeshBase/Base (agent2060 overlay headers CEngineSubPrimitiveBase.h `_ACCESSORS`, CEngineSubPrimitiveMeshBase.h new): RenderFlags +0x04, Alpha +0x08, Parent +0x0C (-> CEngineInternalPrimitiveBase::AlphaRef +0x28).
- CEngineInternalPrimitiveMeshBase (engine/CEngineInternalPrimitiveMeshBase.h, needs FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE for the real hierarchy): OcclusionInfo CIVCountedPointer<CEnginePrimitiveOcclusionInfo>[2] +0x48, MeshInfo +0x50, Scale +0x58, TintColour CRGBColour +0x5C (B,G,R,A bytes: R=+0x5E), MeshEffect CIVCountedPointer<CEngineMeshEffect> +0x68 (raw ptr passed), ThemeChannelDesc +0x6C (3 bytes).
- CEnginePrimitiveOcclusionInfo (engine/CEnginePrimitiveOcclusionInfo.h -- members PRIVATE, no AddPixelCountTest): OcclusionTestHandles CArray<unsigned long> +0x0C/+0x10/+0x14, TestFrame +0x1C. Needs an overlay opt-in with inline `AddPixelCountTest(unsigned long)` (FableWin name ?AddPixelCountTest@CEnginePrimitiveOcclusionInfo@@QAEXK@Z); body = landed 0x00B91140 lines 173-177.
- CEngineRenderInfoMeshBase (no header): Lights CEngineLocalLightArray* +0, Mesh C3DMesh2* +4 (engine/C3DMesh2.h: Materials +0x8C, GetMaterial). CEngineLocalLightArray::GetCount() = dword +0.
- C3DMeshMaterial (engine/C3DMeshMaterial.h, 0x30): DiffuseMapBankIndex +0x10, SelfIllumination +0x24, TwoSided +0x28, Transparent +0x29.
- C3DMeshInfo::PeekTextureBank() (engine/C3DMeshInfo.h:59, FABLE_RETAIL_CMESHDATABANK_TEXTUREBANK): MeshInfo +4 -> +0x3D4. CGraphicDataBank::PeekGraphic (vector of 0x2C CGraphicInfo at +0x1E0/+0x1E4, bounds-checked -> 0) and CGraphicInfo::GetWidth (+6) -- agent2060 overlay CGraphicInfo.h `_GETWIDTH`.
- CEngineInternalPrimitiveMorphInfo: CompositeSurfaces at +0x10 (retail; PDB 0x14) -- agent2060 overlay CEngineInternalPrimitiveMorphedStaticMesh.h.

## 5. Visibility
- Out-of-line calls that must STAY calls: RestoreCaptureBlock, Enable/Disable*Shaders, ApplyVertex/PixelShader, GetDiffuseTexture,
  UpdateGlobalLightingStates, SetupShadowFuncRenderStates, SetupShaderSpotLight, the Setup* outline helpers, SetupStencilMask,
  RenderPolygons (same class, 0x00BB2540 = earlier in the same TU -- probably defined above; affects register liveness: check edx
  use after the call; none observed), _M_insert_overflow (out of line, as landed 0x00B91140 keeps it noinline), Start/EndOcclusionTest
  (empty bodies, still called -> declare only, do not define).
- Inline expansions (no retail copy): StartCaptureBlock, CScopedShadersEnable ctor/dtor, AllocateOccluderHandle, AddPixelCountTest
  (vector clear via __copy_trivial + push_back), CEnginePrimitiveRenderUtil::SetTextureFactor(const CRGBFloatColour&) with
  CRGBFloatColour::GetRGBColour (FABLE_RETAIL_CRGBFLOATCOLOUR + FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_SETTEXTUREFACTOR; body text
  in 0x00BA1B00 lines 110-116), SetCurrentEnvironmentThemeChannel, GetAppropriateMipmap (FABLE_FORCEINLINE, agent2060 body),
  SetupAlphaBlendingStatesFromRenderFlags (FABLE_FORCEINLINE, agent2060 body), FlushRender (0x00BA1B00 lines 98-109),
  SetSelfIlluminationAmbient, SetEnableFadedShadowFactor, RestoreDefaultPrimitiveParameters.

## 6. Siblings to copy idioms from
- /opt/fable_lane/w22/agent2060/differ/00ba5260.cpp (+hdr/): same helpers; NO pragma, no Flags; macro list lines 31-72.
- rebuild/src/compiled/00/bb/CEnginePrimitiveManagerParticleGroup_RenderSubPrimitive_00bbc460.cpp: `// Flags: none`; CRenderStateScopedCaptureBlock + CScopedShadersEnable(true,true); FABLE_RETAIL_CENGINELIGHTINGMANAGER, FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS.
- rebuild/src/compiled/00/ba/CEnginePrimitiveManagerStaticMeshesBase_DisplayMirroredTextures_00ba1b00.cpp: SetTextureFactor / FlushRender / SetPixelShaderConstant bodies; macros FABLE_RETAIL_CRGBFLOATCOLOUR, _SETTEXTUREFACTOR, _SHAREDDATA_MEMBERS, FABLE_RETAIL_CENGINEPRIMITIVERENDERER.
- rebuild/src/compiled/00/b9/CEnginePrimitiveManagerOcclusionBase_RenderBoundingBox_00b91140.cpp: AllocateOccluderHandle / clear / push_back model (local; port into shared-header opt-ins).
- Same class: 00/bb/CEngineClothRenderer_FrameStart_00bb1ef0.cpp, _GetPositionBuffer_00bb1e30, _GetNormalBuffer_00bb1e90, ctor/dtor 00bb6730 (all local class models, no pragmas).
