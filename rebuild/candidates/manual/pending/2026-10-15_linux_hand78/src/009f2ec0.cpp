// CSurface::InitialiseFromPreallocatedSurface(IDirect3DSurface9* surface, bool addRef) -- release the current Direct3D surface (Uninitialise, inlined), adopt the given COM surface, record the allocation source as 1 when a surface was given (0 for null), and AddRef it when the caller asks
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F2EC0, length 65 bytes.
// Callees: IDirect3DSurface9::Release (COM slot 2) / AddRef (slot 1), `this` pushed.
// Shape: the inlined Uninitialise (0x009F2E20), then both arguments loaded together, `xor edx,edx ; test eax,eax ;
// setne dl` for `surface != 0` stored as the source word, `pop esi` before the flag branch, and the AddRef on the
// ARGUMENT register with no null test (`if (addRef) surface->AddRef();`), `ret 8`.
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
    void InitialiseFromPreallocatedSurface(IDirect3DSurface9* surface, bool addRef);
};

void CSurface_Methods::Uninitialise()
{
    if (PD3DSurface) {
        PD3DSurface->Release();
        PD3DSurface = 0;
        AllocationSource = 0;
    }
}

void CSurface_Methods::InitialiseFromPreallocatedSurface(IDirect3DSurface9* surface, bool addRef)
{
    Uninitialise();
    PD3DSurface = surface;
    AllocationSource = surface != 0;
    if (addRef)
        surface->AddRef();
}
