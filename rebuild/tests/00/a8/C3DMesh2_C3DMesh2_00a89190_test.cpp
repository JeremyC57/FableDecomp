#include <cstdio>
#include <new>
struct C3DMeshStats {
    void* MemoryPool;
    C3DMeshStats() : MemoryPool(0) {}
    virtual ~C3DMeshStats() {}
};
struct CMatrix3x4 { float m_row[3][4]; };
struct C3DMesh2 : C3DMeshStats {
    long MemoryHandle, PrimMemoryHandle, BoneNameMemoryHandle;
    unsigned long MaterialCount; void* Materials; unsigned long PrimitiveCount; void* Primitives;
    unsigned long BoneCount; void* Bones; void* BoneKeyframes; unsigned short* BoneNameIndices;
    char* BoneNameData; unsigned long BoneNameDataSize; CMatrix3x4 InverseTransform; void* BoneSpaceTransforms;
    bool ClothFlag; bool LockedFromMemoryRelocation; unsigned long MeshID;
    C3DMesh2()
        : MemoryHandle(-1), PrimMemoryHandle(-1), BoneNameMemoryHandle(-1),
          MaterialCount(0), Materials(0), PrimitiveCount(0), Primitives(0), BoneCount(0), Bones(0),
          BoneKeyframes(0), BoneNameIndices(0), BoneNameData(0), BoneNameDataSize(0)
    {
        LockedFromMemoryRelocation = false;
        ClothFlag = false;
    }
};
int main()
{
    unsigned char raw[sizeof(C3DMesh2)];
    for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0xAB;
    C3DMesh2* m = new (raw) C3DMesh2;
    int ok = m->MemoryHandle == -1 && m->PrimMemoryHandle == -1 && m->BoneNameMemoryHandle == -1 &&
             m->MaterialCount == 0 && m->Materials == 0 && m->PrimitiveCount == 0 && m->Primitives == 0 &&
             m->BoneCount == 0 && m->Bones == 0 && m->BoneKeyframes == 0 && m->BoneNameIndices == 0 &&
             m->BoneNameData == 0 && m->BoneNameDataSize == 0 && !m->ClothFlag && !m->LockedFromMemoryRelocation;
    m->~C3DMesh2();
    if (!ok) { std::printf("00a89190_TEST FAIL\n"); return 1; }
    std::printf("00a89190_TEST PASS\n");
    return 0;
}