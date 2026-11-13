# DOSSIER 0x00B5CA70 -- CEngineScreenEffectRadialBlurRenderer::CaptureBackBuffer (5748 bytes)

No prior scouting existed ($W/prior/ empty; /opt/fable_lane/w22/agent2086 holds only the brief + dis_all, no cand/ or notes).
$W/dis_short.txt = dis_all.txt without the byte column (made by this scout). $W/rdc.py reads retail constants
(`python3 $W/rdc.py f<addr> d<addr> h<addr> i<addr>` = float / double / 6 x u16 / u32).

## 1. Retail shape (xdis 00b5ca70 0x1684; dis_all.txt)
- `__thiscall`, `this` -> esi for the whole body (except pass D tail, which reuses esi, see OUTLINE). Plain `ret` (0x00B5E0E3, `c3`).
- NO frame pointer: `sub esp,88h ; (mov eax,[13CB508h]) ; push ebx ; push ebp ; push esi ; (mov esi,ecx) ; push edi`. Epilogue
  `pop edi/esi/ebp/ebx ; add esp,88h ; ret`. Frame = 0x88 locals, saved regs pushed AFTER the sub.
- NO SEH prologue although the body has destructible locals (CRenderTarget x4, CSurface temps, scoped capture block / layout /
  shader enables): the TU is built without /EHsc -- the lane default BASE_FLAGS (/O2 /Oy /W3) is right; no `// Flags:` needed
  (siblings ApplyRadialBlur 0x00B5E0F0 / ShowBlurTexture 0x00B5E700 / Render 0x00B5EA30 carry none either). No /GS cookie.
- ebx = 0 for the whole function (`xor ebx,ebx` @0x00B5CAF6), used for every 0/false/NULL.
- Single function: last instruction is the ret at 0x00B5E0E3 (+int3 to 0x00B5E0F0 = ApplyRadialBlur). Length 5748 is right.

## 2. Name / signature (two+ sources)
- `?CaptureBackBuffer@CEngineScreenEffectRadialBlurRenderer@@AAEXXZ` -- private, `void CaptureBackBuffer()`, non-const.
  Sources: (a) Ego_r.pdb (ghidra_out/egor_pdb_names.tsv 0x00A0E550, between CaptureEnvironmentParameters 0x00A0E420 and
  ApplyRadialBlur 0x00A0FE90 -- same order as retail 0x00B5BD70 .. 0x00B5CA70 .. 0x00B5E0F0); (b) FableWin.pdb
  (naming_stragglers/FableWin.exe.signatures.tsv 0x02D1E360, debug body 6243 b / 1514 instr -> fwsize oracle for the row
  with FW_EXTRA=/EHsc); (c) the only caller is Render 0x00B5EA30 (call @0x00B5EA6D), whose landed source already declares
  `void CaptureBackBuffer(); // 0x00B5CA70` and calls it right before ApplyRadialBlur. Manifest label is CORRECT.
