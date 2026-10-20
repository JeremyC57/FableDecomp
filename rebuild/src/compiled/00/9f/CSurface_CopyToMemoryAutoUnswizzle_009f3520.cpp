// CSurface::CopyToMemoryAutoUnswizzle(void* data, long pitch) -- lock the whole surface and copy it row by row into a linear buffer with the given pitch (the PC build does no unswizzling), then unlock
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F3520, 163 bytes (+ int3 padding), `ret 8`. Ego_r.pdb `?CopyToMemoryAutoUnswizzle@CSurface@@QAEXPAXJ@Z`.
// Identity: the catalog label "CopyFromMemoryAutoSwizzle" is copied from the sibling 0x009F3470; this body copies the
//   locked surface INTO the buffer (source = PMem), which is the PDB's CopyToMemoryAutoUnswizzle.
// Leaf: CopyToMemoryAutoUnswizzle
// Callees: CSurface::Lock(unsigned long) @ 0x009F33E0 (the 16-byte CSurfaceLockedRect by hidden pointer), IDirect3DSurface9::GetDesc
//   (COM slot 12, +0x30) and UnlockRect (slot 14, +0x38, through the inline Unlock), CPixelFormat::Initialise(D3DFORMAT) @ 0x009E3830,
//   CPixelFormat::GetByteColourDepth() const @ 0x009E3800.
// Shape: the frame is {format +0, `this` spill +4 (esi is taken by the block copy), locked rect +8, surface desc +0x18}; the byte
//   depth is fetched once, the row size (width * depth) is the loop-invariant memcpy length hoisted after the entry test, the
//   intrinsic memcpy is `rep movsd ; rep movsb`, the row counter lives in the dead `data` argument slot (the pointer itself
//   rides in eax), and both cursors step by their own pitch.
// Model: engine/CSurface.h (+4 PD3DSurface) and engine/CPixelFormat.h with methods structs; PDB CSurfaceLockedRect.
#include "engine/CSurface.h"
#include "engine/CPixelFormat.h"
#include <string.h>

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

// The COM surface (completes the header's forward declaration): IUnknown + IDirect3DResource9 slots.
struct IDirect3DSurface9 {
    virtual long __stdcall QueryInterface(const void* iid, void** object);   // slot 0
    virtual unsigned long __stdcall AddRef();                                // slot 1 (+0x04)
    virtual unsigned long __stdcall Release();                               // slot 2 (+0x08)
    virtual long __stdcall GetDevice(void** device);                         // slot 3
    virtual long __stdcall SetPrivateData(const void* guid, const void* data, unsigned long size, unsigned long flags);
    virtual long __stdcall GetPrivateData(const void* guid, void* data, unsigned long* size);
    virtual long __stdcall FreePrivateData(const void* guid);
    virtual unsigned long __stdcall SetPriority(unsigned long priority);
    virtual unsigned long __stdcall GetPriority();
    virtual void __stdcall PreLoad();
    virtual long __stdcall GetType();
    virtual long __stdcall GetContainer(const void* iid, void** container);  // slot 11
    virtual long __stdcall GetDesc(D3DSURFACE_DESC_Local* desc);             // slot 12 (+0x30)
    virtual long __stdcall LockRect(void* lockedRect, const void* rect, unsigned long flags);   // slot 13 (+0x34)
    virtual long __stdcall UnlockRect();                                     // slot 14 (+0x38)
};

struct C2DExtentsI {
    long X;
    long Y;
};

struct CSurfaceLockedRect {                        // PDB CSurfaceLockedRect, 16 bytes
    C2DExtentsI    Dimensions;                     // +0x00
    long           Pitch;                          // +0x08
    unsigned char* PMem;                           // +0x0c
};

struct CPixelFormat_Methods : CPixelFormat {
    void Initialise(unsigned long d3dFormat);      // 0x009E3830 (D3DFORMAT)
    unsigned long GetByteColourDepth() const;      // 0x009E3800
};

struct CSurface_Methods : CSurface {
    CSurfaceLockedRect Lock(unsigned long lockFlags);   // 0x009F33E0
    void Unlock() { PD3DSurface->UnlockRect(); }
    void CopyToMemoryAutoUnswizzle(void* data, long pitch);
};

struct D3DLOCKED_RECT_Local { long Pitch; void* pBits; };
struct RECT_Local { long left, top, right, bottom; };
// Same-TU stand-in for CSurface::Lock 0x009F33E0 (GetDesc, LockRect over the whole surface; zero rect on failure): with its
// body in view VC7.1 knows the returned rect's address goes no further, so `X * depth` is hoisted into the loop guard as retail.
#pragma auto_inline(off)
__declspec(noinline) CSurfaceLockedRect CSurface_Methods::Lock(unsigned long lockFlags)
{
    D3DSURFACE_DESC_Local desc;
    PD3DSurface->GetDesc(&desc);
    RECT_Local rect;
    rect.left = 0; rect.top = 0; rect.right = (long)desc.Width; rect.bottom = (long)desc.Height;
    D3DLOCKED_RECT_Local lr;
    CSurfaceLockedRect result;
    if (PD3DSurface->LockRect(&lr, &rect, lockFlags) >= 0) {
        result.Dimensions.X = (long)desc.Width; result.Dimensions.Y = (long)desc.Height;
        result.Pitch = lr.Pitch; result.PMem = (unsigned char*)lr.pBits;
    } else {
        result.Dimensions.X = 0; result.Dimensions.Y = 0; result.Pitch = 0; result.PMem = 0;
    }
    return result;
}
#pragma auto_inline(on)
void CSurface_Methods::CopyToMemoryAutoUnswizzle(void* data, long pitch)
{
    CSurfaceLockedRect locked = Lock(0);
    D3DSURFACE_DESC_Local desc;
    PD3DSurface->GetDesc(&desc);
    CPixelFormat_Methods format;
    format.Initialise(desc.Format);
    unsigned long depth = format.GetByteColourDepth();
    for (long y = 0; y < locked.Dimensions.Y; ++y) {
        memcpy((unsigned char*)data + y * pitch, locked.PMem + y * locked.Pitch, locked.Dimensions.X * depth);
    }
    Unlock();
}