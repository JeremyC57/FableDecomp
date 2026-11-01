// C3DMesh2::~C3DMesh2() -- mesh destructor: when the main mesh block is allocated, destroy its materials and primitives in place and return the block to the memory pool; return the primitive and bone-name blocks too; then ~C3DMeshStats
// Leaf: ??1C3DMesh2@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A895F0, 175 bytes, thiscall, tail jump to C3DMeshStats::~C3DMeshStats 0x00A2F190.
// Identity (proven): the first store is vtable 0x0129DEF0 (rtti-vtables.tsv: C3DMesh2, RTTI .?AVC3DMesh2@@ : C3DMeshStats :
//   CMovableResource); the sole caller 0x00A8D703 is the scalar deleting destructor 0x00A8D700, slot 0 of that vtable.
//   Ego_r.pdb / FableWin.pdb ??1C3DMesh2@@UAE@XZ; the catalog label is right. The tail target 0x00A2F190 is the landed
//   ??1C3DMeshStats (its own vtable store 0x0129CF04).
// Callees: each material's / primitive's virtual destructor (slot 0, flag 0: an explicit in-place `~T()` call; the objects
//   live in the pool block), CMovableResourceMemoryPool::FreeResource(long) 0x00A62ED0 (x3, ?FreeResource@
//   CMovableResourceMemoryPool@@QAEXJ@Z in both PDBs), ??1C3DMeshStats@@UAE@XZ 0x00A2F190 (tail).
// Layout: retail C3DMeshStats is 0x7C (Ego_r 0x84 less 8 for the two 12-byte STLport containers; MemoryPool +0x04 as the
//   landed ~C3DMeshStats), so the Ego_r C3DMesh2 members sit 8 bytes lower: MemoryHandle +0x7C, PrimMemoryHandle +0x80,
//   BoneNameMemoryHandle +0x84, MaterialCount +0x88, Materials +0x8C (C3DMeshMaterial 0x30), PrimitiveCount +0x90,
//   Primitives +0x94 (C3DPrimitive2, retail 0xA0), BoneCount +0x98 ... No engine/C3DMesh2.h; local model.
// Shape: the in-place destruction loops are the PDB template GFExplicitDelete<T>(T*, unsigned long) (__fastcall), defined in
//   the TU and inlined by VC7.1: `if (objects) for (i < count) objects[i].~T();` on its by-value copies (count loaded first:
//   right-to-left argument evaluation; pointer walk with a count-down register and the unsigned `jbe` entry guard). The
//   same loops written on the members re-read MaterialCount / Materials every pass (probe 1, 221v175). Each handle is
//   freed and reset to -1 behind a `!= -1` test.
// Layout: engine/CMovableResource.h and engine/CMovableResourceMemoryPool.h are flattened records (vptr / bases as data, no
//   member functions), so CMovableResource (polymorphic base) and the pool's FreeResource are local declarations.
class CMovableResourceMemoryPool { public:
    void FreeResource(long handle);                         // 0x00A62ED0
};
class CCharStringData;
class CCharString { public:
    ~CCharString();                                         // 0x0099EAE0
    CCharStringData* PStringData;
};
class CMovableResource { public:                            // vtable 0x0129B860
    virtual bool CanRelocateData(long handle) const;
    virtual void RelocateData(long handle, void* oldAddress, void* newAddress, long size);
};
class C3DMeshStats : public CMovableResource {              // vtable 0x0129CF04, retail 0x7C bytes
public:
    virtual ~C3DMeshStats();                                // 0x00A2F190
    CMovableResourceMemoryPool* MemoryPool;                 // +0x04
    long                        StatsMemoryHandle;          // +0x08
    CCharString                 Name;                       // +0x0C
    unsigned char               _body[0x7C - 0x10];         // BoundingSphere .. AnimatedFlag (retail containers)
};
class C3DMeshMaterial {                                     // 0x30 bytes (CBaseClass-derived, virtual dtor)
public:
    virtual ~C3DMeshMaterial();
    unsigned char _body[0x30 - 4];
};
class C3DPrimitive2 {                                       // retail 0xA0 bytes (CMovableResource-derived, virtual dtor)
public:
    virtual ~C3DPrimitive2();
    unsigned char _body[0xA0 - 4];
};
// Ego_r.pdb / FableWin.pdb ??$GFExplicitDelete@VC3DMeshMaterial@@@@YIXPAVC3DMeshMaterial@@K@Z and the C3DPrimitive2 twin:
//   in-place destruction of a pool-allocated object array (the GFExplicitNew counterpart); inlined here.
template <class T> void __fastcall GFExplicitDelete(T* objects, unsigned long count)
{
    if (objects) {
        for (unsigned long i = 0; i < count; ++i)
            objects[i].~T();
    }
}
class C3DBone2;
class C3DKeyframe;
class CMatrix4x4;
class CMatrix3x4 { public: float M[12]; };
class C3DMesh2 : public C3DMeshStats {                      // vtable 0x0129DEF0
public:
    virtual ~C3DMesh2();
    long             MemoryHandle;                          // +0x7C
    long             PrimMemoryHandle;                      // +0x80
    long             BoneNameMemoryHandle;                  // +0x84
    unsigned long    MaterialCount;                         // +0x88
    C3DMeshMaterial* Materials;                             // +0x8C
    unsigned long    PrimitiveCount;                        // +0x90
    C3DPrimitive2*   Primitives;                            // +0x94
    unsigned long    BoneCount;                             // +0x98
    C3DBone2*        Bones;                                 // +0x9C
    C3DKeyframe*     BoneKeyframes;                         // +0xA0
    unsigned short*  BoneNameIndices;                       // +0xA4
    char*            BoneNameData;                          // +0xA8
    unsigned long    BoneNameDataSize;                      // +0xAC
    CMatrix3x4       InverseTransform;                      // +0xB0
    CMatrix4x4*      BoneSpaceTransforms;                   // +0xE0
    bool             ClothFlag;                             // +0xE4
    bool             LockedFromMemoryRelocation;            // +0xE5
    unsigned long    MeshID;                                // +0xE8
};

C3DMesh2::~C3DMesh2()
{
    if (MemoryHandle != -1) {
        GFExplicitDelete(Materials, MaterialCount);
        GFExplicitDelete(Primitives, PrimitiveCount);
        MemoryPool->FreeResource(MemoryHandle);
        MemoryHandle = -1;
    }
    if (PrimMemoryHandle != -1) {
        MemoryPool->FreeResource(PrimMemoryHandle);
        PrimMemoryHandle = -1;
    }
    if (BoneNameMemoryHandle != -1) {
        MemoryPool->FreeResource(BoneNameMemoryHandle);
        BoneNameMemoryHandle = -1;
    }
}