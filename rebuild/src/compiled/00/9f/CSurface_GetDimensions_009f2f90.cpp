// CSurface::GetDimensions() const -- the surface description's width and height as a C2DExtentsI (returned through the hidden pointer)
// Leaf: ?GetDimensions@CSurface@@QBE?AVC2DExtentsI@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F2F90, oracle length 39 bytes, thiscall, `ret 4`.
// Retail bytes:  mov eax,[ecx+4] ; mov ecx,[eax] ; sub esp,20h ; lea edx,[esp] ; push edx ; push eax ; call [ecx+30h] ; mov eax,[esp+24h] ; mov ecx,[esp+18h] ; mov edx,[esp+1Ch] ; mov [eax],ecx ; mov [eax+4],edx ; add esp,20h ; ret 4
// Name (agentNF20, 2026-11-11): Ego_r.pdb 0x00B46C70 + FableWin.pdb ?GetDimensions@CSurface@@QBE?AVC2DExtentsI@@XZ (public const,
//   C2DExtentsI by value); was the stand-in CSurface_Methods. engine/CSurface.h (FABLE_RETAIL_CSURFACE: PD3DSurface +0x04);
//   COM slot 12 (+0x30) is IDirect3DSurface9::GetDesc; the 32-byte D3DSURFACE_DESC is filled on the stack and its
//   Width/Height (+0x18/+0x1C) become the returned extents. Same body as the in-TU copy in CSurface::ReadOnlyLock 0x009F3010.
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_BITMAP
#include "engine/Direct3DSurface9.h"
#include "engine/C2DExtentsI.h"
#include "engine/CSurface.h"

C2DExtentsI CSurface::GetDimensions() const
{
    _D3DSURFACE_DESC desc;
    PD3DSurface->GetDesc(&desc);
    return C2DExtentsI(desc.Width, desc.Height);
}
