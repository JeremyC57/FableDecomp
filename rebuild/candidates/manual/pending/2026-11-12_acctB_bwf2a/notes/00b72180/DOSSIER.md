# DOSSIER 0x00B72180 CEngineWaterRenderer::CreateDynamicTextures (5587 bytes)

## 1. Retail shape (xdis 00b72180 0x15e3; $W/dis_all.txt)
- __thiscall, `ret 0Ch` (3 bool args at [ebp+8] / [ebp+0Ch] / [ebp+10h]); returns bool in al (0 at 0xB729EF, 1 at 0xB73748 `mov al,1`).
- Prologue `push ebp ; mov ebp,esp ; and esp,-8 ; sub esp,0A4h ; push ebx ; push esi ; push edi` -- this is VC7.1's
  ALIGNED frame (ebp kept for the args, esp 8-aligned), not /Oy-. No fs:[0] / no EH state: NO /EHsc unwinding although
  the function has destructible locals (CGraphicFrame x5, CRenderTarget, CSurface, capture block) -> TU built without
  /EHsc (or the dtors are all nothrow-inline); no /GS cookie. Speed TU (/O2): imul-by-reciprocal size(), no size idioms.
- this is spilled to [esp+10h] (stored at 0xB72193 as [esp+0Ch] before `push edi`).
- Two epilogues: failure return in the middle (0xB729EF..0xB729F7), success at the end (0xB73748..0xB73750). No cold blocks.

## 2. Name / signature (proven, 3 sources)
- Ego_r.pdb 0x00A1EE70 and FableWin.pdb 0x02D515A0: `?CreateDynamicTextures@CEngineWaterRenderer@@AAE_N_N00@Z`
  = private: bool __thiscall CEngineWaterRenderer::CreateDynamicTextures(bool, bool, bool).
- ghidra_out/bsim_port_audit_05final.tsv: b72180 <- same mangled name (sim 0.637, signif 755).
- Ego_r neighbour order: ... RenderBumpMap 0x00A1E110 / BuildConstantBuffers 0x00A1E610 / CreateDynamicTextures 0x00A1EE70.
- Only caller: 0x00B78584 inside CEngineWaterRenderer::Render 0x00B783F0 (vtable slot 4; not landed).
- Manifest label is RIGHT (module + name). Param names not in the name TSVs: use descriptive
  `render_water_bump, render_sea_bump, render_ice_bump` (arg1 drives WaterBumpMap/RenderBumpMap(0), arg2 SeaBumpMap/RenderBumpMap(1),
  arg3 IceBumpMap/RenderIceBumpMap). Header engine/CEngineWaterRenderer.h does NOT declare it -> add an opt-in
  (e.g. FABLE_RETAIL_CENGINEWATERRENDERER_CREATEDYNAMICTEXTURES) with CreateDynamicTextures, RenderBumpMap(EWaterArrayIndexAliases)
  (?RenderBumpMap@CEngineWaterRenderer@@AAEXW4EWaterArrayIndexAliases@@@Z, 0x00B6E890), RenderIceBumpMap() (?..@@AAEXXZ, 0x00B6E3E0)
  and SetRenderTargetToSurface (below). RenderEnvironmentMap() is already declared (0x00B70A90).

