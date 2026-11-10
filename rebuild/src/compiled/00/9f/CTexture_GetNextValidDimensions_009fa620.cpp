// CTexture::GetNextValidDimensions(const C2DExtentsI& size, const CPixelFormat& format) -- the texture size the device will accept: the requested size unchanged, or each side rounded up to the next power of two when the device needs power-of-two textures
// Inline-budget: /d2inlT180 expands GFGetTopBit / GFGetNextPowerOfTwo (plain `inline` in engine/GFGetNextPowerOfTwo.h) at both sides, as retail does; the default budget (and T160) leaves them called.
// Leaf: ?GetNextValidDimensions@CTexture@@SI?AVC2DExtentsI@@ABV2@ABVCPixelFormat@@@Z
// Flags: /d2inlT180
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FA620, 349 bytes, static __fastcall (ecx = hidden C2DExtentsI result, edx = the size, the
//   unused format on the stack), `ret 4`. Ego_r.pdb ?GetNextValidDimensions@CTexture@@SI?AVC2DExtentsI@@ABV2@ABVCPixelFormat@@@Z.
// Callees: GFGetSystemManager 0x009A4EC0; CSystemManager::GetDisplayManager and CDisplayManager::IsNeedingPower2Textures (FableWin.pdb,
//   inline: TextureCaps >> 1 & 1), GFGetNextPowerOfTwo / GFGetTopBit (FableWin.pdb; 0x0099FDB0 expanded twice).
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER_BUFFERTEXTURES
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_PIXELFORMAT
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE_NEXTVALIDDIMENSIONS
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_SCOPED_PTR
#include "engine/CPixelFormat.h"
#include "engine/CDisplayManager.h"
#include "engine/CSystemManager.h"
#include "engine/GFGetNextPowerOfTwo.h"

C2DExtentsI __fastcall CTexture::GetNextValidDimensions(const C2DExtentsI& size, const CPixelFormat& format)
{
    C2DExtentsI result = size;
    if (GFGetSystemManager()->GetDisplayManager()->IsNeedingPower2Textures()) {
        result.X = GFGetNextPowerOfTwo(result.X);
        result.Y = GFGetNextPowerOfTwo(result.Y);
    }
    return result;
}