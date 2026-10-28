// CSurface::CopyToMemory(const C2DBoxI* destBox, void* data, const C2DBoxI* srcBox, long pitch) -- lock the surface and copy it into linear memory: compressed formats row by row (the source rectangle's rows, the given or locked pitch), others as one GetByteLength block; then unlock
// Grade: DIFFER(377v346) -- the copy rectangle is wrong-shaped: retail keeps only top/bottom live (esi/edi: srcBox's TLY/BRY with TLX/BRX spilled to frame words the locked rect reuses; 0 and destBox's height; 0 and the surface height, its second GetDesc's width dead), our C2DBoxI local stores all four fields in every arm and grows the frame to 0x54. 1 grade (probe/f39a0_a); the row/whole copy tail matches in shape.
// Leaf: ?CopyToMemory@CSurface@@QAEXPBVC2DBoxI@@PAX0J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F39A0, 346 bytes, thiscall, `ret 10h`; no direct callers (reached through other
//   modules' pointers or dead in the shipped build).
// Identity: Ego_r.pdb `?CopyToMemory@CSurface@@QAEXPBVC2DBoxI@@PAX0J@Z`: donor order CopyFromSurface (landed 0x009F37E0),
//   CopyToMemory, CopyFromMemory matches retail, and the body (Lock, copy from PMem into the void* argument, Unlock) is the
//   to-memory direction with the PDB's (box, void*, box, long) signature. The manifest label "CSurface_CopySurfaceData" is
//   descriptive.
// Callees: IDirect3DSurface9 GetDesc / UnlockRect (+0x30 / +0x38), CSurface::Lock 0x009F33E0, CPixelFormat::Initialise
//   0x009E3830 (into the dead srcBox argument slot), CSurface::GetByteLength 0x009F2FC0, CPixelFormat::GetByteColourDepth
//   0x009E3800; the pixel-format table 0x0129BA60 (IsCompressed).
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int count);
#pragma intrinsic(memcpy)
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
class C2DBoxI { public:
    long TLX, TLY, BRX, BRY;
    C2DBoxI() { }
    C2DBoxI(long tlx, long tly, long brx, long bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) { }
    long GetWidth() const { return BRX - TLX; }
    long GetHeight() const { return BRY - TLY; }
};
class C2DExtentsI { public: long X; long Y; C2DExtentsI(long x, long y) : X(x), Y(y) {} };
class CSurfaceLockedRect { public: C2DExtentsI Dimensions; long Pitch; unsigned char* PMem; };
struct SPixelFormatInfo {                                  // 36-byte rows
    bool          IsCompressed;
    unsigned char _pad01[0x23];
};
extern SPixelFormatInfo g_PixelFormatInfo[];               // 0x0129BA60
class CPixelFormat {
public:
    long FormatIndex;
    CPixelFormat(unsigned long d3dFormat) { Initialise(d3dFormat); }
    void Initialise(unsigned long d3dFormat);              // 0x009E3830
    unsigned long GetByteColourDepth() const;              // 0x009E3800
    bool IsCompressed() const { return g_PixelFormatInfo[FormatIndex].IsCompressed; }
};
class CSurface {
public:
    virtual ~CSurface();
    IDirect3DSurface9* PD3DSurface;                        // +0x04
    unsigned long      AllocationSource;                   // +0x08
    void*              PAllocatedMemory;                   // +0x0c
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
    unsigned long GetByteLength() const;                   // 0x009F2FC0
    CSurfaceLockedRect Lock(unsigned long flags);          // 0x009F33E0
    void Unlock() const { PD3DSurface->UnlockRect(); }
    void CopyToMemory(const C2DBoxI* destBox, void* data, const C2DBoxI* srcBox, long pitch);
};

void CSurface::CopyToMemory(const C2DBoxI* destBox, void* data, const C2DBoxI* srcBox, long pitch)
{
    C2DBoxI box;
    if (srcBox)
        box = *srcBox;
    else if (destBox)
        box = C2DBoxI(0, 0, destBox->GetWidth(), destBox->GetHeight());
    else
        box = C2DBoxI(0, 0, GetDimensions().X, GetDimensions().Y);
    CSurfaceLockedRect locked = Lock(0);
    if (locked.PMem) {
        if (pitch == 0)
            pitch = locked.Pitch;
        CPixelFormat format = GetPixelFormat();
        if (!format.IsCompressed()) {
            memcpy(data, locked.PMem, GetByteLength());
            Unlock();
            return;
        }
        unsigned long rowBytes = format.GetByteColourDepth() * GetDimensions().X;
        unsigned char* src = locked.PMem;
        unsigned char* dst = (unsigned char*)data;
        for (long rows = box.BRY - box.TLY; rows > 0; --rows) {
            memcpy(dst, src, rowBytes);
            src += locked.Pitch;
            dst += pitch;
        }
        Unlock();
    }
}
