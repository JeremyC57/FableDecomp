# DOSSIER 0x00BAE2D0 -- CEnginePrimitiveManager2DSprites::RenderSubPrimitive (5892 bytes)

No prior scout/draft existed for this row ($W/prior empty; /opt/fable_lane/w22/agent2088 has only empty dirs). Everything below is
from this scout. Helper files: $W/ann.txt (listing with the render-state idiom stripped and every DeviceState offset named),
$W/ann.py (the annotator), $W/r.py (`python3 $W/r.py 00baXXXX-00baYYYY` prints a raw range).

## 1. Retail shape (xdis / dis_all.txt)
- Extent confirmed: entry 0x00BAE2D0 `sub esp,34h`; last instruction `ret 8` at 0x00BAF9D1 (end 0x00BAF9D4 = 0x1704 = 5892). Two
  `ret 8` tails (0x00BAF9BB and 0x00BAF9D1) = the duplicated ~CScopedShadersEnable pixel half + ~CRenderStateScopedCaptureBlock.
- thiscall, `ret 8` (2 stack args: CEngineSubPrimitiveBase& at [esp+48h], ERenderPassType at [esp+4Ch] -- pass is NEVER read).
- No frame pointer (esp frame, /Oy on), no SEH frame, no /GS cookie. Frame 0x34. Saves ebx, ebp, esi (pushed right after
  `sub esp`), edi pushed late at 0x00BAE30D (scheduled between the shader-enable test). Epilogue pops edi, esi, ebp, ebx.
- /O2-style idioms (constant in register: `mov edi,1` for the bool states, `mov edi,2` / `or [..+3D8h],edi`), no /Os signs.
- Same TU as CEnginePrimitiveManagerSprites (0x00BAD2E0 RenderSubPrimitive, 0x00BAE160 AddSprite precede; 0x00BAF9E0
  CEnginePrimitiveManagerSprites::RenderPrimitive follows, then 0x00BAFA00 2DSprites::RenderClockPrim). Landed rows of this TU carry
  no `// Flags:` (default lane flags).

## 2. Identity (label is RIGHT)
- Vtable 0x012A5664 (CEnginePrimitiveManager2DSprites, rebuild/manifest/rtti-vtables.tsv) slot 5 = 0x00BAE2D0 (xref: the only
  reference is .rdata 0x012A5678). Slot 5 of CEnginePrimitiveManager is RenderSubPrimitive (engine/CEnginePrimitiveManager.h).
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv 00a66570) and FableWin.pdb (fablewin_pdb_names.tsv 02e96c60) both name
  `?RenderSubPrimitive@CEnginePrimitiveManager2DSprites@@UAEXAAVCEngineSubPrimitiveBase@@W4ERenderPassType@@@Z`.
  Signature: `virtual void RenderSubPrimitive(CEngineSubPrimitiveBase& subPrimitive, ERenderPassType pass)` (non-const, void).
- Slots 13/14 (0x00BAFA00 RenderClockPrim, 0x00BB0970 RenderSpritePrim, MAEX = protected virtual) are separate functions; this row
  does not call them. Render2DPrimitive 0x00BACED0 (landed) dispatches on type 0x23 the same way this row does.

