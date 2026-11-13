# DOSSIER 0x00B783F0 -- CEngineWaterRenderer::Render(const CEngineComponentRenderParams&)

No prior scout or draft exists ($W/prior/ empty; /opt/fable_lane/w22/agent4025 src/hdr/probe empty). Everything below is new.
Condensed listing (SetRenderState bodies collapsed, DeviceState fields named): $W/tmp/collapsed.txt (script $W/tmp/collapse.py).

## 1. Retail shape (xdis 00b783f0 0x247f; $W/dis_all.txt)
- Extent right: 0x00B783F0..0x00B7A86E (`ret 4` at 0x00B7A86C), int3 at 0x00B7A86F, next function 0x00B7A870 (`jmp 0x0099A300`). 9343 bytes.
- __thiscall, `ret 4` (one ref argument). NO frame pointer (esp-relative; `sub esp,28h; push ebx; push ebp; push esi; push edi`).
  NO SEH prologue / no fs:[0] although three scoped objects with destructors live across calls -> TU built WITHOUT /EHsc (the lane
  base flags; same as landed 0x00B5AF80 / 0x00B5E0F0 which also hold CRenderStateScopedCaptureBlock + CScopedShadersEnable). No /GS cookie.
- No size-optimised idioms seen; plain /O2 code. No jump tables. Only 23 direct calls: almost the whole body is inline expansions of
  CRenderStateManager::SetRenderState (59 reloads of 0x01436E18).
- Epilogue `pop edi; pop esi; pop ebp; pop ebx; add esp,28h; ret 4` at 0x00B7A865 is also the early-return target (0x00B7851D, 0x00B7858B).

## 2. Name / class / signature (two+ sources)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv): 0x00A26380 `?Render@CEngineWaterRenderer@@UAEXABVCEngineComponentRenderParams@@@Z`,
  between RenderSeaNonReflectiveForeground (0x00A25DB0) and GetConsoleEnableFunctionName (0x00A28A70) -- same order as retail
  (0x00B77E20 RenderSeaNonReflectiveForeground lands just before this row).
- FableWin.pdb (ghidra_out/naming_stragglers/FableWin.exe.signatures.tsv): 0x02D4C3C0 same mangled name, debug body 1706 bytes / 504 instr.
- Vtable: rebuild/manifest/rtti-vtables.tsv `CEngineWaterRenderer 012a3364 slot 4 = 00b783f0`; xref.py: only reference is the vtable
  slot (abs .rdata 0x012A3374). Header rebuild/include/engine/CEngineWaterRenderer.h (FABLE_RETAIL_CENGINEWATERRENDERER) already declares
  `virtual void Render(const CEngineComponentRenderParams& params); // slot 4 0x00B783F0`.
- Manifest label is RIGHT (it is the full mangled name; module CEngineWaterRenderer). Leaf for q.py: the mangled name.
- Signature: `void CEngineWaterRenderer::Render(const CEngineComponentRenderParams& params)` (virtual, public, non-const).

