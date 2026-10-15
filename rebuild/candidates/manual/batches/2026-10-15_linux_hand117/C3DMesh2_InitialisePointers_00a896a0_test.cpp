#include <cstdio>
#include <new>
static int g_materials = 0, g_primitives = 0;
template <class T>
inline void ConstructElements(T* elements, unsigned long count)
{
    for (unsigned long i = 0; i < count; ++i, ++elements)
        new (elements) T;
}
static unsigned char g_block[4096];
struct CMovableResourceMemoryPool { void* GetPointerFromHandle(long) { return g_block; } };
struct C3DMeshMaterial { unsigned char _body[0x30]; C3DMeshMaterial() { ++g_materials; _body[0] = 0x4D; } };
struct C3DPrimitive2 { unsigned char _body[0xA0]; C3DPrimitive2() { ++g_primitives; _body[0] = 0x50; } };
struct C3DBone2 { unsigned char _body[0x3C]; };
struct C3DKeyframe { unsigned char _body[0x30]; };
struct CMatrix4x4 { float m[4][4]; };
struct C3DMesh2 {
    CMovableResourceMemoryPool* MemoryPool;
    long MemoryHandle;
    unsigned long MaterialCount; C3DMeshMaterial* Materials;
    unsigned long PrimitiveCount; C3DPrimitive2* Primitives;
    C3DBone2* Bones; C3DKeyframe* BoneKeyframes; CMatrix4x4* BoneSpaceTransforms;
    void InitialisePointers(unsigned long boneCount, bool)
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
};
int main()
{
    CMovableResourceMemoryPool pool;
    C3DMesh2 m; m.MemoryPool = &pool; m.MemoryHandle = 0; m.MaterialCount = 2; m.PrimitiveCount = 3;
    m.InitialisePointers(4, false);
    unsigned char* b = g_block;
    int ok = (unsigned char*)m.BoneSpaceTransforms == b && (unsigned char*)m.BoneKeyframes == b + 4 * 64 &&
             (unsigned char*)m.Bones == b + 4 * 64 + 4 * 48 && (unsigned char*)m.Materials == b + 4 * (64 + 48 + 60) &&
             (unsigned char*)m.Primitives == b + 4 * (64 + 48 + 60) + 2 * 48 &&
             g_materials == 2 && g_primitives == 3 && m.Materials[1]._body[0] == 0x4D && m.Primitives[2]._body[0] == 0x50;
    if (!ok) { std::printf("00a896a0_TEST FAIL\n"); return 1; }
    std::printf("00a896a0_TEST PASS\n");
    return 0;
}
