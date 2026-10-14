// CEnginePrimitiveManagerLines::~CEnginePrimitiveManagerLines() -- release the vertex shader held at +0xc (its inlined destructor: the CVertexShader vtable, drop the shared shader's count and delete it at zero, the CShaderBase vtable), then ~CBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC9A00, length 43 bytes.
// Callees: the shared shader's virtual deleting destructor (slot 0, flag 1), CBase::~CBase @ 0x0099A300 (a tail jump). Vtables 0x0129B3D0 = CVertexShader,
// 0x0129A030 = CShaderBase (rtti-vtables.tsv). Shape: no vtable store of this class's own (`__declspec(novtable)`: the manager base is abstract), the member
// destructor is inlined (`mov ecx,[esi+10h] ; test ; mov [esi+0Ch],CVertexShader ; je ; dec [ecx+10h] ; jne ; call [eax]`), then the base destructor.
// The chunk label "vector deleting destructor" is wrong: there is no flag test; this is the plain destructor.
struct CRefCountedShader_00bc9a00 {              // the shared shader object: an intrusive count at +0x10 and a virtual deleting destructor in slot 0
    virtual ~CRefCountedShader_00bc9a00();
    unsigned char _pad[0xc];
    long          m_refCount;                      // +0x10
};

struct CShaderBase {                               // vtable 0x0129A030
    virtual void v00();
    ~CShaderBase() { }
};

struct CVertexShader : CShaderBase {               // vtable 0x0129B3D0: 8 bytes
    CRefCountedShader_00bc9a00* m_pShader;         // +0x04
    ~CVertexShader()
    {
        CRefCountedShader_00bc9a00* shader = m_pShader;   // a local: the decrement's flags feed the test directly (no re-read of the member)
        if (shader) {
            if (--shader->m_refCount == 0)
                delete shader;
        }
    }
};

struct CBase {
    virtual void v00();
    ~CBase();                                      // 0x0099A300
};

struct __declspec(novtable) CEnginePrimitiveManagerLines : CBase {
    long          m_type;                          // +0x04
    bool          m_enabled;                       // +0x08
    CVertexShader m_vertexShader;                  // +0x0c
    ~CEnginePrimitiveManagerLines();
};

CEnginePrimitiveManagerLines::~CEnginePrimitiveManagerLines()
{
}