## 3. Callees (retail order of first appearance)
| addr | PDB name | landed row | notes |
|---|---|---|---|
| 0x00BEA110 | ?UpdateConstants@CEngineWaterSettings@@QAEXABVCEngineComponentRenderParams@@@Z | not landed | declared in engine/CEngineWaterSettings.h:29. call on `this+0x280` (Settings), arg params |
| 0x00B72180 | ?CreateDynamicTextures@CEngineWaterRenderer@@AAE_N_N00@Z (Ego_r) | not landed | private bool(bool,bool,bool); NOT declared in shared header -> overlay |
| 0x00987FE0 / 0x009880E0 | CShaderRenderManager::EnableVertexShaders / DisableVertexShaders | 00/98/CShaderRenderManager_EnableVertexShaders_00987fe0.cpp, _DisableVertexShaders_009880e0.cpp | via inline CScopedShadersEnable (engine/CScopedShadersEnable.h) |
| 0x00988110 / 0x00988190 | EnablePixelShaders / DisablePixelShaders | 00/98/..._00988110.cpp, ..._00988190.cpp | same |
| 0x00988290 | ?ResetWorldTransform@CShaderRenderManager@@QAEXXZ | 00/98/CShaderRenderManager_ResetWorldTransform_00988290.cpp (MATCH) | called on the POINTER global 0x01436E14 (`mov ecx,[0x1436e14]`), not SingletonInstance |
| 0x00B761D0 | ?RenderForeground@CEngineWaterRenderer@@AAEXM@Z | not landed | declared in shared header |
| 0x00B76BC0 | ?RenderBackground@...@@AAEXM@Z | 00/b7/CEngineWaterRenderer_RenderBackground_00b76bc0.cpp | NOT in shared header -> overlay decl |
| 0x00B751F0 | ?RenderIceForeground@...@@AAEXM@Z | not landed | declared |
| 0x00B75CA0 | ?RenderIceBackground@...@@AAEXM@Z | 00/b7/CEngineWaterRenderer_RenderIceBackground_00b75ca0.cpp | NOT in header -> overlay |
| 0x00B77370 | ?RenderSeaReflectiveForeground@...@@AAEXM@Z | not landed | declared |
| 0x00A05840 | ?RestoreCaptureBlock@CRenderStateManager@@QAEXXZ | 00/a0/CRenderStateManager_RestoreCaptureBlock_00a05840.cpp (MATCH) | 3 calls: mid-function Restore(), inner-block dtor, outer scoped-block dtor |
| 0x00B665E0 | ?GetEnvironmentMap@CEngineSkyRenderer@@QAEXPAVCTexture@@ABVCEngineComponentRenderParams@@@Z | not landed | NOT in engine/CEngineSkyRenderer.h -> overlay. ecx = [0x01436E50] |
| 0x009FAF70 | ?GenerateMipmapsWithGPU@CTexture@@QAEXXZ | not landed | NOT in engine/CTexture.h -> overlay (under FABLE_RETAIL_CTEXTURE) |
| 0x00B77E20 | ?RenderSeaNonReflectiveForeground@...@@AAEXM@Z | 00/b7/..._RenderSeaNonReflectiveForeground_00b77e20.cpp | NOT in header -> overlay |
| 0x00B6FA40 | ?RenderSeaMidground@...@@AAEXM@Z | not landed | declared only under FABLE_RETAIL_CENGINEWATERRENDERER_SEAMIDGROUND |
| 0x00B70230 | ?RenderSeaBackground@...@@AAEXM@Z | not landed | declared |
| 0x00B6D630 | ?FinishUsingDynamicTextures@...@@AAEXXZ | 00/b6/..._FinishUsingDynamicTextures_00b6d630.cpp | declared |
| IDirect3DDevice9 slot 65 (+0x104) | SetTexture | -- | via inline CRenderManagerCore::AttachTextureToStage (FORCEINLINE_HELPERS) |
| slot 100 (+0x190) | SetStreamSource | -- | via inline SelectVertexBuffer(0, NULL) |
| slot 104 (+0x1A0) | SetIndices | -- | via inline SelectIndexBuffer(NULL, 0) |
Spelling flags: manifest calls 0x00B6D4B0 "DeleteConstantBuffers" with a junk signature (not our callee). No landed spelling of a callee
disagrees with the PDB. Globals: landed code spells 0x01436E18 `g_pRenderManagerCore`, 0x01436E14 `g_pShaderRenderManager`,
0x01436E50 `g_pEngineSkyRenderer` (CEngine_Initialise_00b26360.cpp); the PDB symbols are NEngineInternal::GPRenderManager /
GPShaderRenderManager (per comments in engine/CRenderManager2D.h:103 and engine/CShaderRenderManager.h:10); 0x01436E50's PDB name is
unproven (probably NEngineInternal::GPSkyRenderer -- NOT confirmed, keep the landed name). 0x013CB508 =
CRenderStateCaptureBlock::StateManager (engine/CRenderStateCaptureBlock.h), 0x013BC470 = CShaderRenderManager::SingletonInstance
(Get()), VertexShadersEnabled 0x013BC800 / PixelShadersEnabled 0x013BC801.

