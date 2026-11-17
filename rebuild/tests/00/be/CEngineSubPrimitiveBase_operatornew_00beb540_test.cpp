// Behaviour test for 0x00BEB540 CEngineSubPrimitiveBase::operator new (NF79 round 7): the row allocates from the private static pool
// CEngineSubPrimitiveBase::MemoryPool through CSimpleMemoryPool::Allocate. This fixture supplies both imports as link stand-ins with the
// row's mangled names (the static pool pointer and a recording Allocate) and checks the size is forwarded and the pool's block returned.
// Keeps the catalog PassPattern RB_beb540_OK.
#include <cstdio>
class CSimpleMemoryPool { public: void* Allocate(unsigned long size); unsigned long LastSize; char Block[64]; };
class CEngineSubPrimitiveBase {
public:
    static void* __fastcall operator new(unsigned int size);
private:
    static CSimpleMemoryPool* MemoryPool;
    friend int main();
};
static CSimpleMemoryPool g_pool;
CSimpleMemoryPool* CEngineSubPrimitiveBase::MemoryPool = &g_pool;
void* CSimpleMemoryPool::Allocate(unsigned long size) { LastSize = size; return Block; }
int main()
{
    g_pool.LastSize = 0;
    void* p = CEngineSubPrimitiveBase::operator new(24);
    if (p != g_pool.Block || g_pool.LastSize != 24) { std::printf("B1\n"); return 1; }
    std::printf("RB_beb540_OK\n");
    return 0;
}
