# DOSSIER 0x00BA2350 (identity scout, bwf2) -- no prior scout existed ($W/prior/ empty; agent4028 has no notes for it)

## 1. Retail shape (dis_all.txt + xdis.py on Fable.exe)
- 9632 bytes = 0x00BA2350..0x00BA48A6 code + jump table 0x00BA48A8 (18 dwords) -> `ret 1Ch` (0x00BA48A3), the ONLY exit; always returns `true` (`mov al,1`).
- ESP frame, no `push ebp; mov ebp,esp` (ebp is a general register = this / lighting mgr / stages) -> no /Oy- pragma.
  Prologue `sub esp,0B0h; push ebx; push ebp; push esi; push edi; mov ebp,ecx; mov [esp+14h],ebp`. Epilogue pops edi,esi,ebp,ebx; `add esp,0B0h`.
- No fs:0 / SEH, no /GS cookie, no destructor-bearing locals. __thiscall, 7 stack args (0x1C).
- x87: float colour maths (fild/fmul 1/255 at 0x01231724), 255.0 (0x01230014) * value -> `call __ftol2` 0x00BFEA70 (x4, plain C++ float->uchar casts, no asm needed).
- Switch: `mov edx,[esp+48h]; dec edx; jmp [edx*4+0BA48A8h]` at 0x00BA25F8 with NO bounds check -> the source has `default: __assume(0)`-style
  no-default (or NODEFAULT). Table (read from the exe with $W/jt.py): mode1 0xBA31DE, 2 0xBA2600, 3 0xBA2677, 4 0xBA26CF, 5 0xBA270C, 6 0xBA2A4E,
  7 0xBA2B4F, 8 0xBA2C71, 9 0xBA2CEC, 10 0xBA2D92, 11 0xBA2ECD, 12 0xBA2FCB, 13 0xBA307B, **14 = 0x00000000**, 15 0xBA3CF9, 16 0xBA3214,
  17 0xBA3555, 18 0xBA399A. The 0 entry = a hole (no case 14) under an __assume(0) default; VC7.1 emits 0 for the unreachable label.
  Landed rows using __assume(0): rebuild/src/compiled/00/75/CTCCameraPointScriptedSpline_OnSerialseLeaf_0075e4c0.cpp,
  00/56/CPersistContext_TransferCQuestActivityRecord_00569f20.cpp (copy their spelling).
- Size-heavy body = expanded inline SetRenderState (CRenderStateManager::SetRenderState, 0x55-0x5C bytes each, ~40 sites) and
  AttachD3DTextureToStage (~20 sites). Retail cross-jumps (tail-merges) many case ends: 0xBA2C56/0xBA2C57/0xBA2C62/0xBA2CC8/0xBA2FB6/0xBA3FE8/0xBA4013/0xBA405E/0xBA4064.

## 2. Identity (manifest label CORRECT)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv @Ego_r 0x00A58080) and FableWin.pdb (naming_stragglers/FableWin.exe.signatures.tsv 0x02E9CE10,
  5932 debug bytes / 1488 instr; also 0x017AFB93 stub) both:
  `?SetupRenderModeShadersAndConstants@CEnginePrimitiveManagerStaticMeshesBase@@AAE_NW4ERenderPassType@@PBVC3DMeshMaterial@@AAVCEngineSubPrimitiveStaticMesh@@AAVC3DTextureStages@@JPAKPA_N@Z`
  = private, non-virtual, non-const: `bool SetupRenderModeShadersAndConstants(ERenderPassType pass, const C3DMeshMaterial* material,
  CEngineSubPrimitiveStaticMesh& sub, C3DTextureStages& stages, long mipmap_level, unsigned long* shader_flags, bool* degenerate)`.
  Args: [esp+0C4h] pass, +0C8h material, +0CCh sub, +0D0h stages, +0D4h mipmap, +0D8h shader_flags, +0DCh degenerate (post-push esp).
