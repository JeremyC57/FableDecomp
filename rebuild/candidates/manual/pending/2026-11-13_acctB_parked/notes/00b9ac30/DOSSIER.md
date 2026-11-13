# DOSSIER 0x00B9AC30 -- CEnginePrimitiveManagerRepeatedStaticMeshes::RenderSubPrimitive (4229 bytes)

## 1. Retail shape (xdis / dis_all.txt)
- `__thiscall`, `this` = esi for the whole body. Two stack args: arg1 `CEngineSubPrimitiveBase&` ([esp+0xd8] after the
  prologue), arg2 `ERenderPassType` ([esp+0xdc], only used at 0x00B9B903 for PrepareOcclusionTest). `ret 8` @0x00B9BCB2.
- NO frame pointer (no `push ebp; mov ebp,esp`): esp-relative frame -> default /Oy (no `optimize("y",off)`).
- `sub esp,0C4h` then push ebx, ebp, esi, (cookie store), ... push edi scheduled after the first fld (0x00B9AC5A).
  Pops edi, esi, ebp, ebx. Frame: 0xC4 locals + 0x10 saved.
- /GS: `mov eax,[0x0139C8A8]` (__security_cookie) stored at final [esp+0xD0]; checked by `__security_check_cookie`
  0x00BFE9F9 at 0x00B9BCA7 (ecx = [esp+0xD0]). Cookie because of the two local arrays (clip list 0x40 B, wind delays 0x20 B).
- NO SEH (no fs:0, no EH states) even though an inline-dtor object (CRenderStateScopedCaptureBlock) and
  CScopedVertexShaderConstantLayout are live across calls => TU built WITHOUT /EHsc: `// Flags: none` (same as the landed
  sibling 0x00BBC460 ParticleGroup::RenderSubPrimitive and the matched 2059 row 0x00BC25F0, both "Flags: none").
- Speed-optimised (loop alignment pads `mov edi,edi` 0x00B9AC6E / `lea esp,[esp]` 0x00B9AD1C / `lea ebx,[ebx]` 0x00B9ADAA,
  strength-reduced countdown loops, cmov-free min). No `optimize("s")` idioms. No pragma needed.
- x87 only (no SSE in this row; the SIMD work is in the out-of-line ClipSubSectionsSIMD). One double constant (0.015625 qword
  @0x012A2788) -- see camera constant below. No fistp => no ASM_RULES table needed.

## 2. Identity (two sources)
- Ego_r.pdb `00a531e0 ?RenderSubPrimitive@CEnginePrimitiveManagerRepeatedStaticMeshes@@UAEXAAVCEngineSubPrimitiveBase@@W4ERenderPassType@@@Z`
  (ghidra_out/egor_pdb_names.tsv; Ego_r body 0xA54290-0xA531E0 = 4272 B vs retail 4229 B).
- FableWin.pdb same mangled name, body 0x02ED0610 3700 B / 825 instr (naming_stragglers/FableWin.exe.signatures.tsv).
- vtable: rebuild/manifest/rtti-vtables.tsv CEnginePrimitiveManagerRepeatedStaticMeshes vtable 0x012A3E9C slot 5 = 0x00B9AC30
  (the only xref, .rdata 0x012A3EB0). Manifest label is RIGHT.
- Signature: `virtual void RenderSubPrimitive(CEngineSubPrimitiveBase& sub_primitive, ERenderPassType pass);` (public virtual,
  non-const, returns void). Mangled leaf: `?RenderSubPrimitive@CEnginePrimitiveManagerRepeatedStaticMeshes@@UAEXAAVCEngineSubPrimitiveBase@@W4ERenderPassType@@@Z`.
- FableWin COMDAT run after the body: only `?GetParent@CEngineSubPrimitiveRepeatedStaticMesh@@QAEPAVCEngineInternalPrimitiveRepeatedStaticMesh@@XZ`
  (19 B, first used here) -> write `CEngineInternalPrimitiveRepeatedStaticMesh* primitive = sub.GetParent();` (Parent +0x0C, cast).
  All other inline helpers were first emitted earlier in the TU / other TUs.