## 4. Members read (this = CEngineWaterRenderer, shared header engine/CEngineWaterRenderer.h with FABLE_RETAIL_CENGINEWATERRENDERER)
- +0x1fc/+0x200 ForegroundRenderList (CArray begin/end, `!empty()` = cmp+setne); +0x208/0x20c BackgroundRenderList; +0x214/0x218
  AlphaedBackgroundRenderList; +0x220/0x224 IceForegroundRenderList; +0x22c/0x230 IceBackgroundRenderList; +0x238/0x23c
  IceAlphaedBackgroundRenderList; +0x244/0x248 SeaReflectiveForegroundRenderList; +0x250/0x254 SeaNonReflectiveForegroundRenderList;
  +0x25c/0x260 SeaMidgroundRenderList; +0x268/0x26c SeaAlphaedMidgroundRenderList.
- +0x1e0 EnvironmentMap (CTexture*); +0x274 HighendWaterEnabled; +0x276 HaveBackgroundSea.
- Settings (+0x280, engine/CEngineWaterSettings.h, Ego_r names): +0x285 EnableSea, +0x286 EnableSeaForeground, +0x287 EnableSeaMidground,
  +0x288 EnableSeaBackground, +0x289 EnableForeground, +0x28a EnableBackground, +0x28b EnableIceForeground, +0x28c EnableIceBackground,
  +0x28d EnableBumpMap, +0x2dc BumpMatFactor (float).
- params: `params.Params` (+0) -> CEngineComponentParams +0x3c = InterpolationInfo(+4).BulletTimeCurrent(+0x30).WFInterpolate(+8)
  (struct_layouts_egor.tsv CEngineComponentParams / CEngineInterpolationInfo / CEngineInterpolationInfoSet). No shared header for
  CEngineComponentParams / CEngineComponentRenderParams: landed sibling 0x00B662F0 CEngineSkyRenderer::Render models them locally and reads
  `params.Params->BulletTimeCurrent.WFInterpolate` (same field). Keep that or write hdr/engine/CEngineComponentRenderParams.h.
- g_pRenderManagerCore (CRenderManagerCore, engine/CRenderManagerCore.h, FABLE_RETAIL_CRENDERMANAGERCORE): DeviceState CStateInfos at
  0x2818+: FogEnable 0x2944, SpecularEnable 0x29e4, ZBufferEnable 0x2868, ZBufferWriteEnable 0x2854, AlphaTestEnable 0x28cc,
  AlphaTestFunc 0x28a4, AlphaRef 0x28e0, SourceAlphaBlendMode 0x291c, DestAlphaBlendMode 0x2930, AlphaBlendEnable 0x28b8,
  TextureBumpEnvMat00/10/01/11[1] = 0x37b8/0x38f8/0x3858/0x3998, [2] = 0x37cc/0x390c/0x386c/0x39ac; StateStackSize 0x2808,
  StateUpdateListSize 0x280c, CurrentBookmarkMask 0x2814, BookmarkDummyState 0x3a24; TextureStagesInUse 0x3cb8, PD3DDevice 0x3cf0,
  PSelectedIndexBuffer 0x3cf4, CurrentlySelectedTextures 0x3d00.
- Tables: 0x01396DE8 = D3DAlphaFuncModeTable[ALPHA_TEST_GREATER(4)]; 0x01396F78 = D3DAlphaBlendModeTable[ALPHA_BLEND_SRC_ALPHA(4)];
  0x01396F7C = [ALPHA_BLEND_INV_SRC_ALPHA(5)] (engine/RenderStateEnums.h; table bases in CRenderStateManager.h:313-314).
