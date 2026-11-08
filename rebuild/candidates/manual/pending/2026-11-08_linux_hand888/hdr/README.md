# Chunk 887 header overlay (hdr/)

All changes are opt-in (`#ifdef FABLE_RETAIL_*`); the default path of every header is byte-for-byte the repo copy.

| Header | Opt-in | What it adds | Evidence |
|---|---|---|---|
| `engine/CMatrix3x4.h` | `FABLE_RETAIL_CMATRIX3X4_MULVECTOR` | inline `C3DVector operator*(const C3DVector&) const` (transform a point), each row summed `E1k*x+(E2k*y+E3k*z)+E4k` | FableWin.pdb `??DCMatrix3x4@@QBE?AVC3DVector@@ABV1@@Z`; retail 0x00BA62E3..0x00BA6353 loads x/y/z once and sums E3k*z, E2k*y, E1k*x, E4k |
| `engine/CCountedPointer.h` | `FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_DTOR` | declares `~CCountedPointer()` only (body = the out-of-line copy) | retail calls `??1?$CCountedPointer@$$CBVC3DMesh2@@@@QAE@XZ` 0x00A36EC0 at 0x00BA64AB and 0x00BA6CCF instead of expanding it |
| `engine/CTexture.h` | `FABLE_RETAIL_CTEXTURE_FORMATQUERIES` (inside `FABLE_RETAIL_CTEXTURE`) | declarations `CPixelFormat GetPixelFormat() const` 0x009F9D80, `unsigned long GetWidth(unsigned long) const` 0x004372A0, `unsigned long GetHeight(unsigned long) const` 0x004372C0 | FableWin.pdb `?GetPixelFormat@CTexture@@QBE?AVCPixelFormat@@XZ`, `?GetWidth@CTexture@@QBEKK@Z`, `?GetHeight@CTexture@@QBEKK@Z`; all three are landed rows (CTexture_GetPixelFormat_009f9d80.cpp, CTexture_GetWidth_004372a0.cpp, CTexture_GetHeight_004372c0.cpp); called from 0x00BB0DBC / 0x00BB0DD9 / 0x00BB0E1A and 0x00BB0302.. in RenderClockPrim |
| `engine/CPixelFormat.h` | `FABLE_RETAIL_CPIXELFORMAT_ISLINEAR` (inside `FABLE_RETAIL_CPIXELFORMAT`) | inline `bool IsLinear() const { return GD3DFormats[FormatIndex].Linear; }` | FableWin.pdb `?IsLinear@CPixelFormat@@QBE_NXZ`; retail reads byte 0x0129BA60 = GD3DFormats 0x0129BA40 + 0x20 (`Linear`, CD3DFormatInfo.h) at 0x00BB0DC6 and 0x00BB030C |
| `engine/CRenderStateManager.h` | `FABLE_RETAIL_CRENDERSTATEMANAGER_BORDERCOLOUR` (inside `FABLE_RETAIL_CRENDERSTATEMANAGER`) | inline `SetTextureBorderColour(long stage, unsigned long colour)` = `SetRenderState(colour, DeviceState.TextureBorderColour[stage])` | Ego_r.pdb / FableWin.pdb `?SetTextureBorderColour@CRenderStateManager@@QAEXJK@Z`; no out-of-line copy in retail; 0x00BBB611 (RenderLandscapeDecal) expands it on +0x3704 = DeviceState +0xEEC, value 0 |
| `engine/CEngineLightingManager.h` | `FABLE_RETAIL_CENGINELIGHTINGMANAGER_LIGHTSINSPHEREFAST` (inside `FABLE_RETAIL_CENGINELIGHTINGMANAGER`) | declaration `CEngineLocalLightArray* GetLightsInSphereFast(const C3DBoundingSphere&, long, const CEnginePrimitiveThemeChannelDesc&)` 0x00B46B90 | Ego_r.pdb 0x009BC860 / FableWin.pdb 0x02CF1290 `?GetLightsInSphereFast@CEngineLightingManager@@QAEPAVCEngineLocalLightArray@@ABVC3DBoundingSphere@@JABVCEnginePrimitiveThemeChannelDesc@@@Z`; landed row CEngineLightingManager_GetLightsInSphereFast_00b46b90.cpp; called at 0x00BBB4E0 |

Retired during the chunk (not delivered): a `C3DVector` GetMagnitude / GetSquaredMagnitude Y,Z,X opt-in (no effect: member inlines
fix the product order; the row uses a named-local explicit sum instead), and a chunk-private `chunk887/sprite_common.h` (its models are
now inline in the two sprite differ sources).
