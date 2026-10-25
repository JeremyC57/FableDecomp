#include <cstdio>
class CHandleBasedMemoryPool {
public:
    void Free(long handle);
};
class CMovableResourceMemoryPool : public CHandleBasedMemoryPool {
public:
    void FreeResource(long handle);
};
static int g_calls = 0;
static long g_handle = 0;
static void* g_this = 0;
void CHandleBasedMemoryPool::Free(long handle) { g_calls++; g_handle = handle; g_this = this; }

int main() {
    char storage[0x468];
    CMovableResourceMemoryPool* pool = (CMovableResourceMemoryPool*)storage;
    pool->FreeResource(77);
    if (g_calls != 1 || g_handle != 77 || g_this != (void*)storage) { std::printf("FAIL\n"); return 1; }
    pool->FreeResource(-5);
    if (g_calls != 2 || g_handle != -5) { std::printf("FAIL 2\n"); return 1; }
    std::printf("00a62ed0_TEST PASS\n");
    return 0;
}
