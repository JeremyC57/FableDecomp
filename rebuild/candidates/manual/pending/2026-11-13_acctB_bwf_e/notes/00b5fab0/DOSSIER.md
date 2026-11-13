# DOSSIER 0x00B5FAB0 -- CEngineScreenEffectGlowRenderer::CaptureBackBuffer (5302 bytes)

## 1. Retail shape (xdis 00b5fab0 0x14c6, $W/dis_all.txt)
- `sub esp,0C0h ; push ebx ; push ebp ; push esi ; push edi` ... `pop edi/esi/ebp/ebx ; add esp,0C0h ; ret` (0x00B60F65).
  __thiscall, no args, plain `ret` (no `ret N`). NO frame pointer (/Oy on, default). NO SEH prologue (no fs:0, no EH state
  stores) although the body has locals with destructors (CRenderTarget, CPixelShader copy, CScopedShadersEnable, temp CSurface)
  => TU built WITHOUT /EHsc (same as the landed same-TU rows ShowGlowTexture 0x00B61690, Render 0x00B619B0: no Flags line).
  No /GS cookie. Locals at [esp+10h .. esp+0CFh] (0xC0 bytes) -- every "frame offset" below is esp-relative with no pushes pending.
- ebx = 0 for the whole body (`xor ebx,ebx` at 0x00B5FB37), esi = this. Constant 1.0f / 0.0f pushes as immediates.
- One loop (0x00B60650..0x00B60CA6, `jl` back-edge), aligned with a single `nop` at 0x00B6064F.

## 2. Identity (name proven by two sources)
- Ego_r.pdb `?CaptureBackBuffer@CEngineScreenEffectGlowRenderer@@AAEXXZ` (Ego_r 0x00A11CD0, size 0x1570) and FableWin.pdb
  (_fablewin_api.tsv / FableWin.exe.signatures.tsv 0x017DE0B0) -- private, `void CaptureBackBuffer()`, non-const, no params.
- Retail position matches Ego_r order exactly: Ego_r ~dtor 11940 / SetValuesFromEnvironmentTheme 11A70 / CaptureBackBuffer 11CD0 /
  ApplyGlow 13240 / ShowGlowTexture 13920 / Render 13C30  <->  retail 0xB5EE80 / 0xB5F8C0 / 0xB5FAB0 / 0xB60F70 / 0xB61690 / 0xB619B0
  (sizes 0x1570/0x14B6, 0x6E0/0x720, 0x310/0x320 agree).
- Sole caller: Render 0x00B619B0 (`call 0xb5fab0` at 0x00B619C8) -- order there: [if UseEnvironmentTheme] 0xB5F8C0, then
  0xB5FAB0 (this), 0xB60F70 ApplyGlow, [if ShowDebug] 0xB61690 ShowGlowTexture, 0xB5EC90.
- Manifest label CaptureBackBuffer is CORRECT. Leaf: `?CaptureBackBuffer@CEngineScreenEffectGlowRenderer@@AAEXXZ`.
- Header declares neither it nor the other privates; declare it in the hdr overlay under ..._MEMBERS (private: void CaptureBackBuffer();).

### Neighbour mis-names (NAME_FIXES candidates, evidence = Ego_r order/size + Render's call order + bodies)
| retail | landed name | PDB name |
|---|---|---|
| 0x00B5F8C0 | ComputeGlowColor | ?SetValuesFromEnvironmentTheme@CEngineScreenEffectGlowRenderer@@AAEXXZ (Render calls it only if UseEnvironmentTheme +0xC) |
| 0x00B5EC90 | ReleaseGlowTextures | ?FinishWithDynamicTextures@CEngineScreenEffectGlowRenderer@@AAEXXZ (4x EndTextureUse 0xF36F00) |
| 0x00B5ECE0 | CGlowRendererMethods::InitialiseTextures | ?InitialiseTextures@CEngineScreenEffectGlowRenderer@@AAE_NXZ (class wrong) |
| 0x00F36EE0 | CGPUScratchBufferTextureManager::StartSurfaceUse | ?StartTextureUse@CGPUScratchBufferTextureManager@@QAEPAVCTexture@@J@Z |
| 0x00F36F10 | (StartSurfaceUse2 in 0xBAC0F0 model) | ?StartSurfaceUse@CGPUScratchBufferTextureManager@@QAEPAVCSurface@@J@Z |
| 0x00F36F00 | "texture pool Release" | ?EndTextureUse@CGPUScratchBufferTextureManager@@QAEXJ@Z |
Evidence for 0xF36EE0 = StartTextureUse: Ego_r order StartTextureUse 0xB3C370 (0x20), EndTextureUse 390 (0x10), StartSurfaceUse 3A0
(0x20), EndSurfaceUse 3C0 == retail 0xF36EE0 (0x20 body returns entry+4) / F36F00 (`ret 4`) / F36F10 (returns entry+8) / F36F30;
and this row stores 0xF36EE0's result into GlowTexture256..16 (CTexture*).

