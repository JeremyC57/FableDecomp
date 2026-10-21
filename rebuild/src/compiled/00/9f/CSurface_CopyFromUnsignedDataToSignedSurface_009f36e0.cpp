// CSurface::CopyFromUnsignedDataToSignedSurface(const unsigned char* src, const CPixelFormat& srcFormat, unsigned long srcPitch) -- lock the surface and convert each 4-byte unsigned source pixel into a 2-byte signed one (bytes 1 and 2, biased by 0x80), row by row, then unlock
//   desc (and keeps the Pitch read after the entry test), ours keeps a separate 0x10-byte lock slot (0x58 frame), so ebx is
//   pushed late and the register allocation follows
// Leaf: CopyFromUnsignedDataToSignedSurface
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F36E0, 253 bytes (+3 int3 padding), `ret 0Ch` (srcFormat is never read).
// Fix (retry 10): CSurface::Lock has its same-TU body (reconstructed from 0x009F33E0) above the function; with only the
//   declaration VC7.1 treated the returned locked rect as unaliased and hoisted its loads out of the loops.
// Callees: CSurface::Lock(unsigned long) 0x009F33E0 (16-byte CSurfaceLockedRect by hidden pointer, as in the landed
//   SetPixel 0x009F4120), IDirect3DSurface9::GetDesc (COM slot 12) for the inline GetWidth()/GetHeight(),
//   IDirect3DSurface9::UnlockRect (slot 14).
// Layout: CSurface (+4 PD3DSurface); the locked rect {Dimensions, Pitch +8, PMem +0xc}.
// Shape: both loop bounds re-query the surface description each pass (inline GetDimensions().Y / .X: two separate
//   desc frames, the inner one re-using the dead lock result); the source row pointer is a local homed in the `src`
//   slot (the entry self-copy); the row strides `srcPitch / 4 * 4` and `Pitch / 2 * 2` (unsigned: `shr ; shl`) are
//   loop invariants hoisted after the entry test.
// Tried: strides as locals at the top of the row loop (0x5c frame), the lock result in its own block with the strides
//   taken inside it, the lock through an inline LockRows(flags, stride) helper: the lock slot is never shared.
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
struct IDirect3DSurface9 {
    virtual void __stdcall v00(); virtual void __stdcall v04(); virtual void __stdcall v08(); virtual void __stdcall v0c();
    virtual void __stdcall v10(); virtual void __stdcall v14(); virtual void __stdcall v18(); virtual void __stdcall v1c();
    virtual void __stdcall v20(); virtual void __stdcall v24(); virtual void __stdcall v28(); virtual void __stdcall v2c();
    virtual long __stdcall GetDesc(D3DSURFACE_DESC_Local* desc);   // slot 12 (+0x30)
    virtual long __stdcall LockRect(void* locked, const void* rect, unsigned long flags);   // slot 13 (+0x34)
    virtual long __stdcall UnlockRect();                           // slot 14 (+0x38)
};
struct C2DExtentsI {
    long X, Y;
    C2DExtentsI(long x, long y) : X(x), Y(y) {}
};
struct CPixelFormat { long FormatIndex; };
struct CSurfaceLockedRect {
    C2DExtentsI    Dimensions;                     // +0x00
    long           Pitch;                          // +0x08
    unsigned char* PMem;                           // +0x0c
};

struct CSurface {                                  // vtable 0x0122F84C, 0x10 bytes
    virtual ~CSurface();
    IDirect3DSurface9* PD3DSurface;                // +0x04
    unsigned long      AllocationSource;           // +0x08
    void*              PAllocatedMemory;           // +0x0c
    CSurfaceLockedRect Lock(unsigned long flags);  // 0x009F33E0
    void Unlock() { PD3DSurface->UnlockRect(); }
    C2DExtentsI GetDimensions() const
    {
        D3DSURFACE_DESC_Local desc;
        PD3DSurface->GetDesc(&desc);
        return C2DExtentsI(desc.Width, desc.Height);
    }
    unsigned long GetWidth() const { return GetDimensions().X; }
    unsigned long GetHeight() const { return GetDimensions().Y; }
    void CopyFromUnsignedDataToSignedSurface(const unsigned char* src, const CPixelFormat& srcFormat, unsigned long srcPitch);
};

struct SLockRect_s7 { long left, top, right, bottom; };
struct SLockedRect_s7 { long Pitch; void* pBits; };
#pragma auto_inline(off)
// Same-TU body of CSurface::Lock (0x009F33E0, reconstructed from the retail disassembly): GetDesc, a RECT over the whole
// surface, LockRect; the result is {Width, Height, Pitch, bits} on success and all zero on failure.
__declspec(noinline) CSurfaceLockedRect CSurface::Lock(unsigned long flags)
{
    D3DSURFACE_DESC_Local desc;
    PD3DSurface->GetDesc(&desc);
    SLockRect_s7 rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = desc.Width;
    rect.bottom = desc.Height;
    SLockedRect_s7 locked;
    if (PD3DSurface->LockRect(&locked, &rect, flags) >= 0) {
        CSurfaceLockedRect result = { C2DExtentsI(desc.Width, desc.Height), locked.Pitch, (unsigned char*)locked.pBits };
        return result;
    }
    CSurfaceLockedRect none = { C2DExtentsI(0, 0), 0, 0 };
    return none;
}
#pragma auto_inline(on)

void CSurface::CopyFromUnsignedDataToSignedSurface(const unsigned char* src, const CPixelFormat& srcFormat, unsigned long srcPitch)
{
    CSurfaceLockedRect locked = Lock(0);
    unsigned char* destRow = locked.PMem;
    const unsigned char* srcRow = src;
    for (unsigned long y = 0; y < GetHeight(); ++y) {
        unsigned char* dest = destRow;
        const unsigned char* source = srcRow;
        for (unsigned long x = 0; x < GetWidth(); ++x) {
            dest[0] = source[1] + 0x80;
            dest[1] = source[2] + 0x80;
            dest += 2;
            source += 4;
        }
        srcRow += srcPitch / 4 * 4;
        destRow += (unsigned long)locked.Pitch / 2 * 2;
    }
    Unlock();
}