## 3. Callees (landed file | landed name | PDB name | flag)
| addr | landed row | PDB / evidence | note |
|---|---|---|---|
| 0x0041BDF0 | 00/41/CGraphicFrameData_scalardeletingdestructor_0041bdf0.cpp | ??_GCGraphicFrameData@@QAEPAXI@Z | `delete this` in CGraphicFrameData::Release NOT expanded in this TU (10 calls here; other callers 0xB77AE9, 0xB7E1D5 = same water TU). |
| 0x009FBFB0 / 0x009FBF90 | CResource_ListRemove / _ListInsert | ?ListRemove@CResource@@AAEXXZ / ?ListInsert@CResource@@AAEXPAV1@0@Z | private; landed rows use a `CResource_Methods` stand-in class (spelling debt, not yours). |
| 0x009FDE80 | (not landed) | CGraphicInfoFrame::CFrameData::BeginLoading(unsigned long, unsigned long) | 10 calls, args (0,0). |
| 0x009FE380 | CGraphicInfoFrameCFrameData_CFrameData_009fe380.cpp | CFrameData(const CGraphicInfoFrame*) | after operator new(0x44). |
| 0x00BFEA1A | global_operatornew | operator new | |
| 0x00F36EE0 | CGPUScratchBufferTextureManager_StartSurfaceUse_00f36ee0.cpp | **?StartTextureUse@CGPUScratchBufferTextureManager@@QAEPAVCTexture@@J@Z** | NAME_FIX: landed says StartSurfaceUse, but body returns entry+4 (0xF36F10 returns entry+8 = StartSurfaceUse) and the result is stored into CTexture* EnvironmentMap/WaterBumpMap/SeaBumpMap/IceBumpMap. |
| 0x00987FE0 / 0x009880E0 | CShaderRenderManager_EnableVertexShaders / DisableVertexShaders | ?EnableVertexShaders@ / ?DisableVertexShaders@CShaderRenderManager@@ | through header inline SetVertexShadersEnable(bool,bool*) (engine/CShaderRenderManager.h:83). |
| 0x00988110 / 0x00988190 | Enable/DisablePixelShaders | same | SetPixelShadersEnable (header :84). |
| 0x00988020 | CShaderRenderManager_ApplyVertexShader | ApplyVertexShader(const CVertexShader&, bool) | (BumpmapVertexShader +0x178, false) |
| 0x00988140 | CShaderRenderManager_ApplyPixelShader | ApplyPixelShader(const CPixelShader&) | (BumpmapPixelShader +0x180) |
| 0x0099A2D0 / 0x0099A2E0 | CBaseClass_CBaseClass (both files) | ??0CBaseClass / ??1CBaseClass | base of the CRenderTarget local (ctor inline, base called). |
| 0x009BF160 | CDisplayManager_SetRenderTarget | ?SetRenderTarget@CDisplayManager@@QAE_NAAVCRenderTarget@@@Z | on GPDisplayManager 0x01436E1C; bool ignored. |
| 0x009BF220 | CDisplayManager_SetRenderTargetAsBackScreen | ?SetRenderTargetAsBackScreen@CDisplayManager@@QAE_NXZ | |
| 0x009E1DA0 / 0x009E1DD0 | Global_GFAXToVX / global_GFAYToVY | ?GFAXToVX@@YIMM@Z / GFAYToVY | __fastcall float, float pushed. |
| 0x009F2E20 | CSurface_Uninitialise | ?Uninitialise@CSurface@@QAEXXZ | also called by ~CSurface (vptr 0x0122F84C store then call). |
| 0x009F2E70 | CSurface_operator_009f2e70.cpp | ??4CSurface@@QAEAAV0@ABV0@@Z | landed source uses a `CSurface_Methods` stand-in. |
| 0x009F2F90 | CSurface_GetDimensions | ?GetDimensions@CSurface@@QBE?AVC2DExtentsI@@XZ | via inline GetWidth/GetHeight. |
| 0x009F9DB0 | CTexture_GetDimensions | ?GetDimensions@CTexture@@QBE?AVC2DExtentsI@@K@Z | landed uses CTexture_Methods stand-in. |
| 0x009F9DF0 | CTexture_GetNoLevels | ?GetNoLevels@CTexture@@QBEJXZ | (BSim audit wrongly put GetNoLevels on 0x9F9E00; ignore.) |
| 0x009F9E00 | CTexture_GetSurfaceLevel | ?GetSurfaceLevel@CTexture@@QBE?AVCSurface@@J@Z | hidden return (engine/CTexture.h:49). |
| 0x009FAF70 | (not landed) | CTexture::GenerateMipmapsWithGPU() (manifest) | on EnvironmentMap after RenderEnvironmentMap. Verify spelling in PDB TSVs before declaring. |
| 0x00A05840 | CRenderStateManager_RestoreCaptureBlock | ?RestoreCaptureBlock@CRenderStateManager@@ | the capture-block destructor. |
| 0x00A0B560 | CRenderManagerCore_SetVWindow | ?SetVWindow@CRenderManagerCore@@QAEXABVC2DBoxF@@@Z | the landed SetRenderTargetToSurface row calls it "SetViewport" -- PDB spelling is SetVWindow. |
| 0x00A10E60 | CRenderTarget_AttachTarget | ?AttachTarget@CRenderTarget@@QAEXABVCSurface@@@Z | |
| 0x00A10E10 | CRenderTarget_DetachTarget_00a10e10.cpp | ?DetachTarget@CRenderTarget@@QAEXXZ | |
| 0x00A10E20 | CRenderTarget_DetachTarget_00a10e20.cpp | **?DetachZBuffer@CRenderTarget@@QAEXXZ** | NAME_FIX: landed calls it DetachTarget too; body tests +0x18 and uninitialises the +0x14 surface (the 2nd CSurface = ZBuffer); Ego_r has the Attach/Detach Target/ZBuffer pairs. |
| 0x00B2FC50 | CEngineCamera_RefreshD3DTransforms | ?RefreshD3DTransforms@CEngineCamera@@QAEXXZ | on GPCamera 0x01436EA0 (landed source still a CEngineCamera_Methods stand-in). |
| 0x00B6E890 | (not landed) | ?RenderBumpMap@CEngineWaterRenderer@@AAEXW4EWaterArrayIndexAliases@@@Z | args 0 (water) / 1 (sea). |
| 0x00B6E3E0 | CEngineWaterRenderer_RenderIceBumpMap | ?RenderIceBumpMap@CEngineWaterRenderer@@AAEXXZ | |
| 0x00B70A90 | (not landed) | ?RenderEnvironmentMap@CEngineWaterRenderer@@AAEXXZ | |
| vtbl IDirect3DDevice9 +0x104 / +0x190 / +0x1A0 | -- | SetTexture / SetStreamSource / SetIndices | via CRenderManagerCore inline helpers. |
| vtbl CFrameData +4 | -- | CResource slot 1 (CIVCountedPointer release) | `dec [ecx+4] ; jne ; call [vtbl+4]`. |

