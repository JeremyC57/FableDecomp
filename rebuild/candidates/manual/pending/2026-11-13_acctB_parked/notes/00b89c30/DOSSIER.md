# DOSSIER 0x00B89C30 CEngineScreenEffectOutlineGlow::ApplyOutline (6431 bytes = 0x191F, ends 0x00B8B54E `ret`; Render starts 0x00B8B550, no int3 pad)

No prior scouting existed for this row ($W/prior empty; agent2089 dirs empty). Closest previously-scouted twin:
/opt/fable_lane/w22/bwf1/00b87870/DOSSIER.md + OUTLINE.md (ApplyDisplacement: same TU family, same render-state/shader-enable idioms;
read its section 3/4/4b for the header macros, they apply here verbatim). Compact listing: $W/compact.txt (SetRenderState boiler-plate
filtered by $W/compact.py).

## 1. Retail shape
- `__thiscall`, no params, plain `ret` (two epilogue copies: 0x00B8B535 and 0x00B8B54E).
- No frame pointer, no SEH (no fs:0), no /GS cookie. Prologue `sub esp,94h ; push ebp ; push esi ; mov esi,ecx`, early-out test
  `cmp [esi+0Ch],ebp(=0) ; je 0x00B8B546` BEFORE `push ebx ; push edi` (0x00B89C56/57; shrink-wrapped). Save order ebp, esi, ebx, edi.
  Early-exit path 0x00B8B546 = `pop esi ; pop ebp ; add esp,94h ; ret` (no capture-block restore -> the return is before the RAII locals).
- `this` spilled to [esp+10h] (0x00B89C41) and reloaded at 0x00B8A68B, 0x00B8A81C, 0x00B8A94A, 0x00B8AA5C, 0x00B8AC51, 0x00B8ACDA, 0x00B8B43A.
- Register constants: ebp=0 (0x00B89C3D) for the first state block; ebx = 0x0122F84C (CSurface vtable, 0x00B89CDB) for every inline
  ~CSurface vptr store; ebp = 4 (0x00B8A77D / 0x00B8A7B9 / 0x00B8ACCF) used as the texture count / vertex count 4 after the first pass;
  edi = 0 (0x00B8A68F, 0x00B8AAC6) as the 0.0f pushes / ApplyVertexShader `false` / GetSurfaceLevel(0).
- Plain /O2 (16-aligned, no size idioms). No `// Flags:` needed beyond the lane default (no /EHsc: retail has destructible locals
  (CRenderTarget, CSurface temps, CScopedShadersEnable, capture block) but no EH frame -- same as the landed twins ApplyRadialBlur
  0x00B5E0F0 / ShowDisplacementTexture 0x00B88BC0, which have no Flags line and no pragma).
- Ego_r.pdb body 0x00A302E0..0x00A31C00 = 0x1920 v retail 0x191F (near-identical size: same source, same inlining).

## 2. Name / signature (two sources)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv 00a302e0): `?ApplyOutline@CEngineScreenEffectOutlineGlow@@AAEXXZ`.
- FableWin.pdb (ghidra_out/_fablewin_api.tsv thunk 0x0180D464; naming_stragglers/FableWin.exe.signatures.tsv body 0x02DBD200, 2312 b / 619 i):
  same mangled name. => `private: void CEngineScreenEffectOutlineGlow::ApplyOutline()`, non-const, non-virtual. Manifest label CORRECT.
- Ego_r neighbour order: ... BeginOutlineRendering 0x00A30000, DrawDebugTextures 0x00A301D0, ApplyOutline 0x00A302E0, Render 0x00A31C00, ctor
  0x00A31C30. Retail: BeginOutlineRendering 0x00B89A20, ApplyOutline 0x00B89C30, Render 0x00B8B550 (DrawDebugTextures has no retail copy here).
- Only caller: CEngineScreenEffectOutlineGlow::Render 0x00B8B550 (call at 0x00B8B569; landed
  rebuild/src/compiled/00/b8/CEngineScreenEffectOutlineGlow_Render_00b8b550.cpp, local model declaring `void ApplyOutline();`). Not virtual.

