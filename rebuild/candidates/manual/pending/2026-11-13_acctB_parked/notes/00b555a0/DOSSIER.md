# DOSSIER 0x00B555A0 CEngineWeatherRenderer::Render (3832 bytes, no draft, no prior/)

## 1. Retail shape (xdis 00b555a0 0xf08 / dis_all.txt)
- `__thiscall`, `ret 4` (0xB5647F and 0xB56495). One argument: `const CEngineComponentRenderParams&`, passed straight on to RenderRain / RenderSnow / RenderMist (`push ebp`).
- NO frame pointer (`sub esp,0Ch` first) -> no `#pragma optimize("y",off)`. NO SEH / EH prologue (no `fs:[0]`) although three scoped
  objects with destructors live across calls -> built WITHOUT /EHsc: no `// Flags:` line (default). Same as the matched twin
  RenderClouds 0x00B64FA0 (`sub esp,38h`, no EH) and landed ApplyRadialBlur 0x00B5E0F0 (scoped capture block + layout + shaders, no EH).
- No /GS cookie. Frame 0xC bytes; callee-saved pushes ebx, ebp, esi, edi interleaved into the capture-block code (0xB555B7 / BE / BF / EB);
  pops shrink-wrapped at 0xB5643C..3F (before the shader-enable restores), so the two `ret 4` tails have only `add esp,0Ch`.
- Early return `if (!RenderEnabled) return;` jumps to the final `add esp,0Ch ; ret 4` at 0xB56492 (before any push).
- Destructor tail is duplicated: EnablePixelShaders path 0xB5646C -> RestoreCaptureBlock -> ret (0xB5647F); DisablePixelShaders path
  0xB56482 -> falls into 0xB56487 RestoreCaptureBlock -> 0xB56492.

## 2. Identity (two+ sources) -- manifest label is RIGHT
- Ego_r.pdb: 0x009FB880 `?Render@CEngineWeatherRenderer@@UAEXABVCEngineComponentRenderParams@@@Z` (ghidra_out/egor_pdb_names.tsv); Ego_r order
  RenderMist 0x009F8B90, RenderRain 0x009FAAD0, RenderSnow 0x009FB0F0, Render 0x009FB880 = retail order 0xB54310 / 0xB54890 / 0xB54E30 / 0xB555A0.
- FableWin.pdb: 0x02D3B660 same mangled name, debug body 787 bytes / 209 instructions (naming_stragglers/FableWin.exe.signatures.tsv) -> fwsize oracle
  (`FW_EXTRA=/EHsc python3 /opt/fable_lane/tools/fwsize.py <src> Render`).
- Vtable: rtti-vtables.tsv CEngineWeatherRenderer 0x012A2434 slot 4 = 0x00B555A0; engine/CEngineComponent.h slot 4 = `virtual void Render(const CEngineComponentRenderParams&)`.
- Callers: only the vtable (xref.py: abs .rdata 0x012A2444).
- Mangled leaf: `?Render@CEngineWeatherRenderer@@UAEXABVCEngineComponentRenderParams@@@Z` -- public virtual, void, param `const CEngineComponentRenderParams&` (unused except forwarded).

## 3. Callees (all names = PDB spelling; no draft to disagree)
| retail | call site(s) | landed row | name |
|---|---|---|---|
| 0x00987FE0 | 0xB555F7, 0xB5644B | 00/98/CShaderRenderManager_EnableVertexShaders_00987fe0.cpp | CShaderRenderManager::EnableVertexShaders (ecx = singleton 0x013BC470) |
| 0x00988110 | 0xB5560E, 0xB5646C | 00/98/CShaderRenderManager_EnablePixelShaders_00988110.cpp | CShaderRenderManager::EnablePixelShaders |
| 0x009880E0 | 0xB56452 | 00/98/CShaderRenderManager_DisableVertexShaders_009880e0.cpp | CShaderRenderManager::DisableVertexShaders |
| 0x00988190 | 0xB56482 | 00/98/CShaderRenderManager_DisablePixelShaders_00988190.cpp | CShaderRenderManager::DisablePixelShaders |
| 0x0098B5E0 | 0xB55622 (arg 1), 0xB5642D (arg old) | 00/98/CShaderRenderManager_SetVertexShaderConstantLayout_0098b5e0.cpp | CShaderRenderManager::SetVertexShaderConstantLayout(long) |
| 0x00988290 | 0xB55E23 (ecx=[0x01436E14]) | 00/98/CShaderRenderManager_ResetWorldTransform_00988290.cpp | ?ResetWorldTransform@CShaderRenderManager@@QAEXXZ (public in engine/CShaderRenderManager.h:104) |
| 0x00B54890 | 0xB55E3D | NOT landed (parked, agent2008 / overlay) | ?RenderRain@CEngineWeatherRenderer@@AAEXABVCEngineComponentRenderParams@@@Z (Ego_r + FableWin) |
| 0x00B54E30 | 0xB55E4F | NOT landed (parked, agent2031 / overlay) | ?RenderSnow@CEngineWeatherRenderer@@AAEXABVCEngineComponentRenderParams@@@Z |
| 0x00B54310 | 0xB563E1 | 00/b5/CEngineWeatherRenderer_RenderMist_00b54310.cpp | ?RenderMist@CEngineWeatherRenderer@@AAEXABVCEngineComponentRenderParams@@@Z (Ego_r + FableWin); NOT declared in any header/overlay yet |
| 0x00A05840 | 0xB56477, 0xB5648D (ecx=[0x013CB508]) | 00/a0/CRenderStateManager_RestoreCaptureBlock_00a05840.cpp | CRenderStateManager::RestoreCaptureBlock |
| IDirect3DDevice9 +0x1A0 | 0xB56400 | -- | SetIndices(0) (slot 104) via inline CRenderManagerCore::SelectIndexBuffer(0, 0) |
| IDirect3DDevice9 +0x190 | 0xB5641D | -- | SetStreamSource(0,0,0,0) (slot 100) via inline CRenderManagerCore::SelectVertexBuffer(0, 0) |

