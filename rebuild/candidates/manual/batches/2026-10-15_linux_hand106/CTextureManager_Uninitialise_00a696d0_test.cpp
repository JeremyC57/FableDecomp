#include <cstdio>
struct CTextureManager {
    void* __vftable; bool Initialised; unsigned long PoolCount; unsigned long PreallocSizes[16];
    unsigned char PreallocPools[16][0x54]; unsigned long FailedAllocations[16]; long BufferSize; void* TextureBuffer;
    void Uninitialise() { Initialised = false; if (TextureBuffer) TextureBuffer = 0; }
};
int main()
{
    static CTextureManager m;
    static char buf[4];
    m.Initialised = true; m.TextureBuffer = buf; m.BufferSize = 4;
    m.Uninitialise();
    if (m.Initialised || m.TextureBuffer != 0 || m.BufferSize != 4) { std::printf("00a696d0_TEST FAIL\n"); return 1; }
    m.Initialised = true; m.Uninitialise();
    if (m.Initialised || m.TextureBuffer != 0) { std::printf("00a696d0_TEST FAIL\n"); return 1; }
    std::printf("00a696d0_TEST PASS\n");
    return 0;
}