## 3. Callees (all landed; retail address -> landed file -> PDB)
| retail | landed row (rebuild/src/compiled/00/..) | PDB / note |
|---|---|---|
| 0x00987FE0 / 0x009880E0 | 98/CShaderRenderManager_EnableVertexShaders / _DisableVertexShaders | inline CScopedShadersEnable (engine/CScopedShadersEnable.h) |
| 0x00988110 / 0x00988190 | 98/.._EnablePixelShaders / _DisablePixelShaders | same |
| 0x00988020 | 98/CShaderRenderManager_ApplyVertexShader_00988020.cpp | ?ApplyVertexShader@CShaderRenderManager@@QAEXABVCVertexShader@@_N@Z (x3, bool false) |
| 0x00988140 | 98/CShaderRenderManager_ApplyPixelShader_00988140.cpp | ?ApplyPixelShader@CShaderRenderManager@@QAEXABVCPixelShader@@@Z (x3) |
| 0x00989B00 | 98/CShaderRenderManager_SetUserVertexShaderConstant_00989b00.cpp | ?SetUserVertexShaderConstant@CShaderRenderManager@@QAEXKMMMM@Z (x12) |
| 0x0098B930 0x00988A20 0x00989BF0 0x00989760 0x009897C0 0x0098A760 0x00988290 0x00988A50 | 98/CShaderRenderManager_SetVSConstants / UpdatePixelShader / UpdatePixelShaderConstants / UpdateAmbient / UpdateFogColour / UpdateLightGlobals / ResetWorldTransform / UpdateCombinedProjectionTransform | only via inline PrepareForRender(0) (x4) |
| 0x0099A2D0 / 0x0099A2E0 | 99/CBaseClass_CBaseClass_0099a2d0 / _0099a2e0 | ??0CBaseClass / ??1CBaseClass@@UAE@XZ (inline CRenderTarget ctor / dtor; engine/CBaseClass.h) |
| 0x00A10DC0 | a1/CRenderTarget_Constructor_00a10dc0.cpp | Ego_r ??0CRenderTarget@@QAE@AAVCSurface@@0@Z = CRenderTarget(CSurface&, CSurface&) -- **landed row declares `const CSurface&` (ABV) -- PDB is NON-const (AAV)**; called by the inline CDisplayManager::GetRenderTarget |
| 0x009F2E70 | 9f/CSurface_operator_009f2e70.cpp | CSurface::operator=(const CSurface&) (x2, inside the inline CRenderTarget::operator=; FableWin ??4CRenderTarget@@QAEAAV0@ABV0@@Z 64 b is debug-only) |
| 0x009F2E20 | 9f/CSurface_Uninitialise_009f2e20.cpp | ?Uninitialise@CSurface@@QAEXXZ (inline ~CSurface x11) |
| 0x009F9E00 | 9f/CTexture_GetSurfaceLevel_009f9e00.cpp | ?GetSurfaceLevel@CTexture@@QBE?AVCSurface@@J@Z (x3, hidden CSurface result) |
| 0x00A10E60 | a1/CRenderTarget_AttachTarget_00a10e60.cpp | ?AttachTarget@CRenderTarget@@QAEXABVCSurface@@@Z (x3) |
| 0x009BF160 | 9b/CDisplayManager_SetRenderTarget_009bf160.cpp | ?SetRenderTarget@CDisplayManager@@QAE_NAAVCRenderTarget@@@Z = `bool SetRenderTarget(CRenderTarget&)` (x3; result dropped). The landed 0x00B89A20 declares `void SetRenderTarget(CRenderTarget*)` -- wrong spelling, do not copy |
| 0x00A0AEA0 | a0/CRenderManagerCore_DrawAsIndexedTris_00a0aea0.cpp | ?DrawAsIndexedTris@CRenderManagerCore@@QAEXPAPBVCTexture@@KPBXJJPAXJ@Z (x4) |
| 0x009BF220 | 9b/CDisplayManager_SetRenderTargetAsBackScreen_009bf220.cpp | ?SetRenderTargetAsBackScreen@CDisplayManager@@QAE_NXZ (result dropped) |
| 0x00B2FC50 | b2/CEngineCamera_RefreshD3DTransforms_00b2fc50.cpp | ?RefreshD3DTransforms@CEngineCamera@@QAEXXZ on g_pEngineCamera 0x01436EA0 |
| 0x00A10E30 | a1/CRenderTarget_Uninitialise_00a10e30.cpp | ?Uninitialise@CRenderTarget@@QAEXXZ (Ego_r 0x00B7EE40), on OldRenderTarget (+0x14C) |
| 0x00A05840 | a0/CRenderStateManager_RestoreCaptureBlock_00a05840.cpp | inline ~CRenderStateScopedCaptureBlock (x2, both epilogues) |
INLINE (no call): StartCaptureBlock (0x00B89C4B..0x00B89C71), CScopedShadersEnable ctor/dtor, CDisplayManager::GetRenderTarget (FableWin
?GetRenderTarget@CDisplayManager@@QAE?AVCRenderTarget@@XZ, by value) and GetViewport (FableWin ?GetViewport@CDisplayManager@@QBEABVC2DBoxI@@XZ),
CRenderTarget default ctor / operator= / dtor, CSurface ctor/dtor, every CRenderStateManager Set*, PrepareForRender(0).
Side note (not this row): landed 0x00B89080 "ReleaseTextures_00b89080" is probably Ego_r ?FinishWithDynamicTextures@CEngineScreenEffectOutlineGlow@@AAEXXZ
(0x00A2FD40, the slot before FinishOutlineRendering in both orders; called from Render after ApplyOutline) -- NAME_FIXES candidate, one source only.

