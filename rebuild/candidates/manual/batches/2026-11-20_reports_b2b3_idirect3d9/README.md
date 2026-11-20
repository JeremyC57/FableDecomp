# Re-port b2 + b3: thirteen landed CDisplayManager rows moved onto engine/IDirect3D9.h (Account B, 2026-11-20)

These rows were newly flagged when hand3c2 b14 added IDirect3D9.h (owner decision 2026-11-20: land first, re-port afterwards). Their row-local
IDirect3D9 interface models, and the D3D structs and enums that collided with the header's SDK types, are replaced by
`#include "engine/IDirect3D9.h"`. Removed lines are left blank so every row keeps its line numbers.

- **Rows:**
  - b2: 0x009BE590, 0x009BE610, 0x009BE6C0, 0x009BE750, 0x009BE830, 0x009BE8B0, 0x009BE930, 0x009BEBB0;
  - b3: 0x009BED10, 0x009BF600, 0x009C02D0, 0x009C05C0, 0x009C0E50.
- **Grades:** all thirteen are RELOCATION_MATCH with their tests passing, as landed. They were landed with vland `--land` NON-STRICT, because
  strict stops every row at HEADER_POLICY for OTHER pre-existing local models (CDisplayManager, CPixelFormat, and in some rows C2DExtentsI,
  CArray, CDisplayModeDesc, IDirect3DDevice9, CRenderManager*). No IDirect3D9 warning is left.
- **Type changes forced by the header's SDK enums** (all byte-neutral; detail in author_README_b2.md / author_README_b3.md):
  - local-model member and helper types: DeviceType becomes D3DDEVTYPE; CurrentModeFormat, GetD3DFormat and CPixelFormatInfo::D3DFormat become
    _D3DFORMAT;
  - 0x009C02D0: the local `long d3dFormat` becomes `_D3DFORMAT`, as Ego_d types it;
  - 0x009C05C0: a `(D3DMULTISAMPLE_TYPE)type` cast;
  - 0x009C0E50: GetDeviceCaps(0, 1/2) becomes D3DDEVTYPE_HAL / D3DDEVTYPE_REF.
- **Header changes:** none.
- **Review:** checked by Account B's coordinator from the diffs, not by the rolling auditor.
- **Name fixes** (NAME_FIXES.tsv): 0x009C0E50 binds 0x009F70E0 / 0x009F7330 as CDynamicTextureList ctor / dtor. They are CVsyncCallbackTimer's,
  and the +0x294 member is VsyncCallbackTimer. That row is therefore also a hidden CVsyncCallbackTimer row.
- **Still held:** 0x009BE870 and 0x009BFB30, which Account A has claimed.