## 3. Callees (all landed; names = PDB spellings, no disagreements found)
| retail | landed file | name |
|---|---|---|
| 0x00987FE0 | 00/98/CShaderRenderManager_EnableVertexShaders_00987fe0.cpp | CShaderRenderManager::EnableVertexShaders (via CScopedShadersEnable) |
| 0x009880E0 | .._DisableVertexShaders_009880e0 | DisableVertexShaders (dtor) |
| 0x00988110 | .._EnablePixelShaders_00988110 | EnablePixelShaders |
| 0x00988190 | .._DisablePixelShaders_00988190 | DisablePixelShaders (dtor) |
| 0x00988020 | .._ApplyVertexShader_00988020 | ApplyVertexShader(const CVertexShader&, bool) -- pushed force=0 |
| 0x00988140 | .._ApplyPixelShader_00988140 | ApplyPixelShader(const CPixelShader&) |
| 0x00989AB0 | .._SetUserVertexShaderConstant_00989ab0 | SetUserVertexShaderConstant(unsigned long, const C4DVector&) (KABVC4DVector) |
| 0x0098B930 | .._SetVSConstants_0098b930 | SetVSConstants (PrepareForRender, UpdateFlags&4) |
| 0x00988A20 / 0x00989BF0 / 0x00989760 / 0x009897C0 / 0x0098A760 / 0x00988290 / 0x00988A50 | landed | UpdatePixelShader / UpdatePixelShaderConstants / UpdateAmbient / UpdateFogColour / UpdateLightGlobals / ResetWorldTransform / UpdateCombinedProjectionTransform (all inside inline PrepareForRender) |
| 0x00A0AEA0 | 00/a0/CRenderManagerCore_DrawAsIndexedTris_00a0aea0.cpp | CRenderManagerCore::DrawAsIndexedTris(const CTexture**, unsigned long, const void*, long, long, void*, long) (7 args, engine/CRenderManagerCore.h:72) |
| 0x00A05840 | 00/a0/CRenderStateManager_RestoreCaptureBlock_00a05840.cpp | CRenderStateManager::RestoreCaptureBlock (scoped capture dtor) |
(0x00989760 has a stray Ego_r name-set hit `IsRunnable@CAIStateGroup_StealStealableItem` -- that is an Ego_r image address, ignore.)

## 4. Globals
- 0x013CB508 CRenderStateCaptureBlock::StateManager (engine/CRenderStateCaptureBlock.h) -- StartCaptureBlock inline at entry.
- 0x013BC470 CShaderRenderManager::SingletonInstance; 0x013BC800 VertexShadersEnabled, 0x013BC801 PixelShadersEnabled (its members,
  read by CScopedShadersEnable / SetVertexShadersEnable). Use engine/CScopedShadersEnable.h exactly as 0x00BBC460 does.
- 0x01436E18 NEngineInternal::GPRenderManager (CRenderManager*), 0x01436E14 NEngineInternal::GPShaderRenderManager.
- 0x01436E84 CEnginePrimitiveTypeInfo* (+0x94 = PrimitiveManagers[33] = ENGINE_PRIMITIVE_2D_VIEWPORT; engine/CEnginePrimitiveTypeInfo.h,
  spelled g_pEnginePrimitiveTypeInfo in 0x00BC25F0 / landed rows; no PDB data name).
- 0x01436E78 the primitive renderer / shared data (PixelShader2D* members at +0x19C..+0x1DC). Landed spellings disagree
  (g_pPrimitiveRenderer, g_pPrimitiveRendererSharedData, ...); 0x00BA1B00 uses `CEnginePrimitiveRenderer* g_pPrimitiveRenderer`.
- Tables (private statics of CRenderStateManager, engine/CRenderStateManager.h): 0x01396DC8/0x01396DD0 D3DTextureUVModeTable[0]/[2]
  (WRAP/CLAMP), 0x01396E58 D3DTextureFilterModeTable[0] (POINT), 0x01396E6C D3DTextureMipMapModeTable[2] (DISABLE),
  0x01396F6C/F74/F78/F7C D3DAlphaBlendModeTable[1]/[3]/[4]/[5] (ONE / INV_SRC_COLOR / SRC_ALPHA / INV_SRC_ALPHA).
- Float pool: 0x01231724 = 1/255, 0x0122DCB4 = 2^32 (unsigned long -> float fix-up).

