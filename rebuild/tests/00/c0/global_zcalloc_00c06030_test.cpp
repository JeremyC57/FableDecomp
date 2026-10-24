#include <cstdio>
#include <cstdlib>
typedef void* voidpf;
static unsigned g_req;
static void* TestMalloc(unsigned n) { g_req = n; return std::malloc(n); }
static void* TestCalloc(unsigned, unsigned) { return 0; }
static voidpf zcalloc(voidpf opaque, unsigned items, unsigned size)
{
    if (opaque) items += size - size; /* make compiler happy */
    return sizeof(unsigned int) > 2 ? (voidpf)TestMalloc(items * size) : (voidpf)TestCalloc(items, size);
}
int main()
{
    int x;
    void* p = zcalloc(0, 3, 7);
    if (!p || g_req != 21) { std::printf("00c06030_TEST FAIL\n"); return 1; }
    std::free(p);
    p = zcalloc(&x, 1, 0x1ba8);
    if (!p || g_req != 0x1ba8) { std::printf("00c06030_TEST FAIL\n"); return 1; }
    std::free(p);
    std::printf("00c06030_TEST PASS\n");
    return 0;
}