Globals: NEngineInternal::GPRenderManager 0x01436E18 (CRenderManager*, : CRenderManagerCore : CRenderStateManager), NEngineInternal::GPShaderRenderManager
0x01436E14, CShaderRenderManager::SingletonInstance 0x013BC470 (VertexShadersEnabled +0x390 = 0x013BC800, PixelShadersEnabled +0x391 = 0x013BC801,
CurrentVSConstantLayoutIndex +0x8 = 0x013BC478), CRenderStateCaptureBlock::StateManager 0x013CB508. Spell the two NEngineInternal externs exactly as the
landed rows do (`namespace NEngineInternal { extern CRenderManager* GPRenderManager; }  // 0x01436E18`, ditto GPShaderRenderManager) -- 18 landed rows.
Tables (CRenderStateManager private statics): D3DAlphaFuncModeTable 0x01396DD8 [ALPHA_TEST_GREATER=4] = 0x01396DE8; D3DAlphaBlendModeTable 0x01396F68
[ALPHA_BLEND_SRC_ALPHA=4] = 0x01396F78, [ALPHA_BLEND_INV_SRC_ALPHA=5] = 0x01396F7C; D3DTextureUVModeTable 0x01396DC8 [WRAP=0] = 0x01396DC8,
[MIRROR=1] = 0x01396DCC, [CLAMP=2] = 0x01396DD0 (enums: engine/RenderStateEnums.h).

## 4. Members
- this+0x08 `bool RenderEnabled` (CEngineComponent; engine/CEngineComponent.h retail branch line 39, FABLE_RETAIL_CENGINECOMPONENT; PeekRenderEnabled() inline exists).
- this+0x5D0 / 0x5D1 / 0x5D2 = Settings(+0x114).Raining (+0x4BC) / Snowing (+0x4BD) / Mist (+0x4BE) (struct_layouts_egor.tsv CEngineWeatherSettings; Ego_r
  member names, no containers before them). Repo has NO engine/CEngineWeatherSettings.h: it lives in the overlay /opt/fable_lane/w22/agent2048/hdr/engine/
  CEngineWeatherSettings.h under FABLE_RETAIL_CENGINEWEATHERSETTINGS_TAIL (lines 61-70); CEngineWeatherRenderer.h overlay types `Settings` under
  FABLE_RETAIL_CENGINEWEATHERRENDERER_BUFFERS (+ _RENDERRAIN, _RENDERSNOW for RenderRain/RenderSnow declarations).
- Missing from every header: `virtual void Render(const CEngineComponentRenderParams&);` override and `void RenderMist(const CEngineComponentRenderParams&);`
  (private, AAE) -> add as a new nested opt-in (e.g. FABLE_RETAIL_CENGINEWEATHERRENDERER_RENDER) in the overlay copy of CEngineWeatherRenderer.h.
