// CPixelShaderData::`scalar deleting destructor'(unsigned int flags) -- the inline destructor (release the Direct3D pixel shader at +4, delete[] the constant index array at +0x10), the out-of-line base destructor, then operator delete when bit 0 of the flags is set
// Leaf: ??_GCPixelShaderData
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5ED00, length 63 bytes.
// Vtables: CPixelShaderData 0x0129D58C (slot 0 is this function, rtti-vtables.tsv).
// Callees: IDirect3DPixelShader9::Release (COM slot 2, `__stdcall`: `push eax ; call [ecx+8]`),
// operator delete[] @ 0x00BFEB1C, the base destructor @ 0x0099A300 (an out-of-line copy that only
// restores the CBaseClass vtable 0x01231710, like 0x0099A2E0), operator delete @ 0x00BFE9BC.
// Shape: PDB CPixelShaderData = vptr, PixelShader +4, ReferenceCount +8, ConstCount +0xC,
// ConstIndices +0x10 (modelled locally: polymorphic). The own vtable store is scheduled after the
// shader load; the shader is released without a clearing store (`if (PixelShader)
// PixelShader->Release()`), `delete[]` of the long array is a bare operator delete[] call; the
// `??_G` needs the virtual destructor `__forceinline` and a `new CPixelShaderData` factory so the
// TU emits the vtable.
void __cdecl operator delete[](void* block);   // MSVCR71 import thunk 0x00BFEB1C (the lane <new> lacks it)
struct IDirect3DPixelShader9 {
    virtual long __stdcall QueryInterface(const void* iid, void** out);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();     // slot 2
};

namespace N_0099a300 { struct CBaseClass {                                // vtable 0x01231710
    CBaseClass() { }
    virtual ~CBaseClass();                         // 0x0099A300
}; } using N_0099a300::CBaseClass;

struct CPixelShaderData : CBaseClass {             // vtable 0x0129D58C
    IDirect3DPixelShader9* PixelShader;            // +0x04
    unsigned long          ReferenceCount;         // +0x08
    unsigned long          ConstCount;             // +0x0c
    long*                  ConstIndices;           // +0x10
    CPixelShaderData() : PixelShader(0), ReferenceCount(1), ConstCount(0), ConstIndices(0) { }
    __forceinline virtual ~CPixelShaderData()
    {
        if (PixelShader)
            PixelShader->Release();
        delete[] ConstIndices;
    }
};

// The allocation that pulls CPixelShaderData's vtable (and its `??_G`) into this translation unit.
CPixelShaderData* NewPixelShaderData()
{
    return new CPixelShaderData;
}