## 3. Callees (all `call rel32`; one vtable call = IDirect3DDevice9::SetTexture slot 0x104 inside the inline AttachTextureToStage)
| retail | PDB name (spelling to use) | landed row | object |
|---|---|---|---|
| 0x00F36EE0 x4 | CGPUScratchBufferTextureManager::StartTextureUse(long) -> CTexture* | 00/f3/..._StartSurfaceUse_00f36ee0.cpp (WRONG name) | GPResourceManager(0x01436E98)+0x3190 |
| 0x0099A2D0 x5 | CBaseClass::CBaseClass() | 00/99/CBaseClass_CBaseClass_0099a2d0.cpp | CRenderTarget base (inline CRenderTarget ctor) |
| 0x0099A2E0 x5 | CBaseClass::~CBaseClass() | 00/99/CBaseClass_CBaseClass_0099a2e0.cpp | inline ~CRenderTarget tail |
| 0x009F9E00 x5 | CTexture::GetSurfaceLevel(long) const -> CSurface (hidden ret) | 00/9f/CTexture_GetSurfaceLevel_009f9e00.cpp | |
| 0x00A10E60 x5 | CRenderTarget::AttachTarget(const CSurface&) (colour overload) | 00/a1/CRenderTarget_AttachTarget_00a10e60.cpp | |
| 0x009F2E20 x15 | CSurface::Uninitialise() | 00/9f/CSurface_Uninitialise_009f2e20.cpp | inline ~CSurface (temp + 2 members) |
| 0x009BF160 x5 | CDisplayManager::SetRenderTarget(CRenderTarget&) -> bool (ignored) | 00/9b/CDisplayManager_SetRenderTarget_009bf160.cpp | GPDisplayManager 0x01436E1C |
| 0x009BF220 x5 | CDisplayManager::SetRenderTargetAsBackScreen() -> bool | 00/9b/CDisplayManager_SetRenderTargetAsBackScreen_009bf220.cpp | 0x01436E1C |
| 0x009BF5E0 x1 | CDisplayManager::CopyBackBufferToTexture() -> CTexture& | 00/9b/CDisplayManager_CopyBackBufferToTexture_009bf5e0.cpp (manifest class `_global`) | 0x01436E1C |
| 0x00987FE0/0x009880E0 | CShaderRenderManager::EnableVertexShaders/DisableVertexShaders | landed 00/98 | static 0x013BC470 |
| 0x00988110/0x00988190 | EnablePixelShaders/DisablePixelShaders | landed 00/98 | static 0x013BC470 |
| 0x00988020 x5 | ApplyVertexShader(const CVertexShader&, bool) | landed | GPShaderRenderManager 0x01436E14 |
| 0x00988140 x5 | ApplyPixelShader(const CPixelShader&) | landed | 0x01436E14 |
| 0x00989B00 x16 | SetUserVertexShaderConstant(unsigned long, float, float, float, float) | landed | 0x01436E14 |
| 0x0098B930, 0x00988A20, 0x00989BF0, 0x00989760, 0x009897C0, 0x0098A760, 0x00988290, 0x00988A50 | SetVSConstants, UpdatePixelShader, UpdatePixelShaderConstants, UpdateAmbient, UpdateFogColour, UpdateLightGlobals, ResetWorldTransform, UpdateCombinedProjectionTransform | landed (inline PrepareForRender(0), x5) | 0x01436E14 |
| 0x00A0AF40 x1 | CRenderManagerCore::DrawAsIndexedTris(const void*, long, long, void*, long) | landed | GPRenderManager 0x01436E18 |
| 0x00A0AEA0 x4 | CRenderManagerCore::DrawAsIndexedTris(const CTexture**, unsigned long, const void*, long, long, void*, long) | landed | 0x01436E18 |
| 0x00B2FC50 | CEngineCamera::RefreshD3DTransforms() | 00/b2/CEngineCamera_RefreshD3DTransforms_00b2fc50.cpp (manifest `_global`) | camera 0x01436EA0 |
| 0x00A05840 | CRenderStateManager::RestoreCaptureBlock() | landed | 0x013CB508 (CRenderStateScopedCaptureBlock dtor) |

