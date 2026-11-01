// CSurface::Clear(const CRGBColour& colour) -- lock the whole surface and fill every pixel with the colour encoded in the surface's pixel format, then unlock
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F3FF0, 159 bytes (+ int3 padding), `ret 4`. Ego_r.pdb `?Clear@CSurface@@QAEXABVCRGBColour@@@Z`.
// Leaf: Clear
// Naming wave 25: CPixelFormat::Initialise 0x009E3830 is ?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z (Ego_r.pdb; no constructor:
//   the body returns nothing), the format a _D3DFORMAT; a CPixelFormat built from a format is the inline constructor calling it.
// Fix (retry 10): CSurface::Lock has its same-TU body (reconstructed from 0x009F33E0) above the function; with only the
//   declaration VC7.1 treated the returned locked rect as unaliased and hoisted its loads out of the loops.
// Callees: CSurface::Lock(unsigned long) @ 0x009F33E0, IDirect3DSurface9::GetDesc (slot 12) / UnlockRect (slot 14),
//   CPixelFormat::Initialise(D3DFORMAT) @ 0x009E3830, CPixelFormat::ConvertRGBToPFDData(const CRGBColour&, unsigned char*) const
//   @ 0x009E3F60, CPixelFormat::GetByteColourDepth() const @ 0x009E3800 (per pixel).
// Shape: two count-down loops (rows in a frame word, columns in edi); the row cursor is ebx, the pixel cursor esi.
enum _D3DFORMAT { D3DFMT_UNKNOWN = 0, D3DFMT_A8R8G8B8 = 21, D3DFMT_D24S8 = 75, D3DFMT_DXT1 = 0x31545844, D3DFMT_DXT3 = 0x33545844, D3DFMT_DXT5 = 0x35545844, D3DFMT_FORCE_DWORD = 0x7fffffff };   // naming wave 25: the D3D format enum (Initialise takes it)
#include "engine/CSurface.h"
#define CPixelFormat CPixelFormatLayout
#include "engine/CPixelFormat.h"
#undef CPixelFormat

class CPixelFormat : public CPixelFormatLayout
{
public:
    unsigned long GetByteColourDepth() const;   // 0x009E3800
    void Initialise(_D3DFORMAT d3dFormat);      // 0x009E3830 (naming wave 25: was on a CPixelFormat_Methods stand-in)
    void ConvertRGBToPFDData(const class CRGBColour& colour, unsigned char* data) const;   // 0x009E3F60 (Ego_r.pdb; was on the stand-in)
};
#include <string.h>

struct D3DSURFACE_DESC_Local {                     // D3DSURFACE_DESC, 0x20 bytes
    _D3DFORMAT Format;                          // +0x00 D3DFORMAT
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

class C2DExtentsI { public:
    long X;
    long Y;
};

class CSurfaceLockedRect { public:                        // PDB CSurfaceLockedRect, 16 bytes
    C2DExtentsI    Dimensions;                     // +0x00
    long           Pitch;                          // +0x08
    unsigned char* PMem;                           // +0x0c
};

class CRGBColour { public: unsigned char B, G, R, A; };


struct CSurface_Methods : CSurface {
    CSurfaceLockedRect Lock(unsigned long lockFlags);   // 0x009F33E0
    void Unlock() { PD3DSurface->UnlockRect(); }
    void Clear(const CRGBColour& colour);
};

struct SLockRect_s7 { long left, top, right, bottom; };
struct SLockedRect_s7 { long Pitch; void* pBits; };
#pragma auto_inline(off)
// Same-TU body of CSurface::Lock (0x009F33E0, reconstructed from the retail disassembly): GetDesc, a RECT over the whole
// surface, LockRect; the result is {Width, Height, Pitch, bits} on success and all zero on failure.
__declspec(noinline) CSurfaceLockedRect CSurface_Methods::Lock(unsigned long flags)
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
        CSurfaceLockedRect result = { { desc.Width, desc.Height }, locked.Pitch, (unsigned char*)locked.pBits };
        return result;
    }
    CSurfaceLockedRect none = { { 0, 0 }, 0, 0 };
    return none;
}
#pragma auto_inline(on)

void CSurface_Methods::Clear(const CRGBColour& colour)
{
    CSurfaceLockedRect locked = Lock(0);
    unsigned char* row = locked.PMem;
    D3DSURFACE_DESC_Local desc;
    PD3DSurface->GetDesc(&desc);
    CPixelFormat format;
    format.Initialise(desc.Format);
    for (long y = locked.Dimensions.Y; y > 0; --y) {
        unsigned char* pixel = row;
        for (long x = locked.Dimensions.X; x > 0; --x) {
            format.ConvertRGBToPFDData(colour, pixel);
            pixel += format.GetByteColourDepth();
        }
        row += locked.Pitch;
    }
    Unlock();
}