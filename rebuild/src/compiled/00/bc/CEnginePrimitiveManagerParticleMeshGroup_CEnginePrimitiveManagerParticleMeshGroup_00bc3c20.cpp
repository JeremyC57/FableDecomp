// CEnginePrimitiveManagerParticleMeshGroup::~CEnginePrimitiveManagerParticleMeshGroup() -- compiler-generated destructor: release the five pixel shaders and the displacement vertex shader, destroy the point-light vertex-shader array, then ~CEnginePrimitiveManagerOcclusionBase
// Leaf: ??1CEnginePrimitiveManagerParticleMeshGroup@@UAE@XZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BC3C20, 217 bytes, thiscall, tail jump to ~CEnginePrimitiveManagerOcclusionBase 0x00B38590.
// Identity (proven): the sole caller 0x00BC3C03 is the scalar deleting destructor 0x00BC3C00, slot 0 of vtable 0x012A63B8
//   (rtti-vtables.tsv / COL: .?AVCEnginePrimitiveManagerParticleMeshGroup@@ : CEnginePrimitiveManagerMeshBase :
//   CEnginePrimitiveManagerOcclusionBase : CEnginePrimitiveManager : CEnginePrimitiveRenderUtil : CBaseClassNonCopyable : CBaseClass,
//   one chain, mdisp 0). No vtable store of its own: compiler-generated (Ego_r / FableWin ??1CEnginePrimitiveManagerParticleMeshGroup
//   @@UAE@XZ). CEnginePrimitiveManagerMeshBase has no data members and an implicit dtor (no store before the tail jump), so the jump
//   lands directly in ~CEnginePrimitiveManagerOcclusionBase (??1CEnginePrimitiveManagerOcclusionBase@@UAE@XZ, bound). Catalog label right.
// Layout: Ego_r.pdb members at retail offsets (CArray 12 vs 16 bytes): VertexShadersMeshPointLights +0x20 (CArray<CVertexShader>:
//   elements destroyed through the virtual `push 0 ; call [vt]`, stride 8), VertexShaderDisplacement +0x2C, PixelShader +0x34,
//   PixelShaderStipple +0x3C, PixelShaderAdditiveAlpha +0x44, PixelShaderAdditiveAlphaStipple +0x4C, PixelShaderDisplacement +0x54,
//   ConstantBlock[40] +0x5C (trivial). The bases are local models: engine/CEnginePrimitiveManagerParticleMeshGroup.h is a flattened
//   record and the landed ~CEnginePrimitiveManagerOcclusionBase 0x00B38590 uses address-suffixed stand-ins (NOTES.md "Header debt").
// Shape: CVertexShader / CPixelShader inline virtual dtors (as the chunk 668 ~CRenderManager2D model): local copy of Data,
//   `dec [data+ReferenceCount]`, the private virtual ~C*ShaderData deleting call with 1, then the CShaderBase vtable 0x0129A030
//   store (FableWin ??1CShaderBase@@MAE@XZ, protected virtual, empty); VC7.1 keeps both vtable constants in ebx/edi.
// Callees: free 0x00BFEA14 (STLport allocator<T>::deallocate), ~CEnginePrimitiveManagerOcclusionBase 0x00B38590; vtables CPixelShader
//   0x0129A054, CShaderBase 0x0129A030, CVertexShader 0x0129B3D0.
// TU: the PDB ctor ??0CEnginePrimitiveManagerParticleMeshGroup@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z is defined with an empty body
//   only to emit the vtable, the ??_G and this implicit ??1 (not graded).
#include "engine/CBaseClassNonCopyable.h"
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
#include "engine/CArray.h"

