## 0x00BCFF10 CEnginePrimitiveManagerAnimatedMeshesBase::CEnginePrimitiveManagerAnimatedMeshesBase -- RELOCATION_MATCH (first compiling grade)
- What made it match: the scout's OUTLINE body as written (61 GetVertexShader statements, two bank-pointer locals declared bank 3 then bank 4,
  PointLightShaderIndex in index order, selectors in retail store order), on the shared headers. No flags (no /EHsc).
- Headers: overlay hdr/engine/CEnginePrimitiveManagerAnimatedMeshesBase.h (NEW, agent4010) + CDisplayManager.h opt-in
  FABLE_RETAIL_CDISPLAYMANAGER_PIXELSHADERVERSION; defines STATICBLOCKS_TEXTURE_HELPERS, CVERTEXSHADER, CENGINERESOURCEMANAGER,
  CCHARSTRING, CARRAY, CDISPLAYMANAGER, SCOPED_PTR; includes engine/Direct3D9Types.h before CDisplayManager.h (HWND / D3DCAPS9).
- Name fixes (from the DOSSIER, not re-proven here): landed 0x00A5D5F0 spelled GetPixelShader is CShaderResource::GetVertexShader;
  landed 0x00BEBC30 base ctor parameter is CArray<CEngineScene*>& (not void*).
