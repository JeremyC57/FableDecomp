## 0x00A04630 CRenderStateManager::SetupRenderStates -- RELOCATION_MATCH (grade 1, strict headers OK)
- Matched on the first full draft: the three private Setup* helpers (0x00A04580/5B0/5F0) defined with their real bodies in the row TU
  before SetupRenderStates (plain member functions, auto-inlined by /O2), retail call order from OUTLINE.md, 8 explicit WRAPn lines.
- Needs only `#define FABLE_RETAIL_CRENDERSTATEMANAGER` (no overlay, no flags); the d3d9types enumerators (_D3DRENDERSTATETYPE /
  _D3DTEXTURESTAGESTATETYPE / _D3DSAMPLERSTATETYPE, SDK values) and D3DPRASTERCAPS_* macros are defined locally (headers forward-declare the enums).
- No landed names proven wrong.