- Opt-in macros available: FABLE_RETAIL_CENGINEWATERRENDERER (+ _SEAMIDGROUND for RenderSeaMidground decl), FABLE_RETAIL_CRENDERMANAGERCORE
  + FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS (inline AttachTextureToStage/AttachD3DTextureToStage; SelectVertexBuffer /
  SelectIndexBuffer are always FORCEINLINE), FABLE_RETAIL_CRENDERSTATEMANAGER (all the Set* setters used here are FORCEINLINE
  unconditionally: SetFogEnable, SetSpecularEnable, SetZBufferEnable, SetZBufferWriteEnable, SetAlphaTestEnable, SetAlphaTestFunc,
  SetAlphaRef, SetSourceAlphaBlendMode, SetDestAlphaBlendMode, SetAlphaBlendEnable), FABLE_RETAIL_CRENDERSTATEMANAGER_EARLY_RETURN (alt.
  SetRenderState spelling), FABLE_RETAIL_CTEXTURE, FABLE_RETAIL_CENGINESKYRENDERER.
- MISSING inline in the shared headers (need hdr/ overlays under new #ifdef opt-ins):
  * CRenderStateManager::SetTextureBumpEnvMatrix(long stage, float, float, float, float) -- header has it out of line (line 244);
    retail has NO copy (absent from Ego_r and from Fable.exe manifest; FableWin debug copy 0x02D54210 1477 b / 375 i) and expands it 6x2 times
    here. Body order in retail: SetRenderState(bits, Mat00[stage]); (Mat10[stage]); (Mat01[stage]); (Mat11[stage]) -- 00,10,01,11 --
    each value passed as float bits through a stack slot (*(unsigned long*)&f, cf. SetTextureMipmapLODBias comment). The four stack
    slots 0x28/0x2c/0x30/0x34 are the inlined float params.
  * CRenderStateCaptureBlock: ctor / dtor / Capture / Restore are declared out of line; retail has no copy (Ego_r none; FableWin
    ctor 20 b, dtor 29 b, Capture 621 b, Restore 621 b debug-with-asserts). Retail expansion: Capture() = StateManager->StartCaptureBlock(this);
    Open = true. Restore() = StateManager->RestoreCaptureBlock(); Open = false (retail stores Open=0 before the call, 0x00B79125).
    ~CRenderStateCaptureBlock() = if (Open) Restore() (0x00B7A805..0x00B7A813). Ctor (Open = false) is dead-stored away.
  * CEngineWaterRenderer::SetCommonRenderStates() (FableWin.pdb `?SetCommonRenderStates@CEngineWaterRenderer@@AAEXXZ` 0x02D4CC20,
    333 b / 106 i; absent from Ego_r and Fable.exe -> inline in release) -- not declared at all. Repeated block (3x) is exactly:
    SetSpecularEnable(false); SetZBufferEnable(true); SetZBufferWriteEnable(true); if (Settings.EnableBumpMap) { SBEM(1,f,0,0,f);
    SBEM(2,f,0,0,f); } else { SBEM(1,0,0,0,0); SBEM(2,0,0,0,0); } (f = Settings.BumpMatFactor re-read per call); SetAlphaTestEnable(true);
    SetAlphaTestFunc(ALPHA_TEST_GREATER); SetAlphaRef(1); SetSourceAlphaBlendMode(ALPHA_BLEND_SRC_ALPHA);
    SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_ALPHA); SetAlphaBlendEnable(true). The SetFogEnable(false) before / SetFogEnable(true) after
    are NOT in the helper: the third expansion (0x00B79DA9..0x00B7A685) has neither (see OUTLINE). Check with fwsize against 333/106
    whether FableWin's helper also holds the fog calls (then retail's 3rd site would differ -- it can't, so expect not).
  * declarations to add (overlay of CEngineWaterRenderer.h): RenderBackground(float), RenderIceBackground(float),
    RenderSeaNonReflectiveForeground(float), bool CreateDynamicTextures(bool,bool,bool), SetCommonRenderStates(); RenderSeaMidground via
    FABLE_RETAIL_CENGINEWATERRENDERER_SEAMIDGROUND. CEngineSkyRenderer::GetEnvironmentMap(CTexture*, const CEngineComponentRenderParams&);
    CTexture::GenerateMipmapsWithGPU().