## 4. Members / types
`this` = engine/CEngineScreenEffectGlowRenderer.h with `#define FABLE_RETAIL_CENGINESCREENEFFECTGLOWRENDERER` +
`..._MEMBERS` (Ego_r names == retail offsets; constructor row 0x00B5F090 uses it). Members used:
+0x10 GlowTexture256, +0x14 GlowTexture64, +0x18 GlowTexture64Alt, +0x1C GlowTexture16 (CTexture*, written then read);
+0x20/24/28/2C GlowTexture*Handle (long); +0x34 VertexShaderCopy, +0x3C VertexShaderBlur (CVertexShader);
+0x4C PixelShaderGlow, +0x54 PixelShaderGlowNeg, +0x5C PixelShaderBlur (CPixelShader); +0x7C BackBufferToTextureVerts[4],
+0xDC Glow256ToGlow64Verts[4], +0x13C Glow64ToGlow16Verts[4] (CTVertexColTex1, U1 +0x10 / V1 +0x14 of each 0x18 vertex);
+0x1FC/200/204 ThresholdR/G/B, +0x208/20C/210 IntensityR/G/B, +0x214 MotionBlur (float).
The header's ..._MEMBERS block lacks CaptureBackBuffer's declaration (add it in the overlay).
Other types (shared headers; NONE needs a local model except CRenderTarget):
- CRenderStateManager (GPRenderManager 0x01436E18 / StateManager 0x013CB508): DeviceState +0x2818: ZBufferEnable +0x2868,
  PolyCullMode +0x2890, AlphaBlendEnable +0x28B8, AlphaTestEnable +0x28CC, SourceAlphaBlendMode +0x291C, DestAlphaBlendMode +0x2930,
  TextureUVModeU[0] +0x2EE4, TextureUVModeV[0] +0x2F88 (struct_layouts_egor CDeviceState); StateStack +8, StateStackSize +0x2808,
  StateUpdateListSize +0x280C, CurrentBookmarkMask +0x2814, BookmarkDummyState +0x3A24. Inline setters in engine/CRenderStateManager.h
  (no macro needed for SetZBufferEnable/SetPolyCullMode/SetAlphaBlendEnable/SetAlphaTestEnable/Set{Source,Dest}AlphaBlendMode/SetTextureUVModeU/V).
- Table values: 0x01396FB8 = D3DPolyCullModeTable[POLY_CULL_NONE]; 0x01396F78 / 0x01396F7C = D3DAlphaBlendModeTable[ALPHA_BLEND_SRC_ALPHA]
  / [ALPHA_BLEND_INV_SRC_ALPHA]; 0x01396DD0 = D3DTextureUVModeTable[TEXTURE_UV_CLAMP] (tables 0x01396FB0 / 0x01396F68 / 0x01396DC8).
- CRenderManagerCore (same object): CurrentlySelectedTextures[0] +0x3D00, PD3DDevice +0x3CF0, TextureStagesInUse +0x3CB8 ->
  inline AttachTextureToStage needs `FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS`; the two DrawAsIndexedTris need
  `FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASINDEXEDTRIS` (7-arg) and `..._DRAWASINDEXEDTRIS_USER` (5-arg), plus FABLE_RETAIL_CRENDERMANAGERCORE.
  CTexture::PD3DTexture is at +0 (`mov eax,[eax]`) -> FABLE_RETAIL_CTEXTURE.
- CShaderRenderManager (engine/CShaderRenderManager.h, no macro): PSConstTable +0x3CC (SetPixelShaderConstant: `*(begin+index)=v;
  UpdateFlags|=2`), UpdateFlags +0x3D8, CurrentVSConstantLayout +4, WorldMatrixIsIdentity +0x1E8; PrepareForRender(0) inline.
