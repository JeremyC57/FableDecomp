// CSurface::CopyFromSurface(const C2DBoxI* dstBox, const CSurface& source, const C2DBoxI* srcBox, unsigned long filter) -- copy a (sub)rectangle of another surface into this one; a signed destination fed from an unsigned source goes through a temporary A8R8G8B8 image (recursive copy, lock, signed conversion), everything else through D3DXLoadSurfaceFromSurface
// Leaf: CopyFromSurface
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F37E0, 446 bytes, `ret 0x10`. Replaces the landed asm_bake of this address.
// Callees: CPixelFormat::Initialise(D3DFORMAT) 0x009E3830, CPixelFormat::IsSigned() const 0x009E3970,
//   CSurface::CanDoProperSignedConversion 0x009F35D0, CSurface::InitialiseAsImage 0x009F3230, CSurface::Lock 0x009F33E0
//   (16-byte CSurfaceLockedRect by hidden pointer), CSurface::CopyFromUnsignedDataToSignedSurface 0x009F36E0,
//   itself (recursive), IDirect3DSurface9 GetDesc/UnlockRect/Release (+0x30/+0x38/+0x08), D3DXLoadSurfaceFromSurface
//   import thunk 0x00BFEFC2; vtable 0x0122F84C (temporary CSurface ctor + inline dtor).
// Shape: GetPixelFormat()/GetDimensions() are the landed inline getters (0x009F2F60/0x009F2F90) returning temporaries, so
//   every format/extent object dies with its full-expression and VC7.1 packs them (the source format lands in the dead
//   `source` argument slot); the temporary-format argument is the inline `CPixelFormat(D3DFORMAT){ Initialise(f); }`
//   ctor (no -1 store), the kept A8R8G8B8 format is the default ctor (`FormatIndex = -1`) + Initialise (the -1 store);
//   the locked rect lives in its own `{}` block so it shares the extents' frame words; the fallback builds both RECT
//   pointers as named locals zeroed up front, members copied top, bottom, left, right (retail store order).
// Layout: engine/CSurface.h models the vptr as a `__vftable` data member, which cannot express the temporary's vtable
//   stores, so the class is modelled locally with the PDB member names (as the landed ctor 0x009F2D60 does).
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
extern "C" long __stdcall D3DXLoadSurfaceFromSurface(IDirect3DSurface9* dst, const void* dstPalette, const RECT_Local* dstRect,
    IDirect3DSurface9* src, const void* srcPalette, const RECT_Local* srcRect, unsigned long filter, unsigned long colourKey);
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
    bool CanDoProperSignedConversion(const C2DBoxI* dst, const C2DBoxI* src, const CPixelFormat* format);   // 0x009F35D0
    bool InitialiseAsImage(const C2DExtentsI& dims, const CPixelFormat& format, bool dynamic);              // 0x009F3230
    CSurfaceLockedRect Lock(unsigned long flags);                                                          // 0x009F33E0
    void Unlock() { PD3DSurface->UnlockRect(); }
    void CopyFromUnsignedDataToSignedSurface(const void* data, const CPixelFormat* format, long pitch);    // 0x009F36E0
    void CopyFromSurface(const C2DBoxI* dstBox, const CSurface& source, const C2DBoxI* srcBox, unsigned long filter);
};

void CSurface::CopyFromSurface(const C2DBoxI* dstBox, const CSurface& source, const C2DBoxI* srcBox, unsigned long filter)
{
    if (GetPixelFormat().IsSigned() && !source.GetPixelFormat().IsSigned()
        && CanDoProperSignedConversion(0, 0, &CPixelFormat(0x15))) {
        CPixelFormat argb;
        argb.Initialise(0x15);
        CSurface temp;
        temp.InitialiseAsImage(GetDimensions(), argb, false);
        temp.CopyFromSurface(dstBox, source, srcBox, 0xffffffff);
        {
            CSurfaceLockedRect locked = temp.Lock(0);
            CopyFromUnsignedDataToSignedSurface(locked.PMem, &argb, locked.Pitch);
        }
        temp.Unlock();
    } else {
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
        D3DXLoadSurfaceFromSurface(PD3DSurface, 0, pDst, source.PD3DSurface, 0, pSrc, filter, 0);
    }
}
