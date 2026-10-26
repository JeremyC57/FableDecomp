#include <cstdio>
class CMovableResourceMemoryPool { public:
    unsigned long GetAllocatedSize(long handle) { return (unsigned long)handle * 100; }
};
class C3DPrimitive2 { public: unsigned char _body[0xA0]; unsigned long m_extra; unsigned long Sizeof() const { return 0xA0 + m_extra; } };
class C3DMesh2 { public:
    CMovableResourceMemoryPool* MemoryPool;
    long StatsMemoryHandle, MemoryHandle, PrimMemoryHandle;
    unsigned long PrimitiveCount;
    C3DPrimitive2* Primitives;
    unsigned long Sizeof() const
    {
        unsigned long size = 0xEC;
        if (MemoryHandle != -1)
            size += MemoryPool->GetAllocatedSize(MemoryHandle);
        if (StatsMemoryHandle != -1)
            size += MemoryPool->GetAllocatedSize(StatsMemoryHandle);
        if (PrimMemoryHandle != -1)
            size += MemoryPool->GetAllocatedSize(PrimMemoryHandle);
        for (unsigned long i = 0; i < PrimitiveCount; ++i)
            size += Primitives[i].Sizeof() - 0xA0;
        return size;
    }
};
int main()
{
    CMovableResourceMemoryPool pool;
    C3DPrimitive2 prims[2]; prims[0].m_extra = 7; prims[1].m_extra = 11;
    C3DMesh2 m; m.MemoryPool = &pool; m.MemoryHandle = 1; m.StatsMemoryHandle = -1; m.PrimMemoryHandle = 3;
    m.PrimitiveCount = 2; m.Primitives = prims;
    if (m.Sizeof() != 0xEC + 100 + 300 + 18) { std::printf("00a893c0_TEST FAIL\n"); return 1; }
    m.MemoryHandle = -1; m.PrimMemoryHandle = -1; m.PrimitiveCount = 0;
    if (m.Sizeof() != 0xEC) { std::printf("00a893c0_TEST FAIL\n"); return 1; }
    std::printf("00a893c0_TEST PASS\n");
    return 0;
}