- Classes the author must NOT model locally (shared headers exist): CEngineWaterRenderer, CEngineWaterSettings, CRenderStateManager,
  CRenderManagerCore, CRenderStateCaptureBlock (+Scoped), CScopedShadersEnable, CShaderRenderManager, CTexture, CEngineSkyRenderer,
  CArray. The older landed water rows (0x00B77E20, 0x00B76BC0, 0x00B75CA0) DO model CShaderRenderManager / IDirect3DDevice9 / CArray
  locally -- don't copy that.

## 5. Callee-body visibility
- Inline (no retail copy anywhere): every Set* render-state setter, SetRenderState, AddStateToUpdateList, StartCaptureBlock,
  SetTextureBumpEnvMatrix, CRenderStateCaptureBlock members, SetCommonRenderStates, AttachTextureToStage / AttachD3DTextureToStage,
  SelectVertexBuffer, SelectIndexBuffer, CArray/vector empty().
- CScopedShadersEnable ctor/dtor: retail HAS out-of-line copies 0x009D8AD0 / 0x009D8B40 but expands them here (header inline
  engine/CScopedShadersEnable.h does exactly that; ctor(true,true) const-folds to `cmp al,1; je`).
- Out of line, not visible / big: all Render* passes, CreateDynamicTextures, UpdateConstants, RestoreCaptureBlock, Enable/Disable*Shaders,
  ResetWorldTransform, GetEnvironmentMap, GenerateMipmapsWithGPU, FinishUsingDynamicTextures. No STLport out-of-line helper is called.

## 6. Sibling landed rows (idioms)
- 0x00B5AF80 CEngineScreenEffectColourFilter::RenderColorFilteredQuad (00/b5/...00b5af80.cpp): defines FABLE_RETAIL_CRENDERMANAGERCORE,
  _FORCEINLINE_HELPERS, FABLE_RETAIL_CRENDERSTATEMANAGER_STENCIL_INLINE, FABLE_RETAIL_CTEXTURE; includes engine/CRenderStateCaptureBlock.h +
  engine/CScopedShadersEnable.h; `CRenderStateScopedCaptureBlock capture;` -- the modern shared-header idiom to copy. No // Flags line.
- 0x00B5E0F0 CEngineScreenEffectRadialBlurRenderer::ApplyRadialBlur: `CRenderStateScopedCaptureBlock capture; ... CScopedShadersEnable shaders(true, true);`
- 0x009DA9F0 CRenderManager2D::Render2DDrawList: FABLE_RETAIL_CRENDERMANAGERCORE + CRENDERSTATEMANAGER + CVERTEXSHADER etc.
- Same class/TU: 0x00B77E20 RenderSeaNonReflectiveForeground, 0x00B76BC0 RenderBackground, 0x00B75CA0 RenderIceBackground,
  0x00B6E3E0 RenderIceBumpMap, 0x00B71FB0 FrameStart -- all default flags (no // Flags:, no pragmas) but older local-modelling style.
- Unmatched family work with fitted render-state overlays: /opt/fable_lane/w22/agent2048 (hdr/engine/CRenderStateManager.h with the
  `desired_state` named-local SetRenderState spelling, LESSONS/NOTES on bookmark test/or operand order residue), agent2060 NOTES (FableWin sizes).
- CRenderStateManager.h header comment: "the bookmark test / or operand order varies block by block in retail and no spelling moves it"
  (0x00B70230, 0x00B761D0, 0x00B751F0) -- expect that residue here too: retail mixes `test esi,edx` (mask in esi) and
  `mov edx,[eax+2814h]; mov esi,[ecx+8]` forms across blocks (e.g. 0x00B78B29 vs 0x00B78BB2).
