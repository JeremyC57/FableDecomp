// Grade: DIFFER(589v582) -- frame 0x84 v 0x78 (VC7.1 places format/this-spill/left in other words; disp32 forms add 7 bytes) and the inner byte swizzle loads s[0]/s[2] in the other order (retail keeps c.R in dl); tried 24 store orders x 24 copy orders, in-place format
// CSurface::CopyToMemoryB8G8R8A8(const C2DBoxI* area, void* data) -- copy the surface (or the box) into a linear
// 32-bit buffer with each texel's bytes reversed; a surface that is not already A8R8G8B8 is first converted through a
// temporary A8R8G8B8 image (InitialiseAsImage + CopyFromSurface), which is locked instead and released afterwards
// Leaf: CopyToMemoryB8G8R8A8
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F3DA0, 582 bytes, `ret 8`.
// Callees: CPixelFormat::Initialise(D3DFORMAT) 0x009E3830, CSurface::Lock 0x009F33E0 (16-byte CSurfaceLockedRect by
//   hidden pointer), CSurface::InitialiseAsImage 0x009F3230, CSurface::CopyFromSurface 0x009F37E0, IDirect3DSurface9
//   GetDesc/UnlockRect/Release (+0x30/+0x38/+0x08); pixel-format table 0x0129BA40 (0x24-byte rows, D3DFORMAT at +0);
//   vtable 0x0122F84C (temporary CSurface).
// Shape: the landed CSurface::CopyFromSurface 0x009F37E0 (local polymorphic CSurface model, inline Uninitialise/dtor).
struct D3DSURFACE_DESC_Local {
    unsigned long Format; unsigned long Type; unsigned long Usage; unsigned long Pool;
    unsigned long MultiSampleType; unsigned long MultiSampleQuality; unsigned long Width; unsigned long Height;
};
struct IDirect3DSurface9 {
    virtual long __stdcall QueryInterface(const void* iid, void** object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual long __stdcall GetDevice(void** device);
    virtual long __stdcall SetPrivateData(const void* guid, const void* data, unsigned long size, unsigned long flags);
    virtual long __stdcall GetPrivateData(const void* guid, void* data, unsigned long* size);
    virtual long __stdcall FreePrivateData(const void* guid);
    virtual unsigned long __stdcall SetPriority(unsigned long priority);
    virtual unsigned long __stdcall GetPriority();
    virtual void __stdcall PreLoad();
    virtual long __stdcall GetType();
    virtual long __stdcall GetContainer(const void* iid, void** container);
    virtual long __stdcall GetDesc(D3DSURFACE_DESC_Local* desc);             // +0x30
    virtual long __stdcall LockRect(void* lockedRect, const void* rect, unsigned long flags);
    virtual long __stdcall UnlockRect();                                     // +0x38
};
class C2DBoxI { public: long left, top, right, bottom; };
struct C2DExtentsI { long X; long Y; C2DExtentsI(long x, long y) : X(x), Y(y) {} };
struct CSurfaceLockedRect { long DimX; long DimY; long Pitch; unsigned char* PMem; };
struct SPixelFormatInfo { unsigned long D3DFormat; unsigned char _rest[0x20]; };
extern SPixelFormatInfo g_pixelFormatInfo[];       // 0x0129BA40
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(const CRGBColour& other) : B(other.B), G(other.G), R(other.R), A(other.A) {}
};
class CPixelFormat {
public:
    long FormatIndex;
    CPixelFormat() : FormatIndex(-1) {}
    CPixelFormat(unsigned long d3dFormat) { Initialise(d3dFormat); }
    void Initialise(unsigned long d3dFormat);      // 0x009E3830
    unsigned long GetD3DFormat() const { return g_pixelFormatInfo[FormatIndex].D3DFormat; }
};
class CSurface {
public:
    virtual ~CSurface() { Uninitialise(); }
    IDirect3DSurface9* PD3DSurface;                // +0x04
    unsigned long      AllocationSource;           // +0x08
    void*              PAllocatedMemory;           // +0x0c
    CSurface() : PD3DSurface(0), AllocationSource(0) {}
    void Uninitialise()
    {
        if (PD3DSurface) {
            PD3DSurface->Release();
            PD3DSurface = 0;
            AllocationSource = 0;
        }
    }
    CPixelFormat GetPixelFormat() const
    {
        D3DSURFACE_DESC_Local desc;
        PD3DSurface->GetDesc(&desc);
        return CPixelFormat(desc.Format);
    }
    C2DExtentsI GetDimensions() const
    {
        D3DSURFACE_DESC_Local desc;
        PD3DSurface->GetDesc(&desc);
        return C2DExtentsI(desc.Width, desc.Height);
    }
    unsigned long GetD3DFormat() const { D3DSURFACE_DESC_Local desc; PD3DSurface->GetDesc(&desc); return desc.Format; }
    unsigned long GetWidth() const { D3DSURFACE_DESC_Local desc; PD3DSurface->GetDesc(&desc); return desc.Width; }
    unsigned long GetHeight() const { D3DSURFACE_DESC_Local desc; PD3DSurface->GetDesc(&desc); return desc.Height; }
    bool InitialiseAsImage(const C2DExtentsI& dims, const CPixelFormat& format, bool dynamic);              // 0x009F3230
    CSurfaceLockedRect Lock(unsigned long flags);                                                          // 0x009F33E0
    void Unlock() { PD3DSurface->UnlockRect(); }
    void CopyFromSurface(const C2DBoxI* dstBox, const CSurface& source, const C2DBoxI* srcBox, unsigned long filter);   // 0x009F37E0
    void CopyToMemoryB8G8R8A8(const C2DBoxI* area, void* data);
};

void CSurface::CopyToMemoryB8G8R8A8(const C2DBoxI* area, void* data)
{
    long left;
    long top;
    long right;
    long bottom;
    if (area == 0) {
        left = 0;
        top = 0;
        right = GetWidth();
        bottom = GetHeight();
    } else {
        left = area->left;
        top = area->top;
        right = area->right;
        bottom = area->bottom;
    }
    long width = right - left;
    long height = bottom - top;
    CPixelFormat format(GetD3DFormat());
    bool native = format.GetD3DFormat() == 0x15;
    CSurface temp;
    CSurfaceLockedRect locked;
    if (native) {
        locked = Lock(0);
    } else {
        CPixelFormat argb;
        argb.Initialise(0x15);
        temp.InitialiseAsImage(GetDimensions(), argb, false);
        temp.CopyFromSurface(0, *this, 0, 0xffffffff);
        locked = temp.Lock(0);
    }
    long pitch = locked.Pitch / 4;
    unsigned char* src = locked.PMem;
    unsigned char* dst = (unsigned char*)data;
    for (long y = top; y < height; ++y) {
        unsigned char* s = src;
        unsigned char* d = dst;
        for (long x = left; x < width; ++x) {
            CRGBColour c = *(const CRGBColour*)s;
            d[1] = c.R;
            d[2] = c.G;
            d[3] = c.B;
            d[0] = c.A;
            s += 4;
            d += 4;
        }
        dst += width * 4;
        src += pitch * 4;
    }
    if (native) {
        Unlock();
    } else {
        temp.Unlock();
        temp.Uninitialise();
    }
}