- Caller: xref.py -> 0x00BA5BF3 in RenderMeshPrimitive 0x00BA5260 (not landed; bwf1/00ba5260 draft passes mipmap/stages/flags/degen in that order).
- Not a vtable slot. Retail neighbourhood: DisplayMirroredTextures 0x00BA1B00 (landed, same class/TU) precedes; SetupStaticBlocksNoMorph 0x00BA48F0 follows (landed).

## 3. Callees (retail | landed file | PDB | flag)
- 0x00988020 CShaderRenderManager::ApplyVertexShader(const CVertexShader&, bool) | 00/98/CShaderRenderManager_ApplyVertexShader_00988020.cpp | Ego_r ?ApplyVertexShader@CShaderRenderManager@@QAEXABVCVertexShader@@_N@Z ok. Always `force=true` (push 1).
- 0x00988140 ApplyPixelShader(const CPixelShader&) | landed | ok.
- 0x00989B00 SetUserVertexShaderConstant(ulong, float,float,float,float) | landed | ?..@@QAEXKMMMM@Z ok (case 5, index 0).
- 0x00989A60 SetUserVertexShaderConstant(ulong, const C3DVector&, float) | landed | ?..@@QAEXKABVC3DVector@@M@Z ok (2D cases, index 0).
- 0x00BA9830 CEnginePrimitiveManagerMeshBase::GetReflectionTexture | manifest name (not landed) | Ego_r ?GetReflectionTexture@CEnginePrimitiveManagerMeshBase@@SIPBVCTexture@@PBVC3DMeshMaterial@@KAA_NPBVCGraphicDataBank@@_N@Z
  (static __fastcall: ecx=material, edx=mipmap, stack: bool& out, bank, bool). NB egor_pdb_names lists 0x00BA9830 as a CGuiControlNumber name: that is Ego_r address space, ignore.
- 0x00BA9A70 Get2DAlphaMaskTexture | 00/ba/CEnginePrimitiveManagerMeshBase_Get2DAlphaMaskTexture_00ba9a70.cpp | ?Get2DAlphaMaskTexture@..@@SIPBVCTexture@@KKAA_NPBVCGraphicDataBank@@_N@Z (fastcall ecx=id, edx=0).
- 0x00B44F20 CEngineLightingManager::SetupShaderPointLights(const CEngineLocalLightArray&) | landed | ok.
- 0x00B46C80 UpdateGlobalLightingStates() | not landed | ?UpdateGlobalLightingStates@CEngineLightingManager@@QAEXXZ ok (header decl CEngineLightingManager.h:93).
- 0x00B47390 SetupShaderSpotLight(const CMatrix3x4&) | not landed | ?SetupShaderSpotLight@CEngineLightingManager@@QAEXABVCMatrix3x4@@@Z ok (overload; the XZ one is different).
- 0x00B4FC40 CEngineShadowRenderer::SetupShadowFuncRenderStates(long, const CMatrix3x4&, const CColorBufferDepthPackParameters&) | not landed | Ego_r ?..@@QAEXJABVCMatrix3x4@@ABUCColorBufferDepthPackParameters@1@@Z (nested struct, `U` = struct). On g_pEngineShadowRenderer 0x01436E60.
- 0x00BEFD90 CEnginePrimitiveRenderUtil::SetupOutlineSmoothStencilStates(const CEngineMeshEffectOutline&, float) | not landed | ok.
- 0x00BF01E0 SetupOutlineSmoothAlphaStencilMaskStates(const CEngineMeshEffectOutline&) | landed | ok.
- 0x00BF2240 SetupOutlineSmoothFillStates(const CEngineMeshEffectOutline&, float, bool, long) | not landed | ?..@@QAEXABVCEngineMeshEffectOutline@@M_NJ@Z ok.
- 0x00BF0740 = SetupOutlineStencilStates (pass 12 OUTLINE_STENCIL) | not landed | ?SetupOutlineStencilStates@CEnginePrimitiveRenderUtil@@QAEXABVCEngineMeshEffectOutline@@M@Z.
- 0x00BF0B20 = **SetupOutlineFillStates** (pass 13 OUTLINE_FILL / else) | not landed | ?SetupOutlineFillStates@CEnginePrimitiveRenderUtil@@QAEXABVCEngineMeshEffectOutline@@M@Z.
  FLAG: manifest labels BOTH 0xBF0740 and 0xBF0B20 "SetupOutlineStencilStates"; 0xBF0B20 is SetupOutlineFillStates (Ego_r name set has exactly these two (M) outline setters; pass order STENCIL=12/FILL=13 in CEngineRenderEnums.h). Single structural source -> say so in NAME_FIXES.
