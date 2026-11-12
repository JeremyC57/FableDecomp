// CSurface::CopyFromMemory(const C2DBoxI* destRect, const void* source, const C2DBoxI* sourceRect, long sourcePitch) -- copy a
//   linear buffer (or a box of it) into the surface: one block copy when both boxes cover the whole surface and the pitches agree,
//   else row by row at the surface's bits per pixel. A missing destRect defaults to the source box's size at the origin (else the
//   whole surface), a missing sourceRect to the destination's size; a zero pitch means the locked pitch. Rows are offset by
//   sourceRect's TLY on both sides (destRect's TLY is never read: original behaviour, kept).
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F3B00, 664 bytes, `ret 10h`.
// Leaf: ?CopyFromMemory@CSurface@@QAEXPBVC2DBoxI@@PBX0J@Z
// Name: Ego_r.pdb 0x00B477F0 + FableWin.pdb ?CopyFromMemory@CSurface@@QAEXPBVC2DBoxI@@PBX0J@Z; called by the six-argument
//   CopyFromMemory 0x009F4390.
// Callees: CSurface::Lock 0x009F33E0 (hidden CSurfaceLockedRect result), CSurface::GetByteLength 0x009F2FC0, CSurface::GetWidth
//   0x00415BD0 / GetHeight 0x00415BF0 (out-of-line COMDAT copies once the inline budget is spent), CPixelFormat::Initialise(_D3DFORMAT)
//   0x009E3830 (inline GetPixelFormat), IDirect3DSurface9::GetDesc (+0x30) / UnlockRect (+0x38, through the inline Unlock).
// Shape: the default boxes are C2DBoxI::Initialise calls, so the two inline GetDimensions of the both-null arm stay alive in one
//   full-expression (two desc slots, frame 0x9C); the row copy is an indexed loop that VC7.1 strength-reduces into two cursors and a
//   down-counter. Lock is defined earlier in retail's CSurface.cpp: its real body sits here out of line (auto_inline off).
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_COPYFROMMEMORY
#define FABLE_RETAIL_CSURFACE_COPYFROMMEMORY_BODY
#define FABLE_RETAIL_CSURFACE_BITMAP
#define FABLE_RETAIL_CSURFACE_DIMENSIONS_INLINE
#define FABLE_RETAIL_CSURFACE_LOCK
#include <stddef.h>
#include <string.h>
#include "engine/Direct3DSurface9.h"
#include "engine/C2DExtentsI.h"
#include "engine/C2DBoxI.h"
#include "engine/CPixelFormat.h"
#include "engine/CSurfaceLockedRect.h"
#include "engine/CSurface.h"

#pragma auto_inline(off)
CSurfaceLockedRect CSurface::Lock(unsigned long flags)
{
    C2DExtentsI dimensions = GetDimensions();
    tagRECT rect; rect.top = 0; rect.left = 0; rect.bottom = dimensions.Y; rect.right = dimensions.X;
    _D3DLOCKED_RECT locked;
    if (PD3DSurface->LockRect(&locked, &rect, flags) >= 0) {
        CSurfaceLockedRect result; result.Dimensions = dimensions; result.Pitch = locked.Pitch; result.PMem = (unsigned char*)locked.pBits; return result;
    }
    CSurfaceLockedRect result; result.Pitch = 0; result.PMem = 0; result.Dimensions = C2DExtentsI(0, 0); return result;
}
#pragma auto_inline(on)

void CSurface::CopyFromMemory(const C2DBoxI* destRect, const void* source, const C2DBoxI* sourceRect, long sourcePitch)
{
    C2DBoxI dst;
    if (destRect) {
        dst = *destRect;
    } else if (sourceRect) {
        dst.Initialise(0, 0, sourceRect->BRX - sourceRect->TLX, sourceRect->BRY - sourceRect->TLY);
    } else {
        dst.Initialise(0, 0, GetWidth(), GetHeight());
    }
    C2DBoxI src;
    if (sourceRect) {
        src = *sourceRect;
    } else {
        src.Initialise(0, 0, dst.BRX - dst.TLX, dst.BRY - dst.TLY);
    }
    CSurfaceLockedRect locked = Lock(0);
    if (!locked.PMem)
        return;
    if (sourcePitch == 0)
        sourcePitch = locked.Pitch;
    CPixelFormat format = GetPixelFormat();
    if (sourcePitch == locked.Pitch && src.TLX == 0 && src.TLY == 0 && src.BRX == (long)GetWidth() && src.BRY == (long)GetHeight()
        && dst.TLX == 0 && dst.TLY == 0 && dst.BRX == (long)GetWidth() && dst.BRY == (long)GetHeight()) {
        memcpy(locked.PMem, source, GetByteLength());
    } else {
        long bpp = (GetByteLength() * 8) / (GetWidth() * GetHeight());
        long rowBytes = ((src.BRX - src.TLX) * bpp) / 8;
        const unsigned char* from = (const unsigned char*)source + (bpp * src.TLX) / 8;
        unsigned char* to = locked.PMem + (bpp * dst.TLX) / 8;
        for (long y = src.TLY; y < src.BRY; ++y)
            memcpy(to + y * locked.Pitch, from + y * sourcePitch, rowBytes);
    }
    Unlock();
}