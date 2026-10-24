#include <cstdio>
// Test copy of the 0x00BD9CA0 model: FreeVirtualAligned steps back by the offset AllocVirtualAligned stored before the block.
struct CEngineLandscapeMemoryManager {           // stand-in for the engine layout (the method reads no member)
    void* __vftable;
    long  AllocationCount;
};
static void* g_freed = 0;
static int g_frees = 0;
struct CEngineLandscapeMemoryManagerMethods : CEngineLandscapeMemoryManager {
    void FreeVirtual(void* data) { g_freed = data; ++g_frees; }
    void FreeVirtualAligned(void* data);
};
void CEngineLandscapeMemoryManagerMethods::FreeVirtualAligned(void* data)
{
    if (data != 0)
        FreeVirtual((char*)data - ((long*)data)[-1]);
}
int main()
{
    CEngineLandscapeMemoryManagerMethods manager;
    long raw[16];
    char* base = reinterpret_cast<char*>(raw);
    char* aligned = base + 24;                   // AllocVirtualAligned stored (aligned - raw) just before the block
    reinterpret_cast<long*>(aligned)[-1] = 24;
    manager.FreeVirtualAligned(aligned);
    bool ok = g_frees == 1 && g_freed == base;
    manager.FreeVirtualAligned(0);               // a null block is ignored
    if (g_frees != 1) ok = false;
    if (!ok) { std::printf("00bd9ca0_TEST FAIL\n"); return 1; }
    std::printf("00bd9ca0_TEST PASS\n");
    return 0;
}