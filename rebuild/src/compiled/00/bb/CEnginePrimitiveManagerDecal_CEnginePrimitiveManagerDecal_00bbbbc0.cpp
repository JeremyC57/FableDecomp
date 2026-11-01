// CEnginePrimitiveManagerDecal::~CEnginePrimitiveManagerDecal() -- implicit decal primitive-manager destructor: destroys the landscape decal index / vertex buffers, the five pixel shaders, the three stencil vertex shaders, the two CVertexShader[3] tables and the mesh decal buffers in reverse declaration order, then ~CBaseClassNonCopyable
// Leaf: ??1CEnginePrimitiveManagerDecal
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BBBBC0, 488 bytes, thiscall, tail jump to ~CBaseClassNonCopyable 0x0099A300.
// Identity (proven): the sole caller 0x00BBBBA3 is the scalar deleting destructor 0x00BBBBA0 (landed; slot 0 of vtable 0x012A5B6C,
//   RTTI .?AVCEnginePrimitiveManagerDecal@@ : CEnginePrimitiveManager : CEnginePrimitiveRenderUtil : CBaseClassNonCopyable :
//   CBaseClass); Ego_r.pdb ??1CEnginePrimitiveManagerDecal@@UAE@XZ; the member offsets are Ego_r.pdb's (0x124) as the landed ctor
//   0x00BBA480 lays them out. The body stores no vtable of its own (and none for the two intermediate bases, which the PDBs give
//   no destructor): compiler-generated destructors all the way down to CBaseClassNonCopyable. The landed ??_G 0x00BBBBA0 bound this
//   address as `CEnginePrimitiveManagerDecal_dtor` (NAME_FIXES.tsv).
// Callees: CIndexBuffer::Uninitialise 0x00AC1EE0, CVertexBufferWin32::Uninitialise 0x00A63200, ~CBaseClassNonCopyable 0x0099A300
//   (after each vertex buffer, through the implicit ~CVertexBufferBase, and as the tail), the shader data objects' virtual deleting
//   destructors (CPixelShaderData refcount +0x08, CVertexShaderData refcount +0x10).
// Shape: no user destructor; the PDB constructor is defined with an empty body (retail's TU defines it, 0x00BBA480) so that the
//   vtable, the scalar deleting destructor and this implicit destructor are emitted; only the destructor is graded. The CVertexShader[3]
//   arrays destroy as the compiler's counted-down element loops (`mov ebx,3 ; sub edi,8`).
// Layout: engine/CEngineComponent.h is unrelated; there is no engine/CEnginePrimitiveManagerDecal.h / CEnginePrimitiveManager.h /
//   CEnginePrimitiveRenderUtil.h: the chain is modelled from the RTTI and Ego_r.pdb (CEnginePrimitiveManager 0xc: SceneList, Enable).
#include "engine/CBaseClassNonCopyable.h"            // CBaseClass() 0x0099A2D0 / ~CBaseClass 0x0099A2E0, ~CBaseClassNonCopyable 0x0099A300
// Layout: engine/CVertexShader.h, engine/CIndexBuffer.h, engine/CVertexBufferWin32.h and engine/CMovableResource.h flatten the base
//   chains (no CShaderBase / CMovableResource / CVertexBufferBase bases, no inline destructors), so the shader and buffer members are
//   restated here with their RTTI chains and the inline destructors retail expands (the landed 0x00BBA480 / 0x00B52250 models).
class CVertexShaderData { public:                          // Ego_r.pdb, 0x14: ReferenceCount +0x10
    virtual ~CVertexShaderData();                          // slot 0 (private in Ego_r: ??1CVertexShaderData@@EAE@XZ)
    void*       VertexDeclaration;                         // IDirect3DVertexDeclaration9*
    void*       VertexShader;                              // IDirect3DVertexShader9*
    const void* FormatDecl;                                // _D3DVERTEXELEMENT9*
    unsigned long ReferenceCount;                          // +0x10
    unsigned long RemoveReference() { return --ReferenceCount; }
};
class CPixelShaderData { public:                           // Ego_r.pdb, 0x14: ReferenceCount +0x08
    virtual ~CPixelShaderData();                           // slot 0 (private in Ego_r: ??1CPixelShaderData@@EAE@XZ)
    void*         PixelShader;                             // IDirect3DPixelShader9*
    unsigned long ReferenceCount;                          // +0x08
    unsigned long ConstCount;
    long*         ConstIndices;
    unsigned long RemoveReference() { return --ReferenceCount; }
};
class CShaderBase { public:                                // vtable 0x0129A030
    virtual ~CShaderBase() { }
};
class CVertexShader : public CShaderBase { public:         // vtable 0x0129B3D0, 8 bytes
    CVertexShader();                                       // 0x00A62FB0
    virtual ~CVertexShader() { if (Data) { CVertexShaderData* current = Data; if (!current->RemoveReference()) delete current; } }
    CVertexShaderData* Data;                               // +0x04
};
class CPixelShader : public CShaderBase { public:          // vtable 0x0129A054, 8 bytes
    CPixelShader();                                        // 0x00A5EC00
    virtual ~CPixelShader() { if (Data) { CPixelShaderData* current = Data; if (!current->RemoveReference()) delete current; } }
    CPixelShaderData* Data;                                // +0x04
};
struct IDirect3DVertexBuffer9;
struct IDirect3DIndexBuffer9;
class CVertexCompressionParams { public:
    float PosScaleX, PosScaleY, PosScaleZ, PosScaleW;
    float PosBiasX, PosBiasY, PosBiasZ, PosBiasW;
};
class CVertexBufferBase : public CBaseClassNonCopyable { public:   // 0x34 bytes, implicit destructor (no vtable store)
    IDirect3DVertexBuffer9*  PVertexBuffer;                // +0x04
    unsigned long            Length;                       // +0x08
    long                     Type;                         // +0x0c EVertexType
    unsigned char            Stride;                       // +0x10
    CVertexCompressionParams CompressionParams;            // +0x14
};
class CVertexBufferWin32 : public CVertexBufferBase { public:      // vtable 0x0129BA20
    CVertexBufferWin32();
    virtual ~CVertexBufferWin32() { Uninitialise(); }
    void Uninitialise();                                   // 0x00A63200 (?Uninitialise@CVertexBufferWin32@@QAEXXZ)
};
class CMovableResource { public:                           // vtable 0x0129B860 (only FableWin has an out-of-line ??1)
    virtual ~CMovableResource() { }
};
class CIndexBuffer : public CMovableResource { public:     // vtable 0x0129EF2C, 0x14 bytes
    CIndexBuffer();                                        // 0x00AC1EC0
    virtual ~CIndexBuffer() { Uninitialise(); }
    void Uninitialise();                                   // 0x00AC1EE0 (?Uninitialise@CIndexBuffer@@QAEXXZ)
    IDirect3DIndexBuffer9* PD3DIndexBuffer;                // +0x04
    unsigned long          ByteLength;                     // +0x08
    long                   PrimitiveType;                  // +0x0c EPrimitiveType
    void*                  PAllocatedBuffer;               // +0x10
};
class CEngineScene;
template <class T> class CArray;
class CEnginePrimitiveRenderUtil : public CBaseClassNonCopyable { };    // RTTI base, no members, implicit destructor
class CEnginePrimitiveManager : public CEnginePrimitiveRenderUtil { public:   // Ego_r.pdb 0xc, implicit destructor
    CEnginePrimitiveManager(CArray<CEngineScene*>& scenes);                   // 0x00BACDB0 (??0CEnginePrimitiveManager@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z)
    CArray<CEngineScene*>* SceneList;                                         // +0x04
    bool                   Enable;                                            // +0x08
};
class CEnginePrimitiveManagerDecal : public CEnginePrimitiveManager { public:   // vtable 0x012A5B6C, 0x124 bytes
    CEnginePrimitiveManagerDecal(CArray<CEngineScene*>& scenes);                // 0x00BBA480 (landed)
    CVertexBufferWin32 MeshDecalVertexBuffer;                   // +0x0c
    CIndexBuffer       MeshDecalIndexBuffer;                    // +0x40
    unsigned long      PointLightShaderIndex[6];                // +0x54
    CVertexShader      LandscapeDecalVertexShader[3];           // +0x6c
    CVertexShader      MeshDecalVertexShader[3];                // +0x84
    CVertexShader      MeshDecalTwoPassStencilVertexShader;     // +0x9c
    CVertexShader      MeshDecalOnePassStencilVertexShader;     // +0xa4
    CVertexShader      MeshDecalStencilClearVertexShader;       // +0xac
    CPixelShader       LandscapeDecalPixelShader;               // +0xb4
    CPixelShader       MeshDecalPixelShader;                    // +0xbc
    CPixelShader       MeshDecalTwoPassStencilPixelShader;      // +0xc4
    CPixelShader       MeshDecalOnePassStencilPixelShader;      // +0xcc
    CPixelShader       MeshDecalStencilClearPixelShader;        // +0xd4
    CVertexBufferWin32 LandscapeDecalVertexBuffer;              // +0xdc
    CIndexBuffer       LandscapeDecalIndexBuffer;               // +0x110
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveManagerDecal) == 0x124);

CEnginePrimitiveManagerDecal::CEnginePrimitiveManagerDecal(CArray<CEngineScene*>& scenes)   // the real body is the landed 0x00BBA480
    : CEnginePrimitiveManager(scenes)
{
}