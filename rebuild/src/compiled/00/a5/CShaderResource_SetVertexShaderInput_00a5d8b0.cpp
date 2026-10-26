// CShaderResource::SetVertexShaderInput(const CVertexShaderInput& input) -- assigns both arrays of the vertex-shader input (out-of-line CArray assignments) and points the declaration at the element array's data
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5D8B0, length 66 bytes.
// Callees: CArray<SStreamInput>::operator= @ 0x00A5E9C0, CArray<_D3DVERTEXELEMENT9>::operator= @ 0x00A5E2E0.
// Shape: the implicit CVertexShaderInput::operator= inlined (the vptr is not copied: +4 and +0x10 of the input),
// then the element array's data pointer -- the first element, or the array object itself when it is empty --
// through ONE inline GetData(), so the declaration store is tail-duplicated into both arms (`ret 4` twice).
struct _D3DVERTEXELEMENT9 {                     // 8 bytes
    unsigned short Stream;
    unsigned short Offset;
    unsigned char  Type;
    unsigned char  Method;
    unsigned char  Usage;
    unsigned char  UsageIndex;
};

struct SStreamInput;

template <class T> class CArray { public:              // retail CArray: first, last, end (12 bytes)
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    CArray& operator=(const CArray& other);     // out of line (0x00A5E9C0 / 0x00A5E2E0)
    unsigned long Size() const { return m_pLast - m_pFirst; }
    T* GetData() { return Size() > 0 ? m_pFirst : (T*)this; }
};

class CBaseClass { public:                             // vtable 0x01231710
    virtual ~CBaseClass();
};

class CVertexShaderInput : public CBaseClass { public:        // vtable 0x0129EA08, 0x1c bytes
    CArray<SStreamInput>       Streams;         // +0x04
    CArray<_D3DVERTEXELEMENT9> Elements;        // +0x10
};

class CShaderDataBank;
class CShaderRenderManager;
class CShaderBase;

class CShaderResource { public:
    void*                 __vftable;                // +0x00
    CShaderDataBank*      ShaderBank;               // +0x04
    CArray<CShaderBase*>  Shaders;                  // +0x08
    CShaderRenderManager* ShaderRenderManager;      // +0x14
    CVertexShaderInput    VertexShaderInput;        // +0x18
    _D3DVERTEXELEMENT9*   VertexShaderDeclaration;  // +0x34
    long                  VertexFormat;             // +0x38
    void SetVertexShaderInput(const CVertexShaderInput& input);
};

void CShaderResource::SetVertexShaderInput(const CVertexShaderInput& input)
{
    VertexShaderInput = input;
    VertexShaderDeclaration = VertexShaderInput.Elements.GetData();
}