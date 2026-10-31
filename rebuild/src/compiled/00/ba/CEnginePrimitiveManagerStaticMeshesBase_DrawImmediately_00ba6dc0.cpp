// CEnginePrimitiveManagerStaticMeshesBase::DrawImmediately(CEngineInternalPrimitiveStaticMesh& primitive, ERenderMode mode, ERenderPassType pass) -- render a static mesh primitive at once: every mesh primitive with triangles whose material has a diffuse map and is not hidden by the morph info is wrapped in a stack CEngineSubPrimitiveStaticMesh and rendered
// Leaf: ?DrawImmediately@CEnginePrimitiveManagerStaticMeshesBase@@AAEXAAVCEngineInternalPrimitiveStaticMesh@@W4ERenderMode@@W4ERenderPassType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BA6DC0, 291 bytes (`ret 0Ch`).
// Identity: Ego_r.pdb ?DrawImmediately@CEnginePrimitiveManagerStaticMeshesBase@@AAEXAAVCEngineInternalPrimitiveStaticMesh@@W4ERenderMode@@W4ERenderPassType@@@Z
//   (private); the inline FableWin.pdb ?GetHiddenMaterials@CEngineInternalPrimitiveMorphInfo@@QAEPBEABVC3DMesh2@@@Z.
// Callees: CEngineInternalPrimitiveMorphInfo::InitialiseHiddenMaterials(const C3DMesh2&) @ 0x00BEC8D0 (Ego_r.pdb; the reloc binding
//   ?UpdateVisibilityVector@CEngineMeshVisibility@@ is wrong, NAME_FIXES.tsv), CBaseClassNonCopyable::CBaseClassNonCopyable /
//   ~CBaseClassNonCopyable @ 0x0099A2F0 / 0x0099A300, CEnginePrimitiveManagerStaticMeshesBase::RenderMeshPrimitive(
//   CEngineSubPrimitiveStaticMesh&, ERenderPassType) @ 0x00BA5260 (Ego_r.pdb; NAME_FIXES.tsv). Vtable 0x012A41C8 =
//   CEngineSubPrimitiveStaticMesh.
// Layout: Ego_r.pdb names at retail offsets (STLport containers 12 bytes, CVectorMap 16): the primitive's Type +8 and
//   RenderInfo.Mesh +0xa4; CEngineInternalPrimitiveMorphedStaticMesh::Morph +0xac (HiddenMaterialNames set node count +0x24,
//   HiddenMaterials CArray<unsigned char> +0x2c); C3DMesh2 Materials +0x8c (C3DMeshMaterial 0x30, DiffuseMapBankIndex +0x10),
//   PrimitiveCount +0x90, Primitives +0x94 (C3DPrimitive2 0xa0: MaterialIndex +4, TriangleCount +0x5c);
//   CEngineSubPrimitiveMeshBase (0x24): RenderFlags +4, Parent +0xc, NextSubPrimitive +0x10, MeshPrimitive +0x18,
//   RenderMode +0x1c, IsCloth +0x20. engine/CEngineInternalPrimitiveStaticMesh.h and CEnginePersistentPrimitiveBase.h are
//   flattened (no vptr), so the primitive chain (RTTI: StaticMesh : MeshBase : OcclusionBase : InternalPrimitiveBase :
//   PersistentPrimitiveBase : CIVCountedPointeeBase) is a local model over the retail CIVCountedPointeeBase.h.
// Shape: what matched (2026-10-31) was the PDB signature set: the private DrawImmediately on the static-mesh class, the inline
//   GetHiddenMaterials(const C3DMesh2&) returning 0 when no names are hidden (so the result is assigned straight into ebp),
//   the InitialiseHiddenMaterials callee by const reference and RenderMeshPrimitive by reference. The earlier stand-in (a
//   hidden-table select inline in the caller) computed the table in eax and copied it to ebp.
// `this` (the manager) is only passed on to RenderMeshPrimitive: engine/CEnginePrimitiveManagerStaticMeshesBase.h is the flattened
// Ego_r layout (struct key, `void* __vftable`), pulled in under its Layout name; the class below declares the two members used.
#define CEnginePrimitiveManagerStaticMeshesBase CEnginePrimitiveManagerStaticMeshesBaseLayout
#include "engine/CEnginePrimitiveManagerStaticMeshesBase.h"
#undef CEnginePrimitiveManagerStaticMeshesBase
#include "engine/CBaseClassNonCopyable.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CIVCountedPointeeBase.h"

enum ERenderMode { };
enum ERenderPassType { };
class CEnginePrimitiveManager;
class CCharString;

class C3DMeshMaterial { public: unsigned char _pad00[0x10]; unsigned long DiffuseMapBankIndex; unsigned char _pad14[0x1c]; };   // 0x30
class C3DPrimitive2 { public: unsigned char _pad00[4]; long MaterialIndex; unsigned char _pad08[0x54]; unsigned long TriangleCount; unsigned char _pad60[0x40]; };   // 0xa0
class C3DMesh2 {
public:
    unsigned char    _pad00[0x8c];
    C3DMeshMaterial* Materials;                    // +0x8c
    unsigned long    PrimitiveCount;               // +0x90
    C3DPrimitive2*   Primitives;                   // +0x94
};

