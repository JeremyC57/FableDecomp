// CSurface::Swizzle(unsigned long lockFlags) -- lock the whole surface (CSurface::Lock, the CSurfaceLockedRect returned through a frame temporary) and unlock it again; the PC build keeps only the lock/unlock bracket of the (console) swizzle pass
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F4360, length 39 bytes.
// Callees: CSurface::Lock(unsigned long) @ 0x009F33E0 (`__fastcall`, the 16-byte CSurfaceLockedRect by hidden
// pointer), IDirect3DSurface9::UnlockRect (COM slot 14, `call [edx+38h]`, `this` pushed).
// Shape: the flags argument loaded before the 0x10-byte frame, the hidden result pointer pushed after it
// (`lea ecx,[esp+8] ; push ecx`), `this` kept in esi then overwritten by the surface pointer, `ret 4`.
// 0x009F4330 and 0x009F4360 are byte-identical twins (same callee, same frame).
// Model: engine/CSurface.h (+4 PD3DSurface) with a methods struct; PDB CSurfaceLockedRect.
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

struct CRGBColour {                                // a 4-byte colour passed by value
    unsigned char B, G, R, A;
};

struct C2DExtentsI {
    long Width;
    long Height;
};

struct CSurfaceLockedRect {                        // PDB CSurfaceLockedRect, 16 bytes
    C2DExtentsI    Dimensions;                     // +0x00
    long           Pitch;                          // +0x08
    unsigned char* PMem;                           // +0x0c
};

// CPixelFormat with its D3DFORMAT constructor and depth queries (the engine header holds only the layout).
struct CPixelFormatFromD3D {
    long FormatIndex;                              // +0x00 (PDB CPixelFormat::FormatIndex)
    CPixelFormatFromD3D(unsigned long d3dFormat);  // 0x009E3830 (CPixelFormat::Initialise / constructor)
    unsigned long GetColourDepth() const;          // 0x009E3820 (bits per pixel)
    unsigned long GetByteColourDepth() const;      // 0x009E3800 (bytes per pixel)
    void WriteColour(CRGBColour colour, unsigned char* destination) const;   // 0x009E3F60 (encode one pixel)
};

struct CSurface_Methods : CSurface {
    CSurfaceLockedRect Lock(unsigned long lockFlags);   // 0x009F33E0
    void Swizzle(unsigned long lockFlags);
};

void CSurface_Methods::Swizzle(unsigned long lockFlags)
{
    CSurfaceLockedRect rect = Lock(lockFlags);
    PD3DSurface->UnlockRect();
}