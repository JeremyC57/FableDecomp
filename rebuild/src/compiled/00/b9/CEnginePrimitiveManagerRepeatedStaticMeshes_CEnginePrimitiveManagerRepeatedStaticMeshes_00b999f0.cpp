// CEnginePrimitiveManagerRepeatedStaticMeshes::~CEnginePrimitiveManagerRepeatedStaticMeshes() -- free the aligned SIMD variable block, then release the twelve pixel/vertex shaders in reverse order and ~CEnginePrimitiveManagerOcclusionBase
// Leaf: ??1CEnginePrimitiveManagerRepeatedStaticMeshes@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B999F0, 337 bytes, thiscall, tail jump to ~CEnginePrimitiveManagerOcclusionBase
//   0x00B38590; sole caller the scalar deleting dtor 0x00B9C6D0 (slot 0 of the class vtable 0x012A3E9C).
// Identity (proven): the body's first store is vtable 0x012A3E9C, whose rtti-vtables.tsv rows / RTTI COL name
//   .?AVCEnginePrimitiveManagerRepeatedStaticMeshes@@ (slot 0 = ??_G 0x00B9C6D0, the only caller); the vtable is also stored by
//   the ctor 0x00B99D90 (at 0x00B99DAA). Ego_r.pdb ??1CEnginePrimitiveManagerRepeatedStaticMeshes@@UAE@XZ (0x00A51BB0) right
//   after CalcFrameConstantSIMDVars. The own vtable store = a user-written destructor. Catalog label right.
// Layout: engine/CEnginePrimitiveManagerRepeatedStaticMeshes.h offsets (flattened byte arrays, no bases or methods; Ego_r 0x94
//   = retail), restated with the member types as the landed ~CEnginePrimitiveManagerRepeatedZSprites 0x00BABE60 does
//   (NOTES.md "Header debt"): PixelShader +0x20 / VertexShader +0x28 pairs up to VertexShaderColourBufferShadowedStippleAlpha
//   +0x78, the four shader pointers +0x80..+0x8c, SIMDVariableBlock +0x90. The retail bytes agree: refcount +0x08 (CPixelShaderData)
//   at the even pairs, +0x10 (CVertexShaderData) at the odd ones.
// Inline member dtors: Ego_r has no ??1 for CPixelShader / CVertexShader / CShaderBase, so each is an inline user dtor (own
//   vtable store 0x0129A054 / 0x0129B3D0, release of the counted shader data through its virtual deleting dtor with 1, then
//   CShaderBase's store 0x0129A030).
// Callees: CMemoryAllocator::DeallocateAligned 0x009F2B20 (static __fastcall, as the landed ~CEngineLightingManager 0x00B47B50),
//   ~CEnginePrimitiveManagerOcclusionBase 0x00B38590.
#include "engine/CCountedPointer.h"
#include "engine/CBaseClassNonCopyable.h"
// Shader models as the landed 0x00BABB50 / 0x00BAD040 / 0x00BABE60 (RTTI: CVertexShader 0x0129B3D0 / CPixelShader 0x0129A054 :
// CShaderBase 0x0129A030, 8 bytes each). The shader data objects are intrusively counted (ReferenceCount +0x10 / +0x08) with a
// virtual deleting dtor.
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
class CMemoryAllocator {
public:
    static void __fastcall DeallocateAligned(void* p);  // 0x009F2B20
};
class CEngineScene;
template <class T> class CArray;
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
struct CSIMDVariableBlock;
class CEnginePrimitiveManagerRepeatedStaticMeshes : public CEnginePrimitiveManagerOcclusionBase {
public:
    CEnginePrimitiveManagerRepeatedStaticMeshes(CArray<CEngineScene*>& scenes);
    virtual ~CEnginePrimitiveManagerRepeatedStaticMeshes();
    CPixelShader        PixelShader;                                     // +0x20
    CVertexShader       VertexShader;                                    // +0x28
    CPixelShader        PixelShaderStippleAlpha;                         // +0x30
    CVertexShader       VertexShaderStippleAlpha;                        // +0x38
    CPixelShader        PixelShaderDepthBufferShadowed;                  // +0x40
    CVertexShader       VertexShaderDepthBufferShadowed;                 // +0x48
    CPixelShader        PixelShaderDepthBufferShadowedStippleAlpha;      // +0x50
    CVertexShader       VertexShaderDepthBufferShadowedStippleAlpha;     // +0x58
    CPixelShader        PixelShaderColourBufferShadowed;                 // +0x60
    CVertexShader       VertexShaderColourBufferShadowed;                // +0x68
    CPixelShader        PixelShaderColourBufferShadowedStippleAlpha;     // +0x70
    CVertexShader       VertexShaderColourBufferShadowedStippleAlpha;    // +0x78
    CPixelShader*       PixelShaderShadowed;                             // +0x80
    CVertexShader*      VertexShaderShadowed;                            // +0x84
    CPixelShader*       PixelShaderShadowedStippleAlpha;                 // +0x88
    CVertexShader*      VertexShaderShadowedStippleAlpha;                // +0x8c
    CSIMDVariableBlock* SIMDVariableBlock;                               // +0x90
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveManagerRepeatedStaticMeshes) == 0x94);
CEnginePrimitiveManagerRepeatedStaticMeshes::~CEnginePrimitiveManagerRepeatedStaticMeshes()
{
    CMemoryAllocator::DeallocateAligned(SIMDVariableBlock);
}