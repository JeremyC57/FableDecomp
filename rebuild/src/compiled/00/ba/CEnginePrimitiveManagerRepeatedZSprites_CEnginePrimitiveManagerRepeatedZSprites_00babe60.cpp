// CEnginePrimitiveManagerRepeatedZSprites::~CEnginePrimitiveManagerRepeatedZSprites() -- free the dynamic texture page, then destroy PrimitiveSortList, IndexBuffer, VertexBuffer and the three shaders in reverse order and ~CEnginePrimitiveManagerOcclusionBase
// Leaf: ??1CEnginePrimitiveManagerRepeatedZSprites@@UAE@XZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BABE60, 200 bytes, tail jump to ~CEnginePrimitiveManagerOcclusionBase 0x00B38590; sole
//   caller the scalar deleting dtor 0x00BAC970 (slot 0 of the class vtable 0x012A5314).
// Identity (proven): rtti-vtables.tsv / the RTTI COL at 0x012A5314-4 (.?AVCEnginePrimitiveManagerRepeatedZSprites@@ :
//   CEnginePrimitiveManagerOcclusionBase : CEnginePrimitiveManager : CEnginePrimitiveRenderUtil : CBaseClassNonCopyable :
//   CBaseClass) and the body's own vtable store; Ego_r.pdb ??1CEnginePrimitiveManagerRepeatedZSprites@@UAE@XZ. Label right.
// Layout: engine/CEnginePrimitiveManagerRepeatedZSprites.h offsets (flattened byte arrays, no bases or methods), restated with
//   the member types as the landed ctor 0x00BABB50 does (NOTES.md "Header debt"): VertexShader +0x20, PixelShader +0x28,
//   PixelShaderNoStipple +0x30, VertexBuffer +0x38 (CVertexBufferWin32 : CVertexBufferBase : CBaseClassNonCopyable, 0x34),
//   IndexBuffer +0x6c (CIndexBuffer : CMovableResource, 0x14), PrimitiveSortList +0x80 (CArray = STLport vector, 12 bytes),
//   BillboardTexture +0x8c, BillboardTextureHandle +0xa4, ZBufferTextureHandle +0xa8, DynamicTexturePageHandle +0xac, ... 0xc4.
// Inline member dtors: Ego_r has no ??1 for CIndexBuffer / CVertexBufferWin32 / CMovableResource / the shaders, so each is an
//   inline user dtor (vtable store + Uninitialise; CMovableResource's own store; CVertexBufferBase implicit = no store).
// Layout: engine/CMovableResource.h, engine/CIndexBuffer.h, engine/CVertexBufferWin32.h and engine/CVertexShader.h are
//   flattened pack(1) records (void* vptr slot, byte-array bases, no virtual functions or methods), so the polymorphic classes
//   with their RTTI bases and inline dtors are restated locally (NOTES.md "Header debt").
// Callees: CGPUScratchBufferTextureManager::FreeMemoryPage(long) 0x00F37030, free 0x00BFEA14, CIndexBuffer::Uninitialise
//   0x00AC1EE0, CVertexBufferWin32::Uninitialise 0x00A63200, ~CBaseClassNonCopyable 0x0099A300,
//   ~CEnginePrimitiveManagerOcclusionBase 0x00B38590.
extern "C" void __cdecl free(void* __p);                 // 0x00BFEA14
#include "engine/CArray.h"
// Shader models as the landed 0x00BABB50 / 0x00BAD040 (RTTI: CVertexShader 0x0129B3D0 / CPixelShader 0x0129A054 : CShaderBase
// 0x0129A030, 8 bytes each; Ego_r has no ??1 for any of the three, so their user dtors are inline). The shader data objects are
// intrusively counted (ReferenceCount +0x10 / +0x08) with a virtual deleting dtor.
struct CVertexShaderData {
    virtual ~CVertexShaderData();                  // slot 0
    void*         VertexDeclaration;               // +0x04
    void*         VertexShader;                    // +0x08
    const void*   FormatDecl;                      // +0x0c
    long          ReferenceCount;                  // +0x10
    long RemoveReference() { return --ReferenceCount; }
};
struct CPixelShaderData {
    virtual ~CPixelShaderData();                   // slot 0
    void*         PixelShader;                     // +0x04
    long          ReferenceCount;                  // +0x08
    long RemoveReference() { return --ReferenceCount; }
};
class CShaderBase { public:                        // vtable 0x0129A030
    virtual ~CShaderBase() { }
};
class CVertexShader : public CShaderBase { public: // vtable 0x0129B3D0
    CVertexShader();                               // 0x00A62FB0
    virtual ~CVertexShader() { if (Data) { CVertexShaderData* current = Data; if (!current->RemoveReference()) delete current; } }
    CVertexShaderData* Data;                       // +0x04
};
class CPixelShader : public CShaderBase { public:  // vtable 0x0129A054
    CPixelShader();                                // 0x00A5EC00
    virtual ~CPixelShader() { if (Data) { CPixelShaderData* current = Data; if (!current->RemoveReference()) delete current; } }
    CPixelShaderData* Data;                        // +0x04
};
void operator delete(void* p);                          // 0x00BFE9BC
#include "engine/CCountedPointer.h"
#include "engine/CBaseClassNonCopyable.h"
class CGPUScratchBufferTextureManager { public:
    void FreeMemoryPage(long);                     // 0x00F37030
};
struct CEngineGlobal_01436e98 {
    unsigned char                   _pad0000[0x3190];
    CGPUScratchBufferTextureManager ScratchTextures;   // +0x3190
};
extern CEngineGlobal_01436e98* g_pEngine_01436e98;   // 0x01436E98
class CMovableResource {
public:
    virtual ~CMovableResource() { }                // inline in Ego_r (only FableWin lists ??1CMovableResource)
    virtual void RelocateData(long, void*, void*, long) = 0;
    virtual bool CanRelocateData(long) const;
    virtual unsigned long Verify() const;
};
struct IDirect3DIndexBuffer9;
struct IDirect3DVertexBuffer9;
enum EPrimitiveType { };
enum EVertexType { };
class CIndexBuffer : public CMovableResource {     // vtable 0x0129EF2C, 0x14
public:
    CIndexBuffer();                                // 0x00AC1EC0
    virtual ~CIndexBuffer() { Uninitialise(); }
    virtual void RelocateData(long, void*, void*, long);
    void Uninitialise();                           // 0x00AC1EE0
    IDirect3DIndexBuffer9* PD3DIndexBuffer;        // +0x04
    unsigned long          ByteLength;             // +0x08
    EPrimitiveType         PrimitiveType;          // +0x0c
    void*                  PAllocatedBuffer;       // +0x10
};
class CVertexCompressionParams { public:
    float PosScaleX, PosScaleY, PosScaleZ, PosScaleW;
    float PosBiasX, PosBiasY, PosBiasZ, PosBiasW;
};
class CVertexBufferBase : public CBaseClassNonCopyable {   // 0x34
public:
    IDirect3DVertexBuffer9*  PVertexBuffer;        // +0x04
    unsigned long            Length;               // +0x08
    EVertexType              Type;                 // +0x0c
    unsigned char            Stride;               // +0x10
    CVertexCompressionParams CompressionParams;    // +0x14
};
class CVertexBufferWin32 : public CVertexBufferBase {     // vtable 0x0129BA20
public:
    CVertexBufferWin32();
    virtual ~CVertexBufferWin32() { Uninitialise(); }
    void Uninitialise();                           // 0x00A63200
};
class CEngineScene;
class CTexture;
class CSurface;
class C3DVector { public: float X, Y, Z; };
class CEngineBillboard { public:                   // 0x18
    CTexture* Texture;                             // +0x00
    CSurface* ZBuffer;                             // +0x04
    C3DVector Offset;                              // +0x08
    float     Scale;                               // +0x14
};
class CEnginePrimitiveRenderUtil : public CBaseClassNonCopyable { };    // RTTI chain (Ego_r 4 bytes)
class CEnginePrimitiveManager : public CEnginePrimitiveRenderUtil {     // Ego_r 0xc
public:
    CArray<CEngineScene*>* SceneList;              // +0x04
    bool                   Enable;                 // +0x08
};
class C3DMesh2;
class CEnginePrimitiveManagerOcclusionBase : public CEnginePrimitiveManager {   // Ego_r 0x20
public:
    virtual ~CEnginePrimitiveManagerOcclusionBase();   // 0x00B38590
    CVertexShader                   VertexShader;  // +0x0c
    CCountedPointer<const C3DMesh2> UnitBox;       // +0x14
    float                           SilhouetteTreshold;   // +0x1c
};
class CEngineInternalPrimitiveRepeatedZSprite;
class C3DMeshInfo;
class CEnginePrimitiveManagerRepeatedZSprites : public CEnginePrimitiveManagerOcclusionBase {
public:
    CEnginePrimitiveManagerRepeatedZSprites(CArray<CEngineScene*>& scenes);
    virtual ~CEnginePrimitiveManagerRepeatedZSprites();
    CVertexShader      VertexShader;               // +0x20
    CPixelShader       PixelShader;                // +0x28
    CPixelShader       PixelShaderNoStipple;       // +0x30
    CVertexBufferWin32 VertexBuffer;               // +0x38
    CIndexBuffer       IndexBuffer;                // +0x6c
    CArray<CEngineInternalPrimitiveRepeatedZSprite*> PrimitiveSortList;   // +0x80
    CEngineBillboard   BillboardTexture;           // +0x8c
    long               BillboardTextureHandle;     // +0xa4
    long               ZBufferTextureHandle;       // +0xa8
    long               DynamicTexturePageHandle;   // +0xac
    float              NonStippleAlphaFactor;      // +0xb0
    bool               EnableZSpriteAlpha;         // +0xb4
    float              VerticalControlPointWeightingFactor;   // +0xb8
    float              ControlPointWeightingPower; // +0xbc
    C3DMeshInfo*       CurrentRenderedMesh;        // +0xc0
};
CEnginePrimitiveManagerRepeatedZSprites::~CEnginePrimitiveManagerRepeatedZSprites()
{
    g_pEngine_01436e98->ScratchTextures.FreeMemoryPage(DynamicTexturePageHandle);
}