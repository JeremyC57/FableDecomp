// CSurface::ReadOnlyLock(unsigned long flags) const -- lock the whole surface read-only (flags | D3DLOCK_READONLY) and return its extents, pitch and memory, or an all-zero locked rect when the lock fails
// Grade: DIFFER(138v144) -- the failure arm: retail `xor eax,eax ; mov edx,eax ; mov eax,[esp+4Ch]` then stores +8/+0xc before the extents, ours zeroes and stores in member order; on success retail loads the result pointer before pitch/bits
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F3010, 144 bytes; `ret 8` (hidden CSurfaceLockedRect + flags). PDB ?ReadOnlyLock@CSurface@@QBE?AVCSurfaceLockedRect@@K@Z.
// Callees: IDirect3DSurface9::GetDesc (COM slot 12, +0x30) and LockRect (slot 13, +0x34), `this` pushed.
// Model: engine/CSurface.h (+4 PD3DSurface); PDB CSurfaceLockedRect {C2DExtentsI Dimensions, Pitch, PMem}.
// Shape: the landed GetDimensions (0x009F2F90) inlined (width/height kept in ebx/ebp), a frame RECT {0, 0, w, h} and
//        D3DLOCKED_RECT; FAILED(hr) is `hr < 0`; both results are built by the locked-rect constructor.
#include "engine/CSurface.h"

struct D3DSURFACE_DESC_Local {                     // D3DSURFACE_DESC, 0x20 bytes
    unsigned long Format;                          // +0x00 D3DFORMAT
    unsigned long Type;                            // +0x04
    unsigned long Usage;                           // +0x08
    unsigned long Pool;                            // +0x0c
    unsigned long MultiSampleType;                 // +0x10
    unsigned long MultiSampleQuality;              // +0x14
    unsigned long Width;                           // +0x18
    unsigned long Height;                          // +0x1c
};
struct D3DLOCKED_RECT_Local {
    long  Pitch;
    void* pBits;
};
struct RECT_Local {
    long left, top, right, bottom;
};
struct IDirect3DSurface9 {
    virtual void __stdcall v00(); virtual void __stdcall v04(); virtual void __stdcall v08(); virtual void __stdcall v0c();
    virtual void __stdcall v10(); virtual void __stdcall v14(); virtual void __stdcall v18(); virtual void __stdcall v1c();
    virtual void __stdcall v20(); virtual void __stdcall v24(); virtual void __stdcall v28(); virtual void __stdcall v2c();
    virtual long __stdcall GetDesc(D3DSURFACE_DESC_Local* desc);                                       // slot 12 (+0x30)
    virtual long __stdcall LockRect(D3DLOCKED_RECT_Local* lockedRect, const RECT_Local* rect, unsigned long flags);   // slot 13 (+0x34)
};
struct C2DExtentsI {
    long X;
    long Y;
    C2DExtentsI(long x, long y) : X(x), Y(y) {}
};
struct CSurfaceLockedRect {                        // PDB CSurfaceLockedRect, 16 bytes
    C2DExtentsI    Dimensions;                     // +0x00
    long           Pitch;                          // +0x08
    unsigned char* PMem;                           // +0x0c
    CSurfaceLockedRect(const C2DExtentsI& dimensions, long pitch, unsigned char* mem)
        : Dimensions(dimensions), Pitch(pitch), PMem(mem) {}
};
enum { D3DLOCK_READONLY_LOCAL = 0x10 };

struct CSurface_Methods : CSurface {
    C2DExtentsI GetDimensions() const
    {
        D3DSURFACE_DESC_Local desc;
        PD3DSurface->GetDesc(&desc);
        return C2DExtentsI(desc.Width, desc.Height);
    }
    CSurfaceLockedRect ReadOnlyLock(unsigned long flags) const;
};

CSurfaceLockedRect CSurface_Methods::ReadOnlyLock(unsigned long flags) const
{
    C2DExtentsI dimensions = GetDimensions();
    RECT_Local rect;
    rect.top = 0;
    rect.left = 0;
    rect.bottom = dimensions.Y;
    rect.right = dimensions.X;
    D3DLOCKED_RECT_Local locked;
    if (PD3DSurface->LockRect(&locked, &rect, flags | D3DLOCK_READONLY_LOCAL) >= 0)
        return CSurfaceLockedRect(dimensions, locked.Pitch, (unsigned char*)locked.pBits);
    return CSurfaceLockedRect(C2DExtentsI(0, 0), 0, 0);
}
