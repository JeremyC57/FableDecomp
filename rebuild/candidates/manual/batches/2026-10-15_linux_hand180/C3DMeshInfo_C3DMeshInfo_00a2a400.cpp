// C3DMeshInfo::C3DMeshInfo(const C3DMeshInfo&) -- the implicit copy constructor: the CMeshDataBankEntry base, the intrusive MeshData pointer (add-ref), the bounds and the LOD tables
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2A400, length 178 bytes (`ret 4`).
// GRAPHICS/MESH LAYER: true class C3DMeshInfo (stores vtables 0x0129CD28 CMeshDataBankEntry then 0x0129CD3C C3DMeshInfo,
// rtti-vtables.tsv); called by the landed _Uninit_copy<C3DMeshInfo*> 0x00A2AE20.
// Layout (PDB C3DMeshInfo, 0x60, agrees with retail): base CMeshDataBankEntry {vptr, +4, +8}, MeshData
// (CIVCountedPointer<C3DMeshInfo::CMeshData>, the pointee's count at +4) +0xc, BoundingBox +0x10, BoundingSphere +0x28,
// SafeBoundingRadius +0x38, PhysicsMeshIndex +0x3c, LODErrors float[4] +0x40, LODDataSizes ulong[4] +0x50.
// Shape: the compiler-generated memberwise copy (C3DVector's memberwise user copy ctor gives the direct dword
// moves for the bounds; the float/ulong arrays copy as blocks through `lea`), emitted out of line because the TU placement-copies the class.
// Leaf: ??0C3DMeshInfo
inline void* operator new(unsigned int, void* where) { return where; }

struct CIVCountedPointee {
    virtual void Destroy(int flags);
    unsigned long IVRefCount;                       // +4
};
template<class T> struct CIVCountedPointer {
    T* m_pObject;
    CIVCountedPointer(const CIVCountedPointer& other) : m_pObject(other.m_pObject) { if (m_pObject) ++m_pObject->IVRefCount; }
};
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
};
struct C3DBoundingBox { C3DVector MinCoord, MaxCoord; };
struct C3DBoundingSphere { C3DVector Center; float Radius; };

struct CMeshDataBankEntry {                         // vtable 0x0129CD28
    virtual void v00();
    long m_04;                                      // +4
    long m_08;                                      // +8
};
struct C3DMeshInfo : CMeshDataBankEntry {           // vtable 0x0129CD3C
    struct CMeshData : CIVCountedPointee {};
    virtual void v00();
    CIVCountedPointer<CMeshData> MeshData;          // +0x0c
    C3DBoundingBox BoundingBox;                     // +0x10
    C3DBoundingSphere BoundingSphere;               // +0x28
    float SafeBoundingRadius;                       // +0x38
    long PhysicsMeshIndex;                          // +0x3c
    float LODErrors[4];                             // +0x40
    unsigned long LODDataSizes[4];                  // +0x50
};

void CopyMeshInfo(C3DMeshInfo* where, const C3DMeshInfo& other)
{
    new (where) C3DMeshInfo(other);
}