## 4. Members / globals
- this (CEngineWaterRenderer, engine/CEngineWaterRenderer.h, FABLE_RETAIL_CENGINEWATERRENDERER; offsets in the header = retail):
  ForegroundTextureIndexBuffers +0x020 (CIndexBuffer, PD3DIndexBuffer at +4), ForegroundTextureVertexBuffers +0x034 (CVertexBufferWin32:
  PVertexBuffer +4, Stride byte +0x10), BumpmapVertexShader +0x178, BumpmapPixelShader +0x180, EnvironmentMapHandle +0x1DC,
  EnvironmentMap +0x1E0, SeaBumpMapHandle +0x1E4, SeaBumpMap +0x1E8, WaterBumpMapHandle +0x1EC, WaterBumpMap +0x1F0,
  IceBumpMapHandle +0x1F4, IceBumpMap +0x1F8, HighendWaterEnabled +0x274, Settings.EnableReflectionGeneration +0x28F
  (CEngineWaterSettings +0xF; engine/CEngineWaterSettings.h line 50).
  NOTE: landed FinishUsingDynamicTextures 0x00B6D630 models +0x1dc.. LOCALLY with wrong names (m_pReflectionTexture...);
  the shared header names are the Ego_r ones and are right (Handle/CTexture* pairs). InitialiseDynamicTextures 0x00B6D510 uses
  a fake class `CWaterDynamicTextures` -- do not copy either.
