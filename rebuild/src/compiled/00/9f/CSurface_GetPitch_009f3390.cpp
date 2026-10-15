// CSurface::GetPitch() -- bytes per row of the surface: the pixel format's bits per pixel (GetPixelFormat, inlined) times the width (an inline GetWidth: its own GetDesc), divided by 8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F3390, length 72 bytes.
// Callees: IDirect3DSurface9::GetDesc (COM slot 12) twice, CPixelFormat::CPixelFormat(D3DFORMAT) @ 0x009E3830,
// CPixelFormat::GetColourDepth @ 0x009E3820.
// Shape: the pixel format is built first (its GetDesc into the frame at +4), then the inline GetWidth's GetDesc
// (+0x24) whose Width is loaded into esi BEFORE the GetColourDepth call and multiplied as a register
// (`imul eax,esi ; shr eax,3`). Writing the width as a direct `desc.Width` local read gives
// `imul eax,[esp+40h]` after the call instead (70 v 72): the value must come out of an inline getter.
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
    unsigned long GetWidth() const
    {
        D3DSURFACE_DESC_Local desc;
        PD3DSurface->GetDesc(&desc);
        return desc.Width;
    }
    unsigned long GetPitch() const;
};

CPixelFormatFromD3D CSurface_Methods::GetPixelFormat() const
{
    D3DSURFACE_DESC_Local desc;
    PD3DSurface->GetDesc(&desc);
    return CPixelFormatFromD3D(desc.Format);
}

unsigned long CSurface_Methods::GetPitch() const
{
    return GetPixelFormat().GetColourDepth() * GetWidth() / 8;
}