- GPRenderManager members (CRenderStateManager.h, retail branch FABLE_RETAIL_CRENDERSTATEMANAGER, pulled in by FABLE_RETAIL_CRENDERMANAGERCORE):
  StateStack +0x08, StateUpdateList +0x2008, StateStackSize +0x2808, StateUpdateListSize +0x280C, CurrentBookmarkMask +0x2814, DeviceState +0x2818,
  BookmarkDummyState +0x3A24; DeviceState offsets used: ZBufferWriteEnable +0x3C (abs 0x2854, section 1), ZBufferEnable +0x50 (abs 0x2868, Mist section), AlphaTestFunc +0x8C (0x28A4), AlphaBlendEnable +0xA0 (0x28B8),
  AlphaTestEnable +0xB4 (0x28CC), AlphaRef +0xC8 (0x28E0), SourceAlphaBlendMode +0x104 (0x291C), DestAlphaBlendMode +0x118 (0x2930),
  TextureUVModeU[s] +0x6CC+0x14s (0x2EE4..), V +0x76C (0x2F84..), W +0x80C (0x3024..). CStateInfo: DesiredState +4, BookmarkMask +8, DirtyListFlag +0x10.
  CRenderManagerCore (FABLE_RETAIL_CRENDERMANAGERCORE): PD3DDevice +0x3CF0, PSelectedIndexBuffer +0x3CF4.