## 5. Members read (PDB names; Ego_r offsets == retail for all of these)
- this (CEnginePrimitiveManager2DSprites, engine/CEnginePrimitiveManager2DSprites.h is a FLAT generated header, no base, shaders as
  byte arrays): VertexShader2DSprite +0x0C (CVertexShader), PixelShader2DClockSprite +0x1C, PixelShader2DClockSpriteAdditive +0x24
  (CPixelShader), Indices +0x2C (unsigned short[6]). VertexShader2DClockSprite +0x14 is NOT used. Only opt-in:
  FABLE_RETAIL_CENGINEPRIMITIVEMANAGER2DSPRITES_VIRTUAL_DTOR. Author needs a new overlay opt-in: `class CEnginePrimitiveManager2DSprites
  : public CEnginePrimitiveManager` (engine/CEnginePrimitiveManager.h, 0xC, base CEnginePrimitiveRenderUtil) with typed CVertexShader /
  CPixelShader members, slot-5 RenderSubPrimitive and slots 13/14 (RenderClockPrim/RenderSpritePrim).
- CEngineSubPrimitiveBase (engine/CEngineSubPrimitiveBase.h, members PRIVATE): Alpha +0x08 (PeekAlpha, FableWin
  ?PeekAlpha@CEngineSubPrimitiveBase@@QBEEXZ), Parent +0x0C (?GetParentBase / ?PeekParentBase -- opt-in
  FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_PARENTBASE exists only in /opt/fable_lane/w22/agent2059/hdr/engine/CEngineSubPrimitiveBase.h;
  PeekAlpha needs adding too).
- Parent->Type +0x08 (CEnginePersistentPrimitiveBase::GetType(), engine/CEnginePersistentPrimitiveBase.h, under
  FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED), compared with 0x23 = ENGINE_PRIMITIVE_2D_CLOCK_SPRITE (CEnginePrimitiveEnums.h).
- CEngineSubPrimitive2DClockSprite (Ego_r 316 bytes; NO shared header): Vertices +0x18 (CTVertexColTex2[7], stride 0x20), Indices
  +0xF8 (ushort[18]), IndexOffset +0x11C (uchar), Texture +0x120 (CTexture*[2]), Colour +0x128, Colour2 +0x12C (CRGBColour, bytes
  B,G,R,A), AlphaMask +0x130, Filter +0x134, WrapTexture +0x135, BlendMode +0x138.
- CEngineSubPrimitive2DSprite (Ego_r 168 bytes; NO shared header): Vertices +0x18 (CTVertexColTex2[4]), Texture +0x98, AlphaMask
  +0x9C, Filter +0xA0, WrapTexture +0xA1, BlendMode +0xA4 (EEnginePrimitiveBlendMode, engine/CEngineRenderEnums.h).
- C2DAlphaMaskInfo (engine/C2DAlphaMaskInfo.h): AlphaMaskPositionFactors +0x08 (C4DVector), FilterMask +0x29 (bool). It is
  CEnginePrimitive2DViewportManager::AlphaMaskInfo (+0x0C; engine/CEnginePrimitive2DViewportManager.h, opt-in
  FABLE_RETAIL_CENGINEPRIMITIVE2DVIEWPORTMANAGER, declares GetCurrentAlphaMask()).
- CRenderStateManager (GPRenderManager; FABLE_RETAIL_CRENDERSTATEMANAGER via CRenderStateCaptureBlock.h): DeviceState +0x2818;
  states used: ZBufferEnable 0x2868, ZBufferWriteEnable 0x2854, AlphaBlendEnable 0x28B8, AlphaTestEnable 0x28CC, AlphaRef 0x28E0,
  SourceAlphaBlendMode 0x291C, DestAlphaBlendMode 0x2930, TextureFactor 0x29D0, TextureMipMapMode[0/1] 0x2E44/0x2E58,
  TextureUVModeU[0/1] 0x2EE4/0x2EF8, TextureUVModeV[0/1] 0x2F84/0x2F98, TextureFilterModeMin[0/1] 0x30C4/0x30D8,
  TextureFilterModeMag[0/1] 0x3164/0x3178. Every one is the inline SetRenderState(value, state) (bookmark + update list).
