// C3DMesh2::InitialisePointers(unsigned long boneCount, bool) -- carve the mesh's pooled block into the bone transforms, keyframes, bones, materials and primitives, constructing the materials and primitives in place
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A896A0, 160 bytes (`ret 8`, the second argument unused). Callees:
// CHandleBasedMemoryPool::GetPointerFromHandle 0x00A026B0, C3DMeshMaterial::C3DMeshMaterial 0x00ABF7B0,
// C3DPrimitive2::C3DPrimitive2 0x00A90700.
// Retail layout as the landed C3DMesh2 rows (8 bytes below the Ego_r PDB). Element sizes: CMatrix4x4 0x40,
// C3DKeyframe 0x30, C3DBone2 0x3C, C3DMeshMaterial 0x30, C3DPrimitive2 0xA0. The materials and primitives are
// built with a placement `new` per element (null test each) in count-down loops: an inline helper taking the
// count BY VALUE (members read in the loop test re-read the count after every constructor call).
#include <new>
template <class T>
inline void ConstructElements(T* elements, unsigned long count)
{
    for (unsigned long i = 0; i < count; ++i, ++elements)
        new (elements) T;
}

class CHandleBasedMemoryPool
{
public:
    void* GetPointerFromHandle(long);   // 0x00A026B0
};

class CMovableResourceMemoryPool : public CHandleBasedMemoryPool { public:
    unsigned long GetAllocatedSize(long handle);    // 0x00A026D0
};

class C3DMeshStats { public:
    virtual ~C3DMeshStats();
    CMovableResourceMemoryPool* MemoryPool; // +0x04
    long StatsMemoryHandle;                 // +0x08
    unsigned char _stats[0x70];             // +0x0C .. +0x7B (Name, bounds, helper maps, generators)
};

class C3DMeshMaterial { public:                    // 0x30 bytes
    unsigned char _body[0x30];
    C3DMeshMaterial();                      // 0x00ABF7B0
};
class C3DPrimitive2 { public:                      // 0xA0 bytes in retail
    unsigned char _body[0xA0];
    C3DPrimitive2();                        // 0x00A90700
    unsigned long Sizeof() const;           // 0x00A90860
};
class C3DBone2 { public: unsigned char _body[0x3C]; };
class C3DKeyframe { public: unsigned char _body[0x30]; };
class CMatrix4x4 { public: float m[4][4]; };
class CMatrix3x4 { public: float m_row[3][4]; };

class C3DMesh2 : public C3DMeshStats { public:
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
    void InitialisePointers(unsigned long boneCount, bool unused);
};

void C3DMesh2::InitialisePointers(unsigned long boneCount, bool unused)
{
    unsigned char* p = (unsigned char*)MemoryPool->GetPointerFromHandle(MemoryHandle);
    BoneSpaceTransforms = (CMatrix4x4*)p;
    p += boneCount * sizeof(CMatrix4x4);
    BoneKeyframes = (C3DKeyframe*)p;
    p += boneCount * sizeof(C3DKeyframe);
    Bones = (C3DBone2*)p;
    p += boneCount * sizeof(C3DBone2);
    Materials = (C3DMeshMaterial*)p;
    p += MaterialCount * sizeof(C3DMeshMaterial);
    ConstructElements(Materials, MaterialCount);
    Primitives = (C3DPrimitive2*)p;
    ConstructElements(Primitives, PrimitiveCount);
}