class CEnginePersistentPrimitiveBase : public CIVCountedPointeeBase {   // RTTI chain of CEngineInternalPrimitiveStaticMesh
public:
    long Type;                                     // +0x08
};
class CEngineInternalPrimitiveBase : public CEnginePersistentPrimitiveBase {   // 0x48 (engine/CEngineInternalPrimitiveBase.h layout)
public:
    unsigned char _pad0c[0x48 - 0x0c];
};
class CEngineInternalPrimitiveOcclusionBase : public CEngineInternalPrimitiveBase { };
class CEngineInternalPrimitiveMeshBase : public CEngineInternalPrimitiveOcclusionBase { };
class CEngineSubPrimitiveBase : public CBaseClassNonCopyable {   // PDB, 0x18 bytes
public:
    unsigned long                 RenderFlags;      // +0x04
    unsigned char                 Alpha;            // +0x08
    CEngineInternalPrimitiveBase* Parent;           // +0x0c
    CEngineSubPrimitiveBase*      NextSubPrimitive; // +0x10
    CEnginePrimitiveManager*      Manager;          // +0x14
    CEngineSubPrimitiveBase(CEngineInternalPrimitiveBase& parent) : Parent(&parent), NextSubPrimitive(0) { }
};
class CEngineSubPrimitiveMeshBase : public CEngineSubPrimitiveBase {   // PDB, 0x24 bytes
public:
    C3DPrimitive2* MeshPrimitive;                   // +0x18
    ERenderMode    RenderMode;                      // +0x1c
    bool           IsCloth;                         // +0x20
    CEngineSubPrimitiveMeshBase(CEngineInternalPrimitiveBase& parent, C3DPrimitive2* meshPrimitive, ERenderMode mode)
        : CEngineSubPrimitiveBase(parent), MeshPrimitive(meshPrimitive), RenderMode(mode), IsCloth(false) { }
};
class CEngineSubPrimitiveStaticMesh : public CEngineSubPrimitiveMeshBase {   // vtable 0x012A41C8 (one slot: the implicit scalar deleting dtor)
public:
    CEngineSubPrimitiveStaticMesh(CEngineInternalPrimitiveBase& parent, C3DPrimitive2* meshPrimitive, ERenderMode mode)
        : CEngineSubPrimitiveMeshBase(parent, meshPrimitive, mode) { }
};

namespace std {
template <class T> struct less;
template <class T> class allocator;
template <class K, class C = less<K>, class A = allocator<K> > class set {   // STLport 4.6 _Rb_tree (12 bytes)
public:
    void*        _M_header;
    unsigned int _M_node_count;
    unsigned char _M_key_compare;
    unsigned int size() const { return _M_node_count; }
};
}
template <class T> class CArray {                  // retail 12 bytes: first, last, end of storage
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    bool empty() const { return _M_start == _M_finish; }
    const T* GetData() const { return (_M_finish - _M_start) ? _M_start : (const T*)this; }
};
class CEngineInternalPrimitiveMorphInfo {
public:
    unsigned char           TextureDecals[0x10];   // +0x00 CVectorMap
    unsigned char           CompositeSurfaces[0x10];   // +0x10 CVectorMap
    std::set<CCharString>   HiddenMaterialNames;   // +0x20
    CArray<unsigned char>   HiddenMaterials;       // +0x2c
    void InitialiseHiddenMaterials(const C3DMesh2& mesh);   // 0x00BEC8D0
    const unsigned char* GetHiddenMaterials(const C3DMesh2& mesh)
    {
        if (HiddenMaterialNames.size() == 0)
            return 0;
        if (HiddenMaterials.empty())
            InitialiseHiddenMaterials(mesh);
        return HiddenMaterials.GetData();
    }
};
class CEngineInternalPrimitiveStaticMesh : public CEngineInternalPrimitiveMeshBase {
public:
    unsigned char _pad48[0xa4 - 0x48];             // occlusion / mesh-base members, ObjectTransform +0x70, RenderInfo +0xa0
    C3DMesh2*     Mesh;                            // +0xa4 (RenderInfo.Mesh, CEngineRenderInfoMeshBase)
};
class CEngineInternalPrimitiveMorphedStaticMesh : public CEngineInternalPrimitiveStaticMesh {
public:
    unsigned char                     _pada8[4];
    CEngineInternalPrimitiveMorphInfo Morph;       // +0xac
};

class CEnginePrimitiveManagerStaticMeshesBase {
public:
    void RenderMeshPrimitive(CEngineSubPrimitiveStaticMesh& subPrimitive, ERenderPassType pass);   // 0x00BA5260
private:
    void DrawImmediately(CEngineInternalPrimitiveStaticMesh& primitive, ERenderMode mode, ERenderPassType pass);
};

void CEnginePrimitiveManagerStaticMeshesBase::DrawImmediately(CEngineInternalPrimitiveStaticMesh& primitive, ERenderMode mode, ERenderPassType pass)
{
    C3DMesh2* mesh = primitive.Mesh;
    const unsigned char* hidden = 0;
    if (primitive.Type == 10)
        hidden = static_cast<CEngineInternalPrimitiveMorphedStaticMesh&>(primitive).Morph.GetHiddenMaterials(*mesh);
    for (unsigned long i = 0; i < mesh->PrimitiveCount; ++i) {
        C3DPrimitive2* meshPrimitive = &mesh->Primitives[i];
        if (meshPrimitive->TriangleCount == 0)
            continue;
        C3DMeshMaterial* material = &mesh->Materials[meshPrimitive->MaterialIndex];
        if (hidden && hidden[meshPrimitive->MaterialIndex])
            continue;
        if (material->DiffuseMapBankIndex == 0)
            continue;
        CEngineSubPrimitiveStaticMesh subPrimitive(primitive, meshPrimitive, mode);
        subPrimitive.RenderFlags = 2;
        RenderMeshPrimitive(subPrimitive, pass);
    }
}