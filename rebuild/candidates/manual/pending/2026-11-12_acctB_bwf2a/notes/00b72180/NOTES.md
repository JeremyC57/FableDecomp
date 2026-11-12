# 0x00B72180 CEngineWaterRenderer::CreateDynamicTextures -- RELOCATION_MATCH (bwf2 author, first attempt, 26 grades)
- What made it match: (1) the whole function written from OUTLINE/DOSSIER on the 2048 RenderClouds model (FORCEINLINE PeekGraphicInfoFrame / GetFrame
  / ~CGraphicFrame) with SetRenderTargetToSurface as a FABLE_FORCEINLINE member defined in the TU (plain inline or a plain definition stays called);
  (2) the per-level loops as `while (level < N) { if (w < 32 || h < 32) break; ...; ++level; }` -- with `for` loops the c2 inline trace gives the
  success-path ~CGraphicFrame sites static frequency 7.24 and ??_GCGraphicFrameData is expanded there (5852 bytes), `while` keeps it a call at all ten
  sites (5587); (3) the old shader enables are FableWin's CScopedShadersEnable(true, true) (Ego_r members OldVertexEnable/OldPixelEnable = the
  adjacent bytes [esp+1Ch]/[esp+1Dh]), not two named bools -- that also fixed the five frames' slot order; its dtor FORCEINLINE (hdr opt-in).
- Headers / flags: FABLE_HDR_OVERLAY=$W/hdr (README there; new CRenderTarget.h; opt-ins CENGINEWATERRENDERER_CREATEDYNAMICTEXTURES,
  CGPUSCRATCHBUFFERTEXTUREMANAGER_STARTTEXTUREUSE, CTEXTURE_WATERDYNAMIC, CSCOPEDSHADERSENABLE_FORCEINLINE); no compiler flags, no pragma.
  The 2048-derived overlay copies are older than HEAD for 8 headers: re-base before landing.
- Names: EWaterArrayIndexAliases enumerators unknown (forward enum, values 0 / 1 cast); parameter names descriptive (render_water_bump,
  render_sea_bump, render_ice_bump); g_pEngineEnvironmentDef 0x01436E24 descriptive (landed chunk 727 spelling). NAME_FIXES.tsv: 0x00F36EE0, 0x00A10E20.
