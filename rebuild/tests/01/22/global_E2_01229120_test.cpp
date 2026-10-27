// behaviour test for 0x01229120: atexit destructor of a static element vector (model: destroy range out of line, then free)
#include <cstdio>
#include <cstdlib>
static int g_dtors = 0, g_frees = 0;
struct E { int v; ~E() { ++g_dtors; } };
static void __destroy_range(E* first, E* last) { for (; first != last; ++first) first->~E(); }
struct V { E* _M_start; E* _M_finish; E* _M_end;
    void Destroy() { __destroy_range(_M_start, _M_finish); if (_M_start) { std::free(_M_start); ++g_frees; } } };
int main()
{
    V v; v._M_start = (E*)std::malloc(4 * sizeof(E)); v._M_finish = v._M_start + 4; v._M_end = v._M_finish;
    v.Destroy();
    V z; z._M_start = z._M_finish = z._M_end = 0; z.Destroy();
    if (g_dtors != 4 || g_frees != 1) { std::printf("01229120_TEST FAIL\n"); return 1; }
    std::printf("01229120_TEST PASS\n");
    return 0;
}