// Grade: DIFFER(500v507) -- register allocation of the extents: retail keeps dstBox in ebx/srcBox in edi and spills the widths into the dead dstBox/srcBox argument slots; ours picks edi/ebx and keeps a width in ebp (plain longs, POD extents and a GetSize() form tried). The format/conversion/D3DX tail matches the matched sibling CopyFromSurface form
// CSurface::CopyFromMemory(const C2DBoxI* dstBox, const void* data, const C2DBoxI* srcBox, const CPixelFormat& srcFormat, long pitch, unsigned long filter) -- copy a pixel block from memory: signed/unsigned mismatches go through CopyFromUnsignedDataToSignedSurface, a same-format same-size block through the direct copy 0x009F3B00, else D3DXLoadSurfaceFromMemory
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F4390, 512 bytes, `ret 0x18`.
// Leaf: CopyFromMemory
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
struct RECT_Local { long left, top, right, bottom; };
extern "C" long __stdcall D3DXLoadSurfaceFromMemory(IDirect3DSurface9* dst, const void* dstPalette, const RECT_Local* dstRect,
    const void* srcMemory, unsigned long srcFormat, unsigned long srcPitch, const void* srcPalette, const RECT_Local* srcRect,
    unsigned long filter, unsigned long colourKey);
extern "C" long __stdcall D3DXLoadSurfaceFromSurface(IDirect3DSurface9* dst, const void* dstPalette, const RECT_Local* dstRect,
    IDirect3DSurface9* src, const void* srcPalette, const RECT_Local* srcRect, unsigned long filter, unsigned long colourKey);
struct CRGBColour { unsigned char B, G, R, A; };
class C2DBoxI {
public:
    long left, top, right, bottom;
    long GetWidth() const { return right - left; }
    long GetHeight() const { return bottom - top; }
};
struct C2DExtentsI { long X; long Y; };
struct CSurfaceLockedRect { long DimX; long DimY; long Pitch; unsigned char* PMem; };
struct SPixelFormatInfo {                          // 0x24-byte rows of the pixel-format table
    unsigned long D3DFormat;                       // +0x00
    unsigned char _pad[0x20];
};
extern const SPixelFormatInfo g_PixelFormatInfo[];   // 0x0129BA40
class CPixelFormat {
public:
    long FormatIndex;
    CPixelFormat() : FormatIndex(-1) {}
    CPixelFormat(unsigned long d3dFormat) { Initialise(d3dFormat); }
    void Initialise(unsigned long d3dFormat);      // 0x009E3830
    bool IsSigned() const;                         // 0x009E3970
    bool operator==(const CPixelFormat& other) const;   // 0x009E37F0
    unsigned long GetD3DFormat() const { return g_PixelFormatInfo[FormatIndex].D3DFormat; }
    unsigned long GetByteColourDepth() const;      // 0x009E3800
    void WritePixel(const CRGBColour& colour, void* dest) const;   // 0x009E3F60
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
        C2DExtentsI e; e.X = desc.Width; e.Y = desc.Height; return e;
    }
    bool InitialiseAsImage(const C2DExtentsI& dims, const CPixelFormat& format, bool dynamic);              // 0x009F3230
    CSurfaceLockedRect Lock(unsigned long flags);                                                          // 0x009F33E0
    void Unlock() { PD3DSurface->UnlockRect(); }
    unsigned long GetWidth() const
    {
        D3DSURFACE_DESC_Local desc;
        PD3DSurface->GetDesc(&desc);
        return desc.Width;
    }
    unsigned long GetHeight() const
    {
        D3DSURFACE_DESC_Local desc;
        PD3DSurface->GetDesc(&desc);
        return desc.Height;
    }
    void CopyFromMemory(const C2DBoxI* dstBox, const void* data, const C2DBoxI* srcBox, const CPixelFormat& srcFormat,
        long pitch, unsigned long filter);
    void CopyFromMemoryDirect(const C2DBoxI* dstBox, const void* data, const C2DBoxI* srcBox, long pitch);   // 0x009F3B00
    void CopyFromUnsignedDataToSignedSurface(const void* data, const CPixelFormat& format, long pitch);       // 0x009F36E0
    bool CanDoProperSignedConversion(const C2DBoxI* dst, const C2DBoxI* src, const CPixelFormat& format);    // 0x009F35D0
    void CopyFromSurface(const C2DBoxI* dstBox, const CSurface& source, const C2DBoxI* srcBox, unsigned long filter);
};

void CSurface::CopyFromMemory(const C2DBoxI* dstBox, const void* data, const C2DBoxI* srcBox, const CPixelFormat& srcFormat,
    long pitch, unsigned long filter)
{
    C2DExtentsI dstSize;
    if (dstBox) {
        dstSize.X = dstBox->GetWidth();
        dstSize.Y = dstBox->GetHeight();
    } else {
        dstSize.X = GetWidth();
        dstSize.Y = GetHeight();
    }
    C2DExtentsI srcSize;
    if (srcBox) {
        srcSize.X = srcBox->GetWidth();
        srcSize.Y = srcBox->GetHeight();
    } else {
        srcSize = dstSize;
    }
    if ((GetPixelFormat().IsSigned() || srcFormat.IsSigned())
        && !(GetPixelFormat().IsSigned() && srcFormat.IsSigned())
        && CanDoProperSignedConversion(dstBox, srcBox, srcFormat)) {
        CopyFromUnsignedDataToSignedSurface(data, srcFormat, pitch);
        return;
    }
    if (srcFormat == GetPixelFormat() && srcSize.X == dstSize.X && srcSize.Y == dstSize.Y) {
        CopyFromMemoryDirect(dstBox, data, srcBox, pitch);
        return;
    }
    RECT_Local* pDst = 0;
    RECT_Local* pSrc = 0;
    RECT_Local dst;
    RECT_Local src;
    if (dstBox) {
        dst.top = dstBox->top; dst.bottom = dstBox->bottom; dst.left = dstBox->left; dst.right = dstBox->right;
        pDst = &dst;
    }
    if (srcBox) {
        src.top = srcBox->top; src.bottom = srcBox->bottom; src.left = srcBox->left; src.right = srcBox->right;
        pSrc = &src;
    }
    D3DXLoadSurfaceFromMemory(PD3DSurface, 0, pDst, data, srcFormat.GetD3DFormat(), pitch, 0, pSrc, filter, 0);
}
