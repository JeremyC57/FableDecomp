// CShaderResource::CShaderResource() -- CBaseClassNonCopyable base, this class's vtable, the shader bank and the shader array cleared, the vertex-shader input built out of line, the declaration / vertex format cleared and the render manager cleared in the body
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5D870, length 51 bytes.
// Leaf: ??0CShaderResource
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable() @ 0x0099A2F0, CVertexShaderInput::CVertexShaderInput() @ 0x00AB3B10.
// Layout: retail CArray is 12 bytes, so everything after Shaders sits 4 bytes below the PDB offsets
// (ShaderRenderManager +0x14, VertexShaderInput +0x18 (0x1c bytes), VertexShaderDeclaration +0x34, VertexFormat +0x38).
// Shape: vtable 0x0129D580 stored right after the base call, then the init-list zeroes in declaration order
// (+4, the array's three words) through edi, the member constructor, +0x34 / +0x38, and +0x14 LAST: a body assignment.
// The Shaders CArray is an STLport vector built by its default constructor (_Vector_base(allocator_type()) with the
// _STLP_alloc_proxy end-of-storage): that structure puts `lea ecx,[esi+18h]` after the third zero store as in retail
// (a plain three-pointer CArray schedules it right after the first -- the 24i "lane gap").
struct CBaseClassNonCopyable {                  // vtable 0x0129A7C4
    virtual ~CBaseClassNonCopyable();
    CBaseClassNonCopyable();                    // 0x0099A2F0
};

struct CShaderDataBank;
struct CShaderRenderManager;
struct CShaderBase;
struct _D3DVERTEXELEMENT9;

template <class T> struct CAllocator {         // STLport allocator<T>: empty
    CAllocator() { }
};

template <class T> struct CAllocProxy : CAllocator<T> {    // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* m_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), m_data(p) { }
};

template <class T> struct CVectorBase {        // STLport _Vector_base<T, allocator<T> >
    T*             m_pStart;
    T*             m_pFinish;
    CAllocProxy<T> m_endOfStorage;
    CVectorBase(const CAllocator<T>& alloc) : m_pStart(0), m_pFinish(0), m_endOfStorage(alloc, 0) { }
};

template <class T> struct CSTLportVector : CVectorBase<T> {     // STLport vector<T>
    explicit CSTLportVector(const CAllocator<T>& alloc = CAllocator<T>()) : CVectorBase<T>(alloc) { }
};

template <class T> struct CArray : CSTLportVector<T> { };


struct CVertexShaderInput {                     // vtable 0x0129EA08, 0x1c bytes
    virtual ~CVertexShaderInput();
    unsigned char _body[0x18];
    CVertexShaderInput();                       // 0x00AB3B10
};

struct CShaderResource : CBaseClassNonCopyable {    // vtable 0x0129D580
    CShaderDataBank*      ShaderBank;               // +0x04
    CArray<CShaderBase*>  Shaders;                  // +0x08
    CShaderRenderManager* ShaderRenderManager;      // +0x14
    CVertexShaderInput    VertexShaderInput;        // +0x18
    _D3DVERTEXELEMENT9*   VertexShaderDeclaration;  // +0x34
    long                  VertexFormat;             // +0x38
    CShaderResource();
    virtual ~CShaderResource();
};

CShaderResource::CShaderResource()
    : ShaderBank(0), VertexShaderDeclaration(0), VertexFormat(0)
{
    ShaderRenderManager = 0;
}