- 0x00B89070 | landed placeholder rebuild/src/compiled/00/b8/E2_E2_00b89070.cpp `?M@C00b89070@@QAEXH@Z` (manifest Nop_ret4) | on NEngineInternal::GPScreenEffectOutlineGlow 0x01436E3C with arg &parent->ObjectTransform:
  Ego_r ?SetupDepthBufferTransform@CEngineScreenEffectOutlineGlow@@QAEXABVCMatrix3x4@@@Z (the only CEngineScreenEffectOutlineGlow member taking a CMatrix3x4; empty body in TLC = `ret 4`). Evidence: Ego_r name + call shape; file NAME_FIXES (descriptive until a 2nd source).
- 0x00BFEA70 __ftol2 (CRT, toolchain).
- IDirect3DDevice9::SetTexture slot +0x104 (inline AttachD3DTextureToStage).
- Globals: GPShaderRenderManager 0x01436E14 (NEngineInternal), GPRenderManager 0x01436E18 (NEngineInternal; label g_pRenderManagerCore), g_pPrimitiveRenderer 0x01436E78,
  g_pEngineLightingManager 0x01436E9C, g_pEngineShadowRenderer 0x01436E60, CEngineResourceManager* 0x01436E98 (stipple / bump-env textures +0x2AAC, +0x2AD4),
  g_pEngineCamera 0x01436EA0 (+0x14..+0x20 floats), g_pEnginePrimitiveTypeInfo 0x01436E84 (majority landed spelling; +0x94 -> 2D-mask params ptr),
  GPScreenEffectOutlineGlow 0x01436E3C; floats 1/255 0x01231724, 255.0 0x01230014, 0x0125B24C, 0x012A41B0, double 0x012A2788 (case 5 camera constant).
  D3D tables: D3DTextureUVModeTable 0x01396DC8 ([2]=0x01396DD0), D3DTextureFilterModeTable 0x01396E58, D3DTextureMipMapModeTable 0x01396E64 ([2]=0x01396E6C), D3DTextureTransformModeTable[5] 0x01396FE0.

## 4. Members (Ego_r layouts match retail for every offset checked)
- this (CEnginePrimitiveManagerStaticMeshesBase, struct_layouts_egor): PointLightShaderIndex[6] +0x20; CVertexShader arrays (8 bytes each): VertexShadersStaticSpotlight +0x38,
  StippleAlpha +0x58, Static +0x78, StaticEnv +0x98; VertexShaders2DStatic +0x1D8, 2DStaticEnv +0x1E0, 2DStaticDP3Bump +0x1F0; PixelShaderDiffuseFogStippleAlpha +0x2C8;
  VertexShaderOutline +0x2D0, DP3BumpOutline +0x2D8, OutlineSmoothFill +0x2E0, DP3BumpOutlineSmoothFill +0x2E8; pointer arrays VertexShadersStaticShadow +0x2F0,
  EnvShadow +0x2F4, DP3BumpShadow +0x2FC, SpotShadow +0x300, EnvSpotShadow +0x304; StippleAlphaSolidFactor +0x314.
  Repo header rebuild/include/engine/CEnginePrimitiveManagerStaticMeshesBase.h (generated; opt-ins _DISPLAYMIRRORED, _STATICBLOCKS, STATICBLOCKS_TEXTURE_HELPERS) -- check it names these; add a
  FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_SETUPRENDERMODE overlay declaration if the method is missing. Base CEnginePrimitiveRenderUtil is at offset 0 (ecx=this for util calls).
