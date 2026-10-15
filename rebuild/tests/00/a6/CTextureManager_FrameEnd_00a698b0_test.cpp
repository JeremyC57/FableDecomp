#include <cstdio>
static int g_calls = 0;
struct CPreallocTexturePool { unsigned char _body[0x54]; void FrameEnd() { ++g_calls; _body[0] = 1; } };
struct CTextureManager {
    void* __vftable; bool Initialised; unsigned long PoolCount; unsigned long PreallocSizes[16];
    CPreallocTexturePool PreallocPools[16]; unsigned long FailedAllocations[16]; long BufferSize; void* TextureBuffer;
    void FrameEnd()
    {
        unsigned long i;
        for (i = 0; i < PoolCount; ++i)
            PreallocPools[i].FrameEnd();
        unsigned long count = PoolCount;
        unsigned long* failed = FailedAllocations;
        for (i = 0; i < count; ++i)
            failed[i] = 0;
    }
};
int main()
{
    static CTextureManager m;
    int i;
    for (i = 0; i < 16; ++i) { m.FailedAllocations[i] = 7; m.PreallocPools[i]._body[0] = 0; }
    m.PoolCount = 3;
    m.FrameEnd();
    bool ok = g_calls == 3;
    for (i = 0; i < 16; ++i) {
        if (m.FailedAllocations[i] != (i < 3 ? 0u : 7u)) ok = false;
        if (m.PreallocPools[i]._body[0] != (i < 3 ? 1 : 0)) ok = false;
    }
    m.PoolCount = 0; m.FrameEnd();
    if (g_calls != 3 || m.FailedAllocations[3] != 7) ok = false;
    if (!ok) { std::printf("00a698b0_TEST FAIL\n"); return 1; }
    std::printf("00a698b0_TEST PASS\n");
    return 0;
}