// CEnginePrimitiveManagerParticleDecalGroup::~CEnginePrimitiveManagerParticleDecalGroup() -- compiler-generated destructor: the mesh and landscape index/vertex buffers released, the five pixel shaders, the three stencil vertex shaders and the two CVertexShader[3] tables released (reverse order), then the CBaseClassNonCopyable root
// Leaf: ??1CEnginePrimitiveManagerParticleDecalGroup
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BB99A0, length 427 bytes (tail `jmp` to the base destructor).
// Callees: CIndexBuffer::Uninitialise @ 0x00AC1EE0, CVertexBufferWin32::Uninitialise @ 0x00A63200, ~CBaseClassNonCopyable
//   @ 0x0099A300 (vertex buffer base and the class root). Vtables: 0x0129EF2C CIndexBuffer, 0x0129B860 CMovableResource,
//   0x0129BA20 CVertexBufferWin32, 0x0129A054 CPixelShader, 0x0129B3D0 CVertexShader, 0x0129A030 CShaderBase.
// Layout: engine/CEnginePrimitiveManagerParticleDecalGroup.h offsets (no CArrays, so the PDB offsets hold in retail); the header
//   keeps every member opaque, so the member classes are modelled locally exactly as the landed sibling
//   ~CEnginePrimitiveManagerParticleSpriteGroup 0x00BC2280 does.
// Shape: no user destructor (no own vtable store); members destroyed in reverse order through their inline destructors;
//   each CVertexShader[3] is the eh-vector destructor loop (`mov ebx,3`, pointer walked down by 8); the root dtor is tail-jumped.
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

struct CShaderBase {                               // vtable 0x0129A030
    virtual ~CShaderBase() { }
};
struct CPixelShader : CShaderBase {                // vtable 0x0129A054
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
struct CVertexShader : CShaderBase {               // vtable 0x0129B3D0
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

struct CVertexBufferWin32 : CBaseClassNonCopyable {   // vtable 0x0129BA20, 0x34 bytes
    unsigned char _body[0x30];
    void Uninitialise();                           // 0x00A63200
    __forceinline virtual ~CVertexBufferWin32() { Uninitialise(); }
};

struct CMovableResource {                          // vtable 0x0129B860
    virtual void v00();
    ~CMovableResource() { }                        // inline: the vtable store
};
struct IDirect3DIndexBuffer9;
struct CIndexBuffer : CMovableResource {           // vtable 0x0129EF2C, 0x14 bytes
    IDirect3DIndexBuffer9* PD3DIndexBuffer;        // +0x04
    unsigned long          ByteLength;             // +0x08
    long                   PrimitiveType;          // +0x0c
    void*                  PAllocatedBuffer;       // +0x10
    void Uninitialise();                           // 0x00AC1EE0
    __forceinline virtual ~CIndexBuffer() { Uninitialise(); }
};

struct CEnginePrimitiveManagerParticleDecalGroup : CBaseClassNonCopyable {
    unsigned char      _pad_0x04[0x8];                          // +0x04 primitive manager base words
    unsigned long      PointLightShaderIndex[6];                // +0x0c
    CVertexShader      LandscapeDecalVertexShader[3];           // +0x24
    CVertexShader      MeshDecalVertexShader[3];                // +0x3c
    CVertexShader      MeshDecalTwoPassStencilVertexShader;     // +0x54
    CVertexShader      MeshDecalOnePassStencilVertexShader;     // +0x5c
    CVertexShader      MeshDecalStencilClearVertexShader;       // +0x64
    CPixelShader       LandscapeDecalPixelShader;               // +0x6c
    CPixelShader       MeshDecalPixelShader;                    // +0x74
    CPixelShader       MeshDecalTwoPassStencilPixelShader;      // +0x7c
    CPixelShader       MeshDecalOnePassStencilPixelShader;      // +0x84
    CPixelShader       MeshDecalStencilClearPixelShader;        // +0x8c
    CVertexBufferWin32 LandscapeDecalVertexBuffer;              // +0x94
    CIndexBuffer       LandscapeDecalIndexBuffer;               // +0xc8
    CVertexBufferWin32 MeshDecalVertexBuffer;                   // +0xdc
    CIndexBuffer       MeshDecalIndexBuffer;                    // +0x110
};

void DestroyDecalGroup_00bb99a0(CEnginePrimitiveManagerParticleDecalGroup* group)   // emits the implicit destructor; not part of the retail body
{
    group->CEnginePrimitiveManagerParticleDecalGroup::~CEnginePrimitiveManagerParticleDecalGroup();
}
