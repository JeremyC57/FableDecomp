// CEnginePrimitiveManagerParticleMeshGroup::~CEnginePrimitiveManagerParticleMeshGroup() -- compiler-generated destructor: release the five pixel shaders and the displacement vertex shader, destroy the point-light vertex-shader array, then ~CEnginePrimitiveManagerOcclusionBase
// Leaf: ??1CEnginePrimitiveManagerParticleMeshGroup@@UAE@XZ
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
namespace std {
template <class _Tp> class allocator { public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first) _Destroy(&*__first);
}
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> { public:
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
} // namespace std
template <class T> class CArray : public std::vector<T> {   // retail CArray<T>: the 12-byte STLport vector
};

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