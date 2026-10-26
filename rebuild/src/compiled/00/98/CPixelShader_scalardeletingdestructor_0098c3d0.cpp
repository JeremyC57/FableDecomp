// CPixelShader::`scalar deleting destructor'(unsigned int) -- release the shared CPixelShaderData (delete at zero references), CShaderBase vtable restore, free on bit 0
// Leaf: ??_GCPixelShader
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0098C3D0, length 55 bytes.
// Callees: operator delete @ 0x00BFE9BC; the data object's virtual destructor is called through slot 0 with
// the delete flag 1. Vtables: 0x0129A054 = CPixelShader (rtti-vtables.tsv, slot 0 is this function),
// 0x0129A030 = CShaderBase (the inline base destructor's store). The chunk label
// "CVertexShader::`vector_deleting_destructor'" is the propagated name of the byte-identical shape at
// 0x009C1AE0 (CVertexShader, whose data object counts at +0x10 instead of +8).
// Shape: `~CPixelShader() { T* d = Data; if (d) { if (--d->m_refCount == 0) delete d; } }` -- the explicit
// null test, `dec dword ptr [ecx+8] ; jne`, the plain `delete` without a second null test, then the
// CShaderBase vtable store and the `test byte ptr [esp+8],1` free. engine/CVertexShader.h gives the
// 8-byte {vptr, Data} layout; it is a flat record so the polymorphic model is local.
struct CPixelShaderData {
    virtual ~CPixelShaderData();              // slot 0: the deleting destructor is called with 1
    void* m_pShader;                          // +0x04
    long  m_refCount;                         // +0x08
};

class CShaderBase { public:                          // vtable 0x0129A030
    virtual ~CShaderBase() { }
};

class CPixelShader : public CShaderBase { public:           // vtable 0x0129A054
    CPixelShaderData* Data;                   // +0x04
    __forceinline virtual ~CPixelShader()      // __forceinline keeps the release in the ??_G body (else it collapses to `call ??1`)
    {
        CPixelShaderData* data = Data;              // a local copy: the member form re-reads Data after the decrement
        if (data) {
            if (--data->m_refCount == 0)
                delete data;
        }
    }
};

CPixelShader* CreatePixelShader() { return new CPixelShader; }   // pulls the vtable (and ??_G) into the TU