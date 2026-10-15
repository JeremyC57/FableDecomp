// CSurface::CSurface(const CSurface& other) -- copy constructor: share the other surface's Direct3D surface and allocation source, copy the allocated-memory pointer only when the source owns one (AllocationSource 2), and AddRef the shared COM surface
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F2D60, length 55 bytes.
// Leaf: ??0CSurface
// Callees: IDirect3DSurface9::AddRef (COM slot 1, `call [edx+4]`, `this` pushed); vtable 0x0122F84C.
// Shape: the vtable store first, then the members copied in order through eax with the `cmp eax,2` on the
// copied source word, the memory pointer copied only on `== 2`, then the AddRef on the member re-read
// (`mov eax,[esi+4]`), `mov eax,esi`, `ret 4`.
struct IDirect3DSurface9 {                         // IUnknown slots of the COM surface
    virtual long __stdcall QueryInterface(const void* iid, void** object);   // slot 0
    virtual unsigned long __stdcall AddRef();                                // slot 1 (+0x04)
    virtual unsigned long __stdcall Release();                               // slot 2 (+0x08)
};

// CSurface with its vtable (0x0122F84C: one slot, the virtual destructor). engine/CSurface.h holds the same
// layout but models the vptr as a plain `__vftable` data member, which cannot express a constructor's vtable
// store, so the class is modelled locally with the PDB member names and offsets.
class CSurface {
public:
    virtual ~CSurface();                           // slot 0 (0x0041C9C0 deleting; calls Uninitialise 0x009F2E20)
    IDirect3DSurface9* PD3DSurface;                // +0x04
    unsigned long      AllocationSource;           // +0x08 (2 = owns PAllocatedMemory, 3 = texture mipmap)
    void*              PAllocatedMemory;           // +0x0c
    CSurface(const CSurface& other);
};

CSurface::CSurface(const CSurface& other)
{
    PD3DSurface = other.PD3DSurface;
    AllocationSource = other.AllocationSource;
    if (AllocationSource == 2)
        PAllocatedMemory = other.PAllocatedMemory;
    if (PD3DSurface)
        PD3DSurface->AddRef();
}
