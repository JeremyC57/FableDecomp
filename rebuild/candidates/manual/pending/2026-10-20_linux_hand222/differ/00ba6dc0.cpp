// Grade: DIFFER(291v291) -- one register move: the hidden-material table select (`size ? first : &vector`, one inline GetData) is computed in eax and copied to ebp where retail loads ebp directly (the 2-byte `mov ebp,eax` comes back as the loop-head padding `mov edi,edi` v `lea esp,[esp]`). Fixed by this form: the morph reference taken before its name-count test, and an implicit (compiler-generated) sub-primitive destructor (no vtable reset before ~CBase). Tried: a data local inside GetData, the select written in place.
// CEnginePrimitiveManagerStaticMeshesBase::DrawImmediately(CEngineInternalPrimitiveBase& primitive, ERenderMode mode, ERenderPassType pass) -- render a static mesh primitive at once: every mesh primitive with render data whose material is present and not hidden by the morph info is wrapped in a stack CEngineSubPrimitiveStaticMesh and rendered
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BA6DC0, length 289 bytes (`ret 0Ch`).
// Callees: the morph info's hidden-material builder @ 0x00BEC8D0 (manifest "CEngineInternalPrimitive_UpdateVisibilityVector"; on the
// morph info, the mesh info pushed), CBase::CBase / ~CBase @ 0x0099A2F0 / 0x0099A300, CEnginePrimitiveManagerStaticMeshesBase::
// RenderMeshPrimitive(CEngineSubPrimitiveStaticMesh*, ERenderPassType) @ 0x00BA5260. Vtable 0x012A41C8 = CEngineSubPrimitiveStaticMesh.
// Layout: the primitive's Type +8, MeshInfo +0xa4, the morphed mesh's Morph (CEngineInternalPrimitiveMorphInfo) +0xac with its
// HiddenMaterialNames count +0x24 and HiddenMaterials CArray<unsigned char> +0x2c (retail 16-byte vector maps); the mesh info's
// materials (0x30 bytes, +0x10 present) +0x8c, primitive count +0x90, C3DPrimitive2 array (0xa0 bytes: MaterialIndex +4, render
// data +0x5c) +0x94; CEngineSubPrimitiveMeshBase (PDB, 0x24): RenderFlags +4, Parent +0xc, NextSubPrimitive +0x10, MeshPrimitive
// +0x18, RenderMode +0x1c, IsCloth +0x20.
// Shape: the hidden table is `size ? first : (unsigned char*)&vector` through one inline GetData (ebp, null when not morphed); the
// sub-primitive is a loop-scope local (inline constructor after the out-of-line CBase, RenderFlags = 2 assigned after it).
class CBase {
public:
    virtual ~CBase();                              // 0x0099A300
    CBase();                                       // 0x0099A2F0
};
class CEngineInternalPrimitiveBase;
class CEnginePrimitiveManager;
struct C3DMaterialEntry { unsigned char _pad[0x10]; void* Present; unsigned char _pad14[0x1c]; };   // 0x30 bytes
struct C3DPrimitive2 { unsigned char _pad00[4]; long MaterialIndex; unsigned char _pad08[0x54]; void* RenderData; unsigned char _pad60[0x40]; };   // 0xa0
struct C3DMeshInfo {
    unsigned char     _pad[0x8c];
    C3DMaterialEntry* Materials;                   // +0x8c
    unsigned long     PrimitiveCount;              // +0x90
    C3DPrimitive2*    Primitives;                  // +0x94
};

