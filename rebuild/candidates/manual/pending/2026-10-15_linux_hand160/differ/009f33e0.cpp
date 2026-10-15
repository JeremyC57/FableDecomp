// CSurface::Lock(unsigned long flags) -- lock the whole surface (rect 0,0,Width,Height from GetDesc) and return {Dimensions, Pitch, PMem}; all zero when LockRect fails
// Grade: DIFFER(139v141) -- retail keeps desc.Width/Height in ebx/ebp loaded after `mov esi,[esi+4]` and stores rect bottom/right from them after the pushes (ours loads into eax/ecx and stores bottom early); failure arm retail `xor eax,eax ; mov edx,eax` zero pair vs our two xors
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F33E0, length 141 bytes (+3 int3 padding). Returns CSurfaceLockedRect (16 bytes,
// PDB: C2DExtentsI Dimensions +0, long Pitch +8, uchar* PMem +0xC) through the hidden pointer; `ret 8`.
// Callees: IDirect3DSurface9::GetDesc (vtable +0x30), IDirect3DSurface9::LockRect (vtable +0x34).
// Shape: D3DSURFACE_DESC at esp+0x28 (Width/Height = desc+0x18/+0x1C kept in ebx/ebp), RECT at esp+0x18, D3DLOCKED_RECT at
// esp+0x10. The rect fill interleaves with the argument pushes; the failure arm builds C2DExtentsI(0,0) through edx/ecx and
// reuses the rect's zero register for Pitch/PMem.
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
struct C2DExtentsI {
    long X, Y;
    C2DExtentsI(long x, long y) : X(x), Y(y) {}
};
struct CSurfaceLockedRect {
    C2DExtentsI Dimensions;     // +0x00
    long Pitch;                 // +0x08
    unsigned char* PMem;        // +0x0c
    CSurfaceLockedRect(const C2DExtentsI& dims, long pitch, unsigned char* mem) : Dimensions(dims), Pitch(pitch), PMem(mem) {}
};

struct CSurface {                               // vtable 0x0122F84C, 0x10 bytes
    virtual ~CSurface();
    IDirect3DSurface9* PD3DSurface;             // +0x04
    unsigned long AllocationSource;             // +0x08
    void* PAllocatedMemory;                     // +0x0c
    CSurfaceLockedRect Lock(unsigned long flags);
};

CSurfaceLockedRect CSurface::Lock(unsigned long flags)
{
    D3DSURFACE_DESC desc;
    PD3DSurface->GetDesc(&desc);
    C2DExtentsI size(desc.Width, desc.Height);
    RECT rect;
    rect.top = 0;
    rect.left = 0;
    rect.bottom = size.Y;
    rect.right = size.X;
    D3DLOCKED_RECT locked;
    if (PD3DSurface->LockRect(&locked, &rect, flags) >= 0)
        return CSurfaceLockedRect(size, locked.Pitch, (unsigned char*)locked.pBits);
    return CSurfaceLockedRect(C2DExtentsI(0, 0), 0, 0);
}