- CShaderRenderManager (engine/CShaderRenderManager.h): VertexShadersEnabled/PixelShadersEnabled, WorldMatrixIsIdentity +0x1E8,
  PixelShadersEnabled +0x391 (GetPixelShadersEnabled), PSConstTable +0x3CC (CArray<C4DVector> begin), UpdateFlags +0x3D8,
  CurrentVSConstantLayout +0x04 (->ReservedPresets +0x140/+0x144, Reserved.Offset +8) -- all through inline PrepareForRender /
  SetPixelShaderConstant.
- CEnginePrimitiveRendererSharedData at *0x01436E78: PixelShader2DDiffuseOnly +0x19C, PixelShader2DDiffuse +0x1AC,
  PixelShader2DDiffuseOnlyAdditiveAlpha +0x1CC, PixelShader2DDiffuseAdditiveAlpha +0x1DC (Ego_r struct_layouts). The shared
  header's FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_MEMBERS stops at +0xFC..+0x262 opaque: overlay must add these (also
  2DTextureOnly +0x1A4, 2DDiffuseEnvBump +0x1BC per struct_layouts).

## 6. Inline helpers retail expanded (visibility; FableWin COMDAT run right after the row proves the set)
FableWin.pdb order: 02e96c40 CEngineScreenEffectDisplacementRenderer::GetEnable2DDisplacement (not used here), 02e96c60 THIS ROW,
then 02e97220 `?SetupAlphaBlendingStatesFromBlendMode@CEnginePrimitiveRenderUtil@@QAEXW4EEnginePrimitiveBlendMode@@KAAK@Z` (0xC0),
02e972e0 `?Setup2DDiffuseShading@CEnginePrimitiveRenderUtil@@QAEX_NKKW4EEnginePrimitiveBlendMode@@@Z` (0xD0), 02e973b0
`?GetPixelShader2DDiffuseOnly@CEnginePrimitiveRenderUtil@@SIABVCPixelShader@@W4EEnginePrimitiveBlendMode@@@Z` (0x50). Elsewhere:
GetPixelShader2DDiffuse (SI, 02e739a0, 0x50), GetCurrent2DAlphaMaskInfo (QAE PBV, 02e740e0, 0x40),
CEnginePrimitive2DViewportManager::GetCurrentAlphaMask (02e74120, 0x20), DisableTextureFiltering(K) (QAEX, 02e74090, 0x50),
SetTextureFactor(const CRGBFloatColour&) (SI, 02e73e50), the bool overload SetupAlphaBlendingStatesFromBlendMode (QAE_N, 02eac0c0, 0xA0).
None of these is in the Ego_r name set and none has a retail out-of-line copy reached from here -> all inline (FABLE_FORCEINLINE in
the row / overlay, like 0x00BC25F0 and 0x00BA1B00). Reconstructed bodies (match the retail blocks exactly):
- DisableTextureFiltering(stage): SetTextureFilterModeMag(stage, POINT); SetTextureFilterModeMin(stage, POINT);
  SetTextureMipMapMode(stage, TEXTURE_MIPMAP_DISABLE) -- retail order Mag, Min, MipMap.
- SetupAlphaBlendingStatesFromBlendMode(mode, alpha, colour_scale&): ADDITIVE(3): Source ONE, Dest ONE, scale = alpha;
  ADDSMOOTH(4): Source ONE, Dest INV_SRC_COLOR, scale = alpha; default: Source SRC_ALPHA, Dest INV_SRC_ALPHA, scale = 0xFF
  (retail: `sub ecx,3 ; je ; dec ecx ; je`, default first; result in eax: `mov eax,ebx` / `mov eax,0FFh`). The bool overload's
  body text is in /opt/fable_lane/w22/agent2059/src/00bc25f0.cpp:150 (switch returns true/true/false); FableWin 0xC0 vs 0xA0 says
  the KAAK one has its own switch (fwsize it).
