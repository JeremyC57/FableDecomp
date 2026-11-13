# bwf1 row 0x00BB3E80 header overlay (FABLE_HDR_OVERLAY=/opt/fable_lane/w22/bwf1/00bb3e80/hdr)
Start: agent2060's overlay (StaticMeshesBase::RenderMeshPrimitive), re-based onto the CURRENT repo headers (C3DPrimitive2, C3DVector,
CIVCountedPointer, CEnginePrimitiveRenderUtil, CGraphicInfo rebuilt as repo copy + the 2060 blocks; stale CRenderStateManager replaced).
Unused 2060 copies (C3DTextureStages, CEngineInternalPrimitiveStaticMesh, CEnginePrimitiveManagerStaticMeshesBase/OcclusionBase,
CEngineSubPrimitiveStaticMesh, CRenderManagerCore, CRenderStateCaptureBlock, CEngineInternalPrimitiveMorphedStaticMesh new header) kept as is.
One line per opt-in added or re-based here (default paths unchanged):
- C3DPrimitive2.h FABLE_RETAIL_C3DPRIMITIVE2_RENDER: re-based (BoundingSphere typed C3DBoundingSphere + GetMaterialIndex/GetBoundingSphere/GetAverageTextureStretch; VertexBuffer left raw so it composes with _INDEXBUFFER).
- C3DVector.h _NAMED_RESULT_SUB / _MAGNITUDE_NAMED_SQUARED: 2060 blocks re-based as #if/#elif heads of the repo chains.
- CIVCountedPointer.h _GET, CGraphicInfo.h _GETWIDTH, CEnginePrimitiveRenderUtil.h _MESHRENDER: 2060 blocks re-based.
- CEnginePrimitiveRenderUtil.h FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_CLOTHRENDER: outline set-ups 0x00BEFD90 / 0x00BF01E0 / 0x00BF2240 / SetupOutlineStencilStates 0x00BF0740 / SetupOutlineFillStates 0x00BF0B20, static GetCurrentOcclusionLayer / GetOcclusionEnable (FableWin names).
- CEnginePrimitiveRendererSharedData.h _CLOTH (nested in _MEMBERS): CurrentOcclusionLayer +0x78, OcclusionEnable +0xBD (+ accessors), PixelShaderOutlineSmoothFill(+Alpha) +0x1FC/+0x204, PixelShaderDiffuseFogShadow(+OutputFactor)* +0x224/+0x228 (Ego_r offsets).
- CEngineOcclusionManager.h FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_ALLOCATE (default flattened branch): COccluderEntry (Ego_r), FABLE_FORCEINLINE AllocateOccluderHandle (landed 0x00B91140 text), Start/EndOcclusionTest decls.
- CEnginePrimitiveOcclusionInfo.h FABLE_RETAIL_CENGINEPRIMITIVEOCCLUSIONINFO_ADDOCCLUSIONTEST: AddOcclusionTest decl (body in row) + private ClearHandles (FableWin 13i; Ego_r copy 0x009D01F0); members public.
- CEngineShadowRenderer.h FABLE_RETAIL_CENGINESHADOWRENDERER_SHADOWFUNC (default branch): CColorBufferDepthPackParameters (Ego_r) + SetupShadowFuncRenderStates decl 0x00B4FC40.
- CEngineLightingManager.h FABLE_RETAIL_CENGINELIGHTINGMANAGER_SPOTLIGHT: SetupShaderSpotLight decl 0x00B47390 + CEngineLocalLightArray::GetCount (FableWin 9i SAME); _FADED_ENABLE_FIRST (diagnostic, did not move).
- C3DMeshMaterial.h FABLE_RETAIL_C3DMESHMATERIAL_FLAGS: GetSelfIllumination / IsTwoSided / IsTransparent (FableWin 9i SAME each).
- CRGBColour.h FABLE_RETAIL_CRGBCOLOUR_CTOR_INITIALISE (FableWin EEEE ctor 18i = Initialise call; did not move the row, unused in best).
- CRGBFloatColour.h FABLE_RETAIL_CRGBFLOATCOLOUR_GETRGBCOLOUR_NAMED_RESULT (used; + _NAMED, _RGB_THEN_A variants): channels into named bytes, R,G,B,A order, then built and returned (retail 0x00BB415C..0x00BB41A8). FITTED.
- CRenderStateManager.h FABLE_RETAIL_CRENDERSTATEMANAGER_SETRENDERSTATE_DESIRED: agent2048's text re-based onto the current header (used: 41 -> 37 hunks).
- CEngineSubPrimitiveBase.h FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_ALPHAASFLOAT: FableWin GetAlphaAsFloat (diagnostic, worse; unused).