#include "engine/CCountedPointer.h"
struct IDirect3DVertexDeclaration9;
struct IDirect3DVertexShader9;
struct IDirect3DPixelShader9;
struct _D3DVERTEXELEMENT9;
class CVertexShaderData {
    friend class CVertexShader;
    virtual ~CVertexShaderData();                           // private virtual (Ego_r / FableWin ??1CVertexShaderData@@EAE@XZ)
public:
    IDirect3DVertexDeclaration9* VertexDeclaration;         // +0x04
    IDirect3DVertexShader9*      VertexShader;              // +0x08
    _D3DVERTEXELEMENT9*          FormatDecl;                // +0x0C
    unsigned long                ReferenceCount;            // +0x10
};
class CPixelShaderData {
    friend class CPixelShader;
    virtual ~CPixelShaderData();                            // private virtual (Ego_r / FableWin ??1CPixelShaderData@@EAE@XZ)
public:
    IDirect3DPixelShader9* PixelShader;                     // +0x04
    unsigned long          ReferenceCount;                  // +0x08
    unsigned long          ConstCount;                      // +0x0C
    long*                  ConstIndices;                    // +0x10
};
class CShaderBase { public:                                 // vtable 0x0129A030
protected:
    CShaderBase() { }
    virtual ~CShaderBase() { }                              // FableWin ??1CShaderBase@@MAE@XZ (protected virtual)
};
class CVertexShader : public CShaderBase { public:          // vtable 0x0129B3D0
    CVertexShader() : Data(0) { }
    virtual ~CVertexShader()
    {
        CVertexShaderData* data = Data;
        if (data) {
            if (--data->ReferenceCount == 0)
                delete data;
        }
    }
    CVertexShaderData* Data;                                // +0x04
};
class CPixelShader : public CShaderBase { public:           // vtable 0x0129A054
    CPixelShader() : Data(0) { }
    virtual ~CPixelShader()
    {
        CPixelShaderData* data = Data;
        if (data) {
            if (--data->ReferenceCount == 0)
                delete data;
        }
    }
    CPixelShaderData* Data;                                 // +0x04
};
class CEngineScene;
class C3DMesh2;
class CEnginePrimitiveRenderUtil : public CBaseClassNonCopyable { public:
};
class CEnginePrimitiveManager : public CEnginePrimitiveRenderUtil { public:
    CArray<CEngineScene*>* SceneList;                       // +0x04
    bool                   Enable;                          // +0x08
};
class CEnginePrimitiveManagerOcclusionBase : public CEnginePrimitiveManager { public:
    CEnginePrimitiveManagerOcclusionBase(CArray<CEngineScene*>& sceneList);
    virtual ~CEnginePrimitiveManagerOcclusionBase();        // 0x00B38590
    CVertexShader                    VertexShader;          // +0x0C
    CCountedPointer<const C3DMesh2>  UnitBox;               // +0x14
    float                            SilhouetteTreshold;    // +0x1C
};
class CEnginePrimitiveManagerMeshBase : public CEnginePrimitiveManagerOcclusionBase { public:
    CEnginePrimitiveManagerMeshBase(CArray<CEngineScene*>& sceneList);
};
class CEngineParticleConstantBlock { public:
    float X, Y, Z, W;
};
class CEnginePrimitiveManagerParticleMeshGroup : public CEnginePrimitiveManagerMeshBase { public:   // vtable 0x012A63B8
    CEnginePrimitiveManagerParticleMeshGroup(CArray<CEngineScene*>& sceneList);
    CArray<CVertexShader>        VertexShadersMeshPointLights;   // +0x20
    CVertexShader                VertexShaderDisplacement;  // +0x2C
    CPixelShader                 PixelShader;               // +0x34
    CPixelShader                 PixelShaderStipple;        // +0x3C
    CPixelShader                 PixelShaderAdditiveAlpha;  // +0x44
    CPixelShader                 PixelShaderAdditiveAlphaStipple;   // +0x4C
    CPixelShader                 PixelShaderDisplacement;   // +0x54
    CEngineParticleConstantBlock ConstantBlock[40];         // +0x5C
};
CEnginePrimitiveManagerParticleMeshGroup::CEnginePrimitiveManagerParticleMeshGroup(CArray<CEngineScene*>& sceneList)
    : CEnginePrimitiveManagerMeshBase(sceneList)
{
}