// CSurface::Clear() -- lock the surface, zero Width * Height * bits-per-pixel / 8 bytes of it, unlock
// Grade: DIFFER(120v120) -- scheduling only: retail does both imuls before `mov edi,[locked.PMem]`, ours loads edi between them | tried: expression order X*Y*depth, named byte-count local | retry batch 5: 3 forms tried (byte count + pointer locals, split product, X*Y factor order): the mov edi,[locked.PMem] always lands between the imuls; scheduling-only lane gap, parked
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F40A0, length 120 bytes (+8 int3 padding).
// Callees: CSurface::Lock @ 0x009F33E0 (returns CSurfaceLockedRect by hidden pointer), IDirect3DSurface9::GetDesc (vtable +0x30)
// twice, CPixelFormat::Initialise(_D3DFORMAT) @ 0x009E3830, CPixelFormat::GetColourDepth @ 0x009E3820,
// IDirect3DSurface9::UnlockRect (vtable +0x38). The byte fill is the inline memset (`rep stosd ; rep stosb`).
// Shape: two GetDesc calls = two inline by-value accessors (GetSize, then GetPixelFormat); the locked rect at esp+0xC, the
// two descs at esp+0x1C/+0x3C, the pixel format at esp+8. Byte count `depth * Height * Width >> 3` (unsigned).
// Model: engine/CSurface.h member names; polymorphic local model as in the landed CRenderTarget rows.
struct D3DSURFACE_DESC {
    long Format, Type;
    unsigned long Usage;
    long Pool, MultiSampleType;
    unsigned long MultiSampleQuality, Width, Height;
};
struct RECT { long left, top, right, bottom; };
struct D3DLOCKED_RECT { long Pitch; void* pBits; };
struct IDirect3DSurface9 {
    virtual long __stdcall QueryInterface(void*, void**);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void __stdcall v03(); virtual void __stdcall v04(); virtual void __stdcall v05(); virtual void __stdcall v06();
    virtual void __stdcall v07(); virtual void __stdcall v08(); virtual void __stdcall v09(); virtual void __stdcall v10();
    virtual void __stdcall v11();
    virtual long __stdcall GetDesc(D3DSURFACE_DESC* desc);                                          // +0x30
    virtual long __stdcall LockRect(D3DLOCKED_RECT* locked, const RECT* rect, unsigned long flags);  // +0x34
    virtual long __stdcall UnlockRect();                                                            // +0x38
};
extern "C" void* __cdecl memset(void*, int, unsigned int);
#pragma intrinsic(memset)
struct C2DExtentsI {
    long X, Y;
    C2DExtentsI(long x, long y) : X(x), Y(y) {}
};
struct CPixelFormat {
    long FormatIndex;
    CPixelFormat(long d3dFormat) { Initialise(d3dFormat); }
    void Initialise(long d3dFormat);            // 0x009E3830
    unsigned long GetColourDepth() const;       // 0x009E3820
};
struct CSurfaceLockedRect {
    C2DExtentsI Dimensions;     // +0x00
    long Pitch;                 // +0x08
    unsigned char* PMem;        // +0x0c
};

struct CSurface {                               // vtable 0x0122F84C, 0x10 bytes
    virtual ~CSurface();
    IDirect3DSurface9* PD3DSurface;             // +0x04
    unsigned long AllocationSource;             // +0x08
    void* PAllocatedMemory;                     // +0x0c
    CSurfaceLockedRect Lock(unsigned long flags);   // 0x009F33E0
    void Unlock() { PD3DSurface->UnlockRect(); }
    C2DExtentsI GetSize() const
    {
        D3DSURFACE_DESC desc;
        PD3DSurface->GetDesc(&desc);
        return C2DExtentsI(desc.Width, desc.Height);
    }
    CPixelFormat GetPixelFormat() const
    {
        D3DSURFACE_DESC desc;
        PD3DSurface->GetDesc(&desc);
        return CPixelFormat(desc.Format);
    }
    void Clear();
};

void CSurface::Clear()
{
    CSurfaceLockedRect locked = Lock(0);
    C2DExtentsI size = GetSize();
    CPixelFormat format = GetPixelFormat();
    memset(locked.PMem, 0, format.GetColourDepth() * size.Y * size.X / 8);
    Unlock();
}
