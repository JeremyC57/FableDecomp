// Grade: DIFFER(1621v1621) -- same length; register allocation of the two ushort counts (retail edi/edx, ours spills one) and of the three sizes (retail spills mainSize, keeps prim/name in edi/esi); primData slot
// C3DMesh2::LoadBinary(CMemoryDataInputStream*, CMovableResourceMemoryPool*, CBankFile*, bool) -- load the mesh stats, the counts, allocate the three resource blocks, read bone tables and names, the materials and (unless skipped) the primitives, with the mesh locked against relocation throughout
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A8AD40, 1632 bytes, ret 10h.
// Callees: C3DMeshStats::LoadBinary 0x00A2F2D0, ReadWithSrcChunkOverflow 0x00993CA0, stream slot 4 (+0x10) Read,
//   CMovableResourceMemoryPool::AllocateResource 0x00A62E40, C3DMesh2::InitialisePointers 0x00A896A0,
//   InitialiseBoneNamePointers 0x00A892E0, C3DMeshMaterial::LoadBinary 0x00ABF100,
//   CHandleBasedMemoryPool::GetPointerFromHandle 0x00A026B0 / Lock 0x00A026F0 / Unlock 0x00A02710,
//   C3DPrimitive2::LoadBinary 0x00A92900.
// Layout: Ego_r.pdb C3DMesh2 minus 8 (the retail C3DMeshStats base is 0x7c, PDB 0x84): MemoryHandle +0x7c ..
//   LockedFromMemoryRelocation +0xe5. Local model.
#include <string.h>
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CCharString {
    void* m_pStringData;
    ~CCharString();                                       // 0x0099EAE0
    CCharString& operator=(const CCharString& other);     // 0x0099EFB0
};
struct C3DVector { float X, Y, Z; };

