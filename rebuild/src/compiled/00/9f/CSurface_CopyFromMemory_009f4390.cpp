// CSurface::CopyFromMemory(const C2DBoxI* destRect, const void* source, const C2DBoxI* sourceRect, const CPixelFormat& sourceFormat,
//   long sourcePitch, unsigned long colourKey) -- copy a block of pixels from memory into this surface: when exactly one side is a
//   signed format and CanDoProperSignedConversion allows it, through CopyFromUnsignedDataToSignedSurface; an equivalent format of the
//   same size through the direct four-argument CopyFromMemory overload; anything else through D3DXLoadSurfaceFromMemory.
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F4390, 507 bytes, `ret 18h`.
// Leaf: ?CopyFromMemory@CSurface@@QAEXPBVC2DBoxI@@PBX0ABVCPixelFormat@@JK@Z
// Name: Ego_r.pdb ?CopyFromMemory@CSurface@@QAEXPBVC2DBoxI@@PBX0ABVCPixelFormat@@JK@Z + engine/CSurface.h (CStaticFontBank ctor
//   0x00AB8E10 calls it with six arguments).
// Callees: IDirect3DSurface9::GetDesc (COM slot 12, `call [ecx+30h]`), CPixelFormat::Initialise(_D3DFORMAT) 0x009E3830 (the inline
//   CPixelFormat(_D3DFORMAT) constructor of each inline GetPixelFormat), CPixelFormat::IsSigned 0x009E3970, CPixelFormat::IsEquivalent
//   0x009E37F0, CSurface::CanDoProperSignedConversion 0x009F35D0, CSurface::CopyFromUnsignedDataToSignedSurface 0x009F36E0,
//   CSurface::CopyFromMemory(4 args) 0x009F3B00, D3DXLoadSurfaceFromMemory import thunk 0x00BFEFC8.
// Shape: the two extents are four scalars written as conditional expressions (`dstBox ? box width : GetWidth()` per field). VC7.1
//   merges each pair of tests into one, keeps the widths in the dead destRect / sourceRect argument slots (both boxes stay in ebx /
//   edi, the source format in ebp) and tail-merges the null-sourceRect arm into the height store of the box arm (0x009F44A3 jumps
//   back to 0x009F4409). An if/else spelling of the same extents gives the widths a register and loses ebp for the format (frame
//   0x38). GetPixelFormat / GetWidth / GetHeight are expanded in place (per-TU inline opt-ins); the two later GetPixelFormat
//   temporaries are built in the dead sourceFormat slot. The colourKey argument is passed as D3DX's filter, the colour key is 0.
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CPIXELFORMAT_SIGNED
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_COPYFROMMEMORY
#define FABLE_RETAIL_CSURFACE_COPYFROMMEMORY_BODY
#define FABLE_RETAIL_CSURFACE_BITMAP
#define FABLE_RETAIL_CSURFACE_DIMENSIONS_INLINE
#include <stddef.h>
#include "engine/Direct3DSurface9.h"
#include "engine/C2DExtentsI.h"
#include "engine/C2DBoxI.h"
#include "engine/CPixelFormat.h"
#include "engine/CSurface.h"

extern "C" long __stdcall D3DXLoadSurfaceFromMemory(IDirect3DSurface9* pDestSurface, const struct tagPALETTEENTRY* pDestPalette,
    const tagRECT* pDestRect, const void* pSrcMemory, _D3DFORMAT SrcFormat, unsigned int SrcPitch,
    const struct tagPALETTEENTRY* pSrcPalette, const tagRECT* pSrcRect, unsigned long Filter, unsigned long ColorKey);   // 0x00BFEFC8

void CSurface::CopyFromMemory(const C2DBoxI* destRect, const void* source, const C2DBoxI* sourceRect,
                              const CPixelFormat& sourceFormat, long sourcePitch, unsigned long colourKey)
{
    long destWidth = destRect ? destRect->BRX - destRect->TLX : GetWidth();
    long destHeight = destRect ? destRect->BRY - destRect->TLY : GetHeight();
    long sourceWidth = sourceRect ? sourceRect->BRX - sourceRect->TLX : destWidth;
    long sourceHeight = sourceRect ? sourceRect->BRY - sourceRect->TLY : destHeight;
    if ((GetPixelFormat().IsSigned() || sourceFormat.IsSigned())
        && !(GetPixelFormat().IsSigned() && sourceFormat.IsSigned())
        && CanDoProperSignedConversion(destRect, sourceRect, sourceFormat)) {
        CopyFromUnsignedDataToSignedSurface(source, sourceFormat, sourcePitch);
        return;
    }
    if (sourceFormat.IsEquivalent(GetPixelFormat()) && sourceWidth == destWidth && sourceHeight == destHeight) {
        CopyFromMemory(destRect, source, sourceRect, sourcePitch);
        return;
    }
    {
        tagRECT* pDest = 0;
        tagRECT* pSource = 0;
        tagRECT dest;
        tagRECT src;
        if (destRect) {
            dest.top = destRect->TLY; dest.bottom = destRect->BRY; dest.left = destRect->TLX; dest.right = destRect->BRX;
            pDest = &dest;
        }
        if (sourceRect) {
            src.top = sourceRect->TLY; src.bottom = sourceRect->BRY; src.left = sourceRect->TLX; src.right = sourceRect->BRX;
            pSource = &src;
        }
        D3DXLoadSurfaceFromMemory(PD3DSurface, 0, pDest, source, sourceFormat.GetAsD3DPixelFormat(), sourcePitch, 0, pSource,
                                  colourKey, 0);
    }
}