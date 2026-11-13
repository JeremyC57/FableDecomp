# hdr overlay for 0x00B783F0 CEngineWaterRenderer::Render (full copies of the current repo headers + opt-ins)
- engine/CEngineWaterRenderer.h: FABLE_RETAIL_CENGINEWATERRENDERER_RENDER -- decls RenderBackground/RenderIceBackground/RenderSeaNonReflectiveForeground/CreateDynamicTextures (Ego_r) + FORCEINLINE SetCommonRenderStates body (FableWin name, no retail copy).
- engine/CRenderStateManager.h: FABLE_RETAIL_CRENDERSTATEMANAGER_BUMPENVMATRIX_INLINE -- FORCEINLINE SetTextureBumpEnvMatrix(stage, mat00, mat10, mat01, mat11), states in order 00,10,01,11 (no retail copy).
- engine/CRenderStateCaptureBlock.h: FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_INLINE -- inline ctor/dtor/Capture/Restore (no retail copy).
- engine/CScopedShadersEnable.h: FABLE_RETAIL_CSCOPEDSHADERSENABLE_FORCEINLINE -- FORCEINLINE ctor/dtor (plain inline runs out of /Ob2 budget in this 9 KB row: dtor called out of line).
- engine/CEngineSkyRenderer.h: FABLE_RETAIL_CENGINESKYRENDERER_GETENVIRONMENTMAP -- GetEnvironmentMap(CTexture*, const CEngineComponentRenderParams&) 0x00B665E0 decl.
- engine/CTexture.h: FABLE_RETAIL_CTEXTURE_GENERATEMIPMAPSWITHGPU -- GenerateMipmapsWithGPU() 0x009FAF70 decl.
