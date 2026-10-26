#include <string.h>
// C3DMesh2::ConvertToMeshStats() -- destroy mesh arrays and migrate the retained bone block into stats memory. Retail Fable.exe @ 0x00A8B3A0, 276 bytes; calls AllocateResource @ 0x00A62E40, GetPointerFromHandle @ 0x00A026B0, Free @ 0x00A62ED0, and virtual RelocateData slot +4. No C3DMesh2 header exists: layout follows landed C3DMesh2::Sizeof/InitialisePointers and RelocateData. The lock assignment uses an ordinary bool pointer to the named member; this preserves the actual field access while matching retail ordering without volatile.
class C3DMesh2;
class CMovableResourceMemoryPool { public:
    bool AllocateResource(long* handle, C3DMesh2* owner, unsigned long size, unsigned long flags); // 0x00A62E40
    void* GetPointerFromHandle(long handle); // 0x00A026B0
    void Free(long handle);                 // 0x00A62ED0
};
class C3DMeshStats { public:
    virtual ~C3DMeshStats();
    CMovableResourceMemoryPool* MemoryPool;
    long StatsMemoryHandle;
    unsigned char _stats[0x70];
    virtual void RelocateData(long,void*,void*,long);
};
class C3DMeshMaterial { public: virtual void Destroy(unsigned long flags); unsigned char _body[0x2c]; };
class C3DPrimitive2 { public: virtual void Destroy(unsigned long flags); unsigned char _body[0x9c]; virtual void RelocateData(long,void*,void*,long); };
class C3DBone2 { public: unsigned char _body[0x3c]; };
class C3DKeyframe { public: unsigned char _body[0x30]; };
class CMatrix4x4 { public: float m[16]; };
class C3DMesh2 : public C3DMeshStats { public:
    long MemoryHandle;                 // +0x7c
    long PrimMemoryHandle;             // +0x80
    long BoneNameMemoryHandle;         // +0x84
    unsigned long MaterialCount;       // +0x88
    C3DMeshMaterial* Materials;        // +0x8c
    unsigned long PrimitiveCount;      // +0x90
    C3DPrimitive2* Primitives;         // +0x94
    unsigned long BoneCount;           // +0x98
    C3DBone2* Bones;                   // +0x9c
    C3DKeyframe* BoneKeyframes;        // +0xa0
    unsigned short* BoneNameIndices;   // +0xa4
    char* BoneNameData;                // +0xa8
    unsigned long BoneNameDataSize;    // +0xac
    unsigned char _inverse[0x30];      // +0xb0
    CMatrix4x4* BoneSpaceTransforms;   // +0xe0
    bool ClothFlag;                    // +0xe4
    bool LockedFromMemoryRelocation;   // +0xe5
    unsigned long MeshID;              // +0xe8
    virtual void RelocateData(long,void*,void*,long);
    void ConvertToMeshStats();
};
void C3DMesh2::ConvertToMeshStats()
{
    bool* relocationLock = &LockedFromMemoryRelocation;
    *relocationLock=true;
    C3DMeshMaterial* material=Materials;
    unsigned long materialCount=MaterialCount;
    if (material) {
        if (materialCount > 0) do { material->Destroy(0); material+=1; } while (--materialCount);
    }
    C3DPrimitive2* primitive=Primitives;
    unsigned long primitiveCount=PrimitiveCount;
    if (primitive) {
        if (primitiveCount > 0) do { primitive->Destroy(0); primitive+=1; } while (--primitiveCount);
    }
    Materials=0;
    MaterialCount=0;
    Primitives=0;
    PrimitiveCount=0;
    unsigned long bytes=BoneCount*0xac;
    long newHandle;
    if (MemoryPool->AllocateResource(&newHandle,this,bytes,2)) {
        char* oldBuffer=(char*)MemoryPool->GetPointerFromHandle(MemoryHandle);
        char* newBuffer=(char*)MemoryPool->GetPointerFromHandle(newHandle);
        memcpy(newBuffer,oldBuffer,bytes);
        RelocateData(MemoryHandle,oldBuffer,newBuffer,bytes);
        MemoryPool->Free(MemoryHandle);
        MemoryHandle=newHandle;
    }
    if (PrimMemoryHandle!=-1) {
        MemoryPool->Free(PrimMemoryHandle);
        PrimMemoryHandle=-1;
    }
    *relocationLock=false;
}