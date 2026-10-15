// CVertexShader::~CVertexShader() -- release the counted vertex-shader data at +4 (count at +0x10, deleted through its virtual deleting destructor), then the inline CShaderBase destructor
// Leaf: ??1CVertexShader
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BE1E0, length 35 bytes.
// Catalog label "CPhysicsMeshInfo::~CPhysicsMeshInfo" is propagated: the vtables stored here are
// CVertexShader (0x0129B3D0) and its base CShaderBase (0x0129A030) per rtti-vtables.tsv.
// Callees: the data object's virtual `scalar deleting destructor` (vtable slot 0, `push 1 ; call [eax]`).
// Shape: the CPixelShader twin (0x00987E20): own vtable store, the data pointer read once into a
// local for the null test, `dec [ecx+10h] ; jne`, the virtual delete, then the base's inline
// destructor stores its vtable. The vertex-shader data keeps its count at +0x10 (declaration and
// two more words before it).
struct CVertexShaderData {
    virtual ~CVertexShaderData();                  // slot 0 (the `??_G` takes the flag 1)
    void* m_pShader;                               // +0x04
    void* m_pDeclaration;                          // +0x08
    long  m_stride;                                // +0x0c
    long  m_refCount;                              // +0x10
};

struct CShaderBase {                               // vtable 0x0129A030
    virtual void v00();
    ~CShaderBase() { }
};

struct CVertexShader : CShaderBase {               // vtable 0x0129B3D0
    CVertexShaderData* m_pData;                    // +0x04
    virtual void v00();
    ~CVertexShader();
};

CVertexShader::~CVertexShader()
{
    CVertexShaderData* data = m_pData;
    if (data) {
        if (--data->m_refCount == 0)
            delete data;
    }
}