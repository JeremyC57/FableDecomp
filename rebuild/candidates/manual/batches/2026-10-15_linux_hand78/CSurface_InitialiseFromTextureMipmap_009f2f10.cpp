// CSurface::InitialiseFromTextureMipmap(IDirect3DSurface9* mipmapSurface) -- release the current Direct3D surface (Uninitialise, inlined) and adopt the given texture mip-level surface without an AddRef (the caller's GetSurfaceLevel reference is taken over), marking the allocation source as 3 (texture mipmap)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F2F10, length 66 bytes.
// Callees: IDirect3DSurface9::Release (COM slot 2, `this` pushed).
// Shape: the inlined Uninitialise (0x009F2E20) with its two zero stores KEPT even though the next two stores
// overwrite them, and the adopt-tail duplicated into both arms of the null test (each arm ends `pop esi ; ret 4`).
#include "engine/CSurface.h"

// The COM surface (completes the header's forward declaration): the IUnknown slots.
struct IDirect3DSurface9 {
    virtual long __stdcall QueryInterface(const void* iid, void** object);   // slot 0
    virtual unsigned long __stdcall AddRef();                                // slot 1 (+0x04)
    virtual unsigned long __stdcall Release();                               // slot 2 (+0x08)
};

// engine/CSurface.h (+4 PD3DSurface, +8 AllocationSource, +0xc PAllocatedMemory) with the methods this TU
// defines; Uninitialise (0x009F2E20) is defined in the same TU and auto-inlined (/Ob2), as in retail.
struct CSurface_Methods : CSurface {
    void Uninitialise();
    void InitialiseFromTextureMipmap(IDirect3DSurface9* mipmapSurface);
};

void CSurface_Methods::Uninitialise()
{
    if (PD3DSurface) {
        PD3DSurface->Release();
        PD3DSurface = 0;
        AllocationSource = 0;
    }
}

void CSurface_Methods::InitialiseFromTextureMipmap(IDirect3DSurface9* mipmapSurface)
{
    Uninitialise();
    PD3DSurface = mipmapSurface;
    AllocationSource = 3;
}
