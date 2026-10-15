// C3DMesh2::C3DMesh2() -- base C3DMeshStats, memory handles -1, every count/pointer zeroed, both flags cleared
// Leaf: ??0C3DMesh2
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A89190, 112 bytes. Callees: C3DMeshStats::C3DMeshStats 0x00A2F210.
// Vtable: C3DMesh2 0x0129DEF0.
// Retail C3DMesh2 sits 8 bytes below the Ego_r PDB layout (as in the landed InitialiseBoneNamePointers
// 0x00A892E0): MemoryHandle +0x7C, PrimMemoryHandle +0x80, BoneNameMemoryHandle +0x84, MaterialCount +0x88 ..
// BoneNameDataSize +0xAC, InverseTransform +0xB0, BoneSpaceTransforms +0xE0, ClothFlag +0xE4,
// LockedFromMemoryRelocation +0xE5, MeshID +0xE8. All stores precede the vtable store; the handles are an
// init list (declaration order, `or eax,-1`), the zeroes too, and the two flags are body assignments in
// the retail order (+0xE5 before +0xE4). InverseTransform, BoneSpaceTransforms and MeshID are left alone.
struct C3DMeshStats {
    virtual ~C3DMeshStats();
    void* MemoryPool;                       // +0x04 CHandleBasedMemoryPool*
    unsigned char _stats[0x74];             // +0x08 .. +0x7B
    C3DMeshStats();                         // 0x00A2F210
};

struct C3DMeshMaterial;
struct C3DPrimitive2;
struct C3DBone2;
struct C3DKeyframe;
struct CMatrix4x4;
struct CMatrix3x4 { float m_row[3][4]; };

struct C3DMesh2 : C3DMeshStats {
    long MemoryHandle;                      // +0x7C
    long PrimMemoryHandle;                  // +0x80
    long BoneNameMemoryHandle;              // +0x84
    unsigned long MaterialCount;            // +0x88
    C3DMeshMaterial* Materials;             // +0x8C
    unsigned long PrimitiveCount;           // +0x90
    C3DPrimitive2* Primitives;              // +0x94
    unsigned long BoneCount;                // +0x98
    C3DBone2* Bones;                        // +0x9C
    C3DKeyframe* BoneKeyframes;             // +0xA0
    unsigned short* BoneNameIndices;        // +0xA4
    char* BoneNameData;                     // +0xA8
    unsigned long BoneNameDataSize;         // +0xAC
    CMatrix3x4 InverseTransform;            // +0xB0
    CMatrix4x4* BoneSpaceTransforms;        // +0xE0
    bool ClothFlag;                         // +0xE4
    bool LockedFromMemoryRelocation;        // +0xE5
    unsigned long MeshID;                   // +0xE8
    C3DMesh2();
    virtual ~C3DMesh2();
};

C3DMesh2::C3DMesh2()
    : MemoryHandle(-1), PrimMemoryHandle(-1), BoneNameMemoryHandle(-1),
      MaterialCount(0), Materials(0), PrimitiveCount(0), Primitives(0), BoneCount(0), Bones(0),
      BoneKeyframes(0), BoneNameIndices(0), BoneNameData(0), BoneNameDataSize(0)
{
    LockedFromMemoryRelocation = false;
    ClothFlag = false;
}