## 4. Members / globals
this = CEngineScreenEffectOutlineGlow (engine/CEngineScreenEffectOutlineGlow.h; Ego_r struct_layouts_egor.tsv, sizeof 0x1E4 = retail):
| off | member (Ego_r) | type | use |
|---|---|---|---|
| +0x0C | OutlineTexture | CTexture* | early-out `if (!OutlineTexture) return` (0x00B89C3A); pass-0 source texture x4 |
| +0x10 | Blur1Texture | CTexture* | pass-0 target surface; pass-A source x4; pass-B target; final source |
| +0x14 | Blur2Texture | CTexture* | pass-A target surface; pass-B source x4 |
| +0x2C | OutlineToBlurVerts | CTVertexColTex1[4] | pass-0 verts |
| +0x8C | BlurToBlurVerts | CTVertexColTex1[4] | loop verts (ebp = this+0x8C, 0x00B8A8BD) |
| +0xEC | BlurToBackBufferVerts | CTVertexColTex1[4] | final verts |
| +0x14C | OldRenderTarget | CRenderTarget (0x24: vptr, RenderToSurface +4 = +0x150, ZBufferSurface +0x14 = +0x160) | assigned at start; Uninitialise at end |
| +0x170 | OldViewport | C2DBoxI | 16-byte copy of GPDisplayManager->ViewportBox (+0x1F4) |
| +0x180/+0x188 | OutlineToBlurVertexShader / PixelShader | CVertexShader / CPixelShader | pass 0 |
| +0x190/+0x198 | BlurToBlurVertexShader / PixelShader | | loop |
| +0x1A0/+0x1A8 | BlurToBackBufferVertexShader / PixelShader | | final |
| +0x1D8 | BlurStepCount | long | `for (long i = 0; i < BlurStepCount; ++i)` (signed jle/jl) |
Header state: the FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW branch is CEngineComponent + OPAQUE `_pad_0xc[0x1d8]`; only opt-in is
FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW_OUTLINERENDERING (Begin/FinishOutlineRendering decls). NO _MEMBERS opt-in, no ApplyOutline decl.
=> overlay $W/hdr/engine/CEngineScreenEffectOutlineGlow.h: add `FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW_MEMBERS` modelled on
engine/CEngineScreenEffectRadialBlurRenderer.h `_MEMBERS` (includes CVertexShader.h with FABLE_RETAIL_CVERTEXSHADER, CPixelShader.h,
CTVertexColTex1.h, C2DBoxI.h; typed members in Ego_r order; `private: void ApplyOutline();`). CRenderTarget has NO shared header
(engine/CRenderTarget.h missing) -> either add one in the overlay (CBaseClass base, vtable 0x0129C800, CSurface RenderToSurface +4 /
ZBufferSurface +0x14, ctor(CSurface&,CSurface&) 0x00A10DC0, inline default ctor, inline operator= of the two surfaces, AttachTarget
0x00A10E60, Uninitialise 0x00A10E30) or model it locally like the landed 0x00B89A20 (copy that model, fix SetRenderTarget).
Classes the landed sibling 0x00B89A20 models LOCALLY although a shared header exists: CSurface (engine/CSurface.h), CDisplayManager
(engine/CDisplayManager.h), C2DBoxI (engine/C2DBoxI.h), CTexture (engine/CTexture.h), CRGBColour, CRenderStateManager
(engine/CRenderStateManager.h); use the shared headers.
- CSurface (engine/CSurface.h, FABLE_RETAIL_CSURFACE): vtable 0x0122F84C, PD3DSurface +4, AllocationSource +8, PAllocatedMemory +0xC; inline ctor
  zeroes +4/+8 only (retail stores exactly those); the inline virtual dtor needs FABLE_RETAIL_CSURFACE_COPYFROMMEMORY (defines
  `virtual ~CSurface(){Uninitialise();}`; add FABLE_RETAIL_CSURFACE_FORCEINLINE_DTOR if VC7.1 calls ??1CSurface instead). operator= (0x009F2E70)
  is NOT declared in CSurface.h -> overlay add `CSurface& operator=(const CSurface&);`.
