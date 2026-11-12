# DOSSIER 0x00B87870 CEngineScreenEffectDisplacementRenderer::ApplyDisplacement (4944 bytes = 0x1350, no int3 padding: ShowDisplacementTexture starts at 0x00B88BC0)

## 1. Retail shape (xdis / dis_all.txt)
- `__thiscall`, no params, plain `ret` (two copies: 0x00B88BAB and 0x00B88BBF, tail-duplicated epilogue).
- No frame pointer: `sub esp,24h` then `push ebx; push ebp; push esi; push edi` (save order ebx, ebp, esi, edi). No `push ebp; mov ebp,esp`.
- No SEH prologue, no /GS cookie, despite two RAII locals (capture block + shader enables) -- same as the landed twins
  ShowDisplacementTexture 0x00B88BC0 and ApplyRadialBlur 0x00B5E0F0 (their inline ctors/dtors cannot throw -> no EH state).
- Register constants for the whole body: `ebx = 1` (0x00B878A8, used for every `true`/1 value and the DirtyListFlag store),
  `ebp = 0` (0x00B878E0, ApplyVertexShader's `false`, every 0 state value, the 0.0f pushes). `this` spilled to [esp+10h] at 0x00B878AF
  (esi is destroyed by `add esi,13Ch` for ApplyPixelShader) and reloaded at 0x00B87FA4, 0x00B8816A, 0x00B88330, 0x00B884E2, 0x00B88B28.
- No size-optimisation idioms; plain /O2 like the twins. Neither twin has `// Flags:` or a pragma.

## 2. Name / signature (two sources)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv, Ego_r 0x00A2DEF0): `?ApplyDisplacement@CEngineScreenEffectDisplacementRenderer@@AAEXXZ`.
- FableWin.pdb (ghidra_out/_fablewin_api.tsv thunk 0x017C8332; naming_stragglers/FableWin.exe.signatures.tsv body 0x02DBA9B0, 888 b / 250 i):
  same mangled name. -> private (`AAE`), `void ApplyDisplacement()`, non-const, non-virtual. Manifest label is CORRECT.
- Neighbour order identical in Ego_r and retail: BeginDisplacementRendering (Ego_r 0x00A2DD00 / retail 0x00B87680) -> ApplyDisplacement
  (0x00A2DEF0 / 0x00B87870) -> ShowDisplacementTexture (0x00A2F190 / 0x00B88BC0). Ego_r body 0x12A0 v retail 0x1350.
- Caller: only CEngineScreenEffectDisplacementRenderer::Render 0x00B88ED0 (call at 0x00B88F06; landed
  rebuild/src/compiled/00/b8/CEngineScreenEffectDisplacementRenderer_Render_00b88ed0.cpp, declares it in a derived shell).
  Not virtual (no vtable slot).

## 3. Callees (all landed; PDB spellings agree with the landed names)
| retail | landed row | name / PDB leaf |
|---|---|---|
| 0x00987FE0 | 00/98/CShaderRenderManager_EnableVertexShaders_00987fe0.cpp | ?EnableVertexShaders@CShaderRenderManager@@QAEXXZ (via inline SetVertexShadersEnable) |
| 0x009880E0 | .._DisableVertexShaders_009880e0.cpp | DisableVertexShaders (dtor path only) |
| 0x00988110 | .._EnablePixelShaders_00988110.cpp | EnablePixelShaders |
| 0x00988190 | .._DisablePixelShaders_00988190.cpp | DisablePixelShaders (dtor path only) |
| 0x00988020 | .._ApplyVertexShader_00988020.cpp | ?ApplyVertexShader@CShaderRenderManager@@QAEXABVCVertexShader@@_N@Z |
| 0x00988140 | .._ApplyPixelShader_00988140.cpp | ?ApplyPixelShader@CShaderRenderManager@@QAEXABVCPixelShader@@@Z |
| 0x00989B00 | .._SetUserVertexShaderConstant_00989b00.cpp | ?SetUserVertexShaderConstant@CShaderRenderManager@@QAEXKMMMM@Z (index is `unsigned long`; was respelled from J on 2026-11-12) |
| 0x0098B930 | .._SetVSConstants_0098b930.cpp | ?SetVSConstants@CShaderRenderManager@@AAEXJPBXK@Z (inside inline PrepareForRender) |
| 0x00988A20 / 0x00989BF0 / 0x00989760 / 0x009897C0 / 0x0098A760 / 0x00988290 / 0x00988A50 | landed | UpdatePixelShader / UpdatePixelShaderConstants / UpdateAmbient / UpdateFogColour / UpdateLightGlobals / ResetWorldTransform / UpdateCombinedProjectionTransform (inline PrepareForRender(0)) |
| 0x00A0AEA0 | 00/a0/CRenderManagerCore_DrawAsIndexedTris_00a0aea0.cpp | ?DrawAsIndexedTris@CRenderManagerCore@@QAEXPAPBVCTexture@@KPBXJJPAXJ@Z (textured, 7 args) |
| 0x00A05840 | 00/a0/CRenderStateManager_RestoreCaptureBlock_00a05840.cpp | ?RestoreCaptureBlock@CRenderStateManager@@QAEXXZ |
No landed spelling disagrees with the PDB. Everything else is INLINE (no retail copy): StartCaptureBlock, SetRenderState/AddStateToUpdateList,
all Set* state setters below, SetTextureBumpEnvMatrix, PrepareForRender, CShaderRenderManager::Get/Set{Vertex,Pixel}ShadersEnable.

## 4. Members / globals
this (CEngineScreenEffectDisplacementRenderer, rebuild/include/engine/CEngineScreenEffectDisplacementRenderer.h; Ego_r names in
ghidra_out/struct_layouts_egor.tsv:152508..):
- +0x0C `CTexture* BackBufferCopyTexture` (textures[1], textures[2]); +0x10 `CTexture* DisplacementTexture` (textures[0]);
- +0x80 `CTVertexColTex1 TextureToBackBufferVerts[4]` (vertices, stride 0x18 = sizeof(CTVertexColTex1));
- +0xE8 `float DisplaceAmount` (read 5x: never CSE'd across the inline stores);
- +0x134 `CVertexShader DisplacementVertexShader`, +0x13C `CPixelShader DisplacementPixelShader`.
- Header state: FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER branch = CEngineComponent base + OPAQUE `_pad_0xc[0x1a0]` (no members);
  the default generated branch is flat with `unsigned char ScreenCopyVertexShader[8]` style members (ShowDisplacementTexture casts).
  NO `_MEMBERS` opt-in exists. Recommended overlay: add `FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_MEMBERS` copying the
  pattern of engine/CEngineScreenEffectRadialBlurRenderer.h (`..._MEMBERS`: includes CVertexShader.h / CPixelShader.h / CTVertexColTex1.h,
  typed members in Ego_r order, private method declarations). No CRenderTarget.h exists -> keep OldRenderTarget opaque [0x24]; C2DBoxI.h exists.
  Declare `void ApplyDisplacement();` (private, as PDB `AAE`) in it.
Render state core: NEngineInternal::GPRenderManager (0x01436E18, CRenderManager -> ... -> CRenderStateManager, engine/CRenderStateManager.h
  retail branch FABLE_RETAIL_CRENDERSTATEMANAGER via FABLE_RETAIL_CRENDERMANAGERCORE). Offsets = 0x2818 (DeviceState) + CDeviceState field:
  AlphaBlendEnable +0x28B8, AlphaTestEnable +0x28CC, AlphaTestFunc +0x28A4, AlphaRef +0x28E0, SourceAlphaBlendMode +0x291C,
  DestAlphaBlendMode +0x2930, StencilEnable +0x2B88, StencilFunc +0x2BD8, StencilOpFail +0x2B9C, StencilOpZFail +0x2BB0, StencilOpPass +0x2BC4,
  StencilRef +0x2BEC, StencilMask +0x2C00, ZBufferEnable +0x2868, PolyCullMode +0x2890, DitherEnable +0x2A0C,
  TextureBumpEnvMat00[s] +0x37A4+0x14s, Mat01[s] +0x3844+0x14s, Mat10[s] +0x38E4+0x14s, Mat11[s] +0x3984+0x14s,
  TextureFilterModeMin[s] +0x30C4+0x14s, TextureFilterModeMag[s] +0x3164+0x14s, TextureUVModeU[s] +0x2EE4+0x14s, TextureUVModeV[s] +0x2F84+0x14s.
  StateStack +0x08, StateUpdateList +0x2008, StateStackSize +0x2808, StateUpdateListSize +0x280C, CurrentBookmarkMask +0x2814, BookmarkDummyState +0x3A24.
- CRenderStateCaptureBlock::StateManager 0x013CB508 (StartCaptureBlock inline in prologue, RestoreCaptureBlock in epilogue).
- CShaderRenderManager::SingletonInstance 0x013BC470 (VertexShadersEnabled +0x390 = 0x013BC800, PixelShadersEnabled +0x391 = 0x013BC801).
- NEngineInternal::GPShaderRenderManager 0x01436E14 (UpdateFlags +0x3D8, CurrentVSConstantLayout +4, WorldMatrixIsIdentity +0x1E8).
- D3D tables (values read from Fable.exe): D3DAlphaFuncModeTable[6]=0x01396DF0 (7) -> ALPHA_TEST_GREATER_EQUAL; D3DAlphaBlendModeTable[4]
  0x01396F78 (5) SRC_ALPHA, [5] 0x01396F7C (6) INV_SRC_ALPHA; D3DStencilFuncModeTable[2] 0x01396E20 (3) STENCIL_FUNC_EQUAL;
  D3DStencilOperationModeTable[0] 0x01396F38 (1) STENCIL_OP_MODE_KEEP, [1] 0x01396F3C (2) STENCIL_OP_MODE_ZERO; D3DPolyCullModeTable[2]
  0x01396FB8 POLY_CULL_NONE; D3DTextureFilterModeTable[1] 0x01396E5C TEXTURE_FILTER_LINEAR; D3DTextureUVModeTable[0] 0x01396DC8 WRAP,
  [2] 0x01396DD0 CLAMP.
- Float constants: 0x01238798 = -0.5f, 0x012A3AF4 = 0.001953125f (1/512).
- 0x0139AAFC: unsigned short[6] {0,1,3,1,2,3} = this function's `static unsigned short QuadIndices[6]` (ShowDisplacementTexture's is the
  next one, 0x0139AB0C; RadialBlur/ColourFilter rows spell it as a function-local static, e.g. 0x0139A544 / 0x0139A4EC).
- 0x0139AB08: a 1-byte bool, initial value 1 (.data), between the two index arrays -> a static bool of this TU defined after
  ApplyDisplacement's QuadIndices (function-local `static bool` in ApplyDisplacement, or file static right after); no PDB/strings name found
  -> descriptive name needed (e.g. `UseStencilMask`), mark descriptive. It gates the whole stencil block.

## 4b. Opt-in macros
Exist: FABLE_RETAIL_CRENDERMANAGERCORE, FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASINDEXEDTRIS (textured overload, `const CTexture**`),
FABLE_RETAIL_CTEXTURE, FABLE_RETAIL_CRENDERSTATEMANAGER_STENCIL_INLINE (SetStencilEnable/Func/Ref/Mask inline),
FABLE_RETAIL_CRENDERSTATEMANAGER_OUTLINE_INLINE (SetStencilOpPass inline; also turns SetColourWriteEnable(bool)/StencilWriteMask inline - harmless),
FABLE_RETAIL_CRENDERSTATEMANAGER_EARLY_RETURN (alt. SetRenderState spelling). Always-inline already: SetAlphaBlendEnable, SetAlphaTestEnable,
SetAlphaTestFunc, SetAlphaRef, Set{Source,Dest}AlphaBlendMode, SetZBufferEnable, SetPolyCullMode, SetDitherEnable, SetTextureFilterModeMin/Mag,
SetTextureUVModeU/V; PrepareForRender (CShaderRenderManager.h:164), CScopedShadersEnable, CRenderStateScopedCaptureBlock.
MISSING (author must add in an hdr/ overlay, `#ifdef FABLE_RETAIL_*` additions only):
- inline `SetStencilOpFail(STENCIL_OP_MODE)` / `SetStencilOpZFail(STENCIL_OP_MODE)` (both declared out of line in both stencil branches;
  FableWin has debug copies 0x02DBB250 / 0x02DBB290 (41 b), no Ego_r/retail copy; retail expands them at 0x00B87C5C / 0x00B87CC8)
  -> `SetRenderState(D3DStencilOperationModeTable[mode], DeviceState.StencilOpFail / StencilOpZFail)`.
- inline `SetTextureBumpEnvMatrix(long stage, float m00, float m01, float m10, float m11)` (declared out of line; FableWin debug 0x02D54210
  1477 b; no Ego_r/retail copy). Retail write order per stage: Mat00, Mat10, Mat01, Mat11 (addresses below); each value is the float's
  bits: the params get stack homes and are reloaded as dwords (`*(unsigned long*)&m00`, same idiom as SetFogStart in the header).
  Since the two middle values are both 0.0f here, which param feeds Mat10 vs Mat01 is not observable; spell the body 00,10,01,11.

## 5. Visibility
All callees are out-of-line .cpp rows in other TUs (CShaderRenderManager 0x0098xxxx, CRenderManagerCore/StateManager 0x00A0xxxx) -> plain
declarations. No STLport use. The only inline helpers are the header FORCEINLINE setters above. Same-class neighbours (BeginDisplacementRendering
0x00B87680 above, ShowDisplacementTexture 0x00B88BC0 below) are not called.

## 6. Sibling rows to copy idioms from
- rebuild/src/compiled/00/b5/CEngineScreenEffectRadialBlurRenderer_ApplyRadialBlur_00b5e0f0.cpp -- BEST TEMPLATE: same shape
  (`static unsigned short QuadIndices[6]`, CRenderStateScopedCaptureBlock, CScopedShadersEnable(true,true), ApplyVertexShader(member,false),
  ApplyPixelShader, GPRenderManager->Set* inline setters incl. the same AlphaBlend/ZBuffer/PolyCull/Dither/Filter states,
  PrepareForRender(0), textured DrawAsIndexedTris). Defines: FABLE_RETAIL_CRENDERMANAGERCORE, _DRAWASINDEXEDTRIS, FABLE_RETAIL_CTEXTURE,
  FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER(_MEMBERS); includes CRenderManager.h, CShaderRenderManager.h,
  CRenderStateCaptureBlock.h, CScopedShadersEnable.h; `namespace NEngineInternal { extern CRenderManager* GPRenderManager; }` (0x01436E18),
  `extern CShaderRenderManager* GPShaderRenderManager;` (0x01436E14). No Flags, no pragma.
- rebuild/src/compiled/00/b5/CEngineScreenEffectColourFilter_RenderColorFilteredQuad_00b5af80.cpp -- stencil setters via
  FABLE_RETAIL_CRENDERSTATEMANAGER_STENCIL_INLINE, SetUserVertex/PixelShaderConstant calls, alpha-blend modes.
- rebuild/src/compiled/00/b8/CEngineScreenEffectDisplacementRenderer_ShowDisplacementTexture_00b88bc0.cpp -- same class, matched with a
  LOCAL render-manager model (pre-shared-header style; don't copy the local structs, the shared headers now cover them).
- rebuild/src/compiled/00/bf/CEnginePrimitiveRenderUtil_SetupOutlineSmoothAlphaStencilMaskStates_00bf01e0.cpp -- OUTLINE_INLINE stencil ops.
- Lane gap (CRenderStateManager.h header comment): the bookmark `test`/`or` operand order flips block by block in retail
  (`test esi,eax; or eax,esi` at 0x00B87921/0x00B87987 v `test eax,esi; or esi,eax` from 0x00B879F3 on) and no spelling moves it.