- 0x01436E24 env def (`extern const CEnvironmentDef* g_pEngineEnvironmentDef;` descriptive, landed CEngine::Initialise 0x00B26360;
  no PDB name found). engine/CEnvironmentDef.h needs FABLE_RETAIL_CENVIRONMENTDEF_PARENT (+FABLE_RETAIL_DEF_CLASS_BASE, fable_string.h):
  IceBumpMap +0xC8, WaterBumpMap +0xD8, WaterBumpMap2 +0xDC, SeaBumpMap +0xE0, SeaBumpMap2 +0xE4 (Ego_r - 4).
- 0x01436E98 NEngineInternal::GPResourceManager (CEngineResourceManager): GraphicBanks[0] +0x954 (engine/CEngineResourceManager.h:172;
  PeekGraphicBank opt-in FABLE_RETAIL_CENGINERESOURCEMANAGER_GETGRAPHICBANK), scratch texture manager +0x3190 (CGPUScratchBufferTextureManager).
- CGraphicDataBank: GraphicList CArray<CGraphicInfo> at +0x1E0 (12-byte vector, 0x2C elements; `imul 2E8BA2E9h ; sar edx,3`),
  CGraphicInfo::Frames +0x28, Bank +0x20; bank CurrentFrame +0x194. CGraphicInfoFrame: Data +8, GraphicInfo +0xC. CFrameData (0x44):
  RefCount +4, ResourceList +8, LastUsedFrame +0x18, Frame +0x1C, ManagedTexture +0x20 (Data +0x24), CurrentMipmapLevel +0x38,
  TouchedMipmapLevel +0x3A, TouchedFrame +0x3C. CGraphicFrameData: ReferenceCount +0, PTexture.Data +8.
  Shared headers: engine/CGraphicDataBank.h / CGraphicInfo.h / CGraphicInfoFrame.h / CGraphicFrame.h / CResource.h /
  CResourceList.h under FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS; the GetFrame / TouchResource / ReleaseData opt-ins
  exist ONLY in the agent2048 overlay (/opt/fable_lane/w22/agent2048/hdr), not in the repo.
- 0x01436E18 NEngineInternal::GPRenderManager (CRenderManagerCore, FABLE_RETAIL_CRENDERMANAGERCORE): capture stack +0x2808 count,
  +0x2814 mask, dirty list +0x2008 / +0x280C, tracked states (value +4, mask +8, queued +0x10). States written here:
  +0x2868, +0x28CC, +0x29E4, +0x28B8, +0x2944 <- 0 ; +0x2EE4, +0x2F84, +0x2EF8, +0x2F98 <- D3DTextureUVModeTable[0] (0x01396DC8,
  i.e. UV mode WRAP: SetTextureUVModeU/V stage 0, then stage 1 -- same states RenderClouds 0x00B64FA0 sets). Resolve the five
  zero-states to CRenderStateManager setter names from engine/CRenderStateManager.h (FABLE_RETAIL_CRENDERSTATEMANAGER) / the 2048
  overlay (RenderIceBumpMap sets +0x28B8 <- 1 then 0 = an enable flag, probably AlphaBlendEnable). PD3DDevice +0x3CF0,
  PSelectedIndexBuffer +0x3CF4, TextureStagesInUse +0x3CB8, CurrentlySelectedTextures +0x3D00.
- 0x013CB508 CRenderStateManager::StateManager (capture block ctor/dtor), +0x3A24 = bookmark pushed.
- 0x013BC470 CShaderRenderManager::SingletonInstance (VertexShadersEnabled +0x390 = 0x013BC800, PixelShadersEnabled 0x013BC801).
- 0x01436E14 GPShaderRenderManager (Apply*Shader), 0x01436E1C GPDisplayManager, 0x01436EA0 GPCamera, 0x0122DCB4 2^32 float,
  0x0122F84C ??_7CSurface@@6B@, 0x0129C800 ??_7CRenderTarget@@6B@ (inline default ctor stores it).
- Local models to AVOID: the landed RenderIceBumpMap/FinishUsing/Initialise rows model the water class locally; use the shared header.