- CScopedShadersEnable (engine/CScopedShadersEnable.h): bools at frame 0x10 / 0x11; singleton 0x013BC470 (+0x390/+0x391).
- CRenderStateScopedCaptureBlock (engine/CRenderStateCaptureBlock.h): StartCaptureBlock inline at entry, RestoreCaptureBlock at exit.
- CPixelShader (engine/CPixelShader.h, vtable 0x0129A054, base CShaderBase 0x0129A030, Data +4, CPixelShaderData::ReferenceCount +8):
  the header has NO copy ctor; retail expands FableWin ??0CPixelShader@@QAE@ABV0@@Z inline:
  `Data(other.Data) { if (Data) Data->AddReference(); }` -- 5 landed rows model it locally (e.g. 00/bb/..Decal_00bba480.cpp:51).
  Add it to the overlay (new opt-in, e.g. FABLE_RETAIL_CPIXELSHADER_COPYCTOR). Its inline ~CPixelShader (vptr 0x129A054 store,
  `if (Data) Data->Release()` -> `--rc; if (rc==0) Data->vfunc0(1)`, then vptr 0x129A030) is the header's; FABLE_RETAIL_CPIXELSHADERDATA.
- CSurface (engine/CSurface.h, needs FABLE_RETAIL_CSURFACE + FABLE_RETAIL_CSURFACE_COPYFROMMEMORY for the inline ~CSurface
  {Uninitialise();}, and probably FABLE_RETAIL_CSURFACE_FORCEINLINE_DTOR -- this caller is 5 KB, past VC7.1's inline budget, chunk 886
  lesson): vtable 0x0122F84C, PD3DSurface +4, AllocationSource +8 (ctor zeroes these two only), PAllocatedMemory +0xC.
- CRenderTarget: NO shared header (all landed rows model it locally: 00/a1/CRenderTarget_AttachTarget_00a10e60.cpp). Ego_r:
  `CRenderTarget : CBaseClass` (RTTI vtable 0x0129C800), 0x24 bytes, RenderToSurface CSurface +4, ZBufferSurface CSurface +0x14.
  FableWin ??0CRenderTarget@@QAE@XZ (inline in retail: CBaseClass() call, vptr 0x129C800, two inline CSurface()),
  ??1CRenderTarget@@UAE@XZ (inline: ~ZBufferSurface, ~RenderToSurface, then out-of-line ~CBaseClass; NOTE retail stores no
  0x129C800 in the dtor). Model it in a new hdr overlay `engine/CRenderTarget.h` (author's choice) or locally (verifier warns only
  when a shared header exists -- none does).
- CBaseClass (engine/CBaseClass.h): out-of-line ctor 0x99A2D0 / dtor 0x99A2E0.
- CEngineCamera::RefreshD3DTransforms: FABLE_RETAIL_CENGINECAMERA + FABLE_RETAIL_CENGINECAMERA_REFRESHD3DTRANSFORMS.
- CDisplayManager: FABLE_RETAIL_CDISPLAYMANAGER + FABLE_RETAIL_CDISPLAYMANAGER_BACKSCREEN (CopyBackBufferToTexture returns CTexture&;
  SetRenderTargetAsBackScreen). CDisplayManager.h does NOT declare SetRenderTarget: the overlay must add
  `bool SetRenderTarget(CRenderTarget& target); // 0x009BF160` (landed row 0x00B7AD20 spelling).
- CEngineResourceManager (FABLE_RETAIL_CENGINERESOURCEMANAGER: GPUScratchBufferTextureManager at +0x3190; generic default has 0x33C0!)
  + GetGPUScratchBufferTextureManager() inline; CGPUScratchBufferTextureManager.h declares only BeginShutdown -> overlay must add
  `CTexture* StartTextureUse(long handle); // 0x00F36EE0`. Retail computes `edi = [0x1436E98]+0x3190` once (a reference local).
- Globals (externs; landed spellings): NEngineInternal::GPRenderManager 0x01436E18, NEngineInternal::GPShaderRenderManager 0x01436E14,
  0x01436E1C display manager (landed: NEngineInternal::GPDisplayManager x15 / g_pDisplayManager_01436e1c), 0x01436E98
  resource manager (NEngineInternal::GPResourceManager / g_pResourceManager), 0x01436EA0 camera (g_pEngineCamera).
