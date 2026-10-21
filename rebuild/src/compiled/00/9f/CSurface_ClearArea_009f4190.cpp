// CSurface::ClearArea(const C2DBoxI& area, const CRGBColour& colour) -- fill a rectangle of the surface with one colour: lock, clamp the box to the surface extents, then write the colour pixel by pixel in the surface's own pixel format
// Leaf: ClearArea
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F4190, 416 bytes (407 + int3 padding), `ret 8`.
// Callees: CSurface::Lock(unsigned long) 0x009F33E0 (16-byte CSurfaceLockedRect by hidden pointer), CPixelFormat::Initialise 0x009E3830,
//   CPixelFormat::GetByteColourDepth() const 0x009E3800 (re-read every pixel), CPixelFormat::WritePixel(const CRGBColour&, void*) const
//   0x009E3F60 (name from the body: packs the colour's channels by the format's shifts), IDirect3DSurface9 GetDesc/UnlockRect (+0x30/+0x38).
// Shape: GetWidth()/GetHeight() are inline GetDesc reads, called twice per clamp (compare, then assign), so each clamp is two COM calls; the
//   box is a local C2DBoxI filled member by member (the copy constructor loads it in a different register order) and VC7.1 keeps
//   left/right in its frame words over the dead GetPixelFormat desc, top/bottom in esi/edi; the row counter lives in the dead `area` slot.
// Layout: as the matched sibling CopyFromSurface (engine/CSurface.h cannot express the vptr; class modelled locally with PDB names).
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
struct CRGBColour { unsigned char B, G, R, A; };
class C2DBoxI { public: long left, top, right, bottom; };
struct C2DExtentsI { long X; long Y; C2DExtentsI(long x, long y) : X(x), Y(y) {} };
struct CSurfaceLockedRect { long DimX; long DimY; long Pitch; unsigned char* PMem; };
class CPixelFormat {
public:
    long FormatIndex;
    CPixelFormat() : FormatIndex(-1) {}
    CPixelFormat(unsigned long d3dFormat) { Initialise(d3dFormat); }
    void Initialise(unsigned long d3dFormat);      // 0x009E3830
    bool IsSigned() const;                         // 0x009E3970
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
        return C2DExtentsI(desc.Width, desc.Height);
    }
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
    void ClearArea(const C2DBoxI& area, const CRGBColour& colour);
};

void CSurface::ClearArea(const C2DBoxI& area, const CRGBColour& colour)
{
    CSurfaceLockedRect locked = Lock(0);
    CPixelFormat format = GetPixelFormat();
    C2DBoxI box;
    box.left = area.left;
    box.top = area.top;
    box.right = area.right;
    box.bottom = area.bottom;
    if (box.left < 0)
        box.left = 0;
    else if (box.left > (long)GetWidth())
        box.left = GetWidth();
    if (box.top < 0)
        box.top = 0;
    else if (box.top > (long)GetHeight())
        box.top = GetHeight();
    if (box.right < 0)
        box.right = 0;
    else if (box.right > (long)GetWidth())
        box.right = GetWidth();
    if (box.bottom < 0)
        box.bottom = 0;
    else if (box.bottom > (long)GetHeight())
        box.bottom = GetHeight();
    unsigned char* row = locked.PMem + format.GetByteColourDepth() * box.left + locked.Pitch * box.top;
    for (long y = box.top; y < box.bottom; ++y) {
        unsigned char* p = row;
        for (long x = box.left; x < box.right; ++x) {
            format.WritePixel(colour, p);
            p += format.GetByteColourDepth();
        }
        row += locked.Pitch;
    }
    Unlock();
}