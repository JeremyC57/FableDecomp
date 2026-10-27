// behaviour test for 0x0122AD60: a function-local static vector's atexit destructor frees only a non-null block
#include <cstdio>
#include <cstdlib>
static int g_frees = 0;
struct V { void* _M_start; void* _M_finish; void* _M_end; void Destroy() { if (_M_start) { std::free(_M_start); ++g_frees; } } };
int main()
{
    V a; a._M_start = std::malloc(16); a._M_finish = a._M_end = a._M_start; a.Destroy();
    V b; b._M_start = b._M_finish = b._M_end = 0; b.Destroy();
    if (g_frees != 1) { std::printf("0122ad60_TEST FAIL\n"); return 1; }
    std::printf("0122ad60_TEST PASS\n");
    return 0;
}