- index idiom: `PointLightShaderIndex[min(lights->Count, 5)]` (`cmp eax,5; jbe; mov eax,5`), lights = parent->RenderInfo.Lights (+0xA0, CEngineRenderInfoMeshBase::Lights, repo header);
  case 1 uses PointLightShaderIndex[0] directly.
- CEngineSubPrimitiveBase: RenderFlags +4 (bit 8 -> stipple), Alpha +8, Parent +0xC. CEngineSubPrimitiveMeshBase: MeshPrimitive +0x18, RenderMode +0x1C (ERenderMode). NO repo header:
  copy bwf1/00ba5260/hdr/engine/CEngineSubPrimitive{Base,MeshBase,StaticMesh}.h (FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_ACCESSORS) -- GetParent(), GetRenderMode().
- Parent (CEngineInternalPrimitiveStaticMesh): EnginePrimitiveFlags +0xC (bit 7 -> bool [esp+38h]), MeshInfo +0x50 (->+4 ->+0x3D4 = CGraphicDataBank*), Scale +0x58,
  TintColour +0x5C (CRGBColour 4 bytes, compared &0xFFFFFF != 0xFFFFFF), MeshEffect +0x68 (CIVCountedPointer, raw ptr read = outline effect), ThemeChannelDesc +0x6C (3 bytes),
  ObjectTransform +0x70, RenderInfo.Lights +0xA0. Headers CEngineInternalPrimitive{Base,MeshBase,StaticMesh}.h, CEnginePrimitiveThemeChannelDesc.h.
- C3DMeshMaterial: DecalID? no; SelfIllumination +0x24 -> SetSelfIlluminationAmbient; Transparent +0x29. MeshPrimitive (C3DPrimitive2) +0x68 bit 1 (`shr 1; test 1`) = "has bump" flag (TLC layout 4 smaller than Ego_r -> Ego_r +0x6C; check bwf1 hdr C3DPrimitive2.h).
- C3DTextureStages: DiffuseTextureStage +0, BumpTextureStage +4, UseBlankTextures +8 (repo C3DTextureStages.h).
- CShaderRenderManager: PixelShadersEnabled +0x391, PSConstTable +0x3CC, UpdateFlags +0x3D8 (repo; inline SetPixelShaderConstant(index, const C4DVector&) FABLE_FORCEINLINE).
- CEngineLightingManager (FABLE_RETAIL_CENGINELIGHTINGMANAGER + _MEMBERS): ThemeChannels +0x3C (CArray, element 0x70, colour C4DVector at +0x30), FadedShadowDiffuseFactorEnabled +0xCE,
  CurrentThemeChannel +0xE0, StateUpdateFlags +0xE4, SelfIlluminationAmbientPercentage +0xE8, ShadowMode +0x4694. Inline setters in repo:
  SetEnableFadedShadowFactor, SetCurrentEnvironmentThemeChannel, SetSelfIlluminationAmbient (all FABLE_FORCEINLINE, exact retail shapes).
- CRenderManagerCore / CRenderStateManager (repo; FABLE_RETAIL_CRENDERMANAGERCORE + _FORCEINLINE_HELPERS): DeviceState base = rm+0x2818 (StateStack +0x8, DirtyList +0x2008,
  StateStackSize +0x2808, DirtyListSize +0x280C, CurrentBookmarkMask +0x2814). Setters used: SetTextureFactor (+0x29D0; declare FABLE_FORCEINLINE in-row as DisplayMirroredTextures does),
  SetFogEnable (+0x2944), SetTextureFilterModeMin/Mag[s], SetTextureMipMapMode[s], SetTextureUVModeU/V[s], SetTextureTransformMode(1, TEXTURE_TRANSFORM_PROJECTED) (+0x3678).
  AttachTextureToStage (inline) + TextureStagesInUse +0x3CB8, PD3DDevice +0x3CF0, CurrentlySelectedTextures +0x3D00.
