// CSurface::Swizzle(unsigned long lockFlags) -- lock the whole surface (CSurface::Lock, the CSurfaceLockedRect returned through a frame temporary) and unlock it again; the PC build keeps only the lock/unlock bracket of the (console) swizzle pass
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F4330, length 39 bytes.
// Callees: CSurface::Lock(unsigned long) @ 0x009F33E0 (`__fastcall`, the 16-byte CSurfaceLockedRect by hidden
// pointer), IDirect3DSurface9::UnlockRect (COM slot 14, `call [edx+38h]`, `this` pushed).
// Shape: the flags argument loaded before the 0x10-byte frame, the hidden result pointer pushed after it
// (`lea ecx,[esp+8] ; push ecx`), `this` kept in esi then overwritten by the surface pointer, `ret 4`.
// 0x009F4330 and 0x009F4360 are byte-identical twins (same callee, same frame).
// Leaf: ?Swizzle@CSurface@@QAEXK@Z
// Name (agentNF20, 2026-11-11): Ego_r.pdb + FableWin.pdb ?Swizzle@CSurface@@QAEXK@Z (Ego_r address order Swizzle 0x00B48030,
//   Unswizzle 0x00B48060 = retail 0x009F4330, 0x009F4360); Lock is ?Lock@CSurface@@QAE?AVCSurfaceLockedRect@@K@Z (was the
//   CSurface_Methods stand-in). Model: the shared engine/CSurface.h (FABLE_RETAIL_CSURFACE, _CLEAR: Lock / Unlock; opt-in _SWIZZLE).
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CPIXELFORMAT_COLOURDEPTH
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_PIXELFORMAT
#define FABLE_RETAIL_CSURFACE_CLEAR
#define FABLE_RETAIL_CSURFACE_SWIZZLE
#include <stddef.h>
#include "engine/Direct3DSurface9.h"
#include "engine/CSurfaceLockedRect.h"
#include "engine/CPixelFormat.h"
#include "engine/CSurface.h"

void CSurface::Swizzle(unsigned long lockFlags)
{
    CSurfaceLockedRect rect = Lock(lockFlags);
    Unlock();
}
