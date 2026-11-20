# b2 (reports re-port, Account B, 2026-11-20): IDirect3D9 part 1 -- 8 rows onto engine/IDirect3D9.h -- READY

Rows (2026-11-20_landing_hand3c2_b12_b14_b16/newly_flagged_rows.tsv): 0x009BE590, 0x009BE610, 0x009BE6C0, 0x009BE750, 0x009BE830,
0x009BE8B0, 0x009BE930, 0x009BEBB0. Skipped and HELD: 0x009BE870 GetBestCompressedAlphaTextureFormat (and 0x009BFB30 ResetDevice, a
later batch). Both are in Account A's NF136 claim, which is "handed-back", not "done".

## Change per row (src.diff = landed -> re-port; every row keeps its line count)
The local `struct IDirect3D9 { ... }` vtable model becomes `#include "engine/IDirect3D9.h"` at the same place, padded with blank lines.
The shared interface takes the SDK types (D3DDEVTYPE, D3DFORMAT), and C++ does not convert long to an enum. So the row-local
CDisplayManager / helper member types that feed the calls take the PDB types. The statements themselves are unchanged:
- 0x009BE590 / 0x009BE610 / 0x009BE6C0 / 0x009BE750 / 0x009BEBB0: the local CDisplayManager's `long DeviceType` (+0x5C,
  D3DDeviceCaps.DeviceType, D3DCAPS9 types it D3DDEVTYPE) -> `D3DDEVTYPE DeviceType`, and `long CurrentModeFormat` (+0x1C4,
  CurrentMode.Format) -> `D3DFORMAT CurrentModeFormat` (4 rows; 0x009BEBB0 passes mode.Format).
- 0x009BE930: the same two members (`D3DDeviceCaps_DeviceType`, `CurrentMode_Format`), plus the row's `static inline long
  GetD3DFormat(const CPixelFormat&)` returns `D3DFORMAT` (it returns CD3DFormatInfo::D3DFormat, a _D3DFORMAT).
- 0x009BE830 / 0x009BE8B0 (compact rows): the local `_D3DFORMAT` / `_D3DDEVTYPE` enums and the `_D3DDISPLAYMODE` / truncated `_D3DCAPS9`
  structs collide with engine/Direct3D9Types.h (included by IDirect3D9.h) and are removed. The header's `_D3DCAPS9` is the same 0x130
  bytes (DeviceType +0, AdapterOrdinal +4). `enum _D3DRESOURCETYPE{D3DRTYPE_TEXTURE=3}` stays local, because the header types RType
  as unsigned long.

## Grades (vland0.log non-strict, --recheck-landed)
All 8: RELOCATION_MATCH PASS, tests PASS. The baseline of the landed text was the same. Strict: all 8 stop at HEADER_POLICY for the
pre-existing local CDisplayManager and CPixelFormat models (not this backlog); no IDirect3D9 warning is left.

## Header: none (IDirect3D9.h as in the tree). Mutants: not re-run (no statement changed; the landed tests pass). Names: none new.