- CDisplayManager (NEngineInternal::GPDisplayManager 0x01436E1C; engine/CDisplayManager.h FABLE_RETAIL_CDISPLAYMANAGER): RenderTargetSurface +0xC,
  RenderTargetDepthBuffer +0x1C, ViewportBox +0x1F4. Opt-ins: FABLE_RETAIL_CDISPLAYMANAGER_GETVIEWPORT (inline GetViewport),
  FABLE_RETAIL_CDISPLAYMANAGER_BACKSCREEN (SetRenderTargetAsBackScreen). MISSING (overlay): `bool SetRenderTarget(CRenderTarget&);` 0x009BF160 and
  inline `CRenderTarget GetRenderTarget(){ return CRenderTarget(RenderTargetSurface, RenderTargetDepthBuffer); }` (members are private).
  FABLE_RETAIL_CDISPLAYMANAGER needs FABLE_RETAIL_SCOPED_PTR + FABLE_RETAIL_CSYSTEMMANAGER (see ColourFilter row defines).
- CTexture (engine/CTexture.h FABLE_RETAIL_CTEXTURE): GetSurfaceLevel(long) const is declared only under FABLE_RETAIL_CTEXTURE_SIZED (line 49) -> define it too.
- CRenderStateManager on NEngineInternal::GPRenderManager 0x01436E18 (DeviceState at +0x2818). Offsets used: AlphaBlendEnable +0x28B8,
  AlphaTestEnable +0x28CC, AlphaTestFunc +0x28A4, AlphaRef +0x28E0, SourceAlphaBlendMode +0x291C, DestAlphaBlendMode +0x2930, ZBufferEnable +0x2868,
  PolyCullMode +0x2890, DitherEnable +0x2A0C, StencilEnable +0x2B88, StencilFunc +0x2BD8, StencilOpPass +0x2BC4, StencilRef +0x2BEC, StencilMask
  +0x2C00, TextureFilterModeMag[s] +0x3164+0x14s, TextureFilterModeMin[s] +0x30C4+0x14s, TextureUVModeU[s] +0x2EE4+0x14s, TextureUVModeV[s]
  +0x2F84+0x14s. Capture/update: StateStack +8, StateUpdateList +0x2008, StateStackSize +0x2808, StateUpdateListSize +0x280C,
  CurrentBookmarkMask +0x2814, BookmarkDummyState +0x3A24.
