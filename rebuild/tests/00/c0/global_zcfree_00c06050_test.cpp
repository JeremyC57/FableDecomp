#include <cstdio>
#include <cstdlib>
typedef void* voidpf;
static void* g_freed;
static void TestFree(void* p) { g_freed = p; std::free(p); }
static void zcfree(voidpf opaque, voidpf ptr)
{
    TestFree(ptr);
    if (opaque) return; /* make compiler happy */
}
int main()
{
    int x;
    void* p = std::malloc(21);
    zcfree(0, p);
    if (g_freed != p) { std::printf("00c06050_TEST FAIL\n"); return 1; }
    p = std::malloc(0x1ba8);
    zcfree(&x, p);
    if (g_freed != p) { std::printf("00c06050_TEST FAIL\n"); return 1; }
    zcfree(&x, 0);
    if (g_freed != 0) { std::printf("00c06050_TEST FAIL\n"); return 1; }
    std::printf("00c06050_TEST PASS\n");
    return 0;
}
