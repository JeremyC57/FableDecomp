// CTexture::UnswizzleAll(unsigned long) -- for every mip level, wrap the level surface in a CSurface and run its 0x009F4330 swizzle pass
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F9FA0, 143 bytes. __thiscall, `ret 4`. RENDER LAYER (D3D9): IDirect3DTexture9 at +0.
// Callees: IDirect3DTexture9::GetLevelCount (COM slot 13, +0x34) and GetSurfaceLevel (slot 18, +0x48);
//          CSurface::InitialiseFromTextureMipmap(IDirect3DSurface9*) 0x009F2F10; CSurface::CSurface(const CSurface&) 0x009F2D60;
//          CSurface::Uninitialise() 0x009F2E20 (inline ~CSurface, vtable 0x0122F84C re-stored first); CSurface member
//          0x009F4330 (catalog "CSurface::Swizzle(unsigned long)"; it is the per-surface pass this function's name says
//          undoes) with the argument.
// Shape: `for (long i = 0; i < levels; ++i)` (signed, levels in ebx, CSurface vtable hoisted into ebp); the landed
//        CTexture::GetSurfaceLevel 0x009F9E00 is INLINED here (out pointer, zeroed local CSurface, adopt, copy out with the
//        copy ctor, destroy the local), then the result runs the pass and is destroyed.
// Layout: PDB CTexture: PD3DTexture +0x00, ByteLength:28 / AllocationSource:4 at +0x04; CSurface vptr +0, PD3DSurface +4,
//         AllocationSource +8, PAllocatedMemory +0xC (models copied from the landed 0x009F9E00 source).
struct IDirect3DSurface9;

struct IDirect3DTexture9 {                         // IUnknown + IDirect3DResource9 + IDirect3DBaseTexture9 + texture slots
    virtual long __stdcall QueryInterface(const void* iid, void** object);   // slot 0
    virtual unsigned long __stdcall AddRef();                                // slot 1
    virtual unsigned long __stdcall Release();                               // slot 2
    virtual long __stdcall GetDevice(void** device);                         // slot 3
    virtual long __stdcall SetPrivateData(const void* guid, const void* data, unsigned long size, unsigned long flags);
    virtual long __stdcall GetPrivateData(const void* guid, void* data, unsigned long* size);
    virtual long __stdcall FreePrivateData(const void* guid);
    virtual unsigned long __stdcall SetPriority(unsigned long priority);
    virtual unsigned long __stdcall GetPriority();
    virtual void __stdcall PreLoad();
    virtual long __stdcall GetType();
    virtual unsigned long __stdcall SetLOD(unsigned long lod);
    virtual unsigned long __stdcall GetLOD();
    virtual unsigned long __stdcall GetLevelCount();
    virtual long __stdcall SetAutoGenFilterType(long filter);
    virtual long __stdcall GetAutoGenFilterType();
    virtual void __stdcall GenerateMipSubLevels();                           // slot 16
    virtual long __stdcall GetLevelDesc(unsigned long level, void* desc);    // slot 17 (+0x44)
    virtual long __stdcall GetSurfaceLevel(unsigned long level, IDirect3DSurface9** surface);   // slot 18 (+0x48)
    virtual long __stdcall LockRect(unsigned long level, void* lockedRect, const void* rect, unsigned long flags);   // slot 19 (+0x4c)
};

class CSurface {                                   // vtable 0x0122F84C
public:
    IDirect3DSurface9* PD3DSurface;                // +0x04
    unsigned long      AllocationSource;           // +0x08
    void*              PAllocatedMemory;           // +0x0c

    CSurface() : PD3DSurface(0), AllocationSource(0) { }
    CSurface(const CSurface& other);                                   // 0x009F2D60
    virtual ~CSurface() { Uninitialise(); }
    void Uninitialise();                                               // 0x009F2E20
    void InitialiseFromTextureMipmap(IDirect3DSurface9* mipmapSurface);   // 0x009F2F10
    void Swizzle(unsigned long format);                                // 0x009F4330 (catalog name; called by UnswizzleAll)
};

class CTexture {
public:
    IDirect3DTexture9* PD3DTexture;                // +0x00
    unsigned long      ByteLength : 28;            // +0x04
    unsigned long      AllocationSource : 4;       // +0x04
    CSurface GetSurfaceLevel(unsigned long level) const
    {
        IDirect3DSurface9* d3dSurface;
        PD3DTexture->GetSurfaceLevel(level, &d3dSurface);
        CSurface surface;
        surface.InitialiseFromTextureMipmap(d3dSurface);
        return surface;
    }
    void UnswizzleAll(unsigned long format);
};

void CTexture::UnswizzleAll(unsigned long format)
{
    long levels = PD3DTexture->GetLevelCount();
    for (long i = 0; i < levels; ++i) {
        CSurface surface = GetSurfaceLevel(i);
        surface.Swizzle(format);
    }
}
