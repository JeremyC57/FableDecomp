// CSurface::GetByteLength() -- size of the surface's pixel data in bytes: one GetDesc for the dimensions, the pixel format (GetPixelFormat, inlined) for the bits per pixel, then depth * Width * Height / 8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F2FC0, length 75 bytes.
// Callees: IDirect3DSurface9::GetDesc (COM slot 12, `call [ecx+30h]`) twice, CPixelFormat::CPixelFormat(D3DFORMAT)
// @ 0x009E3830, CPixelFormat::GetColourDepth @ 0x009E3820.
// Shape: a 0x44-byte frame = the CPixelFormat temporary (+0), this function's D3DSURFACE_DESC (+4) and the
// inlined GetPixelFormat's own description (+0x24); the depth is multiplied straight from the frame by Height
// then Width (`imul eax,[esp+24h] ; imul eax,[esp+20h]`) and `shr eax,3` (unsigned). GetPixelFormat
// (0x009F2F60) is defined in the same TU and auto-inlined (/Ob2), as in retail.
// Model: engine/CSurface.h (+4 PD3DSurface) with a methods struct.
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

// CPixelFormat with its D3DFORMAT constructor and depth queries (the engine header holds only the layout).
struct CPixelFormatFromD3D {
    long FormatIndex;                              // +0x00 (PDB CPixelFormat::FormatIndex)
    CPixelFormatFromD3D(unsigned long d3dFormat);  // 0x009E3830 (CPixelFormat::Initialise / constructor)
    unsigned long GetColourDepth() const;          // 0x009E3820 (bits per pixel)
    unsigned long GetByteColourDepth() const;      // 0x009E3800 (bytes per pixel)
};

struct CSurface_Methods : CSurface {
    CPixelFormatFromD3D GetPixelFormat() const;
    unsigned long GetByteLength() const;
};

CPixelFormatFromD3D CSurface_Methods::GetPixelFormat() const
{
    D3DSURFACE_DESC_Local desc;
    PD3DSurface->GetDesc(&desc);
    return CPixelFormatFromD3D(desc.Format);
}

unsigned long CSurface_Methods::GetByteLength() const
{
    D3DSURFACE_DESC_Local desc;
    PD3DSurface->GetDesc(&desc);
    return GetPixelFormat().GetColourDepth() * desc.Width * desc.Height / 8;
}
