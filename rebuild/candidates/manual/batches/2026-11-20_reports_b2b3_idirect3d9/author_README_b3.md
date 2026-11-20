# b3 (reports re-port, Account B, 2026-11-20): IDirect3D9 part 2 -- 5 rows onto engine/IDirect3D9.h -- READY

Rows: 0x009BED10 GetCurrentWindowedSettings, 0x009BF600 GetClosestMode, 0x009C02D0 EnumerateDisplayModes, 0x009C05C0
EnumerateAntiAliasingModes, 0x009C0E50 Init (all CDisplayManager).

## Change per row (src.diff; every row keeps its line count)
The local IDirect3D9 model becomes `#include "engine/IDirect3D9.h"`, and local D3D types that collide with engine/Direct3D9Types.h are
removed. Where the SDK parameter types need it, the types are adapted:
- 0x009BED10: the local `_D3DFORMAT` enum line becomes the include; the local `D3DDISPLAYMODE` struct and the IDirect3D9 block go.
- 0x009BF600: the local inline `CPixelFormat::GetD3DFormat()` returns `_D3DFORMAT` (was unsigned long; it returns CD3DFormatInfo::D3DFormat).
- 0x009C02D0: the include goes above the local `CPixelFormatInfo` (which uses _D3DFORMAT). `CPixelFormatInfo::D3DFormat` and
  `GetD3DFormat()` become `_D3DFORMAT`, and the local `long d3dFormat` becomes `_D3DFORMAT d3dFormat`. Ego_d.pdb types this local
  `d3d_pixel_format` as _D3DFORMAT (0x22D7), and `mode_count` as long. The local D3DDISPLAYMODE struct goes; the header's is identical.
- 0x009C05C0: the include, `_D3DFORMAT` as above, and the header's full D3DCAPS9 (0x130) replaces the 8-byte local one, so the local
  CDisplayManager's following pad is respelled `_pad_18c[0x298 - 0x18C]`. All offsets are unchanged. The one statement change is
  `(D3DMULTISAMPLE_TYPE)type` in the CheckDeviceMultiSampleType call: d3d9.h types the parameter D3DMULTISAMPLE_TYPE, and C++ will
  not convert between enums. Ego_d has no local for it.
- 0x009C0E50: the local `D3DCAPS9 {char[0x130]}` and the v00..v13 vtable model become the header. `GetDeviceCaps(0, 1, ...)` /
  `(0, 2, ...)` become `D3DDEVTYPE_HAL` / `D3DDEVTYPE_REF` (d3d9.h enumerators 1 and 2: the same values, and an int does not convert to
  the enum).

## Grades (vland0.log non-strict, --recheck-landed)
All 5: RELOCATION_MATCH PASS, tests PASS. The baseline was the same. Strict: HEADER_POLICY for other pre-existing local models only:
- CDisplayManager in all 5
- CPixelFormat in 0x009BED10 / 0x009BF600 / 0x009C02D0 / 0x009C05C0
- C2DExtentsI in 0x009BED10 / 0x009BF600 / 0x009C02D0
- IDirect3DDevice9 in 0x009BF600
- CArray in 0x009C02D0 / 0x009C05C0
- CDisplayModeDesc in 0x009C02D0
- CRenderManagerCore / CRenderManager2D / CRenderManager in 0x009C0E50

## Names (NAME_FIXES.tsv)
0x009C0E50 binds 0x009F70E0 / 0x009F7330 as ??0 / ??1 CDynamicTextureList. They are the landed CVsyncCallbackTimer constructor and
destructor, and the +0x294 member it calls `PDynamicTextures` is Ego_r's VsyncCallbackTimer (a boost::scoped_ptr). Because the row
spells the class differently, the HEADER_POLICY scan does not flag it as a CVsyncCallbackTimer row. 0x009C02D0 / 0x009C05C0 still use
`g_PixelFormatTable` for ?GD3DFormats@@3QBVCD3DFormatInfo@@B, which is already queued (b12_b14_b16 NAME_FIXES line 10).
