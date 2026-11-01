// CSurface::SetPixel(long x, long y, const CRGBColour& colour) -- lock the surface and write one pixel at PMem + x * bytes-per-pixel + y * Pitch in the surface's pixel format, unlock
// Leaf: ?SetPixel@CSurface@@QAEXJJABVCRGBColour@@@Z
// Naming wave 25: CPixelFormat::Initialise 0x009E3830 is ?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z (Ego_r.pdb; no constructor:
//   the body returns nothing), the format a _D3DFORMAT; a CPixelFormat built from a format is the inline constructor calling it.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F4120, length 110 bytes (+2 int3 padding). `ret 0Ch`.
// Callees: CSurface::Lock @ 0x009F33E0, IDirect3DSurface9::GetDesc (vtable +0x30), CPixelFormat::Initialise @ 0x009E3830,
// CPixelFormat::GetByteColourDepth @ 0x009E3800, the pixel-format colour writer @ 0x009E3F60 (thiscall on the format, args
// colour then destination; catalog "Graphics_CopyPixelsToBuffer"), IDirect3DSurface9::UnlockRect (vtable +0x38).
// Shape: same inline accessors as CSurface::Clear (0x009F40A0): GetPixelFormat built as CPixelFormat(desc.Format) in place.
// Model: engine/CSurface.h member names; polymorphic local model as in the landed CRenderTarget rows.
enum _D3DFORMAT { D3DFMT_UNKNOWN = 0, D3DFMT_A8R8G8B8 = 21, D3DFMT_D24S8 = 75, D3DFMT_DXT1 = 0x31545844, D3DFMT_DXT3 = 0x33545844, D3DFMT_DXT5 = 0x35545844, D3DFMT_FORCE_DWORD = 0x7fffffff };   // naming wave 25: the D3D format enum (Initialise takes it)
struct D3DSURFACE_DESC {
    _D3DFORMAT Format; long Type;
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
class C2DExtentsI { public:
    long X, Y;
    C2DExtentsI(long x, long y) : X(x), Y(y) {}
};
class CRGBColour { public: unsigned char B, G, R, A; };
class CPixelFormat { public:
    long FormatIndex;
    CPixelFormat(_D3DFORMAT d3dFormat) { Initialise(d3dFormat); }
    void Initialise(_D3DFORMAT d3dFormat);            // 0x009E3830
    unsigned long GetByteColourDepth() const;   // 0x009E3800
    void ConvertRGBToPFDData(const CRGBColour& colour, unsigned char* data) const;   // 0x009E3F60 (Ego_r.pdb; naming wave 25, was WritePixel)
};
class CSurfaceLockedRect { public:
    C2DExtentsI Dimensions;     // +0x00
    long Pitch;                 // +0x08
    unsigned char* PMem;        // +0x0c
};

class CSurface { public:                               // vtable 0x0122F84C, 0x10 bytes
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
    void SetPixel(long x, long y, const CRGBColour& colour);
};

void CSurface::SetPixel(long x, long y, const CRGBColour& colour)
{
    CSurfaceLockedRect locked = Lock(0);
    CPixelFormat format = GetPixelFormat();
    unsigned char* dest = locked.PMem + format.GetByteColourDepth() * x + locked.Pitch * y;
    format.ConvertRGBToPFDData(colour, dest);
    Unlock();
}