class __declspec(novtable) CEngineSubPrimitiveBase : public CBase {   // PDB, 0x18 bytes
public:
    unsigned long                 RenderFlags;      // +0x04
    unsigned char                 Alpha;            // +0x08
    CEngineInternalPrimitiveBase* Parent;           // +0x0c
    CEngineSubPrimitiveBase*      NextSubPrimitive; // +0x10
    CEnginePrimitiveManager*      Manager;          // +0x14
    CEngineSubPrimitiveBase(CEngineInternalPrimitiveBase& parent) : Parent(&parent), NextSubPrimitive(0) { }
};
class __declspec(novtable) CEngineSubPrimitiveMeshBase : public CEngineSubPrimitiveBase {   // PDB, 0x24 bytes
public:
    C3DPrimitive2* MeshPrimitive;                   // +0x18
    long           RenderMode;                      // +0x1c
    bool           IsCloth;                         // +0x20
    CEngineSubPrimitiveMeshBase(CEngineInternalPrimitiveBase& parent, C3DPrimitive2* meshPrimitive, long mode)
        : CEngineSubPrimitiveBase(parent), MeshPrimitive(meshPrimitive), RenderMode(mode), IsCloth(false) { }
};
class CEngineSubPrimitiveStaticMesh : public CEngineSubPrimitiveMeshBase {   // vtable 0x012A41C8
public:
    CEngineSubPrimitiveStaticMesh(CEngineInternalPrimitiveBase& parent, C3DPrimitive2* meshPrimitive, long mode)
        : CEngineSubPrimitiveMeshBase(parent, meshPrimitive, mode) { }
    virtual void v01();
};

struct CByteArray {                                // CArray<unsigned char>: first, last, end
    unsigned char* m_pFirst;
    unsigned char* m_pLast;
    unsigned char* m_pEnd;
    bool empty() const { return m_pFirst == m_pLast; }
    unsigned char* GetData() { return (m_pLast - m_pFirst) ? m_pFirst : (unsigned char*)this; }
};
struct CEngineInternalPrimitiveMorphInfo {
    unsigned char _pad00[0x24];
    unsigned long HiddenMaterialNameCount;         // +0x24 (HiddenMaterialNames.size())
    unsigned char _pad28[4];
    CByteArray    HiddenMaterials;                 // +0x2c
    void BuildHiddenMaterials(C3DMeshInfo* meshInfo);   // 0x00BEC8D0
};
class CEngineInternalPrimitiveBase {
public:
    unsigned char _pad00[8];
    long          Type;                            // +0x08
    unsigned char _pad0c[0x98];
    C3DMeshInfo*  MeshInfo;                        // +0xa4
    unsigned char _pada8[4];
    CEngineInternalPrimitiveMorphInfo Morph;       // +0xac (morphed meshes, type 10)
};

class CEnginePrimitiveManagerStaticMeshesBase {
public:
    void RenderMeshPrimitive(CEngineSubPrimitiveStaticMesh* subPrimitive, long pass);   // 0x00BA5260
    void DrawImmediately(CEngineInternalPrimitiveBase& primitive, long mode, long pass);
};

void CEnginePrimitiveManagerStaticMeshesBase::DrawImmediately(CEngineInternalPrimitiveBase& primitive, long mode, long pass)
{
    C3DMeshInfo* meshInfo = primitive.MeshInfo;
    unsigned char* hidden = 0;
    if (primitive.Type == 10) {
        CEngineInternalPrimitiveMorphInfo& morph = primitive.Morph;
        if (morph.HiddenMaterialNameCount != 0) {
            if (morph.HiddenMaterials.empty())
                morph.BuildHiddenMaterials(meshInfo);
            hidden = morph.HiddenMaterials.GetData();
        }
    }
    for (unsigned long i = 0; i < meshInfo->PrimitiveCount; ++i) {
        C3DPrimitive2* meshPrimitive = &meshInfo->Primitives[i];
        if (meshPrimitive->RenderData == 0)
            continue;
        C3DMaterialEntry* material = &meshInfo->Materials[meshPrimitive->MaterialIndex];
        if (hidden && hidden[meshPrimitive->MaterialIndex])
            continue;
        if (material->Present == 0)
            continue;
        CEngineSubPrimitiveStaticMesh subPrimitive(primitive, meshPrimitive, mode);
        subPrimitive.RenderFlags = 2;
        RenderMeshPrimitive(&subPrimitive, pass);
    }
}