- Not a virtual (not in rtti-vtables.tsv for vtable 0x012A2658).
- The shared header's retail branch (FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER_MEMBERS) declares only ApplyRadialBlur /
  ShowBlurTexture: an overlay must add `void CaptureBackBuffer(); // 0x00B5CA70 ?CaptureBackBuffer@...@@AAEXXZ` to the private
  method list (copy the current header into $W/hdr/, add under the same #ifdef).

## 3. Callees (landed file / landed name / PDB name)
All landed. Files under rebuild/src/compiled/<aa>/<bb>/.
| retail | landed file | PDB name | note |
|---|---|---|---|
| 0x0098B5E0 | CShaderRenderManager_SetVertexShaderConstantLayout_0098b5e0.cpp | ?SetVertexShaderConstantLayout@CShaderRenderManager@@ (ecx = &SingletonInstance 0x013BC470) | via CScopedVertexShaderConstantLayout(2) |
| 0x00F36EE0 | CGPUScratchBufferTextureManager_StartSurfaceUse_00f36ee0.cpp (struct S::M placeholder) | **?StartTextureUse@CGPUScratchBufferTextureManager@@QAEPAVCTexture@@J@Z** | **NAME_FIX**: landed/manifest "StartSurfaceUse" is wrong. Evidence: result stored into BlurTexture256_1/_2 (CTexture*); Ego_r order StartTextureUse 0x00B3C370 / EndTextureUse 0x00B3C390 / StartSurfaceUse 0x00B3C3A0 / EndSurfaceUse 0x00B3C3C0 = retail 0x00F36EE0 (0x20 b) / 0x00F36F00 (`ret 4` nop, what Render calls to release the textures) / 0x00F36F10 / 0x00F36F30 -- same spacing. Also 0x00F36F00 "Nop_ret4" = ?EndTextureUse@...@@QAEXJ@Z |
| 0x0099A2D0 / 0x0099A2E0 | CBaseClass_CBaseClass_0099a2d0.cpp / _0099a2e0.cpp | CBaseClass ctor / dtor | out of line inside the inline CRenderTarget ctor/dtor (engine/CBaseClass.h) |
| 0x009F9E00 | CTexture_GetSurfaceLevel_009f9e00.cpp | ?GetSurfaceLevel@CTexture@@QBE?AVCSurface@@J@Z | hidden result (`push 0 ; push &tmp`) ; engine/CTexture.h declares it |
| 0x00A10E60 | CRenderTarget_AttachTarget_00a10e60.cpp | ?AttachTarget@CRenderTarget@@QAEXABVCSurface@@@Z (Ego_r 0x00B7EE70) | colour-surface overload |
| 0x009F2E20 | CSurface_Uninitialise_009f2e20.cpp | ?Uninitialise@CSurface@@QAEXXZ | the INLINE ~CSurface (vptr 0x0122F84C stored, then Uninitialise) -> FABLE_RETAIL_CSURFACE_COPYFROMMEMORY (+ _FORCEINLINE_DTOR if VC7.1 refuses to inline in this big body) |
| 0x009BF160 | CDisplayManager_SetRenderTarget_009bf160.cpp | ?SetRenderTarget@CDisplayManager@@QAE_NAAVCRenderTarget@@@Z (Ego_r 0x00B23F00) | reference arg, bool result dropped; NOT declared in engine/CDisplayManager.h (only CRenderTarget& in the water row's local model; the Displacement row's local model spells it with a pointer -- wrong, PDB says `AAV`) |
| 0x009BF5E0 | CDisplayManager_CopyBackBufferToTexture_009bf5e0.cpp | ?CopyBackBufferToTexture@CDisplayManager@@QAEAAVCTexture@@XZ | manifest says `_global::` (wrong module); header FABLE_RETAIL_CDISPLAYMANAGER_BACKSCREEN declares it |
| 0x009BF220 | CDisplayManager_SetRenderTargetAsBackScreen_009bf220.cpp | ?SetRenderTargetAsBackScreen@CDisplayManager@@QAE_NXZ | x5 (once per pass + once after pass D); same opt-in |
| 0x00987FE0/0x009880E0/0x00988110/0x00988190 | CShaderRenderManager_Enable/DisableVertexShaders, Enable/DisablePixelShaders | same | via CScopedShadersEnable (pass A only) |
| 0x00988020 | CShaderRenderManager_ApplyVertexShader_00988020.cpp | ApplyVertexShader(CVertexShader, bool) | args pushed: `push 0 ; push &shader` |
| 0x00988140 | CShaderRenderManager_ApplyPixelShader_00988140.cpp | ApplyPixelShader | |
| 0x00989B00 | CShaderRenderManager_SetUserVertexShaderConstant_00989b00.cpp | ?SetUserVertexShaderConstant@CShaderRenderManager@@QAEXKMMMM@Z | x22 |
| 0x0098B930, 0x00988A20, 0x00989BF0, 0x00989760, 0x009897C0, 0x0098A760, 0x00988290, 0x00988A50 | CShaderRenderManager_SetVSConstants / UpdatePixelShader / UpdatePixelShaderConstants / UpdateAmbient / UpdateFogColour / UpdateLightGlobals / ResetWorldTransform / UpdateCombinedProjectionTransform | same | all from the inline PrepareForRender(0) (x4) |
| 0x00A0AF40 | CRenderManagerCore_DrawAsIndexedTris_00a0af40.cpp | ?DrawAsIndexedTris@CRenderManagerCore@@QAEXPBXJJPAXJ@Z | pass A; FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASINDEXEDTRIS_USER |
| 0x00A0AEA0 | CRenderManagerCore_DrawAsIndexedTris_00a0aea0.cpp | ?DrawAsIndexedTris@CRenderManagerCore@@QAEXPAPBVCTexture@@KPBXJJPAXJ@Z | passes B/C/D, texture count 4; FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASINDEXEDTRIS |
| 0x00A05840 | CRenderStateManager_RestoreCaptureBlock_00a05840.cpp | ?RestoreCaptureBlock@CRenderStateManager@@QAEXXZ | ~CRenderStateScopedCaptureBlock |
| 0x00BFEB40 | global_CIpow_00bfeb40.cpp (toolchain) | MSVCR71 _CIpow | from `pow(double,double)`: `fld dword [this+1A4h] ; fld qword [const] ; call _CIpow` x12 |
| [dev+0x104] | -- | IDirect3DDevice9::SetTexture (slot 65) | inside inline AttachD3DTextureToStage (x2, pass A) |

## 4. Members / globals
`this` = engine/CEngineScreenEffectRadialBlurRenderer.h, opt-ins FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER +
`_MEMBERS` (Ego_r names = struct_layouts_egor.tsv; offsets verified against retail):
BlurTexture256_1 +0x0C (CTexture*, written then read), BlurTexture256_2 +0x10, BlurTextureHandle256_1 +0x14 / _2 +0x18 (long),
ScreenCopyVertexShader +0x20, RadialBlurVertexShader +0x28, ScreenCopyPixelShader +0x30, RadialBlurPixelShader +0x38,
BackBufferToTextureVerts +0x50 (CTVertexColTex1[4], stride 0x18), Blur256ToBlur256Verts +0xB0, BlurCenter +0x170 (C2DVector X/Y),
AreaCenter +0x178, CurrentParameters (CParameters +0x198): ThresholdR/G/B +0x198/19C/1A0, IntensityR +0x1A4, Length +0x1B0,
Range +0x1B8, Twist +0x1BC. Not touched: Fade, IntensityG/B, TextureOnly/TextureDiffuse shaders, the other vert sets.
Globals (spell as the landed siblings ApplyRadialBlur / RenderColorFilteredQuad do):
- 0x01436E14 `NEngineInternal::GPShaderRenderManager` (CShaderRenderManager*): PSConstTable +0x3CC (CArray<C4DVector>, `.begin()`
  = [+0x3CC]), UpdateFlags +0x3D8, CurrentVSConstantLayout +0x4, WorldMatrixIsIdentity +0x1E8 -> engine/CShaderRenderManager.h
  inline SetPixelShaderConstant(index, const C4DVector&) and PrepareForRender(flags).
- 0x013BC470 CShaderRenderManager::SingletonInstance (Get()): CurrentVSConstantLayoutIndex +0x8 (0x013BC478),
  VertexShadersEnabled +0x390 (0x013BC800), PixelShadersEnabled +0x391 -> CScopedVertexShaderConstantLayout.h / CScopedShadersEnable.h.
- 0x01436E18 `NEngineInternal::GPRenderManager` (CRenderManager*): render-state core: StateStack +0x8, dirty list +0x2008, counts
  +0x2808/+0x280C, bookmark mask +0x2814, DeviceState at +0x2818: ZBufferEnable +0x2868, PolyCullMode +0x2890, AlphaBlendEnable
  +0x28B8, TextureUVModeU[0] +0x2EE4, TextureUVModeV[0] +0x2F84; TextureStagesInUse +0x3CB8, PD3DDevice +0x3CF0,
  CurrentlySelectedTextures[0] +0x3D00 -> engine/CRenderStateManager.h + engine/CRenderManagerCore.h setters
  SetZBufferEnable / SetPolyCullMode / SetAlphaBlendEnable / SetTextureUVModeU/V / AttachTextureToStage.
- 0x013CB508 CRenderStateCaptureBlock::StateManager (StartCaptureBlock inline, bookmark dummy +0x3A24) -> CRenderStateCaptureBlock.h.
- 0x01436E1C `NEngineInternal::GPDisplayManager` (CDisplayManager*), 0x01436E98 the resource manager (`mov edi,[1436E98h] ;
  add edi,3190h` = CEngineResourceManager::GetGPUScratchBufferTextureManager(), engine/CEngineResourceManager.h; name the
  global as an existing landed row does -- three spellings exist in landed code: g_pResourceManager / g_pEngine_01436e98 /
  g_pEngineData; pick g_pResourceManager-style with the header type, and note it descriptive).
- Tables: 0x01396FB8 = D3DPolyCullModeTable[POLY_CULL_NONE] (table 0x01396FB0), 0x01396DD0 = D3DTextureUVModeTable[TEXTURE_UV_CLAMP]
  (table 0x01396DC8) -- static members in CRenderStateManager.h.
- Function-local `static unsigned short QuadIndices[6] = {0,1,2,0,2,3};` @ 0x0139A538 (ONE array used by all four draws;
  ApplyRadialBlur's {0,1,3,1,2,3} is 0x0139A544 and ShowBlurTexture's 0x0139A550 -> same TU, defined in this order).
- Locally modelled classes to AVOID: there is no shared header for CRenderTarget (Ego_r ??0CRenderTarget@@QAE@AAVCSurface@@0@Z;
  layout: CBaseClass base vptr 0x0129C800 +0x0, CSurface RenderToSurface +0x4, CSurface ZBufferSurface +0x14, sizeof 0x24? -- the
  retail init writes 0x44..0x68 i.e. RenderToSurface vptr/PD3DSurface/AllocationSource and the same for ZBufferSurface; the
  landed rows 0x00B87680 / 0x00B89A20 / 0x00B7AD20 each model it locally). Model it locally exactly as
  CEngineScreenEffectDisplacementRenderer_BeginDisplacementRendering_00b87680.cpp does (CRenderTarget : CBaseClass, two CSurface,
  inline default ctor `CRenderTarget() {}`), but take CSurface from engine/CSurface.h and CBaseClass from engine/CBaseClass.h.

## 5. Callee-body visibility
- Inline (expanded in retail, no call): StartCaptureBlock, the CScopedVertexShaderConstantLayout ctor/dtor bodies (the calls are
  out of line SetVertexShaderConstantLayout), CScopedShadersEnable (out-of-line Enable/Disable*), SetPixelShaderConstant,
  PrepareForRender(0), the render-state setters (SetRenderState capture/dirty push), AttachTextureToStage /
  AttachD3DTextureToStage (needs FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS, else it calls the COMDAT 0x009A0CF0),
  GetGPUScratchBufferTextureManager, CRenderTarget default ctor + dtor (except CBaseClass parts), ~CSurface (inline:
  vptr store + Uninitialise call; FABLE_RETAIL_CSURFACE_COPYFROMMEMORY gives the inline dtor, add
  FABLE_RETAIL_CSURFACE_FORCEINLINE_DTOR if the compiler emits `call ??1CSurface`), C4DVector(x,y,z,w) ctor.
- Out of line: CBaseClass ctor/dtor, GetSurfaceLevel, AttachTarget, all CDisplayManager calls, SetUserVertexShaderConstant,
  ApplyVertex/PixelShader, the Update* callees, DrawAsIndexedTris (both), RestoreCaptureBlock, _CIpow, StartTextureUse.
- sin/cos are the /Oi intrinsics on float (`fsin`/`fcos` inline): use `sin(float)`/`cos(float)` (math.h C++ overloads -> sinf/cosf)
  or sinf/cosf directly; no asm issue. No fistp, no SSE -> no Retail-asm lines needed.
- No STLport in this body.

## 6. Siblings to copy idioms from
- CEngineScreenEffectRadialBlurRenderer_ApplyRadialBlur_00b5e0f0.cpp (same class, shared-header idiom: defines
  FABLE_RETAIL_CRENDERMANAGERCORE, _DRAWASINDEXEDTRIS, FABLE_RETAIL_CTEXTURE, the two RADIALBLURRENDERER macros; includes
  CRenderManager.h, CShaderRenderManager.h, CRenderStateCaptureBlock.h, CScopedShadersEnable.h, CScopedVertexShaderConstantLayout.h;
  `CRenderStateScopedCaptureBlock capture; CScopedVertexShaderConstantLayout layout(2); CScopedShadersEnable shaders(true,true);`,
  function-local static QuadIndices, NEngineInternal::GP* externs). NO pragmas, NO Flags line. MODEL THIS ONE.
- CEngineScreenEffectColourFilter_RenderColorFilteredQuad_00b5af80.cpp (neighbour TU family: CopyBackBufferToTexture-style
  includes, FABLE_RETAIL_CDISPLAYMANAGER + _BACKSCREEN, FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS for inline
  AttachTextureToStage, _DRAWASINDEXEDTRIS_USER, SetPixelShaderConstant(5..8, C4DVector), SetTextureUVModeU/V(0, TEXTURE_UV_CLAMP)).
- CEngineScreenEffectDisplacementRenderer_BeginDisplacementRendering_00b87680.cpp and
  CEngineScreenEffectOutlineGlow_BeginOutlineRendering_00b89a20.cpp: CRenderTarget + GetSurfaceLevel + AttachTarget + SetRenderTarget.
- CEngineScreenEffectRadialBlurRenderer_Render_00b5ea30.cpp (caller; local texture-pool model at 0x01436E98+0x3190).
- ShowBlurTexture_00b5e700.cpp is the OLD local-model idiom (do not copy its local structs).
