// CEnginePrimitiveManagerParticleSpriteGroup::~CEnginePrimitiveManagerParticleSpriteGroup() -- compiler-generated destructor: the index buffer and vertex buffer released, the three pixel and three vertex displacement shaders released, the three CArray<CVertexShader> point-light shader tables destroyed and freed, then the CBaseClassNonCopyable base
// Leaf: ??1CEnginePrimitiveManagerParticleSpriteGroup
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC2280, length 361 bytes (tail `jmp` to the base destructor).
// Callees: CIndexBuffer::Uninitialise @ 0x00AC1EE0 (manifest "CVolume::ReleaseVolume", propagated), CVertexBufferWin32::
//   Uninitialise @ 0x00A63200, ~CBaseClassNonCopyable @ 0x0099A300 (vertex buffer base and the class's own root),
//   free @ 0x00BFEA14 (MSVCR71 import). Vtables: 0x0129EF2C CIndexBuffer, 0x0129B860 CMovableResource, 0x0129BA20
//   CVertexBufferWin32, 0x0129A054 CPixelShader, 0x0129B3D0 CVertexShader, 0x0129A030 CShaderBase (rtti-vtables.tsv).
// Layout: engine/CEnginePrimitiveManagerParticleSpriteGroup.h has the PDB offsets with 16-byte CArrays; retail CArrays are
//   12 bytes, so VertexShadersBillboard/Crossed/3DFaceMeSpritePointLights sit at +0x0c/+0x18/+0x24, the displacement
//   vertex shaders at +0x30/+0x38/+0x40, PixelShader/Stipple/Displacement at +0x48/+0x50/+0x58, VertexBuffer +0x60,
//   IndexBuffer +0x94 (all 4 bytes per preceding array lower). Modelled locally with the real member classes.
// Shape: the class declares no destructor (no own vtable store); members are destroyed in reverse order with their inline
//   destructors (CIndexBuffer own vtable + release + CMovableResource vtable; the vertex buffer's uninitialise + out-of-line
//   base; each shader's null-tested `dec ; jne ; call [vt]` release; each CArray's virtual element destructor loop with
//   `last` in ebx, then `free(start)` behind a null test), and the root destructor is tail-jumped.
extern "C" void __cdecl free(void* block);              // 0x00BFEA14

class CBaseClassNonCopyable {
public:
    virtual ~CBaseClassNonCopyable();                   // 0x0099A300
};

struct CPixelShaderData {
    virtual ~CPixelShaderData();                   // slot 0: the deleting destructor is called with 1
    void* m_pShader;                               // +0x04
    long  m_refCount;                              // +0x08
};
struct CVertexShaderData {
    virtual ~CVertexShaderData();                  // slot 0
    unsigned char _pad04[0xc];
    long  m_refCount;                              // +0x10
};

class CShaderBase { public:                               // vtable 0x0129A030
    virtual ~CShaderBase() { }
};
class CPixelShader : public CShaderBase { public:                // vtable 0x0129A054
    CPixelShaderData* Data;                        // +0x04
    __forceinline virtual ~CPixelShader()
    {
        CPixelShaderData* data = Data;
        if (data) {
            if (--data->m_refCount == 0)
                delete data;
        }
    }
};
class CVertexShader : public CShaderBase { public:               // vtable 0x0129B3D0
    CVertexShaderData* Data;                       // +0x04
    __forceinline virtual ~CVertexShader()
    {
        CVertexShaderData* data = Data;
        if (data) {
            if (--data->m_refCount == 0)
                delete data;
        }
    }
};

template <class T> __forceinline void DestroyRange(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}

template <class T> class CArray { public:                 // retail CArray<T> (STLport vector), 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    __forceinline ~CArray()
    {
        DestroyRange(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};

class CVertexBufferWin32 : public CBaseClassNonCopyable { public:   // vtable 0x0129BA20, 0x34 bytes
    unsigned char _body[0x30];
    void Uninitialise();                           // 0x00A63200
    __forceinline virtual ~CVertexBufferWin32() { Uninitialise(); }
};

class CMovableResource { public:                          // vtable 0x0129B860
    virtual void v00();
    ~CMovableResource() { }                        // inline: the vtable store
};
struct IDirect3DIndexBuffer9;
class CIndexBuffer : public CMovableResource { public:           // vtable 0x0129EF2C, 0x14 bytes
    IDirect3DIndexBuffer9* PD3DIndexBuffer;        // +0x04
    unsigned long          ByteLength;             // +0x08
    long                   PrimitiveType;          // +0x0c
    void*                  PAllocatedBuffer;       // +0x10
    void Uninitialise();                           // 0x00AC1EE0
    __forceinline virtual ~CIndexBuffer() { Uninitialise(); }
};

class CEnginePrimitiveManagerParticleSpriteGroup : public CBaseClassNonCopyable { public:
    unsigned char         _pad_0x04[0x8];                            // +0x04 primitive manager base words
    CArray<CVertexShader> VertexShadersBillboardSpritePointLights;   // +0x0c
    CArray<CVertexShader> VertexShadersCrossedSpritePointLights;     // +0x18
    CArray<CVertexShader> VertexShaders3DFaceMeSpritePointLights;    // +0x24
    CVertexShader         VertexShadersBillboardSpriteDisplacement;  // +0x30
    CVertexShader         VertexShadersCrossedSpriteDisplacement;    // +0x38
    CVertexShader         VertexShaders3DFaceMeSpriteDisplacement;   // +0x40
    CPixelShader          PixelShader;                               // +0x48
    CPixelShader          PixelShaderStipple;                        // +0x50
    CPixelShader          PixelShaderDisplacement;                   // +0x58
    CVertexBufferWin32    VertexBuffer;                              // +0x60
    CIndexBuffer          IndexBuffer;                               // +0x94
    unsigned char         ConstantBlock[0x280];                      // +0xa8 CEngineParticleConstantBlock[40]
};

void DestroySpriteGroup_00bc2280(CEnginePrimitiveManagerParticleSpriteGroup* group)   // emits the implicit destructor; not part of the retail body
{
    group->CEnginePrimitiveManagerParticleSpriteGroup::~CEnginePrimitiveManagerParticleSpriteGroup();
}