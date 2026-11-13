## 0x00BCFF10 CEnginePrimitiveManagerAnimatedMeshesBase::CEnginePrimitiveManagerAnimatedMeshesBase -- RELOCATION_MATCH (first compiling grade)
- What made it match: the scout's OUTLINE body as written (61 GetVertexShader statements, two bank-pointer locals declared bank 3 then bank 4,
  PointLightShaderIndex in index order, selectors in retail store order), on the shared headers. No flags (no /EHsc).
- Headers: overlay hdr/engine/CEnginePrimitiveManagerAnimatedMeshesBase.h (NEW, agent4010) + CDisplayManager.h opt-in
  FABLE_RETAIL_CDISPLAYMANAGER_PIXELSHADERVERSION; defines STATICBLOCKS_TEXTURE_HELPERS, CVERTEXSHADER, CENGINERESOURCEMANAGER,
  CCHARSTRING, CARRAY, CDISPLAYMANAGER, SCOPED_PTR; includes engine/Direct3D9Types.h before CDisplayManager.h (HWND / D3DCAPS9).
- Name fixes (NAME_FIXES.tsv, each proven by ghidra_out/egor_pdb_names.tsv + ghidra_out/fablewin_pdb_names.tsv):
  0x00A5D5F0 landed as GetPixelShader is ?GetVertexShader@CShaderResource@@QBE?AVCVertexShader@@ABVCCharString@@@Z (header pin; GetPixelShader is 0x00A5D720);
  0x009BEE00 is ?GetPixelShaderVersion@CDisplayManager@@QBEKXZ (Ego_r 00b23a80, FableWin 03022f70): landed caller 0x00B99D90 spells it CalculateTextureMemoryFromQuality;
  0x00BEBC30 is ??0CEnginePrimitiveManagerMeshBase@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z (Ego_r 00aa3e30, FableWin 02ee47a0; parameter CArray<CEngineScene *>&,
  also engine/CEnginePrimitiveManagerMeshBase.h line 13): landed caller 0x00B99D90 spells it CEnginePrimitiveGroupManager(CEngineSceneArray*), landed row 0x00BEBC30 types it (void*).
- Globals: 0x01436E98 is NEngineInternal::GPResourceManager (CEngineResourceManager*), the spelling of the newest label file
  ghidra_out/labels_landing_2026-11-12ef.tsv and of landed rows 0x00BBCC70 / 0x00BADB40 / 0x00B2D9F0 (repair 2026-11-13: replaced the
  superseded descriptive g_pResourceManager from labels_chunks718_725_2026-11-03.tsv; still RELOCATION_MATCH, also strict headers).
  0x01436E1C is NEngineInternal::GPDisplayManager.
