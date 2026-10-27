// behaviour test for 0x005BBA28: the deleting destructor frees the record's block, and the record itself only with flag 1
#include <cstdio>
#include <cstdlib>
static int g_frees = 0, g_deletes = 0;
struct R { void* block; long x; };
static R* Delete(R* r, unsigned int flags) { if (r->block) { std::free(r->block); ++g_frees; } if (flags & 1) ++g_deletes; return r; }
int main()
{
    R a; a.block = std::malloc(8); R b; b.block = 0;
    int ok = Delete(&a, 1) == &a && Delete(&b, 0) == &b && g_frees == 1 && g_deletes == 1;
    if (!ok) { std::printf("005bba28_TEST FAIL\n"); return 1; }
    std::printf("005bba28_TEST PASS\n");
    return 0;
}