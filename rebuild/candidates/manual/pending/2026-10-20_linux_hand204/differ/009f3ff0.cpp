// Grade: DIFFER(155v161) -- retail keeps `this` in edi (re-used as the column counter and restored from its spill after each row) and re-reads the width at the top of every row; the lane keeps `this` in esi and the width in edi (restored after the inner loop); up-counting loops are worse (153v161)
// CSurface::Clear(const CRGBColour& colour) -- lock the whole surface and fill every pixel with the colour encoded in the surface's pixel format, then unlock
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F3FF0, 159 bytes (+ int3 padding), `ret 4`. Ego_r.pdb `?Clear@CSurface@@QAEXABVCRGBColour@@@Z`.
// Leaf: Clear
// Callees: CSurface::Lock(unsigned long) @ 0x009F33E0, IDirect3DSurface9::GetDesc (slot 12) / UnlockRect (slot 14),
//   CPixelFormat::Initialise(D3DFORMAT) @ 0x009E3830, CPixelFormat::ConvertRGBToPFDData(const CRGBColour&, unsigned char*) const
//   @ 0x009E3F60, CPixelFormat::GetByteColourDepth() const @ 0x009E3800 (per pixel).
// Shape: two count-down loops (rows in a frame word, columns in edi); the row cursor is ebx, the pixel cursor esi.
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

struct CRGBColour { unsigned char B, G, R, A; };

struct CPixelFormat_Methods : CPixelFormat {
    void Initialise(unsigned long d3dFormat);      // 0x009E3830 (D3DFORMAT)
    unsigned long GetByteColourDepth() const;      // 0x009E3800
    void ConvertRGBToPFDData(const CRGBColour& colour, unsigned char* data) const;   // 0x009E3F60
};

struct CSurface_Methods : CSurface {
    CSurfaceLockedRect Lock(unsigned long lockFlags);   // 0x009F33E0
    void Unlock() { PD3DSurface->UnlockRect(); }
    void Clear(const CRGBColour& colour);
};

void CSurface_Methods::Clear(const CRGBColour& colour)
{
    CSurfaceLockedRect locked = Lock(0);
    unsigned char* row = locked.PMem;
    D3DSURFACE_DESC_Local desc;
    PD3DSurface->GetDesc(&desc);
    CPixelFormat_Methods format;
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