struct CMemoryDataInputStream {
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void Read(void* buffer, unsigned long size);  // slot 4 (+0x10)
    virtual void v5();
    virtual CCharString ReadNullTerminatedString();       // slot 6 (+0x18)
    C3DVector Read3DVector();                             // 0x009950F0
    unsigned long m_position;              // +4
    long StreamSize;                       // +8
    const unsigned char* m_pData;          // +0xC
    long SourceChunkStreamPos;             // +0x10
    long m_remaining;                      // +0x14
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void ReadInline(void* buffer, long size)
    {
        if (size <= 0)
            return;
        __int64 newPosition = (__int64)(m_position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (size <= m_remaining) {
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    __forceinline bool ReadBool() { unsigned char value; ReadInline(&value, sizeof(value)); return value != 0; }
    __forceinline unsigned long ReadULong() { unsigned long value; ReadInline(&value, sizeof(value)); return value; }
    __forceinline float ReadFloat() { float value; ReadInline(&value, sizeof(value)); return value; }
    __forceinline unsigned short ReadUShort() { unsigned short value; ReadInline(&value, sizeof(value)); return value; }
};


struct CBankFile;
struct CMovableResource { virtual void v0(); };
struct CMovableResourceMemoryPool {
    bool AllocateResource(long* handle, CMovableResource* owner, unsigned long size, unsigned long flags);   // 0x00A62E40
    void* GetPointerFromHandle(long handle);   // 0x00A026B0
    void Lock(long handle);                    // 0x00A026F0
    void Unlock(long handle);                  // 0x00A02710
};
struct C3DMeshMaterial {
    unsigned char _data[0x30];
    void LoadBinary(CMemoryDataInputStream* in, CBankFile* bank);   // 0x00ABF100
};
struct C3DPrimitive2 {
    unsigned char _data[0xa0];
    bool LoadBinary(CMemoryDataInputStream* in, unsigned long flags, unsigned char** data, CMovableResourceMemoryPool* pool);   // 0x00A92900
};
struct C3DBone2 { unsigned char _data[0x3c]; };
struct C3DKeyframe { unsigned char _data[0x30]; };
struct CMatrix4x4 { float m[16]; };
struct CMatrix3x4 { float m[12]; };

struct C3DMeshStats : CMovableResource {
    CMovableResourceMemoryPool* MemoryPool;   // +0x04
    unsigned char _stats[0x74];               // +0x08 .. +0x7b
    bool LoadBinary(CMemoryDataInputStream* in);   // 0x00A2F2D0
};

static __forceinline bool IsValidSize(__int64 size) { return size >= 0 && size <= 0x7fffffff; }

struct C3DMesh2 : C3DMeshStats {
    long MemoryHandle;                     // +0x7c
    long PrimMemoryHandle;                 // +0x80
    long BoneNameMemoryHandle;             // +0x84
    unsigned long MaterialCount;           // +0x88
    C3DMeshMaterial* Materials;            // +0x8c
    unsigned long PrimitiveCount;          // +0x90
    C3DPrimitive2* Primitives;             // +0x94
    unsigned long BoneCount;               // +0x98
    C3DBone2* Bones;                       // +0x9c
    C3DKeyframe* BoneKeyframes;            // +0xa0
    unsigned short* BoneNameIndices;       // +0xa4
    char* BoneNameData;                    // +0xa8
    unsigned long BoneNameDataSize;        // +0xac
    CMatrix3x4 InverseTransform;           // +0xb0
    CMatrix4x4* BoneSpaceTransforms;       // +0xe0
    bool ClothFlag;                        // +0xe4
    bool LockedFromMemoryRelocation;       // +0xe5
    void InitialisePointers(unsigned long boneCount, unsigned long size);   // 0x00A896A0
    void InitialiseBoneNamePointers(unsigned long size);                    // 0x00A892E0
    bool LoadBinary(CMemoryDataInputStream* in, CMovableResourceMemoryPool* pool, CBankFile* bank, bool skipPrimitives);
};

bool C3DMesh2::LoadBinary(CMemoryDataInputStream* in, CMovableResourceMemoryPool* pool, CBankFile* bank, bool skipPrimitives)
{
    LockedFromMemoryRelocation = true;
    MemoryPool = pool;
    if (!C3DMeshStats::LoadBinary(in)) {
        LockedFromMemoryRelocation = false;
        return false;
    }
    MaterialCount = in->ReadULong();
    PrimitiveCount = in->ReadULong();
    BoneCount = in->ReadULong();
    BoneNameDataSize = in->ReadULong();
    ClothFlag = in->ReadBool();
    unsigned long vertexCount = in->ReadUShort();
    unsigned long indexCount = in->ReadUShort();
    if (skipPrimitives)
        PrimitiveCount = 0;
    unsigned long mainSize = BoneCount * (sizeof(C3DBone2) + sizeof(C3DKeyframe) + sizeof(CMatrix4x4))
                           + MaterialCount * sizeof(C3DMeshMaterial) + PrimitiveCount * sizeof(C3DPrimitive2);
    if (!IsValidSize(mainSize)) {
        LockedFromMemoryRelocation = false;
        return false;
    }
    unsigned long primSize = vertexCount * 0x20 + indexCount * 0x30;
    if (!IsValidSize(primSize)) {
        LockedFromMemoryRelocation = false;
        return false;
    }
    unsigned long nameSize = BoneNameDataSize + BoneCount * sizeof(unsigned short);
    if (!IsValidSize(nameSize)) {
        LockedFromMemoryRelocation = false;
        return false;
    }
    if (!MemoryPool->AllocateResource(&MemoryHandle, this, mainSize, 0)) {
        LockedFromMemoryRelocation = false;
        return false;
    }
    if (!MemoryPool->AllocateResource(&PrimMemoryHandle, this, primSize, 0)) {
        LockedFromMemoryRelocation = false;
        return false;
    }
    if (nameSize && !MemoryPool->AllocateResource(&BoneNameMemoryHandle, this, nameSize, 0)) {
        LockedFromMemoryRelocation = false;
        return false;
    }
    InitialisePointers(BoneCount, mainSize);
    InitialiseBoneNamePointers(nameSize);
    in->ReadInline(BoneNameIndices, BoneCount * sizeof(unsigned short));
    in->Read(BoneNameData, BoneNameDataSize);
    const char* name = BoneNameData;
    for (unsigned long i = 0; i < BoneCount; ++i) {
        BoneNameIndices[i] = (unsigned short)(name - BoneNameData);
        name += strlen(name) + 1;
    }
    in->Read(Bones, BoneCount * sizeof(C3DBone2));
    in->Read(BoneKeyframes, BoneCount * sizeof(C3DKeyframe));
    in->Read(BoneSpaceTransforms, BoneCount * sizeof(CMatrix4x4));
    in->ReadInline(&InverseTransform, sizeof(InverseTransform));
    for (unsigned long m = 0; m < MaterialCount; ++m)
        Materials[m].LoadBinary(in, bank);
    if (!skipPrimitives) {
        unsigned char* primData = (unsigned char*)pool->GetPointerFromHandle(PrimMemoryHandle);
        MemoryPool->Lock(MemoryHandle);
        MemoryPool->Lock(PrimMemoryHandle);
        for (unsigned long p = 0; p < PrimitiveCount; ++p) {
            if (!Primitives[p].LoadBinary(in, 0, &primData, MemoryPool)) {
                MemoryPool->Unlock(PrimMemoryHandle);
                MemoryPool->Unlock(MemoryHandle);
                LockedFromMemoryRelocation = false;
                return false;
            }
        }
        MemoryPool->Unlock(PrimMemoryHandle);
        MemoryPool->Unlock(MemoryHandle);
    }
    LockedFromMemoryRelocation = false;
    return true;
}
