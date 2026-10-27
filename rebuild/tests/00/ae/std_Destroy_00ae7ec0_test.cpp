// behaviour test for 0x00AE7EC0: destroying a range frees each element's block exactly once
#include <cstdio>
#include <cstdlib>
static int g_freed = 0;
struct Elem { long a; void* block; void Destroy() { if (block) { std::free(block); ++g_freed; } } };
static void DestroyRange(Elem* first, Elem* last) { for (; first != last; ++first) first->Destroy(); }
int main()
{
    Elem e[3] = { { 1, std::malloc(4) }, { 2, 0 }, { 3, std::malloc(8) } };
    DestroyRange(e, e + 3);
    DestroyRange(e, e);
    if (g_freed != 2) { std::printf("00ae7ec0_TEST FAIL\n"); return 1; }
    std::printf("00ae7ec0_TEST PASS\n");
    return 0;
}