- Function statics in .data (writable, read from memory each use => non-const statics), in definition order:
  0x0139A6B0 float 1.5f (blur offset growth per pass), 0x0139A6B4 long 2 (blur pass count), 0x0139A6B8 unsigned short[6] {0,1,2,0,2,3}
  (QuadIndices). ApplyGlow's own indices are 0x0139A6C4, ShowGlowTexture's 0x0139A6D0 -> all are function-local statics in
  function order; no PDB names (descriptive, mark in NOTES).
- Float constants (.rdata): 0x0122DED8 = 1.0f, 0x0122DEDC = 0.0f, 0x012A28E4 = -0.0078125f (-1/128), 0x012A1F34 = 0.0078125f (1/128);
  immediates 0x3E99999A = 0.3f, 0x3EA8F5C3 = 0.33f, 0x3C000000 = 1/128, 0xBC000000 = -1/128, 0x3D000000 = 1/32, 0x3F800000 = 1.0f.

## 5. Visibility
Every call target is a real out-of-line function (none is an unaligned COMDAT of an inline). All expanded helpers are header
inlines: StartCaptureBlock, Set*(render state), SetPixelShaderConstant, PrepareForRender, AttachTextureToStage, CScopedShadersEnable,
CPixelShader copy ctor/dtor, CSurface ctor/dtor, CRenderTarget ctor/dtor. Given the 5 KB caller, every one must be FABLE_FORCEINLINE /
__forceinline (VC7.1 caller-size budget: chunk 886 / CSurface FORCEINLINE_DTOR precedent). Run
`python3 /opt/fable_lane/tools/fwsize.py <src> <helper>` with FW_EXTRA=/EHsc on each new helper (CRenderTarget ctor/dtor,
CPixelShader copy ctor). Same TU (CEngineScreenEffectGlowRenderer.cpp): Render 0xB619B0 and ShowGlowTexture 0xB61690 come
later; nothing earlier in the TU is called. No STLport helper is called.

## 6. Siblings to copy idioms from
- 00/b5/CEngineScreenEffectColourFilter_RenderColorFilteredQuad_00b5af80.cpp (BEST template: CRenderStateScopedCaptureBlock,
  CScopedShadersEnable, SetPixelShaderConstant, SetTextureUVModeU/V(0, CLAMP), AttachTextureToStage(0,tex)/(0,0), PrepareForRender(0),
  5-arg DrawAsIndexedTris; macros FABLE_RETAIL_CDISPLAYMANAGER, _BACKSCREEN, CRENDERMANAGERCORE, _FORCEINLINE_HELPERS,
  _DRAWASINDEXEDTRIS_USER, CTEXTURE, CENGINECAMERA, _REFRESHD3DTRANSFORMS; no Flags line, no pragma).
- 00/b5/CEngineScreenEffectRadialBlurRenderer_ApplyRadialBlur_00b5e0f0.cpp (7-arg DrawAsIndexedTris with `const CTexture*` array,
  function-local `static unsigned short QuadIndices[6]`, ApplyVertexShader(member,false)/ApplyPixelShader(member)).
- 00/b5/CEngineScreenEffectGlowRenderer_CEngineScreenEffectGlowRenderer_00b5f090.cpp (the class macros _MEMBERS +
  FABLE_RETAIL_CENGINERESOURCEMANAGER).
- 00/b6/CEngineScreenEffectGlowRenderer_ShowGlowTexture_00b61690.cpp (same TU; old local-struct style, do NOT copy its models).
- 00/b7/CEngineWaterRenderer_SetRenderTargetToSurface_00b7ad20.cpp (CRenderTarget + SetRenderTarget(CRenderTarget&) local model).
No pragmas / no `// Flags:` in any of them (default flags, no /EHsc).

## 7. Open points for the author
- Exact form of the threshold vector (W=0.3f stored before the branch at 0x00B5FC1C; R/G/B either copied or `-1.0f`):
  try `C4DVector threshold; threshold.W = 0.3f; if (ThresholdR < 1.0f) {X=R..} else {X=R-1..}` vs ctor forms.
- The two loop-invariant 0.33f C4DVectors (frame 0x18 and 0x38) are stored once before the loop (0x00B6060F..0x00B60647):
  either declared before the loop or hoisted by the optimiser.
- Separate CRenderTarget slots (0x48 for passes 1, 2, loop-A; 0x7C for loop-B and final) and temp CSurface slots
  (0x6C, 0xC0, 0xA0) = separate block scopes; textures arrays (0x38, 0xB0, 0x6C).
