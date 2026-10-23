// Grade: DIFFER(636v664) -- structure matches (box defaults, Lock, budget-limited inline GetWidth then out-of-line GetWidth/GetHeight, rep-movs memcpy); residue is register choice (zero in ebp v ebx, dst.bottom cached v memory) and retail's two descs at distinct frame words; tried member stores, C2DBoxI ctor temporaries, store orders, indexed v pointer-stepped row loop
// CSurface::CopyFromMemory(const C2DBoxI* dstBox, const void* data, const C2DBoxI* srcBox, long srcPitch) -- copy a linear buffer (or a box of it) into the surface: one block copy when both boxes cover the whole surface and the pitches agree, else row by row
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
class C2DBoxI { public: long left, top, right, bottom; };
struct C2DExtentsI { long X; long Y; C2DExtentsI(long x, long y) : X(x), Y(y) {} };
struct CSurfaceLockedRect { long DimX; long DimY; long Pitch; unsigned char* PMem; };
class CPixelFormat {
public:
    long FormatIndex;
    CPixelFormat() : FormatIndex(-1) {}
    CPixelFormat(unsigned long d3dFormat) { Initialise(d3dFormat); }
    void Initialise(unsigned long d3dFormat);      // 0x009E3830
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
    CSurfaceLockedRect Lock(unsigned long flags);                                                          // 0x009F33E0
    unsigned long GetByteLength() const;                                                                    // 0x009F2FC0
    void CopyFromMemory(const C2DBoxI* dstBox, const void* data, const C2DBoxI* srcBox, long srcPitch);
};
extern "C" void* __cdecl memcpy(void* dst, const void* src, unsigned int size);
#pragma intrinsic(memcpy)

void CSurface::CopyFromMemory(const C2DBoxI* dstBox, const void* data, const C2DBoxI* srcBox, long srcPitch)
{
    C2DBoxI dst;
    if (dstBox) {
        dst = *dstBox;
    } else if (srcBox) {
        dst.right = srcBox->right - srcBox->left;
        dst.left = 0; dst.top = 0;
        dst.bottom = srcBox->bottom - srcBox->top;
    } else {
        dst.bottom = GetHeight();
        dst.right = GetWidth();
        dst.left = 0; dst.top = 0;
    }
    C2DBoxI src;
    if (srcBox) {
        src = *srcBox;
    } else {
        src.left = 0; src.top = 0;
        src.right = dst.right - dst.left;
        src.bottom = dst.bottom - dst.top;
    }
    CSurfaceLockedRect locked = Lock(0);
    if (!locked.PMem)
        return;
    if (srcPitch == 0)
        srcPitch = locked.Pitch;
    CPixelFormat format(GetD3DFormat());
    if (srcPitch == locked.Pitch && src.left == 0 && src.top == 0 && src.right == (long)GetWidth() && src.bottom == (long)GetHeight()
        && dst.left == 0 && dst.top == 0 && dst.right == (long)GetWidth() && dst.bottom == (long)GetHeight()) {
        memcpy(locked.PMem, data, GetByteLength());
    } else {
        unsigned long bpp = (GetByteLength() * 8) / (GetWidth() * GetHeight());
        long rowBytes = ((src.right - src.left) * (long)bpp) / 8;
        const unsigned char* from = (const unsigned char*)data + ((long)bpp * src.left) / 8;
        unsigned char* to = locked.PMem + ((long)bpp * dst.left) / 8;
        to += src.top * locked.Pitch;
        from += src.top * srcPitch;
        for (long y = src.top; y < src.bottom; ++y) {
            memcpy(to, from, rowBytes);
            to += locked.Pitch;
            from += srcPitch;
        }
    }
    PD3DSurface->UnlockRect();
}
