// C3DMeshInfo::operator=(const C3DMeshInfo&) -- memberwise assignment of a mesh info record: the two base words, the intrusive MeshData pointer (release old / add-ref new), bounding box and sphere, radius, physics index and the LOD tables
// Leaf: ??4C3DMeshInfo
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2B3C0, length 197 bytes. No direct callees (MeshData release is the
// pointee's virtual slot 1).
// Label correction: catalogued as CAnimationEntry::operator= (propagated; CAnimationEntry is 16 bytes). The 0x60-byte
// layout here is Ego_r.pdb C3DMeshInfo (sizeof 0x60, MeshData +0xc, BoundingBox +0x10, BoundingSphere +0x28,
// SafeBoundingRadius +0x38, PhysicsMeshIndex +0x3c, LODErrors +0x40, LODDataSizes +0x50) and the neighbouring rows are
// C3DMeshInfo / C3DMeshLODInfo helpers.
// Shape: the vptr is not copied (polymorphic base, compiler-style operator=); the base's +4/+8 words are copied first.
// CIVCountedPointer::operator= tests for self-assignment, releases (dec [p+4], virtual slot 1 at zero) and zeroes, then
// stores the new pointer and add-refs it. C3DBoundingBox / C3DBoundingSphere have user copy constructors and implicit
// operator=, which copies each as one `lea` block; the floats/arrays after them are plain dword moves.
// rebuild/include/engine/C3DMeshInfo.h flattens the box/sphere into byte arrays, which cannot reproduce the block copies,
// so this TU uses a local model with the same offsets.
struct CIVCountedPointeeBase {
    virtual void v00();
    virtual void DeleteThis();                     // slot 1
    unsigned long IVRefCount;                      // +0x04
    void AddRef() { ++IVRefCount; }
    void Release() { if (--IVRefCount == 0) DeleteThis(); }
};
template <class T> struct CIVCountedPointer {
    T* Object;
    CIVCountedPointer& operator=(const CIVCountedPointer& o)
    {
        if (Object != o.Object) {
            if (Object) {
                Object->Release();
                Object = 0;
            }
            Object = o.Object;
            if (Object)
                Object->AddRef();
        }
        return *this;
    }
};
struct C3DVector { float X, Y, Z; };
struct C3DBoundingBox {
    C3DVector MinCoord;                            // +0x00
    C3DVector MaxCoord;                            // +0x0c
    C3DBoundingBox() {}
    C3DBoundingBox(const C3DBoundingBox& o) : MinCoord(o.MinCoord), MaxCoord(o.MaxCoord) {}
};
struct C3DBoundingSphere {
    C3DVector Center;                              // +0x00
    float Radius;                                  // +0x0c
    C3DBoundingSphere() {}
    C3DBoundingSphere(const C3DBoundingSphere& o) : Center(o.Center), Radius(o.Radius) {}
};
struct CMeshInfoBase {                             // retail base: vptr +0, two words +4/+8
    virtual ~CMeshInfoBase();
    long m_f4;                                     // +0x04
    long m_f8;                                     // +0x08
};
struct CMeshData : CIVCountedPointeeBase { };

struct C3DMeshInfo : CMeshInfoBase {
    CIVCountedPointer<CMeshData> MeshData;         // +0x0c
    C3DBoundingBox BoundingBox;                    // +0x10
    C3DBoundingSphere BoundingSphere;              // +0x28
    float SafeBoundingRadius;                      // +0x38
    long PhysicsMeshIndex;                         // +0x3c
    float LODErrors[4];                            // +0x40
    unsigned long LODDataSizes[4];                 // +0x50
    C3DMeshInfo& operator=(const C3DMeshInfo& o);
};

C3DMeshInfo& C3DMeshInfo::operator=(const C3DMeshInfo& o)
{
    CMeshInfoBase::operator=(o);
    MeshData = o.MeshData;
    BoundingBox = o.BoundingBox;
    BoundingSphere = o.BoundingSphere;
    SafeBoundingRadius = o.SafeBoundingRadius;
    PhysicsMeshIndex = o.PhysicsMeshIndex;
    LODErrors[0] = o.LODErrors[0];
    LODErrors[1] = o.LODErrors[1];
    LODErrors[2] = o.LODErrors[2];
    LODErrors[3] = o.LODErrors[3];
    LODDataSizes[0] = o.LODDataSizes[0];
    LODDataSizes[1] = o.LODDataSizes[1];
    LODDataSizes[2] = o.LODDataSizes[2];
    LODDataSizes[3] = o.LODDataSizes[3];
    return *this;
}
