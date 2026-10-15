// CSurface::operator=(const CSurface& other) -- release the current Direct3D surface (Uninitialise, inlined), then share the other surface's COM surface and allocation source, copy the allocated-memory pointer when the source owns one (AllocationSource 2), AddRef the shared surface and return *this
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F2E70, length 76 bytes.
// Leaf: ??4CSurface_Methods
// Callees: IDirect3DSurface9::Release (COM slot 2) / AddRef (slot 1), `this` pushed.
// Shape: the inlined Uninitialise (0x009F2E20: null test, Release, two immediate zero stores), then the copy
// exactly as in the copy constructor 0x009F2D60 (`cmp eax,2` on the copied word, the AddRef on the re-read
// member), `mov eax,esi`, `ret 4`. Uninitialise is defined in the same TU and auto-inlined (/Ob2).
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
    CSurface_Methods& operator=(const CSurface& other);
};

void CSurface_Methods::Uninitialise()
{
    if (PD3DSurface) {
        PD3DSurface->Release();
        PD3DSurface = 0;
        AllocationSource = 0;
    }
}

CSurface_Methods& CSurface_Methods::operator=(const CSurface& other)
{
    Uninitialise();
    PD3DSurface = other.PD3DSurface;
    AllocationSource = other.AllocationSource;
    if (AllocationSource == 2)
        PAllocatedMemory = other.PAllocatedMemory;
    if (PD3DSurface)
        PD3DSurface->AddRef();
    return *this;
}