- No local class models needed: no draft exists. Do NOT model CShaderRenderManager / CRenderManagerCore locally (old RenderMist row does -- don't copy that).

## 5. Visibility
- All state setters (SetZBufferWriteEnable, SetZBufferEnable, SetAlphaTestEnable, SetAlphaTestFunc, SetAlphaRef, SetAlphaBlendEnable, Set{Source,Dest}AlphaBlendMode,
  SetTextureUVModeU/V/W), SetRenderState, AddStateToUpdateList, StartCaptureBlock, SelectIndexBuffer, SelectVertexBuffer are FABLE_FORCEINLINE in the repo headers
  (no retail copy). CScopedShadersEnable / CScopedVertexShaderConstantLayout / CRenderStateScopedCaptureBlock: inline header classes (repo engine/CScopedShadersEnable.h,
  CScopedVertexShaderConstantLayout.h, CRenderStateCaptureBlock.h); retail's out-of-line ??0/??1CScopedShadersEnable 0x009D8AD0/0x009D8B40 are NOT called here.
- RenderRain / RenderSnow / RenderMist are same-TU (addresses just before) but too big to inline -> plain declarations; they are called.
- Out of line in other TUs: CShaderRenderManager Enable/Disable*, SetVertexShaderConstantLayout, ResetWorldTransform, RestoreCaptureBlock. No STLport.

## 6. Siblings / idioms to copy
- 00/b5/CEngineScreenEffectRadialBlurRenderer_ApplyRadialBlur_00b5e0f0.cpp (landed): exactly this scope trio with repo headers:
  `#define FABLE_RETAIL_CRENDERMANAGERCORE`, includes engine/CRenderManager.h, CShaderRenderManager.h, CRenderStateCaptureBlock.h, CScopedShadersEnable.h,
  CScopedVertexShaderConstantLayout.h; `NEngineInternal::GPRenderManager->SetAlphaBlendEnable(true);` style. No Flags, no pragmas.
- 00/bf/CEnginePrimitiveRenderUtil_SetupOutlineSmoothAlphaStencilMaskStates_00bf01e0.cpp: SetZBufferWriteEnable(false) / SetAlphaTestEnable(true) /
  SetAlphaTestFunc(ALPHA_TEST_GREATER) on GPRenderManager.
- /opt/fable_lane/w22/agent2048/src/00b64fa0.cpp (RenderClouds, matched 734/734 with a lever): 12 UV setters written out (a stage loop is NOT unrolled by VC7.1),
  overlay opt-ins FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_INLINE, FABLE_RETAIL_CRENDERSTATEMANAGER_SETRENDERSTATE_DESIRED (code-identical, moves c2 tie-breaks).
- Same class landed rows: RenderMist 0x00B54310 (no flags, local models -- old style), WorldFrameUpdate 0x00B52B30 (`#pragma intrinsic(fabs)`),
  UpdateBoxPositions 0x00B52990 uses `optimize("y",off)` -- NOT applicable here (no ebp frame).
- Header overlay: $W/hdr is EMPTY. Start from /opt/fable_lane/w22/agent2048/hdr (has CEngineWeatherRenderer.h with _BUFFERS/_RENDERRAIN/_RENDERSNOW,
  CEngineWeatherSettings.h _TAIL, CWeatherMask.h, CRenderStateManager.h _SETRENDERSTATE_DESIRED, CRenderStateCaptureBlock.h _INLINE) and add the Render / RenderMist opt-in.

## 7. Known residue risk (lane gap, GOTCHAS / CRenderStateManager.h header comment)
Bookmark test / or operand order per SetRenderState block (retail):
- section 1 (0xB55627..0xB55E1D): blocks 1-7 (ZWrite..DestBlend) `test eax,esi ; or esi,eax ; mov [edx+8],esi`; 12 UV blocks `test esi,eax ; or eax,esi ; mov [edx+8],eax`.
  Desired compare form: `mov esi,[eax+X] ; cmp esi,<val>` (constant 0 -> `test esi,esi`; 1 -> `cmp esi,ebx`); table values `mov edi,[tbl] ; mov esi,[eax+X] ; cmp esi,edi`
  (blocks 3,6,7 use ebp for the desired value: `mov esi,[tbl] ; mov ebp,[eax+X] ; cmp ebp,esi ; mov edi,esi`).
- section 2 (Mist): ZBufferEnable(false) block `test esi,eax ; or eax,esi`; UV U0,V0 `test ecx,eax ; or eax,ecx`; W0..W3 (10) `test eax,ecx ; or ecx,eax`. Compare form here is
  `mov ecx,[0x01396DCC] ; cmp [eax+X],ecx ; ... ; mov edi,ecx` (memory operand) -- the very form RenderClouds' residue wanted.
Expect this to be the final residue; agent2048 NOTES "Measurements" lists the levers (c2 record count / named intermediates).

## 8. Re-verification (second scout pass, 2026-11-12) -- everything above re-checked against dis_all.txt and the repo; all CONFIRMED
- Identity, three sources: Ego_r.pdb 0x009FB880 (egor_pdb_names.tsv:28007; ego_r signatures 3677 B / 909 instr), FableWin.pdb 0x02D3B660 (787 B / 209 instr,
  FableWin.exe.signatures.tsv:268522; thunk 0x017E0559), rtti-vtables.tsv:13308 `CEngineWeatherRenderer 012a2434 slot 4 = 00b555a0`. Manifest label correct.
- Settings bools: struct_layouts_egor.tsv CEngineWeatherSettings 1212/1213/1214 = Raining/Snowing/Mist (bool) -> this+0x114+0x4BC.. = 0x5D0/0x5D1/0x5D2.
- RenderRain 0x00B54890 and RenderSnow 0x00B54E30 are still NOT landed in the repo (rebuild/src/compiled/00/b5 has RenderMist, RenderRainBox, RenderSnowBox only);
  a declaration (AAE private, overlay _RENDERRAIN / _RENDERSNOW) is all Render needs.
- Repo engine/CEngineWeatherRenderer.h retail branch is still opaque (`_pad_0xc[0x12f0]`, no Render/RenderMist/Settings) -> the overlay is mandatory; copy
  /opt/fable_lane/w22/agent2048/hdr/engine/{CEngineWeatherRenderer.h,CEngineWeatherSettings.h,CWeatherMask.h,CRenderStateManager.h,CRenderStateCaptureBlock.h,...}
  into $W/hdr and add the Render override + RenderMist declaration under a new nested opt-in.
- Matched model: agent2048 RenderClouds 0x00B64FA0 = RELOCATION_MATCH (STATUS.tsv) with FABLE_RETAIL_CRENDERSTATEMANAGER_SETRENDERSTATE_DESIRED
  (named `const unsigned long desired_state = state.DesiredState;` in SetRenderState) and FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_INLINE. Both are code-neutral
  levers that only move the bookmark `test/or` operand order (GOTCHAS lane gap). Start WITH both; toggle each as the first two probes if blocks 1-7 or the
  mist W-blocks show the or-order residue.
- Scope-object order: ApplyRadialBlur (landed) declares capture, layout(2), shaders(true,true); THIS row is capture, shaders(true,true), layout(1)
  (retail: Enable{Vertex,Pixel}Shaders 0xB555F7/0xB5560E BEFORE SetVertexShaderConstantLayout(1) 0xB55622; dtor layout 0xB5642D first, shaders, capture last).
- CScopedShadersEnable (repo header) ctor/dtor bodies inline via SetVertexShadersEnable/SetPixelShadersEnable (CShaderRenderManager.h:83-84):
  retail ctor `old=[0x013BC800]; if (old != 1) EnableVertexShaders()` -- the `else Disable` arm folds away for constant `true`; dtor
  `if (old != cur) { if (old) Enable else Disable }` -- matches 0xB56432..0xB56482.
- SelectIndexBuffer(0,0) / SelectVertexBuffer(0,0) (CRenderManagerCore.h:98-113, FABLE_FORCEINLINE): retail 0xB563E6 stores PSelectedIndexBuffer=0 then
  SetIndices(0); 0xB56406 reloads GPRenderManager then SetStreamSource(0,0,0,0). Index BEFORE vertex.
