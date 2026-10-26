// C3DMesh2::Sizeof() const -- the object, its three pooled blocks and every primitive's extra bytes
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A893C0, 144 bytes. Callees: CMovableResourceMemoryPool::GetAllocatedSize
// 0x00A026D0 (the manifest label "CDefinitionManager::GetDefClassIndexFromGlobalIndex" is propagated),
// C3DPrimitive2::Sizeof 0x00A90860.
// Retail layout as the landed C3DMesh2 rows (8 bytes below the Ego_r PDB): MemoryPool +4, StatsMemoryHandle +8
// (C3DMeshStats), MemoryHandle +0x7C, PrimMemoryHandle +0x80, PrimitiveCount +0x90, Primitives +0x94 (0xA0-byte
// elements); sizeof(C3DMesh2) = 0xEC. Each primitive adds its own Sizeof() minus the part already counted in
// the primitive array (`lea edi,[edi+eax-0A0h]`); the count is re-read after every call.
class CMovableResourceMemoryPool { public:
    unsigned long GetAllocatedSize(long handle);    // 0x00A026D0
    void* GetPointerFromHandle(long handle);         // 0x00A026B0
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
    unsigned long Sizeof() const;
};

unsigned long C3DMesh2::Sizeof() const
{
    unsigned long size = sizeof(C3DMesh2);
    if (MemoryHandle != -1)
        size += MemoryPool->GetAllocatedSize(MemoryHandle);
    if (StatsMemoryHandle != -1)
        size += MemoryPool->GetAllocatedSize(StatsMemoryHandle);
    if (PrimMemoryHandle != -1)
        size += MemoryPool->GetAllocatedSize(PrimMemoryHandle);
    for (unsigned long i = 0; i < PrimitiveCount; ++i)
        size += Primitives[i].Sizeof() - sizeof(C3DPrimitive2);
    return size;
}