- CEnginePrimitiveRenderer (g_pPrimitiveRenderer) pixel shaders (8 bytes each): +0xDC PixelShaderDiffuseOnly, +0xE4 PixelShaderDiffuseColourTextureDiffuseAlpha, +0xF4 PixelShaderDiffuse
  (CEnginePrimitiveRendererSharedData.h names these three, FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_MEMBERS); +0xFC, +0x104, +0x19C, +0x1AC, +0x1B4, +0x1C4, +0x1FC, +0x204,
  +0x20C, +0x214 are CPixelShader values and +0x224/+0x228/+0x22C/+0x234/+0x238 are CPixelShader* (loaded then pushed) -- repo CEnginePrimitiveRenderer.h has only _pad_ there; Ego_r layout is
  padding too -> names need FableWin.pdb struct info (not in the TSVs); extend SharedData opt-in in hdr/ with descriptive names and say so.

## 5. Visibility
- Expanded inline (forceinline required): CEnginePrimitiveRenderUtil::SetTextureFactor(const CRGBFloatColour&) incl. CRGBFloatColour::GetRGBColour (4 x __ftol2) -- the same text as
  landed DisplayMirroredTextures 0x00BA1B00 (copy it verbatim); CRenderStateManager::SetRenderState family; CRenderManagerCore::AttachTextureToStage/AttachD3DTextureToStage;
  CShaderRenderManager::SetPixelShaderConstant / GetPixelShadersEnabled; CEngineLightingManager setters; GetParent/GetRenderMode accessors; std::min-like clamp.
- Called out of line: all section-3 callees. GetReflectionTexture / Get2DAlphaMaskTexture are static members of the base (out of line at 0xBA9xxx, after this row) -- declaration only.

## 6. Siblings
- DisplayMirroredTextures 0x00BA1B00 (MATCH, same TU): defines FABLE_RETAIL_CRGBFLOATCOLOUR, CRENDERMANAGERCORE_FORCEINLINE_HELPERS, CENGINEPRIMITIVERENDERUTIL_SETTEXTUREFACTOR,
  CENGINEPRIMITIVERENDERERSHAREDDATA_MEMBERS, CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_DISPLAYMIRRORED, CENGINEPRIMITIVERENDERER; no pragma, no Flags. In-row FABLE_FORCEINLINE bodies
  of CRenderStateManager::SetTextureFactor, GetPixelShadersEnabled, CEnginePrimitiveRenderUtil::SetTextureFactor. THE template for this row.
- SetupStaticBlocksNoMorph 0x00BA48F0 (immediately after): STATICBLOCKS_TEXTURE_HELPERS, CIVCOUNTEDPOINTER(_RESET_ADOPT), CENGINEPRIMITIVERENDERER, CTEXTURE ...; empty Flags.
- AddStaticMesh 0x00BA0FC0 uses `// Flags: /d2inlT200` (not needed here unless inlining depth runs out).
- bwf1/00ba5260 (RenderMeshPrimitive, caller) hdr overlay: CEngineSubPrimitive*.h, C3DTextureStages.h (ctor), CRenderStateManager.h (STALE) -- copy & re-base.
- Family: /opt/fable_lane/w22/agent2059 (ParticleMeshGroup::RenderSubPrimitive, shader/constant set-up idioms, matched).

## 7. Open points / traps
- Jump-table hole (mode 14 -> 0) needs `default: __assume(0)` and NO `case 14`; also no `case 0`.
- Case-end tail merging: several cases end in `SetEnableFadedShadowFactor(ShadowMode == SHADOW_MODE_1)` then fall to the common tail; let the compiler merge (write each case fully, `break`).
- The bool& out of GetReflectionTexture/Get2DAlphaMaskTexture shares slot [esp+10h] with the `tinted` bool -> separate block-scoped locals.
- The shadow-param struct {4,5,0} (CEngineShadowRenderer::CColorBufferDepthPackParameters, 12 bytes) is built per case in distinct slots (+0x60, +0x78, +0x9C, +0xA8, +0xB4) -> one block-scoped local per case.
- ERenderMode has no enumerator names anywhere in the repo (empty enum) -> use numeric case labels with a comment (descriptive: shader family per case in OUTLINE).
