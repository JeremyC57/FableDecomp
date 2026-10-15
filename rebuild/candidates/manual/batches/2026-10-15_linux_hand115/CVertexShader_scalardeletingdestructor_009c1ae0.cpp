// CVertexShader::`scalar deleting destructor'(unsigned int) -- release the shared CVertexShaderData (delete at zero references), CShaderBase vtable restore, free on bit 0
// Leaf: ??_GCVertexShader
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C1AE0, length 55 bytes.
// Callees: operator delete @ 0x00BFE9BC; the data object's virtual destructor is called through slot 0 with
// the delete flag 1. Vtables: 0x0129B3D0 = CVertexShader (rtti-vtables.tsv, slot 0 is this function),
// 0x0129A030 = CShaderBase (the inline base destructor's store). The manifest calls it a
// `vector_deleting_destructor'; the `test byte ptr [esp+8],1` / `ret 4` body is the scalar one (??_G).
// Layout: the data object holds two more words before its count (+0x10).
// Shape: `~CVertexShader() { T* d = Data; if (d) { if (--d->m_refCount == 0) delete d; } }` -- the explicit
// null test, `dec dword ptr [ecx+10h] ; jne`, the plain `delete` without a second null test, then the
// CShaderBase vtable store and the `test byte ptr [esp+8],1` free. engine/CVertexShader.h gives the
// 8-byte {vptr, Data} layout; it is a flat record so the polymorphic model is local.
struct CVertexShaderData {
    virtual ~CVertexShaderData();              // slot 0: the deleting destructor is called with 1
    void* m_pShader;                          // +0x04
    void* m_pDeclaration;                     // +0x08
    long  m_handle;                           // +0x0c
    long  m_refCount;                         // +0x10
};

struct CShaderBase {                          // vtable 0x0129A030
    virtual ~CShaderBase() { }
};

struct CVertexShader : CShaderBase {           // vtable 0x0129B3D0
    CVertexShaderData* Data;                   // +0x04
    __forceinline virtual ~CVertexShader()     // __forceinline keeps the release in the ??_G body (else it collapses to `call ??1`)
    {
        CVertexShaderData* data = Data;              // a local copy: the member form re-reads Data after the decrement
        if (data) {
            if (--data->m_refCount == 0)
                delete data;
        }
    }
};

CVertexShader* CreatePixelShader() { return new CVertexShader; }   // pulls the vtable (and ??_G) into the TU