## 5. Callee visibility (what retail expanded vs called)
- EXPANDED inline (write as header inlines / FABLE_FORCEINLINE, as agent2048's RenderClouds/RenderOuterSky which MATCHED):
  CGraphicDataBank::PeekGraphicInfoFrame(n,0) (PeekGraphic twice -> the double size()), CGraphicInfoFrame::GetFrame(frame,0,0) const
  (FableWin ?GetFrame@CGraphicInfoFrame@@QBE_NAAVCGraphicFrame@@KK@Z; landed out-of-line twin 0x0041C260 gives the body),
  CResource::TouchResource, CManagedTexture::TouchTexture, CFrameData release, CGraphicFrame::operator=, ~CGraphicFrame/ReleaseData,
  CRenderStateCaptureBlock ctor/dtor, CShaderRenderManager::SetVertexShadersEnable/SetPixelShadersEnable, SetRenderState x9,
  CRenderManagerCore::SelectVertexBuffer / SelectIndexBuffer / AttachTextureToStage (header FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS),
  CRenderTarget default ctor + dtor, CSurface ctor/dtor, CSurface::GetWidth/GetHeight, CGraphicFrameData::GetTexture.
- SetRenderTargetToSurface(CRenderTarget&, const CSurface&) (FableWin, AAE) is expanded 4x; retail also has an uncalled out-of-line
  copy at 0x00B7AD20 (landed, `ret 8`) -> it is an `inline` member defined in this TU (retail emitted it unreferenced). Its landed text
  is the one statement SetVWindow(C2DBoxF(0,0,GFAXToVX((float)surface.GetWidth()),GFAYToVY((float)surface.GetHeight()))).
- CALLED out of line: ??_GCGraphicFrameData (the ~CGraphicFrameData is not expanded under `delete this` in this TU -- the opposite
  of the sky TU; do not force-inline ~CGraphicFrameData / use CGraphicFrameData::Release with plain `delete this`), BeginLoading,
  CFrameData ctor, the CSurface/CTexture/CRenderTarget members listed in section 3.
- STLport: no container helpers called; only size() expansions.

## 6. Siblings / idiom sources
- /opt/fable_lane/w22/agent2048/src/00b64fa0.cpp (RenderClouds, RELOCATION_MATCH): same GetFrame/PeekGraphicInfoFrame pattern, capture
  block, UV-mode setters; its #define list (lines 27-66) and hdr/ overlay are the starting point (`FABLE_HDR_OVERLAY`). Copy that hdr/
  into $W/hdr and re-base. NOTES.md there documents the SetRenderState c2 record residue (bookmark/or-operand order) -- expect it here (9 setters).
- rebuild/src/compiled/00/b7/CEngineWaterRenderer_SetRenderTargetToSurface_00b7ad20.cpp (the inline helper's text, frame 0x20 lesson).
- rebuild/src/compiled/00/41/CGraphicInfoFrame_GetFrame_0041c260.cpp (GetFrame body, CFrameData layout).
- rebuild/src/compiled/00/ba/CEngineInternalPrimitive2DClockSprite_Update_00badb40.cpp (PeekGraphic/PeekGraphicInfoFrame inline text).
- Water rows in the same TU: FrameStart 0x00B71FB0 (just before this row), RenderIceBumpMap 0x00B6E3E0, RenderBackground 0x00B76BC0 --
  all `// Flags: none`/no pragma, local models (old style). No pragma optimize in any water row.

## NAME_FIXES (for the coordinator)
- 0x00F36EE0: StartSurfaceUse -> ?StartTextureUse@CGPUScratchBufferTextureManager@@QAEPAVCTexture@@J@Z (entry+4 v 0xF36F10 entry+8; result -> CTexture*).
- 0x00A10E20: DetachTarget -> ?DetachZBuffer@CRenderTarget@@QAEXXZ (operates on the +0x14 surface).
- 0x00B6D510 landed leaf `?InitialiseDynamicTextures@CWaterDynamicTextures@@` -> ?InitialiseDynamicTextures@CEngineWaterRenderer@@AAEXXZ (Ego_r).