- Tables (values read from Fable.exe): D3DPolyCullModeTable[POLY_CULL_NONE] 0x01396FB8 (1); D3DTextureFilterModeTable[TEXTURE_FILTER_POINT=0]
  0x01396E58 (1), [TEXTURE_FILTER_LINEAR=1] 0x01396E5C (2); D3DTextureUVModeTable[TEXTURE_UV_CLAMP=2] 0x01396DD0 (3);
  D3DAlphaFuncModeTable[ALPHA_TEST_GREATER_EQUAL=6] 0x01396DF0 (7); D3DAlphaBlendModeTable[ALPHA_BLEND_SRC_ALPHA=4] 0x01396F78 (5),
  [ALPHA_BLEND_INV_SRC_ALPHA=5] 0x01396F7C (6); D3DStencilFuncModeTable[STENCIL_FUNC_EQUAL=2] 0x01396E20 (3);
  D3DStencilOperationModeTable[STENCIL_OP_MODE_ZERO=1] 0x01396F3C (2). Enum names: engine/RenderStateEnums.h.
- CRenderStateCaptureBlock::StateManager 0x013CB508; CShaderRenderManager::SingletonInstance 0x013BC470 (VertexShadersEnabled 0x013BC800,
  PixelShadersEnabled 0x013BC801); NEngineInternal::GPShaderRenderManager 0x01436E14 (UpdateFlags +0x3D8, CurrentVSConstantLayout +4,
  WorldMatrixIsIdentity +0x1E8); g_pEngineCamera 0x01436EA0 (landed spelling, 20+ rows).
- Function-local statics (.data, referenced ONLY by this function -- byte scan of .text):
  0x0139AB54 float = 0.0078125f (1/128) (refs 0x00B8A8F0, 0x00B8A919, 0x00B8AAE9, 0x00B8AB10)
  0x0139AB58 float = 0.0078125f (refs 0x00B8A8D9, 0x00B8A902, 0x00B8A929, 0x00B8AAD4, 0x00B8AAFB, 0x00B8AB20)
  0x0139AB5C unsigned short[6] = {0,1,3,1,2,3} (QuadIndices; refs at all 4 draws).
  Both floats are loaded from memory (fld), so they are NON-const statics (a const would be folded to `push 3C000000h`). Declaration order
  in source = address order: floatA (0x0139AB54), floatB (0x0139AB58), QuadIndices. No name in PDBs/strings -> descriptive names (e.g.
  BlurOffsetU / BlurOffsetV), mark descriptive. 0x0139AB40.. just before is the RTTI type name tail "..ectOutlineGlow@@" (unrelated).
- Literal float pushes: 0x3B800000 = 0.00390625f (1/256) in the pass-0 constants.

## 4b. Opt-in macros to define (union of ApplyRadialBlur 0x00B5E0F0 + ColourFilter 0x00B5AF80 + Displacement dossier)
FABLE_RETAIL_SCOPED_PTR, FABLE_RETAIL_CSYSTEMMANAGER, FABLE_RETAIL_CDISPLAYMANAGER, FABLE_RETAIL_CDISPLAYMANAGER_BACKSCREEN,
FABLE_RETAIL_CDISPLAYMANAGER_GETVIEWPORT, FABLE_RETAIL_CRENDERMANAGERCORE, FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASINDEXEDTRIS (textured 7-arg
overload `const CTexture**`), FABLE_RETAIL_CRENDERSTATEMANAGER_STENCIL_INLINE (SetStencilEnable/Func/Ref/Mask),
FABLE_RETAIL_CRENDERSTATEMANAGER_OUTLINE_INLINE (SetStencilOpPass inline), FABLE_RETAIL_CTEXTURE + _SIZED (GetSurfaceLevel), FABLE_RETAIL_CSURFACE (+_COPYFROMMEMORY for the
inline dtor), FABLE_RETAIL_CENGINECAMERA + _REFRESHD3DTRANSFORMS, FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW (+ new _MEMBERS overlay).
Always-inline already: SetAlphaBlendEnable/AlphaTestEnable/AlphaTestFunc/AlphaRef/Source+DestAlphaBlendMode/ZBufferEnable/PolyCullMode/
DitherEnable/TextureFilterModeMin/Mag/TextureUVModeU/V; PrepareForRender (CShaderRenderManager.h:164); CScopedShadersEnable;
CRenderStateScopedCaptureBlock (engine/CRenderStateCaptureBlock.h).

