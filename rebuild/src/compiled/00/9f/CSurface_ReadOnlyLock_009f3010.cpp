// CSurface::ReadOnlyLock(unsigned long flags) const -- lock the whole surface read-only (flags | D3DLOCK_READONLY) and return its extents, pitch and memory, or an all-zero locked rect when the lock fails
// Leaf: ?ReadOnlyLock@CSurface@@QBE?AVCSurfaceLockedRect@@K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F3010, 144 bytes, thiscall, `ret 8` (hidden CSurfaceLockedRect + flags).
// Name: Ego_r.pdb / FableWin.pdb ?ReadOnlyLock@CSurface@@QBE?AVCSurfaceLockedRect@@K@Z.
// Callees: IDirect3DSurface9::GetDesc (COM slot 12, `call [ecx+30h]`, from the inlined GetDimensions) and LockRect (slot 13,
//   `call [eax+34h]`), both __stdcall with the interface pushed.
// Shape: the read-only twin of the landed CSurface::Lock 0x009F33E0 (same bytes but the `or ecx,10h`): GetDimensions
//   (0x009F2F90, Ego_r.pdb ?GetDimensions@CSurface@@QBE?AVC2DExtentsI@@XZ) is defined in this TU and auto-inlined, the RECT is
//   {0, 0, width, height}, FAILED(hr) is `hr < 0`; each arm returns its own CSurfaceLockedRect (the PDB class has only the
//   default and copy constructors), the failure arm with C2DExtentsI(0, 0) built through Initialise (`xor eax,eax ; mov edx,eax`,
//   0x009F3081; header opt-in FABLE_RETAIL_C2DEXTENTSI_INITIALISE).
// Headers: engine/CSurface.h (FABLE_RETAIL_CSURFACE: PD3DSurface +4), engine/Direct3DSurface9.h, engine/CSurfaceLockedRect.h.
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_BITMAP
#define FABLE_RETAIL_CSURFACE_READONLYLOCK
#define FABLE_RETAIL_C2DEXTENTSI_INITIALISE
#include "engine/Direct3DSurface9.h"
#include "engine/C2DExtentsI.h"
#include "engine/CSurfaceLockedRect.h"
#include "engine/CSurface.h"

enum { D3DLOCK_READONLY = 0x10 };   // d3d9.h

// Same TU as retail 0x009F2F90: the surface description's width and height.
C2DExtentsI CSurface::GetDimensions() const
{
    _D3DSURFACE_DESC desc;
    PD3DSurface->GetDesc(&desc);
    return C2DExtentsI(desc.Width, desc.Height);
}

CSurfaceLockedRect CSurface::ReadOnlyLock(unsigned long flags) const
{
    C2DExtentsI dimensions = GetDimensions();
    tagRECT rect;
    rect.top = 0;
    rect.left = 0;
    rect.bottom = dimensions.Y;
    rect.right = dimensions.X;
    _D3DLOCKED_RECT locked;
    if (PD3DSurface->LockRect(&locked, &rect, flags | D3DLOCK_READONLY) >= 0) {
        CSurfaceLockedRect result;
        result.Dimensions = dimensions;
        result.Pitch = locked.Pitch;
        result.PMem = (unsigned char*)locked.pBits;
        return result;
    }
    CSurfaceLockedRect result;
    result.Pitch = 0;
    result.PMem = 0;
    result.Dimensions = C2DExtentsI(0, 0);
    return result;
}