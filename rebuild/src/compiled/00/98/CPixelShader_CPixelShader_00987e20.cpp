// CPixelShader::~CPixelShader() -- release the counted shader data at +4 (count at +8, deleted through its virtual deleting destructor), then the inline CShaderBase destructor
// Leaf: ??1CPixelShader
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00987E20, length 35 bytes.
// Catalog label "CPhysicsMeshInfo::~CPhysicsMeshInfo" is propagated: the vtables stored here are
// CPixelShader (0x0129A054) and its base CShaderBase (0x0129A030) per rtti-vtables.tsv.
// Callees: the data object's virtual `scalar deleting destructor` (vtable slot 0, `push 1 ; call [eax]`).
// Shape: own vtable store first (scheduled after the member load), the data pointer read ONCE into
// a local (ecx) for the null test, the `dec [ecx+8] ; jne` and the virtual delete (a `--m_pData->...`
// form re-reads the member), no clearing store, then the base's inline destructor stores its vtable.
struct CPixelShaderData {
    virtual ~CPixelShaderData();                   // slot 0 (the `??_G` takes the flag 1)
    void* m_pShader;                               // +0x04
    long  m_refCount;                              // +0x08
};

class CShaderBase { public:                               // vtable 0x0129A030
    virtual void v00();
    ~CShaderBase() { }
};

class CPixelShader : public CShaderBase { public:                // vtable 0x0129A054
    CPixelShaderData* m_pData;                     // +0x04
    virtual void v00();
    ~CPixelShader();
};

CPixelShader::~CPixelShader()
{
    CPixelShaderData* data = m_pData;
    if (data) {
        if (--data->m_refCount == 0)
            delete data;
    }
}