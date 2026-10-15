// C3DMeshInfo::C3DMeshInfo(void* unused, long a, long b) -- the base's two words (+4 = b, +8 = a), this class's vtable, MeshData null, the bounding sphere zeroed then given radius -1.0, PhysicsMeshIndex -1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A248F0, length 56 bytes (64 with padding).
// Leaf: ??0C3DMeshInfo
// The chunk label (CSoundInit::CSoundInit) is propagated: the vtable stored here, 0x0129CD3C, is C3DMeshInfo
// (rtti-vtables.tsv). Layout from the PDB (Ego_r C3DMeshInfo, 96 bytes): a 12-byte base (vptr + two words),
// MeshData (CIVCountedPointer) +0x0c, BoundingBox +0x10, BoundingSphere +0x28 (centre + radius), SafeBoundingRadius
// +0x38, PhysicsMeshIndex +0x3c, LODErrors[4] +0x40, LODDataSizes[4] +0x50.
// Shape: `ret 0Ch` with the first argument never read; the two argument stores precede the vtable store
// (the members of a polymorphic root base constructed inline, its own vtable store dead), then the counted
// pointer's and the sphere's inline constructors zero +0x0c and +0x28..+0x34 from one integer register (the
// sphere words are modelled as dwords, as in the landed 0x00A98100), and the body overwrites the radius with
// -1.0f (the dead zero store is kept) and sets PhysicsMeshIndex to -1.
struct C3DMeshInfoBase {                        // the 12-byte root: vptr + two words
    virtual void v00();
    long m_4;                                   // +0x04
    long m_8;                                   // +0x08
    C3DMeshInfoBase(long a4, long a8) : m_4(a4), m_8(a8) { }
};

template <class T>
struct CIVCountedPointer {                      // intrusive counted pointer: the object only
    T* m_pObject;
    CIVCountedPointer() : m_pObject(0) { }
};

struct C3DVector { float x, y, z; };

struct C3DBoundingBox {
    C3DVector MinCoord;                         // +0x00
    C3DVector MaxCoord;                         // +0x0c
};

struct C3DBoundingSphere {                      // centre + radius, zeroed as dwords
    long m_x, m_y, m_z;
    union { long m_radiusBits; float Radius; };
    C3DBoundingSphere() : m_x(0), m_y(0), m_z(0), m_radiusBits(0) { }
};

struct C3DMeshInfo : C3DMeshInfoBase {
    struct CMeshData;
    CIVCountedPointer<CMeshData> MeshData;      // +0x0c
    C3DBoundingBox    BoundingBox;              // +0x10
    C3DBoundingSphere BoundingSphere;           // +0x28
    float             SafeBoundingRadius;       // +0x38
    long              PhysicsMeshIndex;         // +0x3c
    float             LODErrors[4];             // +0x40
    unsigned long     LODDataSizes[4];          // +0x50
    virtual void v00();
    C3DMeshInfo(void* unused, long a, long b);
};

C3DMeshInfo::C3DMeshInfo(void* unused, long a, long b)
    : C3DMeshInfoBase(b, a)
{
    BoundingSphere.Radius = -1.0f;
    PhysicsMeshIndex = -1;
}