## 3. Callees (retail address -> landed file, landed name, PDB name)
| addr | landed row | landed name | PDB / evidence | flag |
|---|---|---|---|---|
| 0x00B98E10 | NOT landed | (manifest ClipSubSectionsSIMD) | Ego_r ?ClipSubSectionsSIMD@CEnginePrimitiveManagerRepeatedStaticMeshes@@AAEXPBUCSubsectionElement@NEnginePrimitiveManagerRepeatedStaticMeshes@@KAAPAUCClipListElement@1@@Z | declare only (private) |
| 0x00B99710 | 00/b9/..._ClipSubSections_00b99710.cpp | ClipSubSections | ?ClipSubSections@..@@AAEXPBUCSubsectionElement@N..@@KM_NAAPAUCClipListElement@1@@Z (const CSubsectionElement*, unsigned long, float, bool, CClipListElement*&) | ok |
| 0x00B99300 | NOT landed | ProcessLightingSW | ?ProcessLightingSW@..@@AAEXAAVCEngineInternalPrimitiveRepeatedStaticMesh@@PBM11PBVC4DVector@@J_N@Z (prim&, const float* x3, const C4DVector*, long, bool) | declare |
| 0x00B99670 | 00/b9/..._UploadShaderConstants_00b99670.cpp | UploadShaderConstants | ?UploadShaderConstants@..@@AAEXAAVCEngineInternalPrimitiveRepeatedStaticMesh@@PBVC4DVector@@1J@Z (prim&, offsets, matrices, count) | landed uses a `_Methods` view struct |
| 0x00B990B0 | 00/b9/CEnginePrimitiveManagerRepeatedStaticMeshesWind_UpdateBoneMatrixForWindSheer_00b990b0.cpp | `CEnginePrimitiveManagerRepeatedStaticMeshes_Wind::UpdateBoneMatrixForWindSheer` | ?UpdateBoneMatrixForWindSheer@CEnginePrimitiveManagerRepeatedStaticMeshes@@AAEXAAVCEngineInternalPrimitiveRepeatedStaticMesh@@PAVC4DVector@@PBEJ@Z | landed CLASS spelling `_Wind` view != PDB class (NAME_FIXES candidate); declare on the real class |
| 0x00B905A0 | NOT landed | PrepareOcclusionTest | Ego_r ?PrepareOcclusionTest@CEnginePrimitiveManagerOcclusionBase@@IAEXAAVCEngineInternalPrimitiveOcclusionBase@@W4ERenderPassType@@AAK@Z (protected, base class) | ok |
| 0x00B8D090 | 00/b8/E2_E2_00b8d090.cpp | `C00b8d090::M` / manifest `Nop_ret0_00b8d090` | ?StartOcclusionTest@CEngineOcclusionManager@@QAEXXZ (Ego_r 0xA347A0, 1 B body; FableWin 0x02DC6520) | WRONG landed name (agent2060 NAME_FIXES already) |
| 0x00B8D0A0 | 00/b8/E2_E2_00b8d0a0.cpp | `C00b8d0a0::M(int)` / `Nop_ret4_00b8d0a0` | ?EndOcclusionTest@CEngineOcclusionManager@@QAEXK@Z (Ego_r 0xA347B0; `ret 4`) | WRONG landed name |
| 0x00B50510 | 00/b5/CEngineShadowRenderer_SetupShadowFuncRenderStates_00b50510.cpp | SetupShadowFuncRenderStates | ?SetupShadowFuncRenderStates@CEngineShadowRenderer@@QAEXJABUCColorBufferDepthPackParameters@1@@Z (the 2-arg overload) | ok |
| 0x00B46C80 | NOT landed | UpdateGlobalLightingStates | ?UpdateGlobalLightingStates@CEngineLightingManager@@QAEXXZ (header CEngineLightingManager.h:93) | ok |
| 0x009FDE80 | NOT landed | (manifest BeginLoading, CGraphicInfoFrame::CFrameData) | Ego_r 0xB583E0 ?BeginLoading@CFrameData@CGraphicInfoFrame@@QAEXKK@Z; CGraphicInfoFrame.h:47 | NOT 0x009FE620 (that is CGraphicInfoFrame::BeginLoading used by GetVolumeTexture) |
| 0x009FE380 | 00/9f/CGraphicInfoFrameCFrameData_CFrameData_009fe380.cpp | CFrameData ctor | ??0CFrameData@CGraphicInfoFrame@@QAE@PBV1@@Z (CGraphicInfoFrame.h:46) | ok |
| 0x00BFEA1A | 00/bf/global_operatornew_00bfea1a.cpp | operator new | `push 44h` = sizeof(CFrameData) | ok |
| 0x009FBFB0 / 0x009FBF90 | 00/9f/CResource_ListRemove / _ListInsert | CResource::ListRemove / ListInsert(CResource*, CResource*) | inside the inline CResource::Touch (x2) | ok |
| 0x00988020 | 00/98/CShaderRenderManager_ApplyVertexShader_00988020.cpp | ApplyVertexShader(CVertexShader*, bool) | manifest + header | ok |
| 0x00988140 | ..._ApplyPixelShader_00988140.cpp | ApplyPixelShader(CPixelShader*) | ok | ok |
| 0x00989B00 | ..._SetUserVertexShaderConstant_00989b00.cpp | SetUserVertexShaderConstant(ulong, float x4) | ok | ok |
| 0x0098B5E0 | ..._SetVertexShaderConstantLayout_0098b5e0.cpp | SetVertexShaderConstantLayout(long) | via CScopedVertexShaderConstantLayout (FableWin ??0/??1CScopedVertexShaderConstantLayout) | ok |
| 0x0098B930, 0x00988A20, 0x00989BF0, 0x00989640, 0x00989760, 0x009897C0, 0x0098A760, 0x00988290, 0x00988A50 | CShaderRenderManager_* rows | SetVSConstants / UpdatePixelShader / UpdatePixelShaderConstants / UpdateWorldSpaceFogTransform / UpdateAmbient / UpdateFogColour / UpdateLightGlobals / ResetWorldTransform / UpdateCombinedProjectionTransform | all inside the inline PrepareForRender (CShaderRenderManager.h:164) | ok |
| 0x00989700 | 00/98/CShaderRenderManager_UpdateAmbient_00989700.cpp | file + body spell `UpdateAmbient` | Ego_r ?UpdateCameraPos@CShaderRenderManager@@AAEXXZ; manifest = UpdateCameraPos; PrepareForRender calls UpdateCameraPos for (flags&4)&&(UpdateFlags&8) | WRONG landed spelling (NAME_FIXES) |
| 0x00A058C0 | 00/a0/CRenderStateManager_RealiseRenderState_00a058c0.cpp | RealiseRenderState | inside DrawBuffersIndexed (RenderManager +0x1D0) | ok |
| 0x00A05840 | 00/a0/CRenderStateManager_RestoreCaptureBlock_00a05840.cpp | RestoreCaptureBlock | ~CRenderStateScopedCaptureBlock | ok |
| 0x00BFE9F9 | __security_check_cookie | /GS | ok |
- D3D9 vtable slots: SetTexture +0x104 (AttachD3DTextureToStage / FlushRender), SetStreamSource +0x190 (SelectVertexBuffer /
  FlushRender's SelectVertexBuffer(0,0)), SetIndices +0x1A0, DrawIndexedPrimitive +0x148.
- Landed 0x00B9AB30 "CreatePrimitive" (ret 8, hidden result + desc) is probably ?AddRepeatedStaticMesh (not proven; not called here).

## 4. Members read / written (retail offsets; header; PDB name)
`this` = CEnginePrimitiveManagerRepeatedStaticMeshes (engine/CEnginePrimitiveManagerRepeatedStaticMeshes.h, generated flat,
NO opt-in macros, NO base class, no RenderSubPrimitive / helper declarations -> an overlay opt-in is needed, e.g.
FABLE_RETAIL_CENGINEPRIMITIVEMANAGERREPEATEDSTATICMESHES: real base CEnginePrimitiveManagerOcclusionBase (PrepareOcclusionTest
is a protected base member called with ecx=this) + private method declarations; see the `_Methods`-view pattern of the landed
UploadShaderConstants for the minimal alternative):
- +0x20 PixelShader (CPixelShader), +0x28 VertexShader, +0x30 PixelShaderStippleAlpha, +0x38 VertexShaderStippleAlpha (by address),
  +0x80 PixelShaderShadowed*, +0x84 VertexShaderShadowed*, +0x88 PixelShaderShadowedStippleAlpha*, +0x8C VertexShaderShadowedStippleAlpha*,
  +0x90 CSIMDVariableBlock* SIMDVariableBlock (re-read at every store: aliasing).
- CSIMDVariableBlock (Ego_r = retail, 0xC00; forward-declared only in the shared header; the landed CalcFrameConstantSIMDVars
  0x00B99860 has a local full struct): NormalX[32] +0, NormalY +0x80, NormalZ +0x100, ObjectMatricies C4DVector[32] +0x300,
  ObjectOffsets +0x500, FadeDistance float[4] +0xBF0.
- Sub-primitive (arg1, CEngineSubPrimitiveBase.h; opt-in FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_RENDERER_FRIEND only; 2045 overlay adds
  _PARENTBASE): RenderFlags +0x04 (bits 0x8 = stipple alpha, 0x200 = shadowed), Parent +0x0C, MeshPrimitive +0x18
  (CEngineSubPrimitiveMeshBase, C3DPrimitive2*). No CEngineSubPrimitiveRepeatedStaticMesh.h exists (GetParent inline needs it or an overlay).
- Primitive CEngineInternalPrimitiveRepeatedStaticMesh (engine/CEngineInternalPrimitiveRepeatedStaticMesh.h, GENERATED, no opt-ins,
  Ego_r offsets = retail): FadeDistance.MaxDrawDistance +0x18 (CEngineInternalPrimitiveBase FadeDistance +0x14 {FadeBeginDistance,
  MaxDrawDistance}), AlphaRef +0x28 (uchar), MeshInfo +0x50 (C3DMeshInfo*), ObjectCount +0x54 (long), VectorNormals +0x8C (float*),
  ObjectMatricies +0x90 (C4DVector*), ObjectOffsets +0x94, SubSections +0x98 (CSubsectionElement*), WindDelayArray +0x9C (uchar*),
  RenderInfo +0xA4 -> RenderInfo.Mesh +0xA8 (C3DMesh2*; agent2060 PeekMesh model).
- C3DMesh2 (C3DMesh2.h; retail Materials +0x8C = Ego_r 0x94 - 8): GetMaterial(index) {&Materials[index]} (0x30 stride). Opt-in for
  GetMaterial in 2059 overlay: FABLE_RETAIL_C3DMESH2_PRIMITIVES (repo C3DMesh2.h:28 already has GetMaterial const/non-const).
- C3DMeshMaterial (0x30): DiffuseMapBankIndex +0x10, TwoSided +0x28 (bool). 2059 opt-in FABLE_RETAIL_C3DMESHMATERIAL_PARTICLEMESH.
- C3DPrimitive2 (retail = Ego_r - 4): MaterialIndex +0x04, VertexBuffer +0x30 (scoped_ptr<CVertexBufferWin32>), IndexBuffer +0x34,
  VertexCount +0x58, TriangleCount +0x5C. Opt-ins: FABLE_RETAIL_C3DPRIMITIVE2_INDEXBUFFER, _SCOPED_PTR (repo), 2059: _PARTICLEMESH.
- C3DMeshInfo -> PeekTextureBank() (C3DMeshInfo.h:59, opt-in FABLE_RETAIL_CMESHDATABANK_TEXTUREBANK): MeshInfo+4 (Bank) ->+0x3D4.
- CGraphicDataBank GraphicList vector<CGraphicInfo> +0x1E0/+0x1E4 (0x2C stride: magic 0x2E8BA2E9 sar 3), CGraphicInfo::Frames +0x28
  (CGraphicInfoFrame*). Pattern `bank->GetGraphicInfo(i) ? bank->GetGraphicInfo(i)->Frames : 0` = landed GetBumpTexture 0x00BA95D0
  (local SVector / GetGraphicInfo text); FableWin inline ?PeekGraphicInfoFrame@CGraphicDataBank@@QBEPBVCGraphicInfoFrame@@KK@Z.
- CGraphicInfoFrame (CGraphicInfoFrame.h, opt-in FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS (+ _CGRAPHICINFOFRAME_FREE)): Data +0x08
  (CIVCountedPointer<CFrameData>: refcount +4, release = virtual slot 1 [vtbl+4]), GraphicInfo +0x0C -> (+0x20 Bank) -> +0x194 CurrentFrame.
  CFrameData: CResource base (ResourceList +0x08, LastUsedFrame +0x18), Frame +0x1C (CGraphicFrame -> FrameData -> PTexture.Data +0x08),
  ManagedTexture +0x20 (resource ptr +0x24), CurrentMipmapLevel +0x38, TouchedMipmapLevel +0x3A, TouchedFrame +0x3C.
- CRenderManagerCore (GPRenderManager 0x01436E18; FABLE_RETAIL_CRENDERMANAGERCORE + _FORCEINLINE_HELPERS): DeviceState at +0x2818
  (SpecularEnable +0x29E4, PolyCullMode +0x2890, AlphaTestEnable +0x28CC, AlphaRef +0x28E0, TextureFilterModeMin[st] +0x30C4+20*st,
  Mag +0x3164, UVModeU +0x2EE4, UVModeV +0x2F84), StateStack/size +0x2808, dirty list +0x2008/+0x280C, CurrentBookmarkMask +0x2814,
  TextureStagesInUse +0x3CB8, PD3DDevice +0x3CF0, CurrentlySelectedTextures +0x3D00. BookmarkDummyState +0x3A24 (StartCaptureBlock).
- CShaderRenderManager (GPShaderRenderManager 0x01436E14; singleton 0x013BC470): CurrentVSConstantLayoutIndex +0x08, RenderManager +0x1D0,
  D3DDevice +0x1D4, WorldMatrixIsIdentity +0x1E8, UpdateFlags +0x3D8; layout->ReservedPresets (+0x140 vector of 16 B).
- CEngineLightingManager (GPLightingManager 0x01436E9C; FABLE_RETAIL_CENGINELIGHTINGMANAGER + _MEMBERS for ShadowMode): FadedShadowDiffuseFactorEnabled
  +0xCE, CurrentThemeChannel +0xE0..E2, StateUpdateFlags +0xE4, SelfIlluminationAmbientPercentage +0xE8, ShadowMode +0x4694.
- CEngineCamera (0x01436EA0): CameraDesc.Window (C2DBoxF) +0x14..+0x20 (TL.X, TL.Y, BR.X, BR.Y).
- Resource manager 0x01436E98 (CEngineResourceManager; landed spellings vary: g_pEngine_01436e98 / g_pEngineData / g_pResourceManager):
  StipleFadeTexture (CTexture, PD3DTexture +0) +0x2AAC (Ego_r 0x39E4; offset arithmetic, probable), VSConstantLayoutRepeatedMesh
  .ObjectMatricies (CShaderConstantRange) Offset +0x3030 / Count +0x3034, .ObjectOffsets.Offset +0x303C (landed UploadShaderConstants reads +0x3030/+0x303C).
- g_pEnginePrimitiveRenderer 0x01436E80 +0x88 = fade-distance scale (landed UploadShaderConstants, GetFadeDistance 0x00B98DA0).
- g_SSEAvailable 0x013D2880 (bool), occlusion manager 0x01436E5C, shadow renderer 0x01436E60, CRenderStateCaptureBlock::StateManager 0x013CB508.
- Local types: CClipListElement (2 bytes: StartIndex, Count) -- NEnginePrimitiveManagerRepeatedStaticMeshes::CClipListElement (nested
  `@1` in the mangled ClipSubSections = nested in the class? mangling `PAUCClipListElement@1@` back-refs NEnginePrimitiveManagerRepeatedStaticMeshes);
  CColorBufferDepthPackParameters {DepthToColourLookupTextureStage, LowByteCarryPixelShaderConstant, LowRepeatFactorUserVertexShaderConstant}.

## 5. Callee-body visibility
- Same TU (FableWin order: ... ProcessLightingSW, UploadShaderConstants, ClipSubSections, CalcFrameConstantSIMDVars,
  ClipSubSectionsSIMD, **RenderSubPrimitive**, GetParent, RenderPrimitive, ...): all five private helpers are defined BEFORE this row
  in the same TU and are still called out of line (not declared inline; /Ob1). Declare them, do not define them.
- Inline expansions (no retail copy): CRenderStateScopedCaptureBlock ctor/dtor (engine/CRenderStateCaptureBlock.h), SetSpecularEnable /
  SetTextureFilterModeMin / Mag / SetTextureUVModeU / V / SetPolyCullMode / SetAlphaTestEnable / SetAlphaRef (CRenderStateManager.h,
  FABLE_RETAIL_CRENDERSTATEMANAGER), AttachTextureToStage (CRenderManagerCore.h, needs FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS --
  without it the TU calls the copy 0x009A0CF0), CScopedVertexShaderConstantLayout (engine/CScopedVertexShaderConstantLayout.h),
  PrepareForRender (CShaderRenderManager.h:164), SelectVertexBuffer(CVertexBufferWin32&) (opt-in FABLE_RETAIL_CSHADERRENDERMANAGER_SELECTVERTEXBUFFER
  in the 2059 overlay /opt/fable_lane/w22/agent2059/hdr/engine/CShaderRenderManager.h:102), DrawBuffersIndexed 6-arg (FABLE_RETAIL_CSHADERRENDERMANAGER_INLINE_BLOCKDRAW),
  CRenderManagerCore::FlushRender (no shared body: landed 0x00BA1B00 defines it locally FABLE_FORCEINLINE), SetEnableFadedShadowFactor /
  RestoreDefaultPrimitiveParameters (CEngineLightingManager.h, FABLE_RETAIL_CENGINELIGHTINGMANAGER), CEngineOcclusionManager Start/EndOcclusionTest
  are OUT of line (empty stubs), CResource::Touch (landed GetBumpTexture 0x00BA95D0 local FABLE_FORCEINLINE body), CGraphicInfoFrame::GetTexture
  (bool, const CTexture** out, mip, flags) -- FableWin 470 B; no shared bool body (GetBumpTexture has a pointer-returning per-row variant).
- No STLport container calls out of line here.

## 6. Siblings to copy idioms from
- rebuild/src/compiled/00/bb/CEnginePrimitiveManagerParticleGroup_RenderSubPrimitive_00bbc460.cpp -- Flags: none; includes
  CRenderManager.h, CRenderStateCaptureBlock.h, CShaderRenderManager.h, CScopedVertexShaderConstantLayout.h, CEngineLightingManager.h;
  `#define FABLE_RETAIL_CENGINELIGHTINGMANAGER`, `FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS`; globals
  `namespace NEngineInternal { extern CRenderManager* GPRenderManager; extern CShaderRenderManager* GPShaderRenderManager; extern CEngineLightingManager* GPLightingManager; }`;
  `CRenderStateScopedCaptureBlock capture; ... CScopedVertexShaderConstantLayout layout(5);`
- /opt/fable_lane/w22/agent2059/src/00bc25f0.cpp (ParticleMeshGroup::RenderSubPrimitive, RELOCATION_MATCH, not landed yet): Flags none,
  full define list (FABLE_RETAIL_DEF_CLASS_BASE ... _CSHADERRENDERMANAGER_SELECTVERTEXBUFFER, _CMESHDATABANK_TEXTUREBANK, _C3DMESH2_PRIMITIVES,
  _STATICBLOCKS_TEXTURE_HELPERS, _CRENDERMANAGERCORE(_FORCEINLINE_HELPERS)), overlay /opt/fable_lane/w22/agent2059/hdr.
- rebuild/src/compiled/00/ba/CEnginePrimitiveManagerMeshBase_GetBumpTexture_00ba95d0.cpp -- the frame lookup + inline GetTexture /
  CResource::Touch text (speed TU, no pragma).
- rebuild/src/compiled/00/ba/CEnginePrimitiveManagerStaticMeshesBase_DisplayMirroredTextures_00ba1b00.cpp -- FlushRender body.
- Same class: rebuild/src/compiled/00/b9/CEnginePrimitiveManagerRepeatedStaticMeshes_UploadShaderConstants_00b99670.cpp (method-view
  struct over the shared header; g_pEngine_01436e98 register reads), _ClipSubSections_00b99710.cpp, _CalcFrameConstantSIMDVars_00b99860.cpp
  (local CSIMDVariableBlock). No pragmas in any of them.
- agent2060 NOTES (StaticMeshesBase::RenderMeshPrimitive): same tail (PrepareOcclusionTest / Start / End / FlushRender /
  RestoreDefaultPrimitiveParameters) and frame-packer lessons.

## 7. Names to hand back (NAME_FIXES candidates)
- 0x00B8D090 = CEngineOcclusionManager::StartOcclusionTest, 0x00B8D0A0 = CEngineOcclusionManager::EndOcclusionTest(unsigned long) (landed E2 stubs).
- 0x00989700 = CShaderRenderManager::UpdateCameraPos (landed file/body say UpdateAmbient; 0x00989760 is the real UpdateAmbient).
- 0x00B990B0 class spelling `CEnginePrimitiveManagerRepeatedStaticMeshes_Wind` (view) vs PDB CEnginePrimitiveManagerRepeatedStaticMeshes.
