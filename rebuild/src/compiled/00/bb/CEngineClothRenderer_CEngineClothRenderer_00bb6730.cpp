// CEngineClothRenderer::~CEngineClothRenderer() -- compiler-generated destructor: memory pools, index buffer, six vertex shaders and six CArray<CVertexShader> members torn down, then ~CBaseClassNonCopyable
// Leaf: ??1CEngineClothRenderer@@UAE@XZ
// Retail 0x00BB6730, 537 bytes. Compiler-generated: emitted by defining the real ctor in the TU (no vtable store of its own).
//   Members in reverse: two CSimpleMemoryPool (~ { ClearMemoryBlocks(); } 0x00A5C600), CIndexBuffer (Uninitialise 0x00AC1EE0),
//   six CVertexShader (shared-data release), six CArray<CVertexShader> (STLport-style storage, free 0x00BFEA14), then
//   ~CBaseClassNonCopyable 0x0099A300. Ego_r CArray is 16 bytes; retail's is 12 (offsets checked against the bytes).
#include "engine/CBaseClassNonCopyable.h"

extern "C" void __cdecl free(void* block);                       // 0x00BFEA14 (MSVCR71)

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* p, unsigned int) { free(p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* p) { p->~_Tp(); }
template <class _Tp> inline void _Destroy(_Tp* first, _Tp* last) { for (; first != last; ++first) _Destroy(first); }
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    vector();
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}

template <class T> class CArray : public std::vector<T> {
};

class CVertexShaderData {
public:
    virtual ~CVertexShaderData();                                // slot 0
    void* VertexDeclaration;
    void* VertexShader;
    const void* FormatDecl;
    long ReferenceCount;                                         // +0x10
    long RemoveReference() { return --ReferenceCount; }
};

class CShaderBase {                                              // vtable 0x0129A030
public:
    virtual ~CShaderBase() { }
};
class CVertexShader : public CShaderBase {                       // vtable 0x0129B3D0, 8 bytes
public:
    CVertexShaderData* Data;
    CVertexShader();                                             // 0x00A62FB0
    virtual ~CVertexShader() { if (Data) { CVertexShaderData* current = Data; if (!current->RemoveReference()) delete current; } }
};

class CMovableResource {                                         // vtable 0x0129B860
public:
    virtual ~CMovableResource() { }
};
class CIndexBuffer : public CMovableResource {                   // vtable 0x0129EF2C, 0x14 bytes
public:
    void* PD3DIndexBuffer;
    unsigned long ByteLength;
    long PrimitiveType;
    void* PAllocatedBuffer;
    CIndexBuffer();                                              // 0x00AC1EC0
    virtual ~CIndexBuffer() { Uninitialise(); }
    void Uninitialise();                                         // 0x00AC1EE0
};

class CSimpleMemoryPool : public CBaseClassNonCopyable {         // vtable 0x0129A064, 0x1C bytes
public:
    CSimpleMemoryPool(unsigned long capacity, unsigned long growthGranularity);   // 0x00A5C570
    virtual ~CSimpleMemoryPool() { ClearMemoryBlocks(); }
private:
    void ClearMemoryBlocks();                                    // 0x00A5C600
    void* FirstMemoryBlock;
    unsigned long Capacity;
    unsigned long GrowthGranularity;
    void* Root[3];
};

class C4DVector;

class CEnginePrimitiveRenderUtil : public CBaseClassNonCopyable {
public:
    virtual void FrameStart();
};

class CEngineClothRenderer : public CEnginePrimitiveRenderUtil {
public:
    CEngineClothRenderer();
    virtual void FrameStart();
    CArray<CVertexShader> TriVertexShaders;                         // +0x04
    CArray<CVertexShader> TriVertexShadersDepthBufferShadow;        // +0x10
    CArray<CVertexShader> TriVertexShadersSpotDepthBufferShadow;    // +0x1C
    CArray<CVertexShader> TriVertexShadersColourBufferShadow;       // +0x28
    CArray<CVertexShader> TriVertexShadersSpotColourBufferShadow;   // +0x34
    CVertexShader* TriVertexShadersShadow;                          // +0x40
    CVertexShader* TriVertexShadersSpotShadow;                      // +0x44
    CArray<CVertexShader> PatchVertexShaders;                       // +0x48
    CVertexShader TriVertexShaderOutline;                           // +0x54
    CVertexShader TriVertexShaderOutlineSmoothFill;                 // +0x5C
    CVertexShader TriVertexShaders2D;                               // +0x64
    CVertexShader TriBackfaceVertexShaders2D;                       // +0x6C
    CVertexShader PatchVertexShaders2D;                             // +0x74
    CVertexShader DebugVertexShader;                                // +0x7C
    C4DVector* VertexNormals;                                       // +0x84
    bool ClothDebug;                                                // +0x88
    CIndexBuffer IndexBuffer;                                       // +0x8C
    long IndexBufferSize;                                           // +0xA0
    bool EnableCloth;                                               // +0xA4
    CSimpleMemoryPool PositionBufferPool;                           // +0xA8
    C4DVector* PositionBuffer;                                      // +0xC4
    unsigned long PositionBufferSize;                               // +0xC8
    CSimpleMemoryPool NormalBufferPool;                             // +0xCC
    C4DVector* NormalBuffer;                                        // +0xE8
    unsigned long NormalBufferSize;                                 // +0xEC
};

CEngineClothRenderer::CEngineClothRenderer()
    : PositionBufferPool(0, 0x1000), NormalBufferPool(0, 0x1000)
{
}