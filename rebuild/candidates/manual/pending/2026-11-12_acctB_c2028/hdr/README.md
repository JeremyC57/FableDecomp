# Chunk 2028 header overlay (FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2028/hdr)

Every change is an opt-in (or a new header); the default text of every copied header is unchanged (one exception noted: the new
`#include "fable_compiler.h"` at the top of CIVCountedPointer.h). Copies taken 2026-11-11/12 from rebuild/include or the named hand-backs;
re-base on HEAD before landing. Row 0x00B8DF50 (RELOCATION_MATCH) uses the first block; the parked 0x00B9A4A0 the second.

| header | opt-in | what | evidence |
|---|---|---|---|
| engine/CEngineOcclusionManager.h | `FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_MEMBERS` | own members typed (Ego_r names, retail offsets; nested COccluderSlot / COccluderEntry / COccluderFlareEntry), FrameStart / FrameEnd virtuals, private SelectFlareTestVertexBuffer | struct_layouts_egor.tsv + fable_types.h; landed 0x00B8D280 / 0x00B8DBE0 offsets; Ego_r / FableWin names |
| engine/CEngineCamera.h (c2008 copy) | `FABLE_RETAIL_CENGINECAMERA_CLIPSPHERE` | FABLE_FORCEINLINE ClipSphere(const C3DVector&, float) const, PeekViewTransform, FABLE_FORCEINLINE TransformWorldToView(const&, &), PeekNonHomogenousViewToViewScaleX / Y | FableWin-only inlines; landed CEngine::ClipSphere 0x00B24150; 0x00B8E0F6.. / 0x00B8E2CB.. |
| engine/CEngineCameraDesc.h | (c2008 copy, unchanged) | | |
| engine/CMatrix3x4.h | `FABLE_RETAIL_CMATRIX3X4_SETTRANSLATEONLY_XYZ` (nested in _SETTRANSLATEONLY), `FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_COPY_SUMS` | SetTranslateOnly(float,float,float) + the vector form forwarding to it; FABLE_FORCEINLINE TransformPoint(const&, &) with Z-term-first rows | FableWin ?SetTranslateOnly@CMatrix3x4@@QAEXMMM@Z; 0x00B8E231..0x00B8E27B, 0x00B8E2CB..0x00B8E2EA |
| engine/CEngineStateBlocks.h | `FABLE_RETAIL_CENGINESTATEBLOCKS_APPLY_INLINE` | inline ApplyDiffuseOnlyStateBlock (DiffuseOnlyMode.Apply()) | FableWin-only; 0x00B8DFDB virtual call on +0x28 |
| engine/CRenderManagerCore.h | `FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASTRIFAN` | DrawCurrentBuffersAsTriFan(long, long) 0x00A0AE40 | Ego_r / FableWin; body DrawPrimitive type 6 |
| engine/CRenderStateCaptureBlock.h | (c2024 copy, unchanged) | scoped capture block | |
| engine/CEnginePrimitiveRenderUtil.h (c2011 copy) | `FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_MESHRENDER` | static GetCurrentOcclusionLayer / GetOcclusionEnable / GetLODErrorTolerance / GetLODVirtualScreenSizeFOVAdjusted, member CalculateSubPrimitiveRenderFlags (bodies in the row) | FableWin names; COMDAT 0x00B98CC0 |
| engine/CEnginePrimitiveManager.h | `FABLE_RETAIL_CENGINEPRIMITIVEMANAGER_BLOCKONLOAD` | static GetBlockOnLoad() | FableWin; COMDAT 0x00B98C10 |
| engine/CEnginePrimitiveRendererSharedData.h | `FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_RENDERSETTINGS` (new first branch) | Ego_r members to +0xC4 + inline getters | struct_layouts_egor.tsv; 0x00B9A4DB.. reads |
| engine/CEngineInternalPrimitiveBase.h | `FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ISPERSISTENT` | IsPersistent() | FableWin; 0x00B9A528 |
| engine/CEngineInternalPrimitiveOcclusionBase.h | `_ISOCCLUDED`, `_SETUP_FORCEINLINE` | HasOcclusionInfo inline, IsOccluded declared (row body); SetupOcclusionInfo forced | FableWin; trace scores 138 / 80 |
| engine/CEnginePrimitiveOcclusionManager.h | `FABLE_RETAIL_CENGINEPRIMITIVEOCCLUSIONMANAGER_OCCLUDED` (with _SET) | GetOccludedFrames(info&) 0x00B2FAC0, inline IsPrimitiveOccluded | FableWin; 0x00B9A5A3 |
| engine/CIVCountedPointer.h | `FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_FORCEINLINE` (nested in _RESET_ADOPT) | FABLE_FORCEINLINE Reset | trace score 80; 0x00B9A564.. |
| engine/C3DMeshInfo.h (c2011 copy) | `FABLE_RETAIL_C3DMESHINFO_LODERROR` | GetLODError, BeginLoadingTextures(EBankLoadPriority) 0x00A23C50 | FableWin; Ego_r donor order |
| engine/CShadowClipVolume.h, CShadowRendererSceneData.h, CEngineShadowRenderer.h, CEngineLightingManager.h, CRenderTarget.h (c2024 copies) | `FABLE_RETAIL_CSHADOWCLIPVOLUME_CLIPBOUNDINGBOX`, `FABLE_RETAIL_CSHADOWRENDERERSCENEDATA_COPY`, `FABLE_RETAIL_CENGINESHADOWRENDERER_PEEKSCENEDATA` | FABLE_FORCEINLINE ClipBoundingBox (landed 0x00B9C2E0 body), declared ctor / copy ctor 0x00B38600, PeekSceneData by value | Ego_r / FableWin; 0x00B9A71D..0x00B9A946 |
| engine/CEngineInternalPrimitiveRepeatedStaticMesh.h | `FABLE_RETAIL_CENGINEINTERNALPRIMITIVEREPEATEDSTATICMESH` | the retail class over the occlusion base, Ego_r members, GetRenderInfo | RTTI chain (tools/rtti.py), Ego_r layout = retail |
| engine/CEnginePrimitiveManagerRepeatedStaticMeshes.h | `FABLE_RETAIL_CENGINEPRIMITIVEMANAGERREPEATEDSTATICMESHES` | the retail class over CEnginePrimitiveManagerMeshBase (STATICBLOCKS chain), typed shader members, RenderPrimitive | RTTI chain; landed ctor 0x00B99D90 |
| engine/CEngineRenderInfoMeshBase.h, CEngineSubPrimitiveBoundingBox.h, CEngineSubPrimitiveRepeatedStaticMesh.h (new) | default | render info (Lights / Mesh / LOD), bounding-box sub-primitive (inline ctor), mesh-base + repeated-mesh sub-primitive (ctor 0x00B98BC0) | fable_types.h layouts; RTTI vtables 0x012A3E84 / 0x012A3E94 |
