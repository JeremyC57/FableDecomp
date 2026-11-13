## 0x00B9ECE0 CEnginePrimitiveManagerStaticMeshesBase ctor -- RELOCATION_MATCH (1st full draft, 3 grades incl. 1 SRC_FAIL + strict)
- Matched as written from OUTLINE: shared StaticMeshesBase retail branch, two bank locals (bank1 then bank2 declared) read once from
  NEngineInternal::GPResourceManager, 64 GetVertexShader assignments in retail block order, ps>=1400 gate, pixel shader re-reading the global, tail in retail store order.
- Headers/flags: no flags; macros FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS / CVERTEXSHADER / CENGINERESOURCEMANAGER / CCHARSTRING /
  CDISPLAYMANAGER_PIXELSHADERVERSION (new hdr opt-in, engine/CDisplayManager.h); includes engine/Direct3D9Types.h before CDisplayManager.h.
- Name fix (from DOSSIER, not re-proved here): landed 0x00BA1A70 models this callee as ctor(long); real parameter is CArray<CEngineScene*>&.