## 5. Visibility
All out-of-line callees live in other TUs (CShaderRenderManager 0x0098xxxx, CBaseClass 0x0099A2xx, CDisplayManager 0x009BFxxx, CSurface/CTexture
0x009F2/0x009F9, CRenderTarget 0x00A10Dxx, RenderStateManager/Core 0x00A0xxxx, camera 0x00B2FC50) -> plain declarations; do NOT give
CRenderTarget(CSurface&,CSurface&), AttachTarget, Uninitialise, CSurface::operator= or Uninitialise bodies (they are calls in retail).
No STLport. Same-TU neighbours (FinishOutlineRendering 0x00B89140, ctor 0x00B891B0/0x00B898D0, BeginOutlineRendering 0x00B89A20 above; Render
0x00B8B550 below) are not called. The inline helpers are header FORCEINLINE / implicit inlines listed above.

## 6. Sibling rows (idioms)
- rebuild/src/compiled/00/b5/CEngineScreenEffectRadialBlurRenderer_ApplyRadialBlur_00b5e0f0.cpp -- BEST TEMPLATE (capture block + CScopedShadersEnable
  (true,true) + ApplyVertexShader(member,false) + ApplyPixelShader + GPRenderManager->Set* + PrepareForRender(0) + `static unsigned short
  QuadIndices[6]` + textured DrawAsIndexedTris). No Flags, no pragma.
- rebuild/src/compiled/00/b5/CEngineScreenEffectColourFilter_RenderColorFilteredQuad_00b5af80.cpp -- shared-header CDisplayManager
  (SetRenderTargetAsBackScreen), g_pEngineCamera->RefreshD3DTransforms(), stencil setters; its #define list (lines 6-21) is the one to copy.
- rebuild/src/compiled/00/b8/CEngineScreenEffectOutlineGlow_BeginOutlineRendering_00b89a20.cpp -- same class, same
  `{ CRenderTarget current(DM->RenderTargetSurface, DM->RenderTargetDepthBuffer); OldRenderTarget = current; } OldViewport = DM->Viewport;
  CRenderTarget target; target.AttachTarget(tex->GetSurfaceLevel(0)); DM->SetRenderTarget(target);` sequence (local models; fix names).
- rebuild/src/compiled/00/b8/CEngineScreenEffectDisplacementRenderer_ShowDisplacementTexture_00b88bc0.cpp and the 0x00B87870 scout
  (ApplyDisplacement, same shape, notes on SetRenderState operand order).
- rebuild/src/compiled/00/bf/CEnginePrimitiveRenderUtil_SetupOutlineSmoothAlphaStencilMaskStates_00bf01e0.cpp -- OUTLINE_INLINE stencil ops.

## 7. Known residue risks
- SetRenderState bookmark operand order flips block by block (lane gap, CRenderStateManager.h comment): first section `test eax,esi ; or esi,eax`
  for AlphaBlend/AlphaTest/ZBuffer/PolyCull/Dither, `test esi,eax`/`test ecx,eax` from StencilEnable on (all filter/UV blocks); second section
  `test esi,eax` Mag0/Min0/AlphaBlend/AlphaTest, `test eax,esi` AlphaFunc/AlphaRef/SrcBlend, `test esi,eax` DestBlend/ZBuffer,
  `test eax,esi` PolyCull..StencilRef.
- Final PrepareForRender re-materialises `mov eax,[01436E18h]` after every Update* call (0x00B8B435..0x00B8B4BA) and the last draw uses
  `mov ecx,eax` (0x00B8B4DE): GPRenderManager is CSE'd from the preceding state setters across the inline PrepareForRender -- spell the last
  draw as `NEngineInternal::GPRenderManager->DrawAsIndexedTris(...)` directly after PrepareForRender(0) like ApplyRadialBlur.
- Stack-slot sharing (OUTLINE frame map): pass 0 shares slots with pass A; pass A and pass B do NOT share slots although targetA is
  destroyed (0x00B8AA9D) before pass B begins -> per-pass blocks; slot allocation is a residue to probe.
