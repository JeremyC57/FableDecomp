// CVertexShaderData::CVertexShaderData(IUnknown* shader, IUnknown* declaration, long handle, long unused4, long unused5) -- CBase, the shader object, own vtable, the declaration and handle, reference count 1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A62FC0, length 48 bytes.
// Callees: CBase::CBase @ 0x0099A2F0. Vtable 0x0129DA50 = CVertexShaderData (rtti-vtables.tsv).
// Shape: `ret 14h` = five stack arguments, only the first three are read; the first is stored at +4 BEFORE
// the vtable store (an argument-carrying `__declspec(novtable)` intermediate base over CBase), the other
// two and the constant 1 (the reference count that the CVertexShader deleting destructor decrements at +0x10)
// after it. The engine header CVertexShader.h has no layout for the data class, so it is modelled here.
struct CBase {
    virtual void v00();
    CBase();                                  // 0x0099A2F0
};

struct __declspec(novtable) CVertexShaderDataBase : CBase {   // the word stored before the derived vtable
    void* m_pShader;                          // +0x04
    CVertexShaderDataBase(void* shader) : CBase(), m_pShader(shader) { }
};

struct CVertexShaderData : CVertexShaderDataBase {            // vtable 0x0129DA50
    void* m_pDeclaration;                     // +0x08
    long  m_handle;                           // +0x0c
    long  m_refCount;                         // +0x10
    virtual void v00();
    CVertexShaderData(void* shader, void* declaration, long handle, long unused4, long unused5);
};

CVertexShaderData::CVertexShaderData(void* shader, void* declaration, long handle, long unused4, long unused5)
    : CVertexShaderDataBase(shader), m_pDeclaration(declaration), m_handle(handle), m_refCount(1)
{
}