- Setup2DDiffuseShading(bool textured, unsigned long colour_scale, unsigned long alpha, mode): SetPixelShaderConstant(2,
  C4DVector(s,s,s,a)) with s = colour_scale/255.f, a = alpha/255.f (unsigned->float: fild + jge + fadd 2^32), then
  ApplyPixelShader(textured ? GetPixelShader2DDiffuse(mode) : GetPixelShader2DDiffuseOnly(mode)); GetPixelShader2DDiffuse: mode ==
  ENGINE_PRIMITIVE_BLEND_ALPHA(2) ? PixelShader2DDiffuse : PixelShader2DDiffuseAdditiveAlpha; ...DiffuseOnly likewise (+0x19C/+0x1CC).
- SetTextureFactor(CRGBFloatColour(1,1,1,1)): body already in 0x00BA1B00 (rebuild/src/compiled/00/ba/
  CEnginePrimitiveManagerStaticMeshesBase_DisplayMirroredTextures_00ba1b00.cpp:110, opt-in
  FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_SETTEXTUREFACTOR + CRenderStateManager::SetTextureFactor + GetPixelShadersEnabled defs);
  retail here: if PixelShadersEnabled -> constant 2 = (1,1,1,1) through stack temp; else TextureFactor state = 0xFFFFFFFF built as
  four 0xFF byte stores into [esp+10h] (folded GetRGBColour()).
- ApplyVertexShader/ApplyPixelShader statics forward to GPShaderRenderManager (force=false) -- bodies in 0x00BC25F0 src.
- GetCurrent2DAlphaMaskInfo(): ((CEnginePrimitive2DViewportManager*)g_pEnginePrimitiveTypeInfo->PrimitiveManagers[ENGINE_PRIMITIVE_2D_VIEWPORT])
  ->GetCurrentAlphaMask() = &AlphaMaskInfo (retail: `mov eax,[1436E84h] ; mov ebx,[eax+94h] ; add ebx,0Ch`).
- PrepareForRender(flags): only the flag-independent tests appear (UpdateFlags 4/1/2/4/0x20000/0x400, !WorldMatrixIsIdentity,
  0x80) -> flags = 0 (as 0x009DA9F0 / 0x00B5AF80 use `PrepareForRender(0)`).
- CRGBFloatColour(const CRGBColour&) (FABLE_RETAIL_CRGBFLOATCOLOUR) + C4DVector(const CRGBFloatColour&) for pixel constant 5.

## 7. Siblings / family models
- 0x00BBC460 CEnginePrimitiveManagerParticleGroup::RenderSubPrimitive (rebuild/src/compiled/00/bb/...00bbc460.cpp, Flags: none):
  shared-header model of exactly this prologue/epilogue: `CRenderStateScopedCaptureBlock capture; CScopedShadersEnable shaders(true,
  true);` + `NEngineInternal::GPRenderManager->Set*(...)`; defines FABLE_RETAIL_CENGINELIGHTINGMANAGER,
  FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS; includes engine/CRenderManager.h, CRenderStateCaptureBlock.h,
  CShaderRenderManager.h, CScopedShadersEnable.h.
- 0x00BC25F0 ParticleMeshGroup::RenderSubPrimitive (agent2059 src + hdr/ overlay, RELOCATION_MATCH): the CEnginePrimitiveRenderUtil
  inline-helper pattern (ApplyVertexShader/ApplyPixelShader/SetupAlphaBlendingStatesFromBlendMode bodies), its macro list.
- 0x00BA1B00 DisplayMirroredTextures: SetTextureFactor + SharedData pixel shaders + PrepareForRender expansion.
- 0x00BAD2E0 CEnginePrimitiveManagerSprites::RenderSubPrimitive (same TU, old local-model style -- do not copy its local structs).
- 0x00BACED0 Render2DPrimitive (same class, local model).
- Lane lesson (agent2059 NOTES): the SetRenderState bookmark test/or operand order varies block by block in retail; helper
  spelling (going through the shared